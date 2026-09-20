#pragma once

#include "voxel/chunk_collection.hpp"
#include "voxel/voxel_mesher.hpp"

namespace voxel
{
	class ChunkMesher final : public VoxelMesher
	{
	private:
		const Chunk::Collection &_chunks;

		[[nodiscard]] Voxel::Cell _outsideCell(
			const VoxelVolume &volume, spk::Vector3Int coordinate) const override;

	public:
		ChunkMesher(const Voxel::Catalog<Voxel::Definition> &catalog, const Chunk::Collection &chunks);

		[[nodiscard]] spk::TextureMesh3D bake(const Chunk &chunk) const;
	};
}
