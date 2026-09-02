#include <sparkle>

#include "engine/camera.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace
{
	bool approximately(float value, float expected)
	{
		return std::abs(value - expected) < 0.0001f;
	}
}

int main()
{
	spk::Camera camera;
	camera.configure(spk::Camera::PerspectiveConfiguration{.verticalFieldOfViewRadians = 1.0f, .nearPlane = 0.1f, .farPlane = 500.0f});
	camera.setViewportSize({1600, 900});

	const spk::Matrix4x4 perspective = camera.projection();
	const float aspectRatio = 1600.0f / 900.0f;
	assert(approximately(perspective[0][0], perspective[1][1] / aspectRatio));

	camera.setViewportSize({800, 800});
	const spk::Matrix4x4 squarePerspective = camera.projection();
	assert(approximately(squarePerspective[0][0], squarePerspective[1][1]));

	camera.configure(spk::Camera::OrthographicConfiguration{.verticalSize = 20.0f, .nearPlane = 0.1f, .farPlane = 500.0f});
	camera.setViewportSize({1600, 800});
	const spk::Matrix4x4 orthographic = camera.projection();
	assert(approximately(orthographic[0][0], 0.05f));
	assert(approximately(orthographic[1][1], 0.1f));

	bool rejectedEmptyViewport = false;
	try
	{
		camera.setViewportSize({1600, 0});
	} catch (const std::invalid_argument &)
	{
		rejectedEmptyViewport = true;
	}
	assert(rejectedEmptyViewport);
	assert(camera.viewportSize() == spk::Vector2UInt(1600, 800));

	spk::Engine engine;
	spk::Camera replacement;
	camera.setAsMain(engine);
	assert(camera.isMain());
	assert(spk::Camera::mainCamera(&engine) == &camera);
	replacement.setAsMain(engine);
	assert(!camera.isMain());
	assert(replacement.isMain());
	assert(spk::Camera::mainCamera(&engine) == &replacement);

	std::cout << "camera tests passed\n";
	return 0;
}
