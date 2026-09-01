#pragma once

#include <string>

#include <sparkle>

#include "engine/entity.hpp"

namespace spk
{
	class Engine
	{
	private:
		Entity _root;

	public:
		void addEntity(Entity *entity);
		void removeEntity(Entity *entity);
	};
}
