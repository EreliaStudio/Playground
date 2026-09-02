#include "voxel/chunk_bake_scheduler.hpp"

#include <utility>

namespace voxel
{
	Chunk::BakeScheduler::BakeScheduler(Collection &chunks, const Baker &baker) :
		_chunks(chunks),
		_baker(baker),
		_available(chunks.subscribeToAvailability([this](Chunk &chunk) {
			_observe(chunk);
			invalidate(chunk.coordinate());
			_invalidateNeighbors(chunk.coordinate());
		})),
		_removed(chunks.subscribeToRemoval([this](Coordinate coordinate) {
			_versions.erase(coordinate);
			_dirty.erase(coordinate);
			_invalidateNeighbors(coordinate);
		}))
	{
	}

	void Chunk::BakeScheduler::_observe(Chunk &chunk)
	{
		const Coordinate coordinate = chunk.coordinate();
		_versions.insert_or_assign(coordinate, chunk.subscribeToVersionEdition([this, coordinate](spk::VersionedTrait *) {
			invalidate(coordinate);
			_invalidateNeighbors(coordinate);
		}));
	}

	void Chunk::BakeScheduler::invalidate(Coordinate coordinate)
	{
		if (_chunks.find(coordinate))
		{
			_dirty.insert(coordinate);
		}
	}

	void Chunk::BakeScheduler::_invalidateNeighbors(Coordinate coordinate)
	{
		invalidate({coordinate.x - 1, coordinate.y, coordinate.z});
		invalidate({coordinate.x + 1, coordinate.y, coordinate.z});
		invalidate({coordinate.x, coordinate.y - 1, coordinate.z});
		invalidate({coordinate.x, coordinate.y + 1, coordinate.z});
		invalidate({coordinate.x, coordinate.y, coordinate.z - 1});
		invalidate({coordinate.x, coordinate.y, coordinate.z + 1});
	}

	std::size_t Chunk::BakeScheduler::process(std::size_t budget)
	{
		std::size_t count = 0;
		while (count < budget && !_dirty.empty())
		{
			const auto coordinate = *_dirty.begin();
			_dirty.erase(_dirty.begin());
			if (auto *chunk = _chunks.find(coordinate))
			{
				const spk::TextureMesh3D mesh = _baker.bake(*chunk);
				_bakeCompletions.trigger(coordinate, mesh);
				++count;
			}
		}
		return count;
	}

	Chunk::BakeScheduler::BakeCompletionProvider::Contract Chunk::BakeScheduler::subscribeToBakeCompletion(BakeCompletionProvider::callback_type callback)
	{
		return _bakeCompletions.subscribe(std::move(callback));
	}
}
