#pragma once

#include "voxel/voxel_world.hpp"

#include <filesystem>
#include <string_view>
#include <vector>

namespace playground_test::golden
{
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

	[[nodiscard]] const Scene &scene(std::string_view name);
	void configureTestPaths();
	void renderScene(const Scene &scene, const std::filesystem::path &path, bool alterFirstUv = false);
	void renderSeededScene(const std::filesystem::path &path);
	[[nodiscard]] bool compareScene(std::string_view name);
	[[nodiscard]] bool compareSeededScene();
	void proveUvSensitivity();
}
