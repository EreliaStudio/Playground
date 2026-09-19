#include "voxel/chunk.hpp"

#include <exception.hpp>
#include <gtest/gtest.h>
#include <type_traits>

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

TEST(ChunkStorageTest, ExposesFixedVolumeMetadata)
{
	static_assert(std::is_base_of_v<voxel::VoxelVolume, voxel::Chunk>);
	const voxel::Chunk chunk({2, -1, 3});
	const auto bounds = chunk.localBounds();

	EXPECT_EQ(chunk.dimensions(), spk::Vector3UInt(16, 16, 16));
	EXPECT_FLOAT_EQ(chunk.voxelSize(), 1.0f);
	EXPECT_EQ(chunk.cells().size(), voxel::Chunk::CellCount);
	EXPECT_EQ(bounds.minimum, spk::Vector3());
	EXPECT_EQ(bounds.maximum, spk::Vector3(16.0f, 16.0f, 16.0f));
}

TEST(ChunkStorageTest, RejectsCoordinatesOutsideChunk)
{
	for (const auto outside : {spk::Vector3Int{-1, 0, 0}, {16, 0, 0}, {0, -1, 0}, {0, 16, 0}, {0, 0, -1}, {0, 0, 16}})
	{
		EXPECT_FALSE(voxel::Chunk::contains(outside));
		EXPECT_THROW(static_cast<void>(voxel::Chunk::index(outside)), std::out_of_range);
	}
}

TEST(ChunkStorageTest, RetrievesFirstInteriorAndLastCellsAndRejectsOutsideAccess)
{
	voxel::Chunk chunk({2, -1, 3});
	const voxel::Voxel::Cell first(7);
	const voxel::Voxel::Cell interior(8, voxel::Voxel::Orientation::NegativeX, voxel::Voxel::Flip::None);
	const voxel::Voxel::Cell last(9, voxel::Voxel::Orientation::PositiveX, voxel::Voxel::Flip::NegativeY);
	{
		auto editor = chunk.edit();
		editor.set({0, 0, 0}, first);
		editor.set({3, 4, 5}, interior);
		editor.set({15, 15, 15}, last);
	}
	EXPECT_EQ(chunk.at({0, 0, 0}), first);
	EXPECT_EQ(chunk.at({3, 4, 5}), interior);
	EXPECT_EQ(chunk.at({15, 15, 15}), last);
	EXPECT_EQ(chunk.cells().front(), first);
	EXPECT_EQ(chunk.cells()[voxel::Chunk::index({3, 4, 5})], interior);
	EXPECT_EQ(chunk.cells()[voxel::Chunk::index({15, 15, 15})], last);
	EXPECT_EQ(chunk.cells().back(), last);
	EXPECT_EQ(chunk.at({15, 15, 15}).packed(), last.packed());
	EXPECT_THROW(static_cast<void>(chunk.at({16, 0, 0})), spk::Exception);
}

TEST(ChunkStorageTest, UsesInheritedAccessDiagnosticsWithoutAliasingStorage)
{
	voxel::Chunk chunk({0, 0, 0});
	chunk.edit().set({0, 0, 0}, voxel::Voxel::Cell(7));
	try
	{
		static_cast<void>(chunk.at({16, 0, 0}));
		FAIL() << "expected spk::Exception";
	} catch (const spk::Exception &exception)
	{
		EXPECT_EQ(exception.message(), "voxel volume coordinate is out of range");
		EXPECT_NE(exception.location().line(), 0u);
	}
	EXPECT_EQ(chunk.at({0, 0, 0}), voxel::Voxel::Cell(7));
}
