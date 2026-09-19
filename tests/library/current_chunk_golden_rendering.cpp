#include "current_chunk_golden_fixture.hpp"

#include "engine/camera.hpp"
#include "rendering/command/camera_ubo_render_command.hpp"
#include "rendering/command/texture_mesh_3d_render_command.hpp"
#include "voxel/chunk_baker.hpp"
#include "voxel/chunk_editor.hpp"
#include "voxel/debug_chunk_generator.hpp"

#include <GL/glew.h>
#include <sparkle_test.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace playground_test::golden
{
	namespace
	{
		constexpr spk::Vector2UInt FramebufferSize{640, 480};
		using Catalog = voxel::Voxel::Catalog<voxel::Voxel::Definition>;

		void require(bool condition, std::string_view message)
		{
			if (!condition) throw std::runtime_error(std::string(message));
		}

		Catalog loadCatalog() { return Catalog::load("resources/catalog_config.json"); }

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

		void prepareContext(sparkle_test::OpenGLTestContext &openGL)
		{
			openGL.reset();
			::glViewport(0, 0, static_cast<GLsizei>(FramebufferSize.x), static_cast<GLsizei>(FramebufferSize.y));
			::glClearColor(0.30f, 0.48f, 0.72f, 1.0f);
			::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			require(::glGetError() == GL_NO_ERROR, "failed to prepare deterministic OpenGL state");
		}

		std::vector<voxel::Chunk::Coordinate> sceneCoordinates(const Scene &fixture)
		{
			std::vector<voxel::Chunk::Coordinate> result;
			for (const auto &cell : fixture.cells)
				if (std::ranges::find(result, cell.chunk) == result.end()) result.push_back(cell.chunk);
			return result;
		}

		void renderChunks(const Catalog &catalog, const voxel::Chunk::Collection &chunks,
			const std::vector<voxel::Chunk::Coordinate> &coordinates, spk::RenderContext &context, bool alterFirstUv)
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
				spk::TextureMesh3DRenderCommand(&catalog.atlas(), mesh,
					spk::Matrix4x4::translation(spk::Vector3(origin))).execute(context);
			}
		}
	}

	void configureTestPaths()
	{
		sparkle_test::configurePaths(PLAYGROUND_TEST_RESOURCE_DIR, PLAYGROUND_TEST_RESULT_DIR);
		auto &openGL = sparkle_test::OpenGLTestContext::instance();
		openGL.makeCurrent();
		std::cout << "OpenGL vendor: " << reinterpret_cast<const char *>(::glGetString(GL_VENDOR)) << '\n'
			<< "OpenGL renderer: " << reinterpret_cast<const char *>(::glGetString(GL_RENDERER)) << '\n'
			<< "OpenGL version: " << reinterpret_cast<const char *>(::glGetString(GL_VERSION)) << '\n';
	}

	void renderScene(const Scene &fixture, const std::filesystem::path &path, bool alterFirstUv)
	{
		auto catalog = loadCatalog();
		voxel::Chunk::Collection chunks;
		for (const auto &cell : fixture.cells) appendCell(chunks, catalog, cell);
		auto &openGL = sparkle_test::OpenGLTestContext::instance();
		prepareContext(openGL);
		auto &context = openGL.renderContext();
		configureCamera(fixture.camera, context);
		renderChunks(catalog, chunks, sceneCoordinates(fixture), context, alterFirstUv);
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
}
