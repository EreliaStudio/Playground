#include "voxel/voxel_model.hpp"

namespace voxel
{
	VoxelModel::VoxelModel(spk::Vector3UInt dimensions, float voxelSize) :
		VoxelVolume(dimensions, voxelSize)
	{
	}
}
