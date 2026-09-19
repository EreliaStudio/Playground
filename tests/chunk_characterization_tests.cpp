#include "voxel/chunk_baker.hpp"
#include "voxel/chunk_editor.hpp"
#include "voxel/voxel_transform.hpp"

#include <gtest/gtest.h>

#include <cmath>
#include <memory>

namespace
{
	using voxel::Chunk;
	using voxel::Voxel::Cell;

	voxel::Voxel::Catalog<voxel::Voxel::Definition> catalog()
	{
		return voxel::Voxel::Catalog<voxel::Voxel::Definition>::load("resources/catalog_config.json");
	}

	std::unique_ptr<Chunk> chunkWithCell(Chunk::Coordinate coordinate, spk::Vector3Int local, Cell cell)
	{
		auto chunk = std::make_unique<Chunk>(coordinate);
		chunk->edit().set(local, cell);
		return chunk;
	}

	void publish(Chunk::Collection &chunks, std::unique_ptr<Chunk> chunk)
	{
		const auto coordinate = chunk->coordinate();
		ASSERT_TRUE(chunks.request(coordinate));
		chunks.publish(std::move(chunk));
	}
}

TEST(ChunkStorageTest, HasFixedDimensionsAndCoordinateIndexing)
{
	static_assert(Chunk::Size == 16);
	static_assert(Chunk::CellCount == 4096);
	EXPECT_EQ(Chunk::index({0, 0, 0}), 0);
	EXPECT_EQ(Chunk::index({15, 0, 0}), 15);
	EXPECT_EQ(Chunk::index({0, 0, 1}), 16);
	EXPECT_EQ(Chunk::index({0, 1, 0}), 256);
	EXPECT_EQ(Chunk::index({15, 15, 15}), 4095);
}

TEST(ChunkStorageTest, RejectsCoordinatesOutsideChunk)
{
	for (const auto outside : {spk::Vector3Int{-1, 0, 0}, {16, 0, 0}, {0, -1, 0}, {0, 16, 0}, {0, 0, -1}, {0, 0, 16}})
	{
		EXPECT_FALSE(Chunk::contains(outside));
		EXPECT_THROW(static_cast<void>(Chunk::index(outside)), std::out_of_range);
	}
}

TEST(VoxelCellTest, PreservesPackedDefinitionOrientationAndFlip)
{
	static_assert(sizeof(Cell) == sizeof(std::uint32_t));
	const Cell transformed(42, voxel::Voxel::Orientation::NegativeX, voxel::Voxel::Flip::NegativeY);
	EXPECT_EQ(transformed.packed(), 0xE000002Au);
	EXPECT_EQ(Cell::fromPacked(0xE000002Au), transformed);
	EXPECT_TRUE(Cell{}.empty());
	EXPECT_EQ(Cell{}.packed(), 0);
	EXPECT_THROW(static_cast<void>(Cell(Cell::IDMask + 1)), std::invalid_argument);
}

TEST(ChunkStorageTest, RetrievesFirstAndLastCellsAndRejectsOutsideAccess)
{
	Chunk chunk({2, -1, 3});
	const Cell first(7);
	const Cell last(9, voxel::Voxel::Orientation::PositiveX, voxel::Voxel::Flip::NegativeY);
	{
		auto editor = chunk.edit();
		editor.set({0, 0, 0}, first);
		editor.set({15, 15, 15}, last);
	}
	EXPECT_EQ(chunk.at({0, 0, 0}), first);
	EXPECT_EQ(chunk.at({15, 15, 15}), last);
	EXPECT_EQ(chunk.cells()[Chunk::index({15, 15, 15})], last);
	EXPECT_THROW(static_cast<void>(chunk.at({16, 0, 0})), std::out_of_range);
}

TEST(ChunkCoordinatesTest, ConvertsPositiveAndNegativeWorldCoordinates)
{
	struct Case
	{
		spk::Vector3Int world;
		Chunk::Coordinate chunk;
		spk::Vector3Int local;
	};
	const Case cases[] = {
		{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
		{{15, 15, 15}, {0, 0, 0}, {15, 15, 15}},
		{{16, 16, 16}, {1, 1, 1}, {0, 0, 0}},
		{{-1, -1, -1}, {-1, -1, -1}, {15, 15, 15}},
		{{-16, -16, -16}, {-1, -1, -1}, {0, 0, 0}},
		{{-17, -17, -17}, {-2, -2, -2}, {15, 15, 15}}};
	for (const auto &value : cases)
	{
		EXPECT_EQ(Chunk::worldToChunk(value.world), value.chunk);
		EXPECT_EQ(Chunk::worldToLocal(value.world), value.local);
		EXPECT_EQ(Chunk::worldOrigin(value.chunk) + value.local, value.world);
	}
}

TEST(VoxelShapeTransformTest, PreservesMaterialAndUvWhileApplyingOrientationAndFlip)
{
	voxel::Voxel::Shape::Polygon polygon{
		.vertices = {{{0.0f, 0.25f, 0.0f}, {0.125f, 0.75f}}, {{1.0f, 0.25f, 0.0f}, {0.875f, 0.75f}}, {{1.0f, 0.75f, 0.0f}, {0.875f, 0.25f}}},
		.materialSlot = "side",
		.normal = {0.0f, 0.0f, -1.0f},
		.outerSide = voxel::Voxel::Side::NegativeZ};
	const Cell transform(1, voxel::Voxel::Orientation::PositiveX, voxel::Voxel::Flip::NegativeY);
	const auto result = voxel::Voxel::transformPolygon(polygon, transform);
	EXPECT_EQ(result.materialSlot, "side");
	EXPECT_EQ(result.vertices[0].uv, spk::Vector2(0.125f, 0.75f));
	EXPECT_EQ(result.vertices[0].position, spk::Vector3(0.0f, 0.75f, 1.0f));
	EXPECT_EQ(result.vertices[2].position, spk::Vector3(0.0f, 0.25f, 0.0f));
	EXPECT_EQ(result.normal, spk::Vector3(-1.0f, 0.0f, 0.0f));
	EXPECT_EQ(result.outerSide, voxel::Voxel::Side::NegativeX);
}

TEST(ChunkBakerTest, PropagatesMaterialSlotsIntoAtlasUvs)
{
	auto content = catalog();
	Chunk::Collection chunks;
	auto chunk = chunkWithCell({0, 0, 0}, {0, 0, 0}, Cell(content.id("grass")));
	Chunk *source = chunk.get();
	publish(chunks, std::move(chunk));
	const auto mesh = Chunk::Baker(content, chunks).bake(*source);
	const auto vertices = mesh.layout().vertexBuffer().cast<spk::Texture3DVertex>();
	ASSERT_EQ(mesh.indexCount(), 36);
	ASSERT_EQ(vertices.size(), 24);
	bool sideUv = false, bottomUv = false, topUv = false;
	for (const auto &vertex : vertices)
	{
		if (std::abs(std::abs(vertex.normal.x) + std::abs(vertex.normal.z) - 1.0f) < 0.0001f &&
			vertex.uv.x >= 0.25f && vertex.uv.x <= 0.5f && vertex.uv.y >= 0.0f && vertex.uv.y <= 0.25f)
			sideUv = true;
		if (vertex.normal == spk::Vector3(0.0f, -1.0f, 0.0f) && vertex.uv.x >= 0.5f && vertex.uv.x <= 0.75f)
			bottomUv = true;
		if (vertex.normal == spk::Vector3(0.0f, 1.0f, 0.0f) && vertex.uv.x >= 0.0f && vertex.uv.x <= 0.25f)
			topUv = true;
	}
	EXPECT_TRUE(sideUv);
	EXPECT_TRUE(bottomUv);
	EXPECT_TRUE(topUv);
}

TEST(ChunkBakerTest, ResolvesAdjacentChunkAndPartialOcclusion)
{
	auto content = catalog();
	Chunk::Collection chunks;
	auto left = chunkWithCell({0, 0, 0}, {15, 0, 0}, Cell(content.id("stone")));
	auto right = chunkWithCell({1, 0, 0}, {0, 0, 0}, Cell(content.id("stone")));
	Chunk *leftSource = left.get();
	Chunk *rightSource = right.get();
	publish(chunks, std::move(left));
	publish(chunks, std::move(right));
	EXPECT_EQ(chunks.worldCell({15, 0, 0}), Cell(content.id("stone")));
	EXPECT_EQ(chunks.worldCell({16, 0, 0}), Cell(content.id("stone")));
	EXPECT_FALSE(chunks.worldCell({32, 0, 0}).has_value());
	Chunk::Baker baker(content, chunks);
	EXPECT_EQ(baker.bake(*leftSource).indexCount(), 30);
	EXPECT_EQ(baker.bake(*rightSource).indexCount(), 30);

	Chunk::Collection partial;
	auto slab = chunkWithCell({0, 0, 0}, {0, 0, 0}, Cell(content.id("debug_slab")));
	{
		auto editor = slab->edit();
		editor.set({1, 0, 0}, Cell(content.id("stone")));
	}
	Chunk *partialSource = slab.get();
	publish(partial, std::move(slab));
	EXPECT_EQ(Chunk::Baker(content, partial).bake(*partialSource).indexCount(), 66);
}
