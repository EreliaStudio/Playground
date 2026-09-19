#include "voxel_test_utils.hpp"

#include <exception.hpp>
#include <gtest/gtest.h>
#include <type_traits>

static_assert(std::is_same_v<voxel::Chunk::Editor, voxel::VoxelVolume::Editor>);

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

TEST(ChunkEditorTest, UsesInheritedEditorDiagnostics)
{
	voxel::Chunk chunk({0, 0, 0});
	auto editor = chunk.edit();

	EXPECT_THROW(editor.set({16, 0, 0}, voxel::Voxel::Cell(7)), spk::Exception);
	editor.commit();
	EXPECT_THROW(editor.set({0, 0, 0}, voxel::Voxel::Cell(7)), spk::Exception);
	EXPECT_EQ(chunk.version(), spk::VersionedTrait::Version{0});
}
