#include "engine/engine.hpp"

#include <utility>

namespace spk
{
	Engine::Engine(std::string name) :
		_name(std::move(name))
	{
	}

	const std::string &Engine::name() const
	{
		return _name;
	}
}
