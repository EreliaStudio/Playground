#include "voxel/chunk_editor.hpp"

#include <stdexcept>
#include <utility>

namespace voxel
{
	Chunk::Editor::Editor(Editor &&other) noexcept :
		_chunk(std::exchange(other._chunk, nullptr)),
		_changed(other._changed)
	{
	}

	Chunk::Editor::~Editor()
	{
		commit();
	}

	bool Chunk::Editor::set(spk::Vector3Int local, Voxel::Cell value)
	{
		if (!_chunk)
		{
			throw std::logic_error("chunk editor was committed");
		}
		auto &cell = _chunk->_cells[Chunk::index(local)];
		if (cell == value)
		{
			return false;
		}
		cell = value;
		_changed = true;
		return true;
	}

	void Chunk::Editor::commit()
	{
		if (!_chunk)
		{
			return;
		}
		auto *chunk = std::exchange(_chunk, nullptr);
		if (_changed)
		{
			chunk->invalidate();
		}
	}
}
