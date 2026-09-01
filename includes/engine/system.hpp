#pragma once

#include "engine/contextualizable_trait.hpp"

#include <sparkle.hpp>

#include <string>

namespace spk
{
	class Engine;

	class System : public ContextualizableTrait<Engine *>,
				   public ActivableTrait,
				   public NameTrait
	{
	public:
		class Participant;
		class Participant2D;
		class Participant3D;

		System(
			const std::string &name = "Unnamed system",
			Engine *engine = nullptr);
		explicit System(Engine *engine);

		virtual ~System() = default;

		virtual void attach(Engine *engine);

		[[nodiscard]] Engine *engine();
		[[nodiscard]] const Engine *engine() const;
	};
}
