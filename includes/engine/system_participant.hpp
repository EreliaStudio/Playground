#pragma once

#include "engine/entity_attachment.hpp"
#include "engine/system.hpp"
#include "engine/registry.hpp"

#include <string>

namespace spk
{
	class Entity;

	class System::Participant : public EntityAttachment,
		public spk::Registry<Engine*, System::Participant>::Object
	{
	public:
		Participant(
			const std::string &name = "Unnamed participant",
			Entity *owner = nullptr);
		explicit Participant(Entity *owner);

		~Participant() override = default;
	};
}
