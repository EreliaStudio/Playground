#include "rendering/command/sprite_mesh_render_command.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

#include "graphics/opengl/program.hpp"
#include "graphics/opengl/uniform_buffer.hpp"
#include "graphics/sprite_sheet.hpp"
#include "rendering/command/camera_ubo_render_command.hpp"
#include "rendering/command/viewport_uniform_render_command.hpp"

namespace spk
{
	Program &SpriteMeshRenderCommand::_sharedProgram()
	{
		static auto program = []() {
			auto result = std::make_unique<Program>(
				R"glsl(#version 460 core
layout(location = 0) in vec2 inPosition;
layout(location = 1) in float inDepth;
layout(location = 2) in vec2 inUV;
layout(std140) uniform ViewportData { mat4 uProjection; };
layout(std140) uniform CameraData { mat4 uCamera; };
layout(std140) uniform ModelData { mat4 uModel; };
layout(location = 0) out vec2 vertexUV;
void main()
{
	gl_Position = uProjection * uCamera * uModel * vec4(inPosition, inDepth, 1.0);
	vertexUV = inUV;
}
)glsl",
				R"glsl(#version 460 core
uniform sampler2D uTexture;
layout(location = 0) in vec2 vertexUV;
layout(location = 0) out vec4 outColor;
void main()
{
	outColor = texture(uTexture, vertexUV);
	if (outColor.a <= 0.0) { discard; }
}
)glsl");
			result->bindUniformBlock("ViewportData", ViewportUniformRenderCommand::MatrixUBOBindingPoint);
			result->bindUniformBlock("CameraData", CameraUBORenderCommand::MatrixUBOBindingPoint);
			result->bindUniformBlock("ModelData", ModelMatrixBindingPoint);
			result->bindSampler("uTexture", TextureSamplerBindingPoint);
			result->validate();
			return result;
		}();
		return *program;
	}

	UniformBuffer &SpriteMeshRenderCommand::_sharedModelBuffer()
	{
		static UniformBuffer buffer(ModelMatrixBindingPoint, sizeof(Matrix4x4));
		return buffer;
	}

	SpriteMeshRenderCommand::SpriteMeshRenderCommand(
		const SpriteSheet *spriteSheet,
		TextureMesh2D mesh,
		const Matrix4x4 &modelMatrix) :
		_spriteSheet(spriteSheet),
		_mesh(std::move(mesh)),
		_modelMatrix(modelMatrix),
		_sampler(TextureSamplerBindingPoint)
	{
		if (_spriteSheet == nullptr)
		{
			throw std::invalid_argument("SpriteMeshRenderCommand sprite sheet cannot be null");
		}
		_sampler.setFiltering(spk::Sampler::Filtering::Nearest);
		_sampler.setTexture(_spriteSheet);
		_sampler.validate();
	}

	void SpriteMeshRenderCommand::execute(RenderContext &renderContext) const
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
		_sampler.activate(renderContext);
		_mesh.layout().activate(renderContext);
		program.render(Program::Primitive::Triangles, _mesh.indexType(), 0, _mesh.indexCount());
	}
}
