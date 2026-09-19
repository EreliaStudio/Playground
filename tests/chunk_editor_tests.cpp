#include "voxel_test_utils.hpp"

#include <gtest/gtest.h>

TEST(ChunkEditorTest, PublishesOneVersionForChangedEdit)
{
	const auto catalog = playground_test::loadVoxelCatalog();
	voxel::Chunk chunk({0, 0, 0});
	int notifications = 0;
	auto contract = chunk.subscribeToVersionEdition([&](spk::VersionedTrait *versioned) {
		++notifications;
		EXPECT_EQ(versioned, &chunk);
	});
	{
		auto editor = chunk.edit();
		editor.set({0, 1, 1}, voxel::Voxel::Cell(catalog.id("stone")));
		editor.set({0, 1, 1}, voxel::Voxel::Cell(catalog.id("stone")));
	}
	EXPECT_EQ(chunk.version(), 1);
	EXPECT_EQ(notifications, 1);
}

TEST(ChunkEditorTest, DoesNotPublishVersionForNoOpEdit)
{
	const auto catalog = playground_test::loadVoxelCatalog();
	voxel::Chunk chunk({0, 0, 0});
	chunk.edit().set({0, 1, 1}, voxel::Voxel::Cell(catalog.id("stone")));
	const auto initialVersion = chunk.version();
	int notifications = 0;
	auto contract = chunk.subscribeToVersionEdition([&](spk::VersionedTrait *) { ++notifications; });
	chunk.edit().set({0, 1, 1}, voxel::Voxel::Cell(catalog.id("stone")));
	EXPECT_EQ(chunk.version(), initialVersion);
	EXPECT_EQ(notifications, 0);
}
