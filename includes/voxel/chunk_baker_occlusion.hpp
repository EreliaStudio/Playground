#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "voxel/chunk_baker.hpp"

namespace voxel
{
	class Chunk::Baker::OcclusionCache final
	{
	public:
		[[nodiscard]] const std::vector<Voxel::Shape::Polygon> &visibleRemnants(
			const Voxel::Shape::Polygon &polygon,
			Voxel::Cell cell,
			const Voxel::Shape &occludingShape,
			Voxel::Cell occludingCell);

	private:
		struct Key
		{
			const Voxel::Shape::Polygon *polygon = nullptr;
			const Voxel::Shape *occludingShape = nullptr;
			std::uint32_t transform = 0;
			std::uint32_t occludingTransform = 0;
			bool operator==(const Key &) const noexcept = default;
		};

		struct KeyHash
		{
			[[nodiscard]] std::size_t operator()(const Key &key) const noexcept;
		};

		[[nodiscard]] static std::vector<Voxel::Shape::Polygon> _computeVisibleRemnants(
			const Voxel::Shape::Polygon &polygon,
			Voxel::Cell cell,
			const Voxel::Shape &occludingShape,
			Voxel::Cell occludingCell);

		std::unordered_map<Key, std::vector<Voxel::Shape::Polygon>, KeyHash> _visibleRemnants;
	};
}
