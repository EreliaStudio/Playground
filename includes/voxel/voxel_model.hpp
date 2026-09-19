#pragma once

#include "voxel/voxel_volume.hpp"

namespace voxel
{
	class VoxelModel final : public VoxelVolume
	{
	public:
		VoxelModel(spk::Vector3UInt dimensions, float voxelSize);
	};
}
