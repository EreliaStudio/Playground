#pragma once

#include <memory>

#include "voxel/chunk_collection.hpp"

namespace voxel
{
	class Chunk::Generator
	{
	public:
		explicit Generator(Collection &collection);
		virtual ~Generator() = default;

	protected:
		[[nodiscard]] virtual std::unique_ptr<Chunk> generate(Coordinate coordinate) const = 0;
		Collection &_collection;

	private:
		Collection::CoordinateProvider::Contract _requests;
	};
}
