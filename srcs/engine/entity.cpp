#include "engine/entity.hpp"

#include "core/context/update_context.hpp"

namespace spk
{
	Entity::Entity(const std::string &name, Entity *parent) :
		NameTrait(name)
	{
		activate();
		setParent(parent);
	}

	bool Entity::_isAcceptingInteraction() const
	{
		return isActive();
	}

	void Entity::_propagateInteraction(
		const std::function<void(EventDispatcher *)> &callback)
	{
		for (Entity *child : children())
		{
			if (child != nullptr)
			{
				callback(child);
			}
		}

		for (const auto &behaviour : behaviours())
		{
			if (behaviour != nullptr)
			{
				callback(behaviour.get());
			}
		}
	}

	void Entity::updateState(UpdateContext &context)
	{
		if (!isActive())
		{
			return;
		}

		for (const auto &behaviour : behaviours())
		{
			if (behaviour != nullptr)
			{
				behaviour->updateState(context);
			}
		}

		for (Entity *child : children())
		{
			if (child != nullptr)
			{
				child->updateState(context);
			}
		}
	}
}
