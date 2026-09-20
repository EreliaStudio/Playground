#pragma once

#include <cstddef>
#include <memory>

#include "voxel/voxel_catalog.hpp"
#include "voxel/voxel_volume.hpp"

namespace voxel
{
	class VoxelMesher
	{
	public:
		struct CacheStatistics
		{
			std::size_t entries = 0;
			std::size_t hits = 0;
			std::size_t misses = 0;
			bool operator==(const CacheStatistics &) const = default;
		};

	private:
		class OcclusionCache;

		const Voxel::Catalog<Voxel::Definition> &_catalog;
		mutable std::unique_ptr<OcclusionCache> _occlusionCache;

		void _validateDefinitions(const VoxelVolume &volume) const;
		void _appendCells(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume) const;
		void _appendCell(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
			spk::Vector3Int coordinate, Voxel::Cell cell) const;
		void _appendVisiblePolygon(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
			const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition,
			spk::Vector3Int coordinate, Voxel::Cell cell) const;
		void _appendPolygon(spk::TextureMesh3D::Builder &builder, const VoxelVolume &volume,
			const Voxel::Shape::Polygon &polygon, const Voxel::Definition &definition,
			spk::Vector3Int coordinate, bool mirrored) const;
		[[nodiscard]] Voxel::Cell _neighbor(const VoxelVolume &volume, spk::Vector3Int coordinate) const;

	protected:
		[[nodiscard]] virtual Voxel::Cell _outsideCell(
			const VoxelVolume &volume, spk::Vector3Int coordinate) const;
		[[nodiscard]] CacheStatistics _cacheStatistics() const noexcept;

	public:
		explicit VoxelMesher(const Voxel::Catalog<Voxel::Definition> &catalog);
		virtual ~VoxelMesher();

		VoxelMesher(const VoxelMesher &) = delete;
		VoxelMesher &operator=(const VoxelMesher &) = delete;
		VoxelMesher(VoxelMesher &&) noexcept = delete;
		VoxelMesher &operator=(VoxelMesher &&) noexcept = delete;

		[[nodiscard]] spk::TextureMesh3D bake(const VoxelVolume &volume) const;
	};
}
