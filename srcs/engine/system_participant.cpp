#include "engine/system_participant.hpp"

namespace spk
{
	System::Participant::Participant(const std::string &name, Entity *owner) :
		EntityAttachment(name, owner)
	{
	}

	System::Participant::Participant(Entity *owner) :
		Participant("Unnamed participant", owner)
	{
	}
}
