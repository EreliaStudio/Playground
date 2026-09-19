#include "engine/camera.hpp"
#include "rendering/command/camera_ubo_render_command.hpp"
#include "rendering/command/texture_mesh_3d_render_command.hpp"
#include "voxel/chunk_baker.hpp"
#include "voxel/chunk_editor.hpp"
#include "voxel/debug_chunk_generator.hpp"

#include <GL/glew.h>
#include <gtest/gtest.h>
#include <sparkle>
#include <sparkle_test.hpp>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace
{
	constexpr spk::Vector2UInt FramebufferSize{640, 480};
	constexpr std::string_view Category = "current_chunk";
	using Catalog = voxel::Voxel::Catalog<voxel::Voxel::Definition>;

	struct Camera
	{
		spk::Vector3 position;
		spk::Vector3 target;
		float verticalFieldOfViewRadians;
		float nearPlane;
		float farPlane;
	};

	struct Cell
	{
		voxel::Chunk::Coordinate chunk;
		spk::Vector3Int local;
		std::string_view definition;
		voxel::Voxel::Orientation orientation = voxel::Voxel::Orientation::PositiveZ;
		voxel::Voxel::Flip flip = voxel::Voxel::Flip::PositiveY;
	};

	struct Scene
	{
		std::string_view name;
		Camera camera;
		std::vector<Cell> cells;
	};

	void require(bool condition, std::string_view message)
	{
		if (!condition)
		{
			throw std::runtime_error(std::string(message));
		}
	}

	Catalog loadCatalog()
	{
		return Catalog::load("resources/catalog_config.json");
	}

	std::vector<Scene> declaredScenes()
	{
		using Orientation = voxel::Voxel::Orientation;
		using Flip = voxel::Voxel::Flip;
		std::vector<Scene> result;
		result.push_back({"solid_cube_boundary", {{24.0f, 18.0f, 28.0f}, {16.0f, 1.0f, 1.0f}, 0.72f, 0.1f, 100.0f},
			{{{0, 0, 0}, {14, 0, 0}, "stone"}, {{0, 0, 0}, {15, 0, 0}, "stone"},
				{{0, 0, 0}, {14, 0, 1}, "stone"}, {{0, 0, 0}, {15, 0, 1}, "stone"},
				{{1, 0, 0}, {0, 0, 0}, "stone"}, {{1, 0, 0}, {0, 0, 1}, "stone"}}});
		result.push_back({"slab_partial_occlusion", {{8.0f, 7.0f, 10.0f}, {1.5f, 0.5f, 0.5f}, 0.62f, 0.1f, 50.0f},
			{{{0, 0, 0}, {0, 0, 0}, "debug_slab"}, {{0, 0, 0}, {1, 0, 0}, "stone"},
				{{0, 0, 0}, {3, 0, 0}, "debug_slab", Orientation::PositiveZ, Flip::NegativeY}}});
		result.push_back({"slope_stair_transforms", {{12.0f, 11.0f, 15.0f}, {4.5f, 1.0f, 2.5f}, 0.72f, 0.1f, 80.0f},
			{{{0, 0, 0}, {0, 0, 0}, "debug_slope", Orientation::PositiveZ},
				{{0, 0, 0}, {2, 0, 0}, "debug_slope", Orientation::PositiveX},
				{{0, 0, 0}, {4, 0, 0}, "debug_slope", Orientation::NegativeZ, Flip::NegativeY},
				{{0, 0, 0}, {6, 0, 0}, "debug_slope", Orientation::NegativeX, Flip::NegativeY},
				{{0, 0, 0}, {0, 0, 3}, "debug_stair", Orientation::PositiveZ},
				{{0, 0, 0}, {2, 0, 3}, "debug_stair", Orientation::PositiveX},
				{{0, 0, 0}, {4, 0, 3}, "debug_stair", Orientation::NegativeZ, Flip::NegativeY},
				{{0, 0, 0}, {6, 0, 3}, "debug_stair", Orientation::NegativeX, Flip::NegativeY}}});
		result.push_back({"materials_and_vegetation", {{10.0f, 8.0f, 12.0f}, {2.5f, 1.0f, 1.5f}, 0.68f, 0.1f, 60.0f},
			{{{0, 0, 0}, {0, 0, 0}, "grass"}, {{0, 0, 0}, {1, 0, 0}, "dirt"},
				{{0, 0, 0}, {2, 0, 0}, "stone"}, {{0, 0, 0}, {0, 1, 2}, "plant"},
				{{0, 0, 0}, {2, 1, 2}, "plant", Orientation::PositiveX}}});
		return result;
	}

	void appendCell(voxel::Chunk::Collection &chunks, const Catalog &catalog, const Cell &cell)
	{
		voxel::Chunk *chunk = chunks.find(cell.chunk);
		if (chunk == nullptr)
		{
			auto created = std::make_unique<voxel::Chunk>(cell.chunk);
			chunk = created.get();
			require(chunks.request(cell.chunk), "fixture Chunk request failed");
			chunks.publish(std::move(created));
		}
		chunk->edit().set(cell.local, voxel::Voxel::Cell(catalog.id(std::string(cell.definition)), cell.orientation, cell.flip));
	}

	void configureCamera(const Camera &fixture, spk::RenderContext &context)
	{
		spk::Camera camera;
		camera.configure(spk::Camera::PerspectiveConfiguration{
			fixture.verticalFieldOfViewRadians, fixture.nearPlane, fixture.farPlane});
		camera.setViewportSize(FramebufferSize);
		spk::Entity3D entity("Golden camera");
		entity.transform().place(fixture.position);
		entity.transform().setRotation(spk::Quaternion::lookAt(fixture.position, fixture.target));
		spk::CameraUBORenderCommand(camera.projection(), entity.transform().inverseModelMatrix()).execute(context);
	}

	void renderChunks(
		const Catalog &catalog,
		const voxel::Chunk::Collection &chunks,
		const std::vector<voxel::Chunk::Coordinate> &coordinates,
		spk::RenderContext &context,
		bool alterFirstUv)
	{
		voxel::Chunk::Baker baker(catalog, chunks);
		bool alterationPending = alterFirstUv;
		for (const auto coordinate : coordinates)
		{
			const voxel::Chunk *chunk = chunks.find(coordinate);
			require(chunk != nullptr, "render fixture references an absent Chunk");
			auto mesh = baker.bake(*chunk);
			if (alterationPending)
			{
				spk::TextureMesh3D::Builder altered;
				const auto vertices = mesh.layout().vertexBuffer().cast<spk::Texture3DVertex>();
				const auto indices = mesh.layout().indexBuffer().cast<spk::TextureMesh3D::Index>();
				for (std::size_t index = 0; index < vertices.size(); ++index)
				{
					auto vertex = vertices[index];
					if (index == 0) vertex.uv.x += 0.25f;
					static_cast<void>(altered.addVertex(vertex));
				}
				for (const auto index : indices) altered.addIndex(index);
				mesh = std::move(altered).build();
				alterationPending = false;
			}
			const auto origin = voxel::Chunk::worldOrigin(coordinate);
			spk::TextureMesh3DRenderCommand(&catalog.atlas(), mesh, spk::Matrix4x4::translation(spk::Vector3(origin))).execute(context);
		}
	}

	void prepareContext(sparkle_test::OpenGLTestContext &openGL)
	{
		openGL.reset();
		auto &context = openGL.renderContext();
		::glViewport(0, 0, static_cast<GLsizei>(FramebufferSize.x), static_cast<GLsizei>(FramebufferSize.y));
		::glClearColor(0.30f, 0.48f, 0.72f, 1.0f);
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		require(::glGetError() == GL_NO_ERROR, "failed to prepare deterministic OpenGL state");
	}

	void reportCanonicalRenderer()
	{
		auto &openGL = sparkle_test::OpenGLTestContext::instance();
		openGL.makeCurrent();
		std::cout << "OpenGL vendor: " << reinterpret_cast<const char *>(::glGetString(GL_VENDOR)) << '\n'
			<< "OpenGL renderer: " << reinterpret_cast<const char *>(::glGetString(GL_RENDERER)) << '\n'
			<< "OpenGL version: " << reinterpret_cast<const char *>(::glGetString(GL_VERSION)) << '\n';
	}

	std::vector<voxel::Chunk::Coordinate> sceneCoordinates(const Scene &scene)
	{
		std::vector<voxel::Chunk::Coordinate> result;
		for (const auto &cell : scene.cells)
		{
			if (std::ranges::find(result, cell.chunk) == result.end()) result.push_back(cell.chunk);
		}
		return result;
	}

	void renderScene(const Scene &scene, const std::filesystem::path &path, bool alterFirstUv = false)
	{
		auto catalog = loadCatalog();
		voxel::Chunk::Collection chunks;
		for (const auto &cell : scene.cells) appendCell(chunks, catalog, cell);
		auto &openGL = sparkle_test::OpenGLTestContext::instance();
		prepareContext(openGL);
		auto &context = openGL.renderContext();
		configureCamera(scene.camera, context);
		renderChunks(catalog, chunks, sceneCoordinates(scene), context, alterFirstUv);
		openGL.save(path);
	}

	void renderSeededScene(const std::filesystem::path &path)
	{
		auto catalog = loadCatalog();
		voxel::Chunk::Collection chunks;
		voxel::Chunk::DebugGenerator generator(chunks, catalog, 0xE7E11AULL);
		const std::vector<voxel::Chunk::Coordinate> coordinates{{-1, 0, -1}, {0, 0, -1}, {-1, 0, 0}, {0, 0, 0}};
		for (const auto coordinate : coordinates) require(chunks.request(coordinate), "seeded Chunk request failed");
		auto &openGL = sparkle_test::OpenGLTestContext::instance();
		prepareContext(openGL);
		auto &context = openGL.renderContext();
		configureCamera({{26.0f, 24.0f, 31.0f}, {0.0f, 5.0f, 0.0f}, 0.90f, 0.1f, 150.0f}, context);
		renderChunks(catalog, chunks, coordinates, context, false);
		openGL.save(path);
	}

	sparkle_test::ImageComparisonOptions comparisonOptions()
	{
		sparkle_test::ImageComparisonOptions options;
		require(options.rgbTolerance == 4, "Sparkle RGB tolerance default changed");
		require(options.alphaTolerance == 8, "Sparkle alpha tolerance default changed");
		require(options.transparentAlphaThreshold == 4, "Sparkle transparent-alpha threshold default changed");
		return options;
	}

	bool compareCandidate(std::string_view name, const std::filesystem::path &actual)
	{
		const auto expected = sparkle_test::expectedImagePath(Category, std::string(name));
		const auto difference = sparkle_test::resultImagePath(Category, std::string(name) + "_difference");
		if (!std::filesystem::is_regular_file(expected))
		{
			std::cerr << "Missing approved reference: " << expected << "\nCandidate preserved: " << actual << '\n';
			return false;
		}
		const auto result = sparkle_test::compareImages(actual, expected, difference, comparisonOptions());
		if (!result.matches)
		{
			std::cerr << "Fixture " << name << " differs: " << result.differentPixelCount << " pixels; actual "
				<< result.actualWidth << 'x' << result.actualHeight << "; expected " << result.expectedWidth << 'x' << result.expectedHeight << '\n';
		}
		return result.matches;
	}

	const Scene &scene(std::string_view name)
	{
		static const auto scenes = declaredScenes();
		const auto found = std::ranges::find(scenes, name, &Scene::name);
		if (found == scenes.end())
		{
			throw std::logic_error("unknown current Chunk golden fixture");
		}
		return *found;
	}

	bool compareScene(std::string_view name)
	{
		const auto actual = sparkle_test::resultImagePath(Category, std::string(name) + "_actual");
		renderScene(scene(name), actual);
		return compareCandidate(name, actual);
	}

	bool compareSeededScene()
	{
		const auto actual = sparkle_test::resultImagePath(Category, "seeded_debug_chunks_actual");
		renderSeededScene(actual);
		return compareCandidate("seeded_debug_chunks", actual);
	}

	void proveUvSensitivity()
	{
		const auto &fixture = scene("solid_cube_boundary");
		const auto temporaryReference = sparkle_test::resultImagePath("sensitivity", "temporary_reference");
		const auto unchanged = sparkle_test::resultImagePath("sensitivity", "unchanged_actual");
		const auto altered = sparkle_test::resultImagePath("sensitivity", "altered_uv_actual");
		const auto restored = sparkle_test::resultImagePath("sensitivity", "restored_actual");
		const auto difference = sparkle_test::resultImagePath("sensitivity", "altered_uv_difference");
		renderScene(fixture, unchanged);
		std::filesystem::create_directories(temporaryReference.parent_path());
		std::filesystem::copy_file(unchanged, temporaryReference, std::filesystem::copy_options::overwrite_existing);
		require(sparkle_test::compareImages(unchanged, temporaryReference, sparkle_test::resultImagePath("sensitivity", "unchanged_difference"), comparisonOptions()).matches,
			"unchanged sensitivity fixture did not match");
		renderScene(fixture, altered, true);
		require(!sparkle_test::compareImages(altered, temporaryReference, difference, comparisonOptions()).matches,
			"controlled UV alteration was not detected");
		require(std::filesystem::is_regular_file(altered) && std::filesystem::is_regular_file(difference),
			"UV mismatch artifacts were not preserved");
		renderScene(fixture, restored);
		require(sparkle_test::compareImages(restored, temporaryReference, sparkle_test::resultImagePath("sensitivity", "restored_difference"), comparisonOptions()).matches,
			"restored sensitivity fixture did not match");
	}

	class CurrentChunkGoldenTest : public ::testing::Test
	{
	protected:
		static void SetUpTestSuite()
		{
			sparkle_test::configurePaths(PLAYGROUND_TEST_RESOURCE_DIR, PLAYGROUND_TEST_RESULT_DIR);
			reportCanonicalRenderer();
		}
	};
}

TEST_F(CurrentChunkGoldenTest, SolidCubeBoundary)
{
	EXPECT_TRUE(compareScene("solid_cube_boundary"));
}

TEST_F(CurrentChunkGoldenTest, SlabPartialOcclusion)
{
	EXPECT_TRUE(compareScene("slab_partial_occlusion"));
}

TEST_F(CurrentChunkGoldenTest, SlopeAndStairTransforms)
{
	EXPECT_TRUE(compareScene("slope_stair_transforms"));
}

TEST_F(CurrentChunkGoldenTest, MaterialsAndVegetation)
{
	EXPECT_TRUE(compareScene("materials_and_vegetation"));
}

TEST_F(CurrentChunkGoldenTest, SeededDebugChunks)
{
	EXPECT_TRUE(compareSeededScene());
}

TEST_F(CurrentChunkGoldenTest, DetectsControlledUvRegression)
{
	EXPECT_NO_THROW(proveUvSensitivity());
}
