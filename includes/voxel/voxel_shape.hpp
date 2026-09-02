#pragma once

#include <array>
#include <set>
#include <string>
#include <vector>

#include <sparkle>

#include "voxel/voxel.hpp"

namespace voxel::Voxel
{
	struct Shape
	{
		struct Vertex
		{
			spk::Vector3 position;
			spk::Vector2 uv;
		};

		struct Polygon
		{
			std::vector<Vertex> vertices;
			std::string materialSlot;
			spk::Vector3 normal;
			Side outerSide = Side::None;
			bool fullCoverage = false;
		};

		std::string name;
		std::vector<Polygon> polygons;
		std::set<std::string> materialSlots;
		std::array<bool, 6> coversSide{};
	};
}
