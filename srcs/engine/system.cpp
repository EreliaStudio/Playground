#include "engine/system.hpp"

namespace spk
{
	System::System(const std::string &name, Engine *engine) :
		NameTrait(name)
	{
		activate();
		attach(engine);
	}

	System::System(Engine *engine) :
		System("Unnamed system", engine)
	{
	}

	void System::attach(Engine *engine)
	{
		changeContext(engine);
	}

	Engine *System::engine()
	{
		return context();
	}

	const Engine *System::engine() const
	{
		return context();
	}
}
