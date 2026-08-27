#pragma once

#include "engine/contextualizable_trait.hpp"

#include <concepts>
#include <functional>
#include <set>
#include <unordered_map>
#include <utility>

namespace spk
{
	template <typename TContext, typename TType>
		requires Hashable<TContext>
	class Registry
	{
	public:
		using ElementSet = std::set<TType *>;

		class Object
		{
		private:
			using ContextualType = ContextualizableTrait<TContext>;
			using ContextEditionContract = typename ContextualType::OnContextEditionContract;

			ContextEditionContract _contextEditionContract;

		protected:
			Object()
			{
				static_assert(std::derived_from<TType, ContextualType>);

				TType *object = static_cast<TType *>(this);
				ContextualType *contextual = static_cast<ContextualType *>(object);

				Registry::add(contextual->context(), object);

				_contextEditionContract = contextual->subscribeToContextEdition(
					[object](const TContext &oldContext, const TContext &newContext) {
						Registry::remove(oldContext, object);
						Registry::add(newContext, object);
					});
			}

			~Object()
			{
				TType *object = static_cast<TType *>(this);
				ContextualType *contextual = static_cast<ContextualType *>(object);

				Registry::remove(contextual->context(), object);
			}

			Object(const Object &) = delete;
			Object &operator=(const Object &) = delete;

			Object(Object &&) = delete;
			Object &operator=(Object &&) = delete;
		};
		
	private:
		static inline std::unordered_map<TContext, ElementSet> _elementsByContext;

		static bool add(const TContext &context, TType *element)
		{
			return _elementsByContext[context].insert(element).second;
		}

		static bool remove(const TContext &context, TType *element)
		{
			auto it = _elementsByContext.find(context);

			if (it == _elementsByContext.end())
			{
				return false;
			}

			const bool result = it->second.erase(element) != 0;

			if (it->second.empty())
			{
				_elementsByContext.erase(it);
			}

			return result;
		}

	public:
		[[nodiscard]] static const ElementSet &elements(const TContext &context)
		{
			static const ElementSet empty;

			auto it = _elementsByContext.find(context);

			if (it == _elementsByContext.end())
			{
				return empty;
			}

			return it->second;
		}
	};
}
