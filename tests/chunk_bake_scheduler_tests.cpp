#include "voxel_test_utils.hpp"

#include "voxel/chunk_bake_scheduler.hpp"

#include <gtest/gtest.h>

TEST(ChunkBakeSchedulerTest, NotifiesEverySubscriberForInitialBakeAndEdit)
{
	auto catalog = playground_test::loadVoxelCatalog();
	voxel::Chunk::Collection chunks;
	voxel::Chunk::Baker baker(catalog, chunks);
	voxel::Chunk::BakeScheduler scheduler(chunks, baker);
	int firstConsumer = 0;
	int secondConsumer = 0;
	auto firstBake = scheduler.subscribeToBakeCompletion(
		[&](voxel::Chunk::Coordinate, const spk::TextureMesh3D &) { ++firstConsumer; });
	auto secondBake = scheduler.subscribeToBakeCompletion(
		[&](voxel::Chunk::Coordinate, const spk::TextureMesh3D &) { ++secondConsumer; });
	ASSERT_TRUE(playground_test::publish(chunks, std::make_unique<voxel::Chunk>(voxel::Chunk::Coordinate{0, 0, 0})));
	scheduler.process();
	EXPECT_EQ(firstConsumer, 1);
	EXPECT_EQ(secondConsumer, 1);

	chunks.find({0, 0, 0})->edit().set({1, 1, 1}, voxel::Voxel::Cell(catalog.id("stone")));
	EXPECT_EQ(scheduler.pendingCount(), 1);
	scheduler.process();
	EXPECT_EQ(firstConsumer, 2);
	EXPECT_EQ(secondConsumer, 2);
}
