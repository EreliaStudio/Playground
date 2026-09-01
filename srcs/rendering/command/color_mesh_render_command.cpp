#include "rendering/command/color_mesh_render_command.hpp"

#include <memory>
#include <utility>

#include "graphics/opengl/program.hpp"
#include "graphics/opengl/uniform_buffer.hpp"
#include "rendering/command/camera_ubo_render_command.hpp"
#include "rendering/command/viewport_uniform_render_command.hpp"

namespace spk
{
	Program &ColorMeshRenderCommand::_sharedProgram()
	{
		static auto program = []() {
			auto result = std::make_unique<Program>(
				R"glsl(#version 460 core
layout(location = 0) in vec2 inPosition;
layout(location = 1) in float inDepth;
layout(location = 2) in vec4 inColor;
layout(std140) uniform ViewportData { mat4 uProjection; };
layout(std140) uniform CameraData { mat4 uCamera; };
layout(std140) uniform ModelData { mat4 uModel; };
layout(location = 0) out vec4 vertexColor;
void main()
{
	gl_Position = uProjection * uCamera * uModel * vec4(inPosition, inDepth, 1.0);
	vertexColor = inColor;
}
)glsl",
				R"glsl(#version 460 core
layout(location = 0) in vec4 vertexColor;
layout(location = 0) out vec4 outColor;
void main()
{
	outColor = vertexColor;
}
)glsl");
			result->bindUniformBlock("ViewportData", ViewportUniformRenderCommand::MatrixUBOBindingPoint);
			result->bindUniformBlock("CameraData", CameraUBORenderCommand::MatrixUBOBindingPoint);
			result->bindUniformBlock("ModelData", ModelMatrixBindingPoint);
			result->validate();
			return result;
		}();
		return *program;
	}

	UniformBuffer &ColorMeshRenderCommand::_sharedModelBuffer()
	{
		static UniformBuffer buffer(ModelMatrixBindingPoint, sizeof(Matrix4x4));
		return buffer;
	}

	ColorMeshRenderCommand::ColorMeshRenderCommand(ColorMesh2D mesh, const Matrix4x4 &modelMatrix) :
		_mesh(std::move(mesh)),
		_modelMatrix(modelMatrix)
	{
	}

	void ColorMeshRenderCommand::execute(RenderContext &renderContext) const
	{
		if (_mesh.empty())
		{
			return;
		}

		UniformBuffer &modelBuffer = _sharedModelBuffer();
		modelBuffer.setData(_modelMatrix);
		modelBuffer.validate();

		Program &program = _sharedProgram();
		program.activate(renderContext);
		CameraUBORenderCommand::activate(renderContext);
		modelBuffer.activate(renderContext);
		_mesh.layout().activate(renderContext);
		program.render(Program::Primitive::Triangles, _mesh.indexType(), 0, _mesh.indexCount());
	}
}
