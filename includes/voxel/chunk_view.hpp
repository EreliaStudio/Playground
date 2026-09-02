#pragma once

#include <sparkle>

#include "engine/texture_mesh_renderer_3d.hpp"
#include "voxel/chunk.hpp"

namespace voxel
{
	class Chunk::View final
	{
	public:
		View(spk::Engine &engine, const spk::Texture *atlas, Coordinate coordinate);
		~View();
		void setMesh(spk::TextureMesh3D mesh);

	private:
		spk::Engine &_engine;
		spk::Entity3D _entity;
		spk::TextureMeshRenderer3D *_renderer;
	};
}
