#pragma once

#include <span>
#include <vector>

#include <math/vector3.hpp>

#include "voxel/voxel_cell.hpp"

namespace voxel
{
	class VoxelVolume
	{
	public:
		struct Bounds
		{
			spk::Vector3 minimum{};
			spk::Vector3 maximum{};
			bool operator==(const Bounds &) const = default;
		};

	private:
		spk::Vector3UInt _dimensions;
		float _voxelSize;
		std::vector<Voxel::Cell> _cells;

		[[nodiscard]] static std::size_t _cellCount(spk::Vector3UInt dimensions);
		[[nodiscard]] static float _validatedVoxelSize(float voxelSize);
		[[nodiscard]] std::size_t _index(spk::Vector3Int coordinate) const;

	protected:
		[[nodiscard]] Voxel::Cell &mutableCellAt(spk::Vector3Int coordinate);

	public:
		VoxelVolume(spk::Vector3UInt dimensions, float voxelSize);
		virtual ~VoxelVolume() = default;

		[[nodiscard]] spk::Vector3UInt dimensions() const noexcept;
		[[nodiscard]] float voxelSize() const noexcept;
		[[nodiscard]] Bounds localBounds() const noexcept;
		[[nodiscard]] bool contains(spk::Vector3Int coordinate) const noexcept;
		[[nodiscard]] Voxel::Cell at(spk::Vector3Int coordinate) const;
		[[nodiscard]] std::span<const Voxel::Cell> cells() const noexcept;
	};
}
