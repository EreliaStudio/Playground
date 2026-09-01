#include "engine/sprite_renderer2d.hpp"

#include <utility>

#include "engine/engine.hpp"
#include "engine/entity2d.hpp"
#include "engine/transform2d.hpp"
#include "graphics/sprite_sheet.hpp"
#include "rendering/command/sprite_mesh_render_command.hpp"

namespace spk
{
	SpriteRenderer2D::SpriteRenderer2D(const std::string &name, Entity2D *owner) :
		System::Participant2D(name, owner)
	{
	}

	SpriteRenderer2D::SpriteRenderer2D(Entity2D *owner) :
		SpriteRenderer2D("Unnamed sprite renderer 2D", owner)
	{
	}

	void SpriteRenderer2D::setSpriteSheet(const SpriteSheet *spriteSheet) noexcept
	{
		_spriteSheet = spriteSheet;
	}

	const SpriteSheet *SpriteRenderer2D::spriteSheet() const noexcept
	{
		return _spriteSheet;
	}

	void SpriteRenderer2D::setMesh(TextureMesh2D mesh)
	{
		_mesh = std::move(mesh);
	}

	const TextureMesh2D &SpriteRenderer2D::mesh() const noexcept
	{
		return _mesh;
	}

	void SpriteRenderer2D::_buildRenderSnapshot(RenderSnapshot::Builder &builder)
	{
		if (_spriteSheet == nullptr || _mesh.empty() || owner() == nullptr)
		{
			return;
		}

		builder.renderPass(Engine::SceneRenderPassKey)
			.emplace<SpriteMeshRenderCommand>(_spriteSheet, _mesh, owner()->transform().modelMatrix());
	}
}
