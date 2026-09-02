#pragma once

#include "voxel/chunk.hpp"

namespace voxel
{
	class Chunk::Editor final
	{
	public:
		Editor(Editor &&other) noexcept;
		~Editor();
		Editor(const Editor &) = delete;
		Editor &operator=(const Editor &) = delete;
		bool set(spk::Vector3Int local, Voxel::Cell value);
		void commit();

	private:
		friend class Chunk;
		explicit Editor(Chunk &chunk) :
			_chunk(&chunk)
		{
		}
		Chunk *_chunk;
		bool _changed = false;
	};
}
