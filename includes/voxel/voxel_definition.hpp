#pragma once

#include <string>
#include <unordered_map>

#include <sparkle>

#include "voxel/voxel_shape.hpp"

namespace voxel::Voxel
{
	struct Definition
	{
		ID id = 0;
		std::string name;
		const Shape *shape = nullptr;
		std::unordered_map<std::string, spk::Vector2UInt> textures;
	};
}
