#pragma once

#include <cstddef>

#include "math/matrix.hpp"
#include "rendering/render_command.hpp"

namespace spk
{
	class UniformBuffer;

	class CameraUBORenderCommand final : public RenderCommand
	{
	private:
		Matrix4x4 _cameraMatrix;

		[[nodiscard]] static UniformBuffer &_sharedBuffer();

	public:
		static constexpr std::size_t MatrixUBOBindingPoint = 1;

		explicit CameraUBORenderCommand(const Matrix4x4 &cameraMatrix);

		static void activate(RenderContext &renderContext);
		void execute(RenderContext &renderContext) const override;
	};
}
