#pragma once

#include "voxel/chunk.hpp"

namespace voxel
{
	class Chunk::Editor final
	{
	private:
		Chunk *_chunk;
		bool _changed = false;

		friend class Chunk;

		explicit Editor(Chunk &chunk) :
			_chunk(&chunk)
		{
		}

	public:
		Editor(Editor &&other) noexcept;
		Editor(const Editor &) = delete;

		~Editor();

		Editor &operator=(const Editor &) = delete;

		bool set(spk::Vector3Int local, Voxel::Cell value);
		void commit();
	};
}
