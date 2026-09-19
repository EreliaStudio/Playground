#include "voxel_test_utils.hpp"

#include "voxel/debug_chunk_generator.hpp"

#include <gtest/gtest.h>

TEST(ChunkCollectionTest, RequestsGeneratesAndFindsChunk)
{
	auto catalog = playground_test::loadVoxelCatalog();
	voxel::Chunk::Collection chunks;
	voxel::Chunk::DebugGenerator generator(chunks, catalog, 1234);
	EXPECT_TRUE(chunks.request({-1, 0, 0}));
	EXPECT_FALSE(chunks.request({-1, 0, 0}));
	EXPECT_NE(chunks.find({-1, 0, 0}), nullptr);
}
