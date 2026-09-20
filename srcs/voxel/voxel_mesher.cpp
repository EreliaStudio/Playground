#include "voxel/voxel_mesher.hpp"

#include <cstdint>
#include <utility>
#include <vector>

#include "voxel/voxel_mesher_occlusion.hpp"
#include "voxel/voxel_transform.hpp"

namespace voxel
{
	VoxelMesher::VoxelMesher(const Voxel::Catalog<Voxel::Definition> &catalog) :
		_catalog(catalog),
		_occlusionCache(std::make_unique<OcclusionCache>())
	{
	}

	VoxelMesher::~VoxelMesher() = default;

	spk::TextureMesh3D VoxelMesher::bake(const VoxelVolume &volume) const
	{
		_validateDefinitions(volume);
		spk::TextureMesh3D::Builder builder;
		_appendCells(builder, volume);
		return std::move(builder).build();
	}

	void VoxelMesher::_validateDefinitions(const VoxelVolume &volume) const
	{
		for (const Voxel::Cell cell : volume.cells())
		{
			if (!cell.empty()) static_cast<void>(_catalog.definition(cell.id()));
		}
	}

	void VoxelMesher::_appendCells(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume) const
	{
		const auto dimensions = volume.dimensions();
		for (std::uint32_t y = 0; y < dimensions.y; ++y)
			for (std::uint32_t z = 0; z < dimensions.z; ++z)
				for (std::uint32_t x = 0; x < dimensions.x; ++x)
				{
					const spk::Vector3Int coordinate{static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)};
					const Voxel::Cell cell = volume.at(coordinate);
					if (!cell.empty()) _appendCell(builder, volume, coordinate, cell);
				}
	}

	void VoxelMesher::_appendCell(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
		spk::Vector3Int coordinate, Voxel::Cell cell) const
	{
		const auto &definition = _catalog.definition(cell.id());
		for (const auto &polygon : definition.shape->polygons)
		{
			_appendVisiblePolygon(builder, volume, polygon, definition, coordinate, cell);
		}
	}

	Voxel::Cell VoxelMesher::_neighbor(const VoxelVolume &volume, spk::Vector3Int coordinate) const
	{
		return volume.contains(coordinate) ? volume.at(coordinate) : _outsideCell(volume, coordinate);
	}

	Voxel::Cell VoxelMesher::_outsideCell(const VoxelVolume &, spk::Vector3Int) const
	{
		return {};
	}

	void VoxelMesher::_appendVisiblePolygon(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
		const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition,
		spk::Vector3Int coordinate, Voxel::Cell cell) const
	{
		const auto transformed = Voxel::transformPolygon(polygon, cell);
		if (transformed.outerSide == Voxel::Side::None)
		{
			_appendPolygon(builder, volume, transformed, definition, coordinate, cell.flip() == Voxel::Flip::NegativeY);
			return;
		}
		const Voxel::Cell neighbor = _neighbor(volume, coordinate + Voxel::sideOffset(transformed.outerSide));
		if (neighbor.empty())
		{
			_appendPolygon(builder, volume, transformed, definition, coordinate, cell.flip() == Voxel::Flip::NegativeY);
			return;
		}
		const auto &occludingShape = *_catalog.definition(neighbor.id()).shape;
		for (const auto &visible : _occlusionCache->visibleRemnants(polygon, cell, occludingShape, neighbor))
			_appendPolygon(builder, volume, visible, definition, coordinate, cell.flip() == Voxel::Flip::NegativeY);
	}

	void VoxelMesher::_appendPolygon(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
		const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition,
		spk::Vector3Int coordinate, bool mirrored) const
	{
		const auto atlas = definition.textures.at(polygon.materialSlot);
		const auto sprites = _catalog.atlas().nbSprite();
		const spk::Vector2 atlasUnit{1.0f / static_cast<float>(sprites.x), 1.0f / static_cast<float>(sprites.y)};
		std::vector<spk::TextureMesh3D::Index> indices;
		indices.reserve(polygon.vertices.size());
		for (const auto &vertex : polygon.vertices)
		{
			const spk::Vector3 local = vertex.position + spk::Vector3(coordinate);
			const float scale = volume.voxelSize();
			const spk::Vector3 position{local.x * scale, local.y * scale, local.z * scale};
			const spk::Vector2 uv{(atlas.x + vertex.uv.x) * atlasUnit.x, (atlas.y + vertex.uv.y) * atlasUnit.y};
			indices.push_back(builder.addVertex({position, polygon.normal, uv}));
		}
		for (std::size_t index = 1; index + 1 < indices.size(); ++index)
			if (mirrored) builder.addTriangle(indices[0], indices[index + 1], indices[index]);
			else builder.addTriangle(indices[0], indices[index], indices[index + 1]);
	}

	VoxelMesher::CacheStatistics VoxelMesher::_cacheStatistics() const noexcept
	{
		return _occlusionCache->statistics();
	}
}
