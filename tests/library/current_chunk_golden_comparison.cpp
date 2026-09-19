#include "current_chunk_golden_fixture.hpp"

#include <sparkle_test.hpp>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace playground_test::golden
{
	namespace
	{
		constexpr std::string_view Category = "current_chunk";

		void require(bool condition, std::string_view message)
		{
			if (!condition) throw std::runtime_error(std::string(message));
		}

		sparkle_test::ImageComparisonOptions comparisonOptions()
		{
			sparkle_test::ImageComparisonOptions options;
			require(options.rgbTolerance == 4, "Sparkle RGB tolerance default changed");
			require(options.alphaTolerance == 8, "Sparkle alpha tolerance default changed");
			require(options.transparentAlphaThreshold == 4, "Sparkle transparent-alpha threshold default changed");
			return options;
		}

		bool compareCandidate(std::string_view name, const std::filesystem::path &actual)
		{
			const auto expected = sparkle_test::expectedImagePath(Category, std::string(name));
			const auto difference = sparkle_test::resultImagePath(Category, std::string(name) + "_difference");
			if (!std::filesystem::is_regular_file(expected))
			{
				std::cerr << "Missing approved reference: " << expected << "\nCandidate preserved: " << actual << '\n';
				return false;
			}
			const auto result = sparkle_test::compareImages(actual, expected, difference, comparisonOptions());
			if (!result.matches)
			{
				std::cerr << "Fixture " << name << " differs: " << result.differentPixelCount << " pixels; actual "
					<< result.actualWidth << 'x' << result.actualHeight << "; expected "
					<< result.expectedWidth << 'x' << result.expectedHeight << '\n';
			}
			return result.matches;
		}
	}

	bool compareScene(std::string_view name)
	{
		const auto actual = sparkle_test::resultImagePath(Category, std::string(name) + "_actual");
		renderScene(scene(name), actual);
		return compareCandidate(name, actual);
	}

	bool compareSeededScene()
	{
		const auto actual = sparkle_test::resultImagePath(Category, "seeded_debug_chunks_actual");
		renderSeededScene(actual);
		return compareCandidate("seeded_debug_chunks", actual);
	}

	void proveUvSensitivity()
	{
		const auto &fixture = scene("solid_cube_boundary");
		const auto temporaryReference = sparkle_test::resultImagePath("sensitivity", "temporary_reference");
		const auto unchanged = sparkle_test::resultImagePath("sensitivity", "unchanged_actual");
		const auto altered = sparkle_test::resultImagePath("sensitivity", "altered_uv_actual");
		const auto restored = sparkle_test::resultImagePath("sensitivity", "restored_actual");
		const auto difference = sparkle_test::resultImagePath("sensitivity", "altered_uv_difference");
		renderScene(fixture, unchanged);
		std::filesystem::create_directories(temporaryReference.parent_path());
		std::filesystem::copy_file(unchanged, temporaryReference, std::filesystem::copy_options::overwrite_existing);
		require(sparkle_test::compareImages(unchanged, temporaryReference,
			sparkle_test::resultImagePath("sensitivity", "unchanged_difference"), comparisonOptions()).matches,
			"unchanged sensitivity fixture did not match");
		renderScene(fixture, altered, true);
		require(!sparkle_test::compareImages(altered, temporaryReference, difference, comparisonOptions()).matches,
			"controlled UV alteration was not detected");
		require(std::filesystem::is_regular_file(altered) && std::filesystem::is_regular_file(difference),
			"UV mismatch artifacts were not preserved");
		renderScene(fixture, restored);
		require(sparkle_test::compareImages(restored, temporaryReference,
			sparkle_test::resultImagePath("sensitivity", "restored_difference"), comparisonOptions()).matches,
			"restored sensitivity fixture did not match");
	}
}
