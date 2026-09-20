#include "rendering/command/voxel_render_command.hpp"

#include <memory>
#include <string>
#include <utility>

#include <exception.hpp>

#include "rendering/command/camera_ubo_render_command.hpp"

namespace voxel
{
	Palette VoxelRenderCommand::_checkedPalette(const Palette *palette, const VoxelMesh &mesh)
	{
		if (palette == nullptr)
			throw spk::Exception("Voxel render command requires a Palette");
		if (palette->empty())
			throw spk::Exception("Voxel render command Palette cannot be empty");
		if (mesh.empty()) return *palette;
		const auto vertices = mesh.layout().vertexBuffer().cast<VoxelVertex>();
		for (const VoxelVertex &vertex : vertices)
		{
			if (!palette->contains(vertex.paletteElementIndex))
				throw spk::Exception("Voxel mesh palette element index " +
					std::to_string(vertex.paletteElementIndex) + " is outside Palette size " +
					std::to_string(palette->size()));
		}
		return *palette;
	}

	spk::Program &VoxelRenderCommand::_sharedProgram()
	{
		static auto program = []() {
			auto result = std::make_unique<spk::Program>(
				spk::readTextFile("resources/shaders/voxel_mesh/vertex.glsl"),
				spk::readTextFile("resources/shaders/voxel_mesh/fragment.glsl"));
			result->bindUniformBlock("CameraData", spk::CameraUBORenderCommand::MatrixUBOBindingPoint);
			result->bindUniformBlock("Model3DData", ModelBindingPoint);
			result->validate();
			return result;
		}();
		return *program;
	}

	spk::UniformBuffer &VoxelRenderCommand::_sharedModelBuffer()
	{
		static spk::UniformBuffer buffer(ModelBindingPoint, sizeof(spk::Matrix4x4));
		return buffer;
	}

	VoxelRenderCommand::VoxelRenderCommand(
		const Palette *palette, VoxelMesh mesh, spk::Matrix4x4 modelMatrix) :
		_mesh(std::move(mesh)),
		_palette(_checkedPalette(palette, _mesh)),
		_modelMatrix(modelMatrix)
	{
	}

	void VoxelRenderCommand::execute(spk::RenderContext &context) const
	{
		if (_mesh.empty()) return;
		auto &program = _sharedProgram();
		program.activate(context);
		_mesh.layout().activate(context);
		_palette.activate(context);
		spk::CameraUBORenderCommand::activate(context);
		auto &model = _sharedModelBuffer();
		model.setData(_modelMatrix);
		model.validate();
		model.activate(context);
		program.render(spk::Program::Primitive::Triangles, _mesh.indexType(), 0, _mesh.indexCount());
	}
}
