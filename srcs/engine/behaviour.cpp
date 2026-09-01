#include "engine/behaviour.hpp"

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
}
