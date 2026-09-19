#include "current_chunk_golden_fixture.hpp"

#include <algorithm>
#include <stdexcept>

namespace playground_test::golden
{
	namespace
	{
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
	}

	const Scene &scene(std::string_view name)
	{
		static const auto scenes = declaredScenes();
		const auto found = std::ranges::find(scenes, name, &Scene::name);
		if (found == scenes.end()) throw std::logic_error("unknown current Chunk golden fixture");
		return *found;
	}
}
