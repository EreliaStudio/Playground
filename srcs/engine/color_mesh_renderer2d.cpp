#include "engine/color_mesh_renderer2d.hpp"

#include <utility>

#include "engine/engine.hpp"
#include "engine/entity2d.hpp"
#include "engine/transform2d.hpp"
#include "rendering/command/color_mesh_render_command.hpp"

namespace spk
{
	ColorMeshRenderer2D::ColorMeshRenderer2D(const std::string &name, Entity2D *owner) :
		System::Participant2D(name, owner)
	{
	}

	ColorMeshRenderer2D::ColorMeshRenderer2D(Entity2D *owner) :
		ColorMeshRenderer2D("Unnamed color mesh renderer 2D", owner)
	{
	}

	void ColorMeshRenderer2D::setMesh(ColorMesh2D mesh)
	{
		_mesh = std::move(mesh);
	}

	const ColorMesh2D &ColorMeshRenderer2D::mesh() const noexcept
	{
		return _mesh;
	}

	void ColorMeshRenderer2D::_buildRenderSnapshot(RenderSnapshot::Builder &builder)
	{
		if (_mesh.empty() || owner() == nullptr)
		{
			return;
		}

		builder.renderPass(Engine::SceneRenderPassKey)
			.emplace<ColorMeshRenderCommand>(_mesh, owner()->transform().modelMatrix());
	}
}
