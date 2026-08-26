#pragma once

#include <string>

namespace spk
{
	class Engine
	{
	private:
		std::string _name;

	public:
		explicit Engine(std::string name);

		[[nodiscard]] const std::string &name() const;
	};
}
