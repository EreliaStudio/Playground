#include "engine/behaviour.hpp"

#include "core/context/update_context.hpp"

namespace spk
{
	Behaviour::Behaviour(const std::string &name, Entity *owner) :
		EntityAttachment(name, owner)
	{
	}

	Behaviour::Behaviour(Entity *owner) :
		Behaviour("Unnamed behaviour", owner)
	{
	}

	bool Behaviour::_isAcceptingInteraction() const
	{
		return isActive();
	}

	void Behaviour::_updateState(UpdateContext &)
	{
	}

	void Behaviour::updateState(UpdateContext &context)
	{
		if (!isActive())
		{
			return;
		}

		_updateState(context);
	}
}
