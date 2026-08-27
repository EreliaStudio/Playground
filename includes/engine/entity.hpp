#pragma once

#include "engine/component.hpp"
#include "engine/registry.hpp"

#include <sparkle.hpp>

#include <concepts>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace spk
{
	class Engine;

	class Entity : public ContextualizableTrait<Engine *>,
				   public Registry<Engine *, Entity>::Object,
				   public InherenceTrait<Entity>,
				   public ActivableTrait,
				   public NameTrait
	{
	private:
		std::vector<std::unique_ptr<Component>> _components;

	public:
		Entity(const std::string &name, Entity *parent = nullptr);

		template <typename TComponentType, typename... TArgs>
			requires std::derived_from<TComponentType, Component>
		TComponentType &addComponent(TArgs &&...args)
		{
			std::unique_ptr<TComponentType> newComponent = std::make_unique<TComponentType>(std::forward<TArgs>(args)...);

			TComponentType &result = *newComponent;
			result.attach(this);
			_components.push_back(std::move(newComponent));

			return result;
		}

		template <typename TComponentType>
			requires std::derived_from<TComponentType, Component>
		[[nodiscard]] TComponentType *getComponent(const std::string &regex = ".*")
		{
			const std::regex expression(regex);

			for (auto &component : _components)
			{
				TComponentType *castedComponent = dynamic_cast<TComponentType *>(component.get());

				if (castedComponent != nullptr && std::regex_search(component->name(), expression))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}

		template <typename TComponentType>
			requires std::derived_from<TComponentType, Component>
		[[nodiscard]] const TComponentType *getComponent(const std::string &regex) const
		{
			const std::regex expression(regex);

			for (const auto &component : _components)
			{
				const TComponentType *castedComponent = dynamic_cast<const TComponentType *>(component.get());

				if (castedComponent != nullptr && std::regex_search(component->name(), expression))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}

		template <typename TComponentType>
			requires std::derived_from<TComponentType, Component>
		[[nodiscard]] std::vector<TComponentType *> getComponents(const std::string &regex)
		{
			std::vector<TComponentType *> result;
			const std::regex expression(regex);

			for (auto &component : _components)
			{
				TComponentType *castedComponent = dynamic_cast<TComponentType *>(component.get());

				if (castedComponent != nullptr && std::regex_search(component->name(), expression))
				{
					result.push_back(castedComponent);
				}
			}

			return result;
		}
	};
}
