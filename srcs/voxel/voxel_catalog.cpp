#include "voxel/voxel_catalog.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "voxel/voxel_cell.hpp"

namespace voxel::Voxel
{
	namespace
	{
		using Polygon = Shape::Polygon;
		using Vertex = Shape::Vertex;

		constexpr float Epsilon = 0.0001f;

		std::size_t sideIndex(Side side)
		{
			return static_cast<std::size_t>(side);
		}

		spk::Vector3Int sideOffset(Side side)
		{
			switch (side)
			{
			case Side::NegativeX:
				return {-1, 0, 0};
			case Side::PositiveX:
				return {1, 0, 0};
			case Side::NegativeY:
				return {0, -1, 0};
			case Side::PositiveY:
				return {0, 1, 0};
			case Side::NegativeZ:
				return {0, 0, -1};
			case Side::PositiveZ:
				return {0, 0, 1};
			default:
				return {};
			}
		}

		Side boundarySide(const std::vector<Vertex> &vertices)
		{
			auto all = [&](auto predicate) {
				return std::ranges::all_of(vertices, predicate);
			};
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.x) < Epsilon;
				}))
			{
				return Side::NegativeX;
			}
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.x - 1.0f) < Epsilon;
				}))
			{
				return Side::PositiveX;
			}
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.y) < Epsilon;
				}))
			{
				return Side::NegativeY;
			}
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.y - 1.0f) < Epsilon;
				}))
			{
				return Side::PositiveY;
			}
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.z) < Epsilon;
				}))
			{
				return Side::NegativeZ;
			}
			if (all([](const auto &vertex) {
					return std::abs(vertex.position.z - 1.0f) < Epsilon;
				}))
			{
				return Side::PositiveZ;
			}
			return Side::None;
		}

		bool fullBoundary(const std::vector<Vertex> &vertices, Side side)
		{
			if (vertices.size() != 4 || side == Side::None)
			{
				return false;
			}
			float minA = 1, minB = 1, maxA = 0, maxB = 0;
			for (const auto &vertex : vertices)
			{
				const float a = (side == Side::NegativeX || side == Side::PositiveX) ? vertex.position.y : vertex.position.x;
				const float b = (side == Side::NegativeZ || side == Side::PositiveZ) ? vertex.position.y : vertex.position.z;
				minA = std::min(minA, a);
				maxA = std::max(maxA, a);
				minB = std::min(minB, b);
				maxB = std::max(maxB, b);
			}
			return minA < Epsilon && minB < Epsilon && maxA > 1 - Epsilon && maxB > 1 - Epsilon;
		}

		spk::Vector3 normalOf(const std::vector<Vertex> &vertices)
		{
			if (vertices.size() < 3)
			{
				throw std::runtime_error("voxel polygon needs at least three vertices");
			}
			return (vertices[1].position - vertices[0].position).cross(vertices[2].position - vertices[0].position).normalized();
		}

		spk::Vector3 readVector3(const spk::JSON::Value &value)
		{
			const auto &array = value.asArray();
			if (array.size() != 3)
			{
				throw std::runtime_error("position must have three numbers");
			}
			return {array[0].as<float>(), array[1].as<float>(), array[2].as<float>()};
		}

		spk::Vector2 readVector2(const spk::JSON::Value &value)
		{
			const auto &array = value.asArray();
			if (array.size() != 2)
			{
				throw std::runtime_error("UV must have two numbers");
			}
			return {array[0].as<float>(), array[1].as<float>()};
		}

		spk::Vector2UInt readAtlasCell(const spk::JSON::Value &value)
		{
			const auto &array = value.asArray();
			if (array.size() != 2)
			{
				throw std::runtime_error("atlas cell must have two integers");
			}
			return {array[0].as<std::uint32_t>(), array[1].as<std::uint32_t>()};
		}
	}

	Catalog Catalog::load(const std::filesystem::path &shapesFile, const std::filesystem::path &definitionsFile, spk::Vector2UInt atlasSize)
	{
		if (atlasSize.x == 0 || atlasSize.y == 0)
		{
			throw std::invalid_argument("voxel atlas dimensions must be non-zero");
		}
		Catalog result;
		result._atlasSize = atlasSize;
		const auto shapes = spk::JSON::Loader::parseFile(shapesFile);
		for (const auto &shapeValue : shapes.asArray())
		{
			spk::JSON::Reader reader(shapeValue, shapesFile);
			reader.forbidUnknown({"id", "polygons"});
			Shape shape;
			shape.name = reader.require<std::string>("id");
			if (shape.name.empty() || result._shapes.contains(shape.name))
			{
				throw std::runtime_error("duplicate or empty voxel shape id '" + shape.name + "'");
			}
			for (const auto &polygonValue : reader.value().at("polygons").asArray())
			{
				spk::JSON::Reader polygonReader(polygonValue, shapesFile);
				polygonReader.forbidUnknown({"slot", "vertices"});
				Polygon polygon;
				polygon.materialSlot = polygonReader.require<std::string>("slot");
				for (const auto &vertexValue : polygonReader.value().at("vertices").asArray())
				{
					spk::JSON::Reader vertexReader(vertexValue, shapesFile);
					vertexReader.forbidUnknown({"position", "uv"});
					Vertex vertex{readVector3(vertexReader.value().at("position")), readVector2(vertexReader.value().at("uv"))};
					if (vertex.position.x < -Epsilon || vertex.position.y < -Epsilon || vertex.position.z < -Epsilon || vertex.position.x > 1 + Epsilon || vertex.position.y > 1 + Epsilon || vertex.position.z > 1 + Epsilon)
					{
						throw std::runtime_error("shape '" + shape.name + "' has a vertex outside normalized cell space");
					}
					polygon.vertices.push_back(vertex);
				}
				polygon.normal = normalOf(polygon.vertices);
				polygon.outerSide = boundarySide(polygon.vertices);
				if (polygon.outerSide != Side::None && polygon.normal.dot(spk::Vector3(sideOffset(polygon.outerSide))) < 0.0f)
				{
					std::ranges::reverse(polygon.vertices);
					polygon.normal = normalOf(polygon.vertices);
				}
				polygon.fullCoverage = fullBoundary(polygon.vertices, polygon.outerSide);
				shape.materialSlots.insert(polygon.materialSlot);
				if (polygon.fullCoverage)
				{
					shape.coversSide[sideIndex(polygon.outerSide)] = true;
				}
				shape.polygons.push_back(std::move(polygon));
			}
			if (shape.polygons.empty())
			{
				throw std::runtime_error("shape '" + shape.name + "' has no polygons");
			}
			result._shapes.emplace(shape.name, std::move(shape));
		}

		const auto definitions = spk::JSON::Loader::parseFile(definitionsFile);
		for (const auto &definitionValue : definitions.asArray())
		{
			spk::JSON::Reader reader(definitionValue, definitionsFile);
			reader.forbidUnknown({"id", "shape", "textures"});
			Definition definition;
			definition.name = reader.require<std::string>("id");
			const auto shapeName = reader.require<std::string>("shape");
			if (definition.name.empty() || result._ids.contains(definition.name))
			{
				throw std::runtime_error("duplicate or empty voxel id '" + definition.name + "'");
			}
			auto shape = result._shapes.find(shapeName);
			if (shape == result._shapes.end())
			{
				throw std::runtime_error("voxel '" + definition.name + "' uses unknown shape '" + shapeName + "'");
			}
			definition.shape = &shape->second;
			for (const auto &[slot, value] : reader.value().at("textures").asObject())
			{
				if (!definition.shape->materialSlots.contains(slot))
				{
					throw std::runtime_error("voxel '" + definition.name + "' has extra texture slot '" + slot + "'");
				}
				auto cell = readAtlasCell(value);
				if (cell.x >= atlasSize.x || cell.y >= atlasSize.y)
				{
					throw std::runtime_error("voxel '" + definition.name + "' atlas cell is out of range");
				}
				definition.textures.emplace(slot, cell);
			}
			for (const auto &slot : definition.shape->materialSlots)
			{
				if (!definition.textures.contains(slot))
				{
					throw std::runtime_error("voxel '" + definition.name + "' is missing texture slot '" + slot + "'");
				}
			}
			if (result._definitions.size() > Cell::IDMask)
			{
				throw std::overflow_error("voxel catalog exceeds packed identifier capacity");
			}
			definition.id = static_cast<ID>(result._definitions.size());
			result._ids.emplace(definition.name, definition.id);
			result._definitions.push_back(std::move(definition));
		}
		return result;
	}

	const Definition &Catalog::definition(ID id) const
	{
		if (id == 0 || id >= _definitions.size())
		{
			throw std::out_of_range("unknown voxel runtime id " + std::to_string(id));
		}
		return _definitions[id];
	}

	const Definition &Catalog::definition(const std::string &name) const
	{
		return definition(id(name));
	}

	ID Catalog::id(const std::string &name) const
	{
		auto found = _ids.find(name);
		if (found == _ids.end())
		{
			throw std::out_of_range("unknown voxel '" + name + "'");
		}
		return found->second;
	}
}
