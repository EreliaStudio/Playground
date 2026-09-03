#pragma once

#include <memory>

#include "voxel/chunk_collection.hpp"
#include "voxel/voxel_catalog.hpp"

namespace voxel
{
	class Chunk::Baker final
	{
	private:
		class OcclusionCache;

		const Voxel::Catalog &_catalog;
		const Collection &_chunks;
		mutable std::unique_ptr<OcclusionCache> _occlusionCache;

		void _appendChunkCellsToMesh(spk::TextureMesh3D::Builder &builder, const Chunk &chunk) const;
		void _appendCellPolygonsToMesh(spk::TextureMesh3D::Builder &builder, spk::Vector3Int localPosition, Voxel::Cell cell, spk::Vector3Int chunkWorldOrigin) const;
		void _appendPolygonVisibleAroundNeighborToMesh(spk::TextureMesh3D::Builder &builder, const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition, Voxel::Cell cell, spk::Vector3Int localPosition, spk::Vector3Int chunkWorldOrigin) const;
		void _appendTransformedPolygonToMesh(spk::TextureMesh3D::Builder &builder, const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition, spk::Vector3Int localPosition, bool mirrored) const;

	public:
		Baker(const Voxel::Catalog &catalog, const Collection &chunks);

		~Baker();

		[[nodiscard]] spk::TextureMesh3D bake(const Chunk &chunk) const;
	};
}
