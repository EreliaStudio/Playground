#pragma once

#include <sparkle.hpp>

#include <concepts>
#include <functional>
#include <utility>

namespace spk
{
	template <typename T>
	concept Hashable =
		std::equality_comparable<T> &&
		requires(const T &value) {
			{ std::hash<T>{}(value) } -> std::convertible_to<std::size_t>;
		};

	template <typename TContext>
		requires Hashable<TContext>
	class ContextualizableTrait
	{
	public:
		using OnContextEditionContractProvider = ContractProvider<const TContext &, const TContext &>;
		using OnContextEditionContract = typename OnContextEditionContractProvider::Contract;
		using OnContextEditionCallback = typename OnContextEditionContractProvider::callback_type;

	private:
		TContext _context;
		OnContextEditionContractProvider _onContextEditionContractProvider;

	public:
		explicit ContextualizableTrait(const TContext &context = {}) :
			_context(context)
		{
		}

		[[nodiscard]] OnContextEditionContract subscribeToContextEdition(OnContextEditionCallback callback)
		{
			return _onContextEditionContractProvider.subscribe(std::move(callback));
		}

		void changeContext(const TContext &newContext)
		{
			if (_context == newContext)
			{
				return;
			}

			_onContextEditionContractProvider.trigger(_context, newContext);
			_context = newContext;
		}

		[[nodiscard]] const TContext &context() const
		{
			return _context;
		}
	};
}
