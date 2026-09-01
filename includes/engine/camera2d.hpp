#pragma once

#include <string>

#include "engine/registry.hpp"
#include "engine/system_participant2d.hpp"
#include "math/matrix.hpp"

namespace spk
{
	class Camera2D : public System::Participant2D,
					 public Registry<Engine *, Camera2D>::Object
	{
	private:
		void _buildRenderSnapshot(RenderSnapshot::Builder &builder) override;

	public:
		Camera2D(
			const std::string &name = "Unnamed camera 2D",
			Entity2D *owner = nullptr);
		explicit Camera2D(Entity2D *owner);

		[[nodiscard]] Matrix4x4 viewMatrix() const;
		[[nodiscard]] static Camera2D *activeCamera(Engine *engine);
		[[nodiscard]] static const Camera2D *activeCamera(const Engine *engine);
	};
}
