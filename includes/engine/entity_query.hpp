#pragma once

#include <concepts>
#include <ranges>
#include <vector>

#include "engine/registry_query.hpp"

namespace spk
{
	template <typename TEntityType>
		requires std::derived_from<TEntityType, spk::Entity>
	class EntitySelection : public RegistrySelection<spk::Entity::Context, TEntityType>
	{
	public:
		explicit EntitySelection(const TEntityType::Context &context) :
			RegistrySelection<spk::Entity::Context, TEntityType>(context)
		{
		}
	};

	template<typename TEntityType, typename TData>
		requires std::derived_from<TEntityType, spk::Entity>
	class EntityQuery :
		public RegistryQuery<spk::Entity::Context, TEntityType, TData>
	{
	private:
		using Base = RegistryQuery<
			spk::Entity::Context,
			TEntityType,
			TData>;

	public:
		using ObtainMode = typename Base::ObtainMode;

		explicit EntityQuery(const TEntityType::Context& context) :
			Base(context)
		{
		}

		template<typename TPredicate>
			requires std::predicate<TPredicate&, TEntityType*, TData&>
		EntityQuery& where(TPredicate&& predicate)
		{
			Base::where(std::forward<TPredicate>(predicate));
			return *this;
		}

		template<typename TMember, typename TProvider>
			requires std::invocable<TProvider&, TEntityType*> &&
					std::assignable_from<
						TMember&,
						std::invoke_result_t<TProvider&, TEntityType*>>
		EntityQuery& obtain(
			TMember TData::* member,
			TProvider&& provider,
			ObtainMode mode = ObtainMode::Mandatory)
		{
			Base::obtain(
				member,
				std::forward<TProvider>(provider),
				mode);

			return *this;
		}

		template<typename TProvider>
			requires std::invocable<TProvider&, TEntityType*>
		EntityQuery& obtain(
			TProvider&& provider,
			ObtainMode mode = ObtainMode::Mandatory)
		{
			Base::obtain(
				std::forward<TProvider>(provider),
				mode);

			return *this;
		}

		template<typename TOtherRoot>
			requires std::convertible_to<TOtherRoot, TEntityType*>
		EntityQuery& unionWith(const Selection<TOtherRoot>& selection)
		{
			Base::unionWith(selection);
			return *this;
		}

		template<typename TOtherRoot>
			requires std::convertible_to<TOtherRoot, TEntityType*>
		EntityQuery& intersectWith(const Selection<TOtherRoot>& selection)
		{
			Base::intersectWith(selection);
			return *this;
		}

		template<typename TComponentType>
		EntityQuery& withComponent(
			TComponentType* TData::* member,
			const std::string& regex = ".*",
			ObtainMode mode = ObtainMode::Mandatory)
		{
			this->obtain(
				member,
				[regex](TEntityType* entity)
				{
					return entity->template getComponent<TComponentType>(regex);
				},
				mode);

			return *this;
		}

		template<typename TComponentType>
		EntityQuery& withComponent(
			const std::string& regex = ".*",
			ObtainMode mode = ObtainMode::Mandatory)
		{
			this->obtain(
				[regex](TEntityType* entity)
				{
					return entity->template getComponent<TComponentType>(regex);
				},
				mode);

			return *this;
		}

		template<typename TComponentType, typename TContainer>
			requires std::ranges::input_range<TContainer> &&
					 std::same_as<
						 std::ranges::range_value_t<TContainer>,
						 TComponentType*> &&
					 std::constructible_from<
						 TContainer,
						 typename std::vector<TComponentType*>::const_iterator,
						 typename std::vector<TComponentType*>::const_iterator>
		EntityQuery& withComponents(
			TContainer TData::* member,
			const std::string& regex = ".*",
			ObtainMode mode = ObtainMode::Mandatory)
		{
			this->obtain(
				member,
				[regex](TEntityType* entity)
				{
					auto components = entity->template getComponents<TComponentType>(regex);
					return TContainer(components.cbegin(), components.cend());
				},
				mode);

			return *this;
		}
	};
}
