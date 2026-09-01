#pragma once

#include <string>

#include "engine/system_participant2d.hpp"
#include "geometry/color_mesh_2d.hpp"

namespace spk
{
	class ColorMeshRenderer2D : public System::Participant2D
	{
	private:
		ColorMesh2D _mesh;

		void _buildRenderSnapshot(RenderSnapshot::Builder &builder) override;

	public:
		ColorMeshRenderer2D(
			const std::string &name = "Unnamed color mesh renderer 2D",
			Entity2D *owner = nullptr);
		explicit ColorMeshRenderer2D(Entity2D *owner);

		void setMesh(ColorMesh2D mesh);
		[[nodiscard]] const ColorMesh2D &mesh() const noexcept;
	};
}
