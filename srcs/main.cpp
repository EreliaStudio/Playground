#include <sparkle>

#include <filesystem>
#include <utility>

#include "engine/camera2d.hpp"
#include "engine/color_mesh_renderer2d.hpp"
#include "engine/engine.hpp"
#include "engine/entity2d.hpp"
#include "engine/sprite_renderer2d.hpp"
#include "engine/transform2d.hpp"
#include "ui/widget/engine_widget.hpp"

namespace
{
	spk::TextureMesh2D spriteQuad(
		const spk::SpriteSheet &spriteSheet,
		const spk::Vector2UInt &sprite,
		const spk::Vector2 &size)
	{
		const spk::Texture::Section &uv = spriteSheet.sprite(sprite);
		const spk::Vector2 halfSize{size.x * 0.5f, size.y * 0.5f};

		spk::TextureMesh2D::Builder builder;
		builder.addShape(
			{{-halfSize.x, -halfSize.y}, 0.0f, uv.anchor},
			{{-halfSize.x, halfSize.y}, 0.0f, {uv.anchor.x, uv.anchor.y + uv.size.y}},
			{{halfSize.x, halfSize.y}, 0.0f, uv.anchor + uv.size},
			{{halfSize.x, -halfSize.y}, 0.0f, {uv.anchor.x + uv.size.x, uv.anchor.y}});
		return std::move(builder).build();
	}

	spk::ColorMesh2D colorQuad(const spk::Vector2 &size, const spk::Color &color)
	{
		const spk::Vector2 halfSize{size.x * 0.5f, size.y * 0.5f};

		spk::ColorMesh2D::Builder builder;
		builder.addShape(
			{{-halfSize.x, -halfSize.y}, 0.0f, color},
			{{-halfSize.x, halfSize.y}, 0.0f, color},
			{{halfSize.x, halfSize.y}, 0.0f, color},
			{{halfSize.x, -halfSize.y}, 0.0f, color});
		return std::move(builder).build();
	}
}

int main()
{
	spk::Application application;
	spk::Window &window = application.createWindow(
		"playground",
		spk::Window::Configuration{
			.title = "Sparkle engine playground",
			.area = spk::Rect2D{spk::Vector2Int{120, 80}, spk::Vector2UInt{960, 640}},
			.backgroundColor = {0.04f, 0.055f, 0.085f, 1.0f}});

	window.root().setGeometry(spk::Rect2D{spk::Vector2Int{0, 0}, spk::Vector2UInt{960, 640}});

	spk::Engine engine;
	spk::EngineWidget engineWidget("Engine view", &window.root());
	engineWidget.setGeometry(spk::Rect2D{spk::Vector2Int{0, 0}, spk::Vector2UInt{960, 640}});
	engineWidget.setEngine(&engine);
	engineWidget.activate();

	spk::SpriteSheet spriteSheet = spk::SpriteSheet::open(
		std::filesystem::path("resources") / "textures" / "meshSpriteSheet.png",
		{4, 4});

	spk::Entity2D floor("Floor");
	floor.transform().place({0.0f, 145.0f});
	floor.transform().setDepth(-0.35f);
	auto &floorRenderer = floor.addParticipant<spk::ColorMeshRenderer2D>("Floor renderer");
	floorRenderer.setMesh(colorQuad({760.0f, 230.0f}, {0.16f, 0.32f, 0.22f, 1.0f}));
	engine.addEntity(&floor);

	spk::Entity2D player("Player");
	player.transform().place({0.0f, 0.0f});
	player.transform().setDepth(0.25f);
	(void)player.addParticipant<spk::Camera2D>("Player camera");
	auto &playerRenderer = player.addParticipant<spk::SpriteRenderer2D>("Player renderer");
	playerRenderer.setSpriteSheet(&spriteSheet);
	playerRenderer.setMesh(spriteQuad(spriteSheet, {0, 0}, {112.0f, 112.0f}));
	engine.addEntity(&player);

	spk::Entity2D secondObject("Second sprite");
	secondObject.transform().place({210.0f, -45.0f});
	secondObject.transform().setDepth(0.10f);
	secondObject.transform().rescale({1.25f, 1.25f});
	auto &secondRenderer = secondObject.addParticipant<spk::SpriteRenderer2D>("Second sprite renderer");
	secondRenderer.setSpriteSheet(&spriteSheet);
	secondRenderer.setMesh(spriteQuad(spriteSheet, {1, 0}, {96.0f, 96.0f}));
	engine.addEntity(&secondObject);

	return application.run();
}
