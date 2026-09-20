#include "semantic_mesh_snapshot.hpp"
#include "voxel_test_utils.hpp"

#include "voxel/chunk_baker.hpp"

#include <gtest/gtest.h>

#include <string>
#include <string_view>

#include <exception.hpp>

namespace
{
	void expectSnapshot(const spk::TextureMesh3D &mesh, std::string_view snapshot)
	{
		EXPECT_EQ(playground_test::semanticMeshSnapshot(mesh),
			playground_test::loadCurrentBakerSemanticSnapshot(snapshot));
	}

	spk::TextureMesh3D bakeSingleCell(
		std::string_view definition,
		spk::Vector3Int position,
		voxel::Voxel::Orientation orientation = voxel::Voxel::Orientation::PositiveZ,
		voxel::Voxel::Flip flip = voxel::Voxel::Flip::PositiveY)
	{
		auto catalog = playground_test::loadVoxelCatalog();
		voxel::Chunk::Collection chunks;
		auto chunk = playground_test::chunkWithCell({0, 0, 0}, position,
			voxel::Voxel::Cell(catalog.id(std::string(definition)), orientation, flip));
		voxel::Chunk *source = chunk.get();
		if (!playground_test::publish(chunks, std::move(chunk))) throw spk::Exception("fixture publication failed");
		return voxel::Chunk::Baker(catalog, chunks).bake(*source);
	}

	spk::TextureMesh3D bakeBoundary(bool publishNeighbor)
	{
		auto catalog = playground_test::loadVoxelCatalog();
		voxel::Chunk::Collection chunks;
		auto left = playground_test::chunkWithCell({0, 0, 0}, {15, 2, 3}, voxel::Voxel::Cell(catalog.id("stone")));
		voxel::Chunk *source = left.get();
		if (!playground_test::publish(chunks, std::move(left))) throw spk::Exception("fixture publication failed");
		if (publishNeighbor)
		{
			auto right = playground_test::chunkWithCell({1, 0, 0}, {0, 2, 3}, voxel::Voxel::Cell(catalog.id("stone")));
			if (!playground_test::publish(chunks, std::move(right))) throw spk::Exception("fixture publication failed");
		}
		return voxel::Chunk::Baker(catalog, chunks).bake(*source);
	}
}

TEST(CurrentBakerSemanticGoldenTest, Cube)
{
	expectSnapshot(bakeSingleCell("grass", {1, 1, 1}), "cube");
}

TEST(CurrentBakerSemanticGoldenTest, Slab)
{
	expectSnapshot(bakeSingleCell("debug_slab", {2, 1, 1}, voxel::Voxel::Orientation::PositiveZ,
		voxel::Voxel::Flip::NegativeY), "slab_flipped");
}

TEST(CurrentBakerSemanticGoldenTest, Slope)
{
	expectSnapshot(bakeSingleCell("debug_slope", {3, 1, 1}, voxel::Voxel::Orientation::PositiveX),
		"slope_positive_x");
}

TEST(CurrentBakerSemanticGoldenTest, Stair)
{
	expectSnapshot(bakeSingleCell("debug_stair", {4, 1, 1}, voxel::Voxel::Orientation::NegativeX,
		voxel::Voxel::Flip::NegativeY), "stair_negative_x_flipped");
}

TEST(CurrentBakerSemanticGoldenTest, Cross)
{
	expectSnapshot(bakeSingleCell("plant", {5, 1, 1}, voxel::Voxel::Orientation::NegativeZ),
		"cross_negative_z");
}

TEST(CurrentBakerSemanticGoldenTest, MissingAdjacentChunkIsEmpty)
{
	expectSnapshot(bakeBoundary(false), "boundary_unloaded_neighbor");
}

TEST(CurrentBakerSemanticGoldenTest, AdjacentChunkOccludesSharedFace)
{
	expectSnapshot(bakeBoundary(true), "boundary_occluded_neighbor");
}

TEST(CurrentBakerSemanticGoldenTest, IdenticalInputIsDeterministic)
{
	EXPECT_EQ(playground_test::semanticMeshSnapshot(bakeSingleCell("debug_stair", {4, 1, 1})),
		playground_test::semanticMeshSnapshot(bakeSingleCell("debug_stair", {4, 1, 1})));
}

TEST(CurrentBakerSemanticGoldenTest, RejectedUnknownDefinitionDoesNotPoisonBaker)
{
	auto catalog = playground_test::loadVoxelCatalog();
	voxel::Chunk::Collection chunks;
	auto valid = playground_test::chunkWithCell({0, 0, 0}, {1, 1, 1}, voxel::Voxel::Cell(catalog.id("grass")));
	auto invalid = playground_test::chunkWithCell({1, 0, 0}, {1, 1, 1}, voxel::Voxel::Cell(catalog.size() + 1));
	voxel::Chunk *validSource = valid.get();
	voxel::Chunk *invalidSource = invalid.get();
	ASSERT_TRUE(playground_test::publish(chunks, std::move(valid)));
	ASSERT_TRUE(playground_test::publish(chunks, std::move(invalid)));
	voxel::Chunk::Baker baker(catalog, chunks);
	EXPECT_THROW((void)baker.bake(*invalidSource), spk::Exception);
	expectSnapshot(baker.bake(*validSource), "cube");
}
