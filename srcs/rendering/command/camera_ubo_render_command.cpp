#include "rendering/command/camera_ubo_render_command.hpp"

#include "graphics/opengl/uniform_buffer.hpp"

namespace spk
{
	UniformBuffer &CameraUBORenderCommand::_sharedBuffer()
	{
		static UniformBuffer buffer(MatrixUBOBindingPoint, sizeof(Matrix4x4));
		return buffer;
	}

	CameraUBORenderCommand::CameraUBORenderCommand(const Matrix4x4 &cameraMatrix) :
		_cameraMatrix(cameraMatrix)
	{
	}

	void CameraUBORenderCommand::activate(RenderContext &renderContext)
	{
		_sharedBuffer().activate(renderContext);
	}

	void CameraUBORenderCommand::execute(RenderContext &renderContext) const
	{
		UniformBuffer &buffer = _sharedBuffer();
		buffer.setData(_cameraMatrix);
		buffer.validate();
		buffer.activate(renderContext);
	}
}
