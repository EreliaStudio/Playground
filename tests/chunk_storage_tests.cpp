#include "voxel/chunk.hpp"

#include <gtest/gtest.h>

TEST(ChunkStorageTest, HasFixedDimensionsAndCoordinateIndexing)
{
	static_assert(voxel::Chunk::Size == 16);
	static_assert(voxel::Chunk::CellCount == 4096);
	EXPECT_EQ(voxel::Chunk::index({0, 0, 0}), 0);
	EXPECT_EQ(voxel::Chunk::index({15, 0, 0}), 15);
	EXPECT_EQ(voxel::Chunk::index({0, 0, 1}), 16);
	EXPECT_EQ(voxel::Chunk::index({0, 1, 0}), 256);
	EXPECT_EQ(voxel::Chunk::index({15, 15, 15}), 4095);
}

TEST(ChunkStorageTest, RejectsCoordinatesOutsideChunk)
{
	for (const auto outside : {spk::Vector3Int{-1, 0, 0}, {16, 0, 0}, {0, -1, 0}, {0, 16, 0}, {0, 0, -1}, {0, 0, 16}})
	{
		EXPECT_FALSE(voxel::Chunk::contains(outside));
		EXPECT_THROW(static_cast<void>(voxel::Chunk::index(outside)), std::out_of_range);
	}
}

TEST(ChunkStorageTest, RetrievesFirstAndLastCellsAndRejectsOutsideAccess)
{
	voxel::Chunk chunk({2, -1, 3});
	const voxel::Voxel::Cell first(7);
	const voxel::Voxel::Cell last(9, voxel::Voxel::Orientation::PositiveX, voxel::Voxel::Flip::NegativeY);
	{
		auto editor = chunk.edit();
		editor.set({0, 0, 0}, first);
		editor.set({15, 15, 15}, last);
	}
	EXPECT_EQ(chunk.at({0, 0, 0}), first);
	EXPECT_EQ(chunk.at({15, 15, 15}), last);
	EXPECT_EQ(chunk.cells()[voxel::Chunk::index({15, 15, 15})], last);
	EXPECT_THROW(static_cast<void>(chunk.at({16, 0, 0})), std::out_of_range);
}
