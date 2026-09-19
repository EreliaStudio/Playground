#include "voxel_volume_test_fixture.hpp"

#include <limits>

#include <gtest/gtest.h>

TEST(VoxelVolumeMetadataTest, ExposesDimensionsScaleAndDerivedLocalBounds)
{
	const voxel::VoxelVolume volume({8, 8, 16}, 0.1f);
	const auto bounds = volume.localBounds();

	EXPECT_EQ(volume.dimensions(), spk::Vector3UInt(8, 8, 16));
	EXPECT_FLOAT_EQ(volume.voxelSize(), 0.1f);
	EXPECT_EQ(volume.cells().size(), 1024);
	EXPECT_TRUE(volume.at({0, 0, 0}).empty());
	EXPECT_EQ(bounds.minimum, spk::Vector3());
	EXPECT_FLOAT_EQ(bounds.maximum.x, 0.8f);
	EXPECT_FLOAT_EQ(bounds.maximum.y, 0.8f);
	EXPECT_FLOAT_EQ(bounds.maximum.z, 1.6f);
}

TEST(VoxelVolumeMetadataTest, RejectsEmptyDimensions)
{
	EXPECT_THROW(voxel::VoxelVolume({0, 1, 1}, 1.0f), std::invalid_argument);
	EXPECT_THROW(voxel::VoxelVolume({1, 0, 1}, 1.0f), std::invalid_argument);
	EXPECT_THROW(voxel::VoxelVolume({1, 1, 0}, 1.0f), std::invalid_argument);
}

TEST(VoxelVolumeMetadataTest, RejectsNonPositiveOrNonFiniteVoxelSize)
{
	EXPECT_THROW(voxel::VoxelVolume({1, 1, 1}, 0.0f), std::invalid_argument);
	EXPECT_THROW(voxel::VoxelVolume({1, 1, 1}, -0.1f), std::invalid_argument);
	EXPECT_THROW(voxel::VoxelVolume({1, 1, 1}, std::numeric_limits<float>::infinity()), std::invalid_argument);
	EXPECT_THROW(voxel::VoxelVolume({1, 1, 1}, std::numeric_limits<float>::quiet_NaN()), std::invalid_argument);
}
