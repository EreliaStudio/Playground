#include "engine/component.hpp"

#include "engine/entity.hpp"

namespace spk
{
	Component::Component(const std::string &name, Entity *owner) :
		NameTrait(name)
	{
		activate();
		attach(owner);
	}

	Component::Component(Entity *owner) :
		Component("Unnamed component", owner)
	{
	}

	void Component::attach(Entity *owner)
	{
		if (_owner == owner)
		{
			return;
		}

		_onOwnerContextEditionContract.resign();
		_owner = owner;

		if (_owner == nullptr)
		{
			changeContext(nullptr);
			return;
		}

		changeContext(_owner->context());

		_onOwnerContextEditionContract = _owner->subscribeToContextEdition(
			[this](Engine *const &, Engine *const &newContext) {
				changeContext(newContext);
			});
	}

	Entity *Component::owner()
	{
		return _owner;
	}

	const Entity *Component::owner() const
	{
		return _owner;
	}
}
