#pragma once

#include <cstddef>

#include "geometry/texture_mesh_2d.hpp"
#include "graphics/opengl/sampler.hpp"
#include "math/matrix.hpp"
#include "rendering/render_command.hpp"

namespace spk
{
	class Program;
	class SpriteSheet;
	class UniformBuffer;

	class SpriteMeshRenderCommand final : public RenderCommand
	{
	private:
		static constexpr std::size_t ModelMatrixBindingPoint = 2;
		static constexpr std::size_t TextureSamplerBindingPoint = 0;

		const SpriteSheet *_spriteSheet;
		TextureMesh2D _mesh;
		Matrix4x4 _modelMatrix;
		Sampler _sampler;

		[[nodiscard]] static Program &_sharedProgram();
		[[nodiscard]] static UniformBuffer &_sharedModelBuffer();

	public:
		SpriteMeshRenderCommand(
			const SpriteSheet *spriteSheet,
			TextureMesh2D mesh,
			const Matrix4x4 &modelMatrix);

		void execute(RenderContext &renderContext) const override;
	};
}
