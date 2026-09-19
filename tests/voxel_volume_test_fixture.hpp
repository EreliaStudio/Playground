#pragma once

#include "voxel/voxel_volume.hpp"

class TestVoxelVolume final : public voxel::VoxelVolume
{
public:
	using VoxelVolume::VoxelVolume;

	void set(spk::Vector3Int coordinate, voxel::Voxel::Cell cell)
	{
		mutableCellAt(coordinate) = cell;
	}
};
