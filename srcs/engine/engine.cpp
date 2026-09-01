#include "engine/engine.hpp"

#include <utility>

namespace spk
{
	void Engine::addEntity(Entity *entity)
	{
		_root.addChild(entity);
	}

	void Engine::removeEntity(Entity *entity)
	{
		_root.removeChild(entity);
	}
}
