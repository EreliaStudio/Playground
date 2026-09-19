#include "voxel_volume_test_fixture.hpp"

#include <gtest/gtest.h>

TEST(VoxelVolumeAccessTest, ResolvesValidCoordinatesToExpectedCells)
{
	TestVoxelVolume volume({2, 3, 4}, 0.5f);
	const voxel::Voxel::Cell first(7);
	const voxel::Voxel::Cell interior(11);
	const voxel::Voxel::Cell last(42, voxel::Voxel::Orientation::NegativeX, voxel::Voxel::Flip::NegativeY);
	volume.set({0, 0, 0}, first);
	volume.set({1, 1, 2}, interior);
	volume.set({1, 2, 3}, last);

	EXPECT_EQ(volume.at({0, 0, 0}), first);
	EXPECT_EQ(volume.at({1, 1, 2}), interior);
	EXPECT_EQ(volume.at({1, 2, 3}), last);
	EXPECT_EQ(volume.cells().size(), 24);
	EXPECT_EQ(volume.cells().front(), first);
	EXPECT_EQ(volume.cells()[13], interior);
	EXPECT_EQ(volume.cells().back(), last);
}

TEST(VoxelVolumeAccessTest, RejectsEveryOneStepOutsideCoordinate)
{
	const TestVoxelVolume volume({2, 3, 4}, 1.0f);
	for (const auto coordinate : {spk::Vector3Int{-1, 0, 0}, {2, 0, 0}, {0, -1, 0}, {0, 3, 0}, {0, 0, -1}, {0, 0, 4}})
	{
		EXPECT_FALSE(volume.contains(coordinate));
		EXPECT_THROW(static_cast<void>(volume.at(coordinate)), std::out_of_range);
	}
}

TEST(VoxelVolumeAccessTest, ScaleDoesNotChangeTopologyOrCellIdentity)
{
	TestVoxelVolume unitScale({2, 1, 1}, 1.0f);
	TestVoxelVolume smallScale({2, 1, 1}, 0.1f);
	for (auto *volume : {&unitScale, &smallScale})
	{
		volume->set({0, 0, 0}, voxel::Voxel::Cell(3));
		volume->set({1, 0, 0}, voxel::Voxel::Cell(9));
	}

	EXPECT_EQ(unitScale.dimensions(), smallScale.dimensions());
	EXPECT_EQ(unitScale.at({0, 0, 0}).packed(), smallScale.at({0, 0, 0}).packed());
	EXPECT_EQ(unitScale.at({1, 0, 0}).packed(), smallScale.at({1, 0, 0}).packed());
	EXPECT_NE(unitScale.localBounds(), smallScale.localBounds());
}
