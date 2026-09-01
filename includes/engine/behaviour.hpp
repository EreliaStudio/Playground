#pragma once

#include "core/event/event_dispatcher.hpp"
#include "engine/entity_attachment.hpp"

#include <string>

namespace spk
{
	struct UpdateContext;
	class Entity;

	class Behaviour : public EntityAttachment,
					  public EventDispatcher
	{
	protected:
		[[nodiscard]] bool _isAcceptingInteraction() const override;

		virtual void _updateState(UpdateContext &context);

	public:
		Behaviour(
			const std::string &name = "Unnamed behaviour",
			Entity *owner = nullptr);
		explicit Behaviour(Entity *owner);

		~Behaviour() override = default;

		void updateState(UpdateContext &context);
	};
}
