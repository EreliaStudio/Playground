#pragma once

#include <cstddef>

#include "geometry/color_mesh_2d.hpp"
#include "math/matrix.hpp"
#include "rendering/render_command.hpp"

namespace spk
{
	class Program;
	class UniformBuffer;

	class ColorMeshRenderCommand final : public RenderCommand
	{
	private:
		static constexpr std::size_t ModelMatrixBindingPoint = 2;

		ColorMesh2D _mesh;
		Matrix4x4 _modelMatrix;

		[[nodiscard]] static Program &_sharedProgram();
		[[nodiscard]] static UniformBuffer &_sharedModelBuffer();

	public:
		ColorMeshRenderCommand(ColorMesh2D mesh, const Matrix4x4 &modelMatrix);

		void execute(RenderContext &renderContext) const override;
	};
}
