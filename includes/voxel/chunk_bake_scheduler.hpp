#pragma once

#include <limits>
#include <set>
#include <unordered_map>

#include "voxel/chunk_baker.hpp"
#include "voxel/chunk_coordinate_hash.hpp"

namespace voxel
{
	class Chunk::BakeScheduler final
	{
	public:
		using BakeCompletionProvider = spk::ContractProvider<Coordinate, const spk::TextureMesh3D &>;

		BakeScheduler(Collection &chunks, const Baker &baker);
		void invalidate(Coordinate coordinate);
		std::size_t process(std::size_t budget = std::numeric_limits<std::size_t>::max());
		[[nodiscard]] BakeCompletionProvider::Contract subscribeToBakeCompletion(BakeCompletionProvider::callback_type callback);
		[[nodiscard]] std::size_t pendingCount() const noexcept
		{
			return _dirty.size();
		}

	private:
		void _observe(Chunk &chunk);
		void _invalidateNeighbors(Coordinate coordinate);
		Collection &_chunks;
		const Baker &_baker;
		BakeCompletionProvider _bakeCompletions;
		std::set<Coordinate> _dirty;
		Collection::ChunkProvider::Contract _available;
		Collection::CoordinateProvider::Contract _removed;
		std::unordered_map<Coordinate, spk::VersionedTrait::Contract, CoordinateHash> _versions;
	};
}
