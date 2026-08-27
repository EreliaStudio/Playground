#pragma once

#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "concept.hpp"

namespace spk
{
	template <typename TRootType>
		requires Hashable<TRootType>
	class Selection
	{
	public:
		using Root = TRootType;
		using Results = std::unordered_set<Root>;

	private:
		Results _results;

	public:
		Selection() = default;

		template <typename TContainer>
			requires std::ranges::input_range<TContainer> &&
					std::convertible_to<
						std::ranges::range_reference_t<TContainer>,
						Root>
		explicit Selection(const TContainer &inputs)
		{
			if constexpr (std::ranges::sized_range<TContainer>)
			{
				_results.reserve(std::ranges::size(inputs));
			}

			for (auto &&root : inputs)
			{
				_results.emplace(static_cast<Root>(root));
			}
		}

		[[nodiscard]] const Results &results() const
		{
			return _results;
		}
	};

	template <typename TRootType, typename TAdditionalData>
		requires Hashable<TRootType>
	class Query
	{
	public:
		using Root = TRootType;
		using Data = TAdditionalData;
		using Results = std::unordered_map<Root, Data>;

		enum class ObtainMode
		{
			Mandatory,
			Optional
		};

	private:
		Results _results;

		template <typename TValue>
		[[nodiscard]] static bool _hasValue(const TValue &value)
		{
			if constexpr (std::is_pointer_v<TValue>)
			{
				return value != nullptr;
			}
			else if constexpr (requires { value.empty(); })
			{
				return value.empty() == false;
			}
			else
			{
				return static_cast<bool>(value);
			}
		}

	public:
		explicit Query(const Selection<Root> &selection)
		{
			_results.reserve(selection.results().size());

			for (Root root : selection.results())
			{
				_results.try_emplace(root);
			}
		}

		template <typename TPredicate>
			requires std::predicate<TPredicate &, Root, Data &>
		Query &where(TPredicate &&predicate)
		{
			auto it = _results.begin();

			while (it != _results.end())
			{
				auto &[root, data] = *it;

				if (std::invoke(predicate, root, data) == false)
				{
					it = _results.erase(it);
				}
				else
				{
					++it;
				}
			}

			return *this;
		}

		template <typename TMember, typename TProvider>
			requires std::invocable<TProvider &, Root> &&
					std::assignable_from<
						TMember &,
						std::invoke_result_t<TProvider &, Root>>
		Query &obtain(
			TMember Data::*member,
			TProvider &&provider,
			ObtainMode mode = ObtainMode::Mandatory)
		{
			auto it = _results.begin();

			while (it != _results.end())
			{
				auto &[root, data] = *it;
				auto value = std::invoke(provider, root);

				if (_hasValue(value) == false &&
					mode == ObtainMode::Mandatory)
				{
					it = _results.erase(it);
					continue;
				}

				data.*member = std::move(value);
				++it;
			}

			return *this;
		}

		template <typename TProvider>
			requires std::invocable<TProvider &, Root>
		Query &obtain(
			TProvider &&provider,
			ObtainMode mode = ObtainMode::Mandatory)
		{
			auto it = _results.begin();

			while (it != _results.end())
			{
				auto &[root, data] = *it;
				auto value = std::invoke(provider, root);

				if (_hasValue(value) == false &&
					mode == ObtainMode::Mandatory)
				{
					it = _results.erase(it);
					continue;
				}

				++it;
			}

			return *this;
		}

		template <typename TOtherRoot>
			requires std::convertible_to<TOtherRoot, Root>
		Query &unionWith(const Selection<TOtherRoot> &selection)
		{
			for (TOtherRoot root : selection.results())
			{
				_results.try_emplace(static_cast<Root>(root));
			}

			return *this;
		}

		template <typename TOtherRoot>
			requires std::convertible_to<TOtherRoot, Root>
		Query &intersectWith(const Selection<TOtherRoot> &selection)
		{
			std::unordered_set<Root> selectedRoots;
			selectedRoots.reserve(selection.results().size());

			for (TOtherRoot root : selection.results())
			{
				selectedRoots.emplace(static_cast<Root>(root));
			}

			std::erase_if(
				_results,
				[&](const auto &result) {
					return selectedRoots.contains(result.first) == false;
				});

			return *this;
		}

		[[nodiscard]] const Results &results() const
		{
			return _results;
		}
	};
}