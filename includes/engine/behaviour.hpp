#pragma once

#include "engine/entity_attachment.hpp"

#include <string>

namespace spk
{
	class Entity;

	class Behaviour : public EntityAttachment
	{
	public:
		Behaviour(
			const std::string &name = "Unnamed behaviour",
			Entity *owner = nullptr);
		explicit Behaviour(Entity *owner);

		~Behaviour() override = default;
	};
}
