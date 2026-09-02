#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include "voxel/voxel_definition.hpp"

namespace voxel::Voxel
{
	class Catalog final
	{
	public:
		static Catalog load(const std::filesystem::path &shapesFile, const std::filesystem::path &definitionsFile, spk::Vector2UInt atlasSize);
		[[nodiscard]] const Definition &definition(ID id) const;
		[[nodiscard]] const Definition &definition(const std::string &name) const;
		[[nodiscard]] ID id(const std::string &name) const;
		[[nodiscard]] spk::Vector2UInt atlasSize() const noexcept
		{
			return _atlasSize;
		}
		[[nodiscard]] std::size_t size() const noexcept
		{
			return _definitions.size() - 1;
		}

	private:
		spk::Vector2UInt _atlasSize{};
		std::unordered_map<std::string, Shape> _shapes;
		std::vector<Definition> _definitions{1};
		std::unordered_map<std::string, ID> _ids;
	};
}
