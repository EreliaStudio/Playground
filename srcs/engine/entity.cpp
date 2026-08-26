#include "engine/entity.hpp"

namespace spk
{
	Entity::Entity(const std::string &name, Entity *parent) :
		NameTrait(name)
	{
		activate();
		setParent(parent);
	}
}
