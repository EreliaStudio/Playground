#include "voxel/chunk_baker.hpp"

#include <utility>
#include <vector>

#include "voxel/chunk_baker_occlusion.hpp"
#include "voxel/voxel_transform.hpp"

namespace voxel
{
	Chunk::Baker::Baker(const Voxel::Catalog &catalog, const Collection &chunks) :
		_catalog(catalog),
		_chunks(chunks),
		_occlusionCache(std::make_unique<OcclusionCache>())
	{
	}

	Chunk::Baker::~Baker() = default;

	spk::TextureMesh3D Chunk::Baker::bake(const Chunk &chunk) const
	{
		spk::TextureMesh3D::Builder builder;
		builder.reserve(4096, 6144);
		_appendChunkCellsToMesh(builder, chunk);
		return std::move(builder).build();
	}

	void Chunk::Baker::_appendChunkCellsToMesh(spk::TextureMesh3D::Builder &builder, const Chunk &chunk) const
	{
		const auto chunkWorldOrigin = Chunk::worldOrigin(chunk.coordinate());
		for (int y = 0; y < Chunk::Size; ++y)
		{
			for (int z = 0; z < Chunk::Size; ++z)
			{
				for (int x = 0; x < Chunk::Size; ++x)
				{
					const spk::Vector3Int localPosition{x, y, z};
					const Voxel::Cell cell = chunk.at(localPosition);
					if (!cell.empty())
					{
						_appendCellPolygonsToMesh(builder, localPosition, cell, chunkWorldOrigin);
					}
				}
			}
		}
	}

	void Chunk::Baker::_appendCellPolygonsToMesh(
		spk::TextureMesh3D::Builder &builder,
		spk::Vector3Int localPosition,
		Voxel::Cell cell,
		spk::Vector3Int chunkWorldOrigin) const
	{
		const auto &definition = _catalog.definition(cell.id());
		for (const auto &polygon : definition.shape->polygons)
		{
			_appendPolygonVisibleAroundNeighborToMesh(builder, polygon, definition, cell, localPosition, chunkWorldOrigin);
		}
	}

	void Chunk::Baker::_appendPolygonVisibleAroundNeighborToMesh(
		spk::TextureMesh3D::Builder &builder,
		const Voxel::Shape::Polygon &polygon,
		const Voxel::Definition &definition,
		Voxel::Cell cell,
		spk::Vector3Int localPosition,
		spk::Vector3Int chunkWorldOrigin) const
	{
		const auto transformed = Voxel::transformPolygon(polygon, cell);
		if (transformed.outerSide == Voxel::Side::None)
		{
			_appendTransformedPolygonToMesh(builder, transformed, definition, localPosition, cell.flip() == Voxel::Flip::NegativeY);
			return;
		}

		const auto neighborWorldPosition = chunkWorldOrigin + localPosition + Voxel::sideOffset(transformed.outerSide);
		const auto neighbor = _chunks.worldCell(neighborWorldPosition);
		if (!neighbor.has_value() || neighbor->empty())
		{
			_appendTransformedPolygonToMesh(builder, transformed, definition, localPosition, cell.flip() == Voxel::Flip::NegativeY);
			return;
		}

		const auto &occludingShape = *_catalog.definition(neighbor->id()).shape;
		for (const auto &visibleRemnant : _occlusionCache->visibleRemnants(polygon, cell, occludingShape, *neighbor))
		{
			_appendTransformedPolygonToMesh(builder, visibleRemnant, definition, localPosition, cell.flip() == Voxel::Flip::NegativeY);
		}
	}

	void Chunk::Baker::_appendTransformedPolygonToMesh(
		spk::TextureMesh3D::Builder &builder,
		const Voxel::Shape::Polygon &polygon,
		const Voxel::Definition &definition,
		spk::Vector3Int localPosition,
		bool mirrored) const
	{
		const auto atlas = definition.textures.at(polygon.materialSlot);
		const spk::Vector2 atlasUnit{1.0f / _catalog.atlasSize().x, 1.0f / _catalog.atlasSize().y};
		std::vector<spk::TextureMesh3D::Index> indices;
		indices.reserve(polygon.vertices.size());
		for (const auto &vertex : polygon.vertices)
		{
			const auto position = vertex.position + spk::Vector3(localPosition);
			const spk::Vector2 uv{(atlas.x + vertex.uv.x) * atlasUnit.x, (atlas.y + vertex.uv.y) * atlasUnit.y};
			indices.push_back(builder.addVertex({position, polygon.normal, uv}));
		}
		for (std::size_t index = 1; index + 1 < indices.size(); ++index)
		{
			if (mirrored)
			{
				builder.addTriangle(indices[0], indices[index + 1], indices[index]);
			}
			else
			{
				builder.addTriangle(indices[0], indices[index], indices[index + 1]);
			}
		}
	}
}
