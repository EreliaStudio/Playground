#include "voxel/voxel_world.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <filesystem>
#include <iostream>

namespace
{
	voxel::Voxel::Catalog<voxel::Voxel::Definition> catalog()
	{
		const std::filesystem::path root = VOXEL_TEST_RESOURCE_DIR;
		return voxel::Voxel::Catalog<voxel::Voxel::Definition>::load(root / "catalog_config.json");
	}

	void assertCounterClockwise(const spk::TextureMesh3D &mesh)
	{
		const auto vertices = mesh.layout().vertexBuffer().cast<spk::Texture3DVertex>();
		const auto indices = mesh.layout().indexBuffer().cast<spk::TextureMesh3D::Index>();
		ASSERT_EQ(indices.size() % 3, 0);
		for (std::size_t index = 0; index < indices.size(); index += 3)
		{
			const auto &a = vertices[indices[index]];
			const auto &b = vertices[indices[index + 1]];
			const auto &c = vertices[indices[index + 2]];
			const auto triangleNormal = (b.position - a.position).cross(c.position - a.position);
			EXPECT_GT(triangleNormal.dot(a.normal), 0.0f);
		}
	}

	void assertConvex(const voxel::Voxel::Shape::Polygon &polygon)
	{
		ASSERT_GE(polygon.vertices.size(), 3);
		for (std::size_t index = 0; index < polygon.vertices.size(); ++index)
		{
			const auto &previous = polygon.vertices[index].position;
			const auto &current = polygon.vertices[(index + 1) % polygon.vertices.size()].position;
			const auto &next = polygon.vertices[(index + 2) % polygon.vertices.size()].position;
			EXPECT_GE((current - previous).cross(next - current).dot(polygon.normal), -0.0001f);
		}
	}
}

TEST(VoxelWorldTest, CharacterizesExistingWorldPipeline)
{
	using namespace voxel;
	static_assert(sizeof(Voxel::Cell) == 4);
	const Voxel::Cell packed(42, Voxel::Orientation::NegativeX, Voxel::Flip::NegativeY);
	EXPECT_EQ(packed.id(), 42);
	EXPECT_EQ(packed.orientation(), Voxel::Orientation::NegativeX);
	EXPECT_EQ(packed.flip(), Voxel::Flip::NegativeY);
	EXPECT_EQ(Voxel::Cell::fromPacked(packed.packed()), packed);
	EXPECT_EQ(Voxel::Cell{}.packed(), 0);
	const auto negativeChunk = Chunk::worldToChunk({-1, -16, -17});
	EXPECT_EQ(negativeChunk, (Chunk::Coordinate{-1, -1, -2}));
	const auto negativeLocal = Chunk::worldToLocal({-1, -16, -17});
	EXPECT_EQ(negativeLocal, spk::Vector3Int(15, 0, 15));

	auto content = catalog();
	EXPECT_EQ(content.size(), 7);
	EXPECT_NE(content.id("grass"), 0);
	bool foundSlabTop = false;
	for (const auto &polygon : content.definition("debug_slab").shape->polygons)
	{
		if (polygon.outerSide == Voxel::Side::None && std::abs(polygon.normal.y) > 0.5f)
		{
			foundSlabTop = true;
			EXPECT_GT(polygon.normal.y, 0.0f);
		}
	}
	EXPECT_TRUE(foundSlabTop);
	bool foundSlopeTop = false;
	for (const auto &polygon : content.definition("debug_slope").shape->polygons)
	{
		if (polygon.outerSide == Voxel::Side::None && std::abs(polygon.normal.y) > 0.5f && std::abs(polygon.normal.z) > 0.5f)
		{
			foundSlopeTop = true;
			EXPECT_GT(polygon.normal.y, 0.0f);
			EXPECT_LT(polygon.normal.z, 0.0f);
		}
	}
	EXPECT_TRUE(foundSlopeTop);
	bool foundStairRiser = false;
	bool foundStairLowerTread = false;
	for (const auto &polygon : content.definition("debug_stair").shape->polygons)
	{
		assertConvex(polygon);
		if (polygon.outerSide == Voxel::Side::None && std::abs(polygon.normal.y) > 0.5f)
		{
			foundStairLowerTread = true;
			EXPECT_GT(polygon.normal.y, 0.0f);
		}
		if (polygon.outerSide == Voxel::Side::None && std::abs(polygon.normal.z) > 0.5f)
		{
			foundStairRiser = true;
			EXPECT_LT(polygon.normal.z, 0.0f);
		}
	}
	EXPECT_TRUE(foundStairLowerTread);
	EXPECT_TRUE(foundStairRiser);
	Chunk edited({0, 0, 0});
	int notifications = 0;
	auto contract = edited.subscribeToVersionEdition([&](spk::VersionedTrait *versioned) {
		++notifications;
		EXPECT_EQ(versioned, &edited);
	});
	{
		auto editor = edited.edit();
		editor.set({0, 1, 1}, Voxel::Cell(content.id("stone")));
		editor.set({0, 1, 1}, Voxel::Cell(content.id("stone")));
	}
	EXPECT_EQ(edited.version(), 1);
	EXPECT_EQ(notifications, 1);
	{
		auto editor = edited.edit();
		editor.set({0, 1, 1}, Voxel::Cell(content.id("stone")));
	}
	EXPECT_EQ(edited.version(), 1);
	EXPECT_EQ(notifications, 1);

	Chunk::Collection chunks;
	Chunk::DebugGenerator generator(chunks, content, 1234);
	EXPECT_TRUE(chunks.request({-1, 0, 0}));
	EXPECT_FALSE(chunks.request({-1, 0, 0}));
	EXPECT_NE(chunks.find({-1, 0, 0}), nullptr);
	Chunk::Collection faces;
	auto left = std::make_unique<Chunk>(Chunk::Coordinate{0, 0, 0});
	{
		auto editor = left->edit();
		editor.set({15, 0, 0}, Voxel::Cell(content.id("stone")));
	}
	auto right = std::make_unique<Chunk>(Chunk::Coordinate{1, 0, 0});
	{
		auto editor = right->edit();
		editor.set({0, 0, 0}, Voxel::Cell(content.id("stone")));
	}
	faces.request({0, 0, 0});
	faces.publish(std::move(left));
	faces.request({1, 0, 0});
	faces.publish(std::move(right));
	Chunk::Baker baker(content, faces);
	EXPECT_EQ(baker.bake(*faces.find({0, 0, 0})).indexCount(), 30);
	EXPECT_EQ(baker.bake(*faces.find({1, 0, 0})).indexCount(), 30);

	Chunk::Collection partialFaces;
	auto leftSlab = std::make_unique<Chunk>(Chunk::Coordinate{0, 0, 0});
	{
		auto editor = leftSlab->edit();
		editor.set({15, 0, 0}, Voxel::Cell(content.id("debug_slab")));
	}
	auto rightSlab = std::make_unique<Chunk>(Chunk::Coordinate{1, 0, 0});
	{
		auto editor = rightSlab->edit();
		editor.set({0, 0, 0}, Voxel::Cell(content.id("debug_slab")));
	}
	partialFaces.request({0, 0, 0});
	partialFaces.publish(std::move(leftSlab));
	partialFaces.request({1, 0, 0});
	partialFaces.publish(std::move(rightSlab));
	Chunk::Baker partialBaker(content, partialFaces);
	EXPECT_EQ(partialBaker.bake(*partialFaces.find({0, 0, 0})).indexCount(), 30);
	EXPECT_EQ(partialBaker.bake(*partialFaces.find({1, 0, 0})).indexCount(), 30);
	Chunk::Collection transformedFaces;
	auto lowerSlab = std::make_unique<Chunk>(Chunk::Coordinate{0, 0, 0});
	{
		auto editor = lowerSlab->edit();
		editor.set({15, 0, 0}, Voxel::Cell(content.id("debug_slab")));
	}
	auto upperSlab = std::make_unique<Chunk>(Chunk::Coordinate{1, 0, 0});
	{
		auto editor = upperSlab->edit();
		editor.set({0, 0, 0}, Voxel::Cell(content.id("debug_slab"), Voxel::Orientation::PositiveZ, Voxel::Flip::NegativeY));
	}
	transformedFaces.request({0, 0, 0});
	transformedFaces.publish(std::move(lowerSlab));
	transformedFaces.request({1, 0, 0});
	transformedFaces.publish(std::move(upperSlab));
	Chunk::Baker transformedBaker(content, transformedFaces);
	EXPECT_EQ(transformedBaker.bake(*transformedFaces.find({0, 0, 0})).indexCount(), 36);
	EXPECT_EQ(transformedBaker.bake(*transformedFaces.find({1, 0, 0})).indexCount(), 36);
	Chunk::Collection slopeFaces;
	auto slopes = std::make_unique<Chunk>(Chunk::Coordinate{0, 0, 0});
	{
		auto editor = slopes->edit();
		for (std::uint32_t transform = 0; transform < 8; ++transform)
		{
			const auto orientation = static_cast<Voxel::Orientation>((transform % 4) << 29);
			const auto flip = transform < 4 ? Voxel::Flip::PositiveY : Voxel::Flip::NegativeY;
			editor.set({static_cast<int>((transform % 4) * 3), static_cast<int>((transform / 4) * 3), 0}, Voxel::Cell(content.id("debug_slope"), orientation, flip));
			editor.set({static_cast<int>((transform % 4) * 3), static_cast<int>((transform / 4) * 3), 3}, Voxel::Cell(content.id("debug_stair"), orientation, flip));
			editor.set({static_cast<int>((transform % 4) * 3), static_cast<int>((transform / 4) * 3), 6}, Voxel::Cell(content.id("debug_slab"), orientation, flip));
		}
	}
	slopeFaces.request({0, 0, 0});
	slopeFaces.publish(std::move(slopes));
	Chunk::Baker slopeBaker(content, slopeFaces);
	assertCounterClockwise(slopeBaker.bake(*slopeFaces.find({0, 0, 0})));

	Chunk::Collection scheduledChunks;
	Chunk::Baker scheduledBaker(content, scheduledChunks);
	Chunk::BakeScheduler scheduler(scheduledChunks, scheduledBaker);
	int firstConsumer = 0, secondConsumer = 0;
	auto firstBake = scheduler.subscribeToBakeCompletion([&](Chunk::Coordinate, const spk::TextureMesh3D &) {
		++firstConsumer;
	});
	auto secondBake = scheduler.subscribeToBakeCompletion([&](Chunk::Coordinate, const spk::TextureMesh3D &) {
		++secondConsumer;
	});
	auto scheduled = std::make_unique<Chunk>(Chunk::Coordinate{0, 0, 0});
	scheduledChunks.request({0, 0, 0});
	scheduledChunks.publish(std::move(scheduled));
	scheduler.process();
	EXPECT_EQ(firstConsumer, 1);
	EXPECT_EQ(secondConsumer, 1);
	{
		auto editor = scheduledChunks.find({0, 0, 0})->edit();
		editor.set({1, 1, 1}, Voxel::Cell(content.id("stone")));
	}
	EXPECT_EQ(scheduler.pendingCount(), 1);
	scheduler.process();
	EXPECT_EQ(firstConsumer, 2);
	EXPECT_EQ(secondConsumer, 2);
}
