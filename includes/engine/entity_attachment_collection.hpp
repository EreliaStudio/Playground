#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <regex>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "engine/entity_attachment.hpp"

namespace spk
{
	template <typename TAttachmentBase>
		requires std::derived_from<TAttachmentBase, EntityAttachment>
	class EntityAttachmentCollection
	{
	private:
		struct AttachmentCache
		{
			std::set<std::type_index> positiveHits;
			std::set<std::type_index> negativeHits;
		};

		std::vector<std::unique_ptr<TAttachmentBase>> _attachments;
		mutable std::unordered_map<std::type_index, std::set<TAttachmentBase *>> _cachedAttachmentsByType;
		mutable std::unordered_map<TAttachmentBase *, AttachmentCache> _typeidsPerAttachment;

		void _cachePositiveHit(
			TAttachmentBase *attachment,
			const std::type_index &typeIndex) const
		{
			_cachedAttachmentsByType[typeIndex].insert(attachment);
			_typeidsPerAttachment[attachment].positiveHits.insert(typeIndex);
		}

		void _cacheNegativeHit(
			TAttachmentBase *attachment,
			const std::type_index &typeIndex) const
		{
			_typeidsPerAttachment[attachment].negativeHits.insert(typeIndex);
		}

		[[nodiscard]] bool _wasParsedAs(
			TAttachmentBase *attachment,
			const std::type_index &typeIndex) const
		{
			const AttachmentCache &cache = _typeidsPerAttachment.at(attachment);

			return cache.positiveHits.contains(typeIndex) ||
				   cache.negativeHits.contains(typeIndex);
		}

		void _removeFromTypeCache(
			TAttachmentBase *attachment,
			const std::type_index &typeIndex)
		{
			auto it = _cachedAttachmentsByType.find(typeIndex);

			if (it == _cachedAttachmentsByType.end())
			{
				return;
			}

			it->second.erase(attachment);

			if (it->second.empty())
			{
				_cachedAttachmentsByType.erase(it);
			}
		}

		void _removeFromTypeCaches(TAttachmentBase *attachment)
		{
			auto it = _typeidsPerAttachment.find(attachment);

			if (it == _typeidsPerAttachment.end())
			{
				return;
			}

			for (const std::type_index &typeIndex : it->second.positiveHits)
			{
				_removeFromTypeCache(attachment, typeIndex);
			}

			_typeidsPerAttachment.erase(it);
		}

		void _removeFromAttachments(TAttachmentBase *attachment)
		{
			std::erase_if(
				_attachments,
				[attachment](const std::unique_ptr<TAttachmentBase> &candidate) {
					return candidate.get() == attachment;
				});
		}

		template <typename TAttachmentType, typename TPredicate>
		[[nodiscard]] TAttachmentType *_findCachedAttachment(
			const std::type_index &typeIndex,
			const TPredicate &predicate) const
		{
			auto it = _cachedAttachmentsByType.find(typeIndex);

			if (it == _cachedAttachmentsByType.end())
			{
				return nullptr;
			}

			for (TAttachmentBase *attachment : it->second)
			{
				auto *castedAttachment = static_cast<TAttachmentType *>(attachment);

				if (predicate(castedAttachment))
				{
					return castedAttachment;
				}
			}

			return nullptr;
		}

		template <typename TAttachmentType>
		[[nodiscard]] TAttachmentType *_parseAttachment(
			TAttachmentBase *attachment,
			const std::type_index &typeIndex) const
		{
			if (_wasParsedAs(attachment, typeIndex))
			{
				return nullptr;
			}

			auto *castedAttachment = dynamic_cast<TAttachmentType *>(attachment);

			if (castedAttachment == nullptr)
			{
				_cacheNegativeHit(attachment, typeIndex);
			}
			else
			{
				_cachePositiveHit(attachment, typeIndex);
			}

			return castedAttachment;
		}

		template <typename TAttachmentType, typename TPredicate>
		[[nodiscard]] TAttachmentType *_findUncachedAttachment(
			const std::type_index &typeIndex,
			const TPredicate &predicate) const
		{
			for (const auto &attachment : _attachments)
			{
				TAttachmentType *result = _parseAttachment<TAttachmentType>(
					attachment.get(),
					typeIndex);

				if (result != nullptr && predicate(result))
				{
					return result;
				}
			}

			return nullptr;
		}

		template <typename TAttachmentType>
		void _parseAllAttachments(const std::type_index &typeIndex) const
		{
			for (const auto &attachment : _attachments)
			{
				(void)_parseAttachment<TAttachmentType>(attachment.get(), typeIndex);
			}
		}

		template <typename TAttachmentType, typename TPredicate>
		[[nodiscard]] std::vector<TAttachmentType *> _findCachedAttachments(
			const std::type_index &typeIndex,
			const TPredicate &predicate) const
		{
			std::vector<TAttachmentType *> result;
			auto it = _cachedAttachmentsByType.find(typeIndex);

			if (it == _cachedAttachmentsByType.end())
			{
				return result;
			}

			result.reserve(it->second.size());

			for (TAttachmentBase *attachment : it->second)
			{
				auto *castedAttachment = static_cast<TAttachmentType *>(attachment);

				if (predicate(castedAttachment))
				{
					result.push_back(castedAttachment);
				}
			}

			return result;
		}

		template <typename TAttachmentType, typename TPredicate>
		[[nodiscard]] std::vector<TAttachmentType *> _getAttachments(
			const TPredicate &predicate) const
		{
			const std::type_index typeIndex = typeid(TAttachmentType);
			_parseAllAttachments<TAttachmentType>(typeIndex);

			return _findCachedAttachments<TAttachmentType>(typeIndex, predicate);
		}

		template <typename TAttachmentType>
		[[nodiscard]] static std::vector<const TAttachmentType *> _makeConst(
			const std::vector<TAttachmentType *> &attachments)
		{
			std::vector<const TAttachmentType *> result;
			result.reserve(attachments.size());

			for (TAttachmentType *attachment : attachments)
			{
				result.push_back(attachment);
			}

			return result;
		}

	protected:
		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		void registerAttachment(std::unique_ptr<TAttachmentType> &&attachment)
		{
			TAttachmentBase *attachmentPtr = attachment.get();

			_typeidsPerAttachment.emplace(attachmentPtr, AttachmentCache{});
			_cachePositiveHit(attachmentPtr, typeid(TAttachmentType));
			_attachments.push_back(std::move(attachment));
		}

		void unregisterAttachment(TAttachmentBase &attachmentToRemove)
		{
			TAttachmentBase *attachment = &attachmentToRemove;

			_removeFromTypeCaches(attachment);
			_removeFromAttachments(attachment);
		}

		template <typename TAttachmentType, typename TPredicate>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] TAttachmentType *getAttachment(const TPredicate &predicate)
		{
			const std::type_index typeIndex = typeid(TAttachmentType);

			if (auto *result = _findCachedAttachment<TAttachmentType>(typeIndex, predicate))
			{
				return result;
			}

			return _findUncachedAttachment<TAttachmentType>(typeIndex, predicate);
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] TAttachmentType *getAttachment()
		{
			return getAttachment<TAttachmentType>(
				[](TAttachmentType *) {
					return true;
				});
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] TAttachmentType *getAttachment(const std::regex &regexExpression)
		{
			return getAttachment<TAttachmentType>(
				[&regexExpression](TAttachmentType *attachment) {
					return std::regex_search(attachment->name(), regexExpression);
				});
		}

		template <typename TAttachmentType, typename TPredicate>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] const TAttachmentType *getAttachment(const TPredicate &predicate) const
		{
			const std::type_index typeIndex = typeid(TAttachmentType);

			if (auto *result = _findCachedAttachment<TAttachmentType>(typeIndex, predicate))
			{
				return result;
			}

			return _findUncachedAttachment<TAttachmentType>(typeIndex, predicate);
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] const TAttachmentType *getAttachment() const
		{
			return getAttachment<TAttachmentType>(
				[](TAttachmentType *) {
					return true;
				});
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] const TAttachmentType *getAttachment(const std::regex &regexExpression) const
		{
			return getAttachment<TAttachmentType>(
				[&regexExpression](TAttachmentType *attachment) {
					return std::regex_search(attachment->name(), regexExpression);
				});
		}

		template <typename TAttachmentType, typename TPredicate>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<TAttachmentType *> getAttachments(const TPredicate &predicate)
		{
			return _getAttachments<TAttachmentType>(predicate);
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<TAttachmentType *> getAttachments()
		{
			return getAttachments<TAttachmentType>(
				[](TAttachmentType *) {
					return true;
				});
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<TAttachmentType *> getAttachments(const std::regex &regexExpression)
		{
			return getAttachments<TAttachmentType>(
				[&regexExpression](TAttachmentType *attachment) {
					return std::regex_search(attachment->name(), regexExpression);
				});
		}

		template <typename TAttachmentType, typename TPredicate>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<const TAttachmentType *> getAttachments(const TPredicate &predicate) const
		{
			auto attachments = _getAttachments<TAttachmentType>(
				[&predicate](TAttachmentType *attachment) {
					return predicate(static_cast<const TAttachmentType *>(attachment));
				});

			return _makeConst(attachments);
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<const TAttachmentType *> getAttachments() const
		{
			return getAttachments<TAttachmentType>(
				[](const TAttachmentType *) {
					return true;
				});
		}

		template <typename TAttachmentType>
			requires std::derived_from<TAttachmentType, TAttachmentBase>
		[[nodiscard]] std::vector<const TAttachmentType *> getAttachments(const std::regex &regexExpression) const
		{
			return getAttachments<TAttachmentType>(
				[&regexExpression](const TAttachmentType *attachment) {
					return std::regex_search(attachment->name(), regexExpression);
				});
		}
	};
}
