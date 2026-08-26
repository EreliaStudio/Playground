#pragma once

#include "engine/contextualizable_trait.hpp"

#include <sparkle.hpp>

#include <string>

namespace spk
{
	class Engine;
	class Entity;

	class Component : public ContextualizableTrait<Engine *>,
					  public ActivableTrait,
					  public NameTrait
	{
	private:
		using OwnerContextualizableTrait = ContextualizableTrait<Engine *>;

		Entity *_owner = nullptr;
		OwnerContextualizableTrait::OnContextEditionContract _onOwnerContextEditionContract;

	public:
		Component(const std::string &name = "Unnamed component", Entity *owner = nullptr);
		explicit Component(Entity *owner);

		virtual ~Component() = default;

		virtual void attach(Entity *owner);

		[[nodiscard]] virtual Entity *owner();
		[[nodiscard]] virtual const Entity *owner() const;
	};
}
