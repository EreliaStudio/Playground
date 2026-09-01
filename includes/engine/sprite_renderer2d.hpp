#pragma once

#include <string>

#include "engine/system_participant2d.hpp"
#include "geometry/texture_mesh_2d.hpp"

namespace spk
{
	class SpriteSheet;

	class SpriteRenderer2D : public System::Participant2D
	{
	private:
		const SpriteSheet *_spriteSheet = nullptr;
		TextureMesh2D _mesh;

		void _buildRenderSnapshot(RenderSnapshot::Builder &builder) override;

	public:
		SpriteRenderer2D(
			const std::string &name = "Unnamed sprite renderer 2D",
			Entity2D *owner = nullptr);
		explicit SpriteRenderer2D(Entity2D *owner);

		void setSpriteSheet(const SpriteSheet *spriteSheet) noexcept;
		[[nodiscard]] const SpriteSheet *spriteSheet() const noexcept;

		void setMesh(TextureMesh2D mesh);
		[[nodiscard]] const TextureMesh2D &mesh() const noexcept;
	};
}
