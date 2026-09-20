#include "current_chunk_golden_fixture.hpp"

#include <sparkle_test.hpp>

#include <filesystem>
#include <string>
#include <string_view>

#include <exception.hpp>

namespace playground_test::golden
{
	namespace
	{
		constexpr std::string_view PaletteCategory = "palette_migration";

		void preservePaletteCandidate(std::string_view name, const std::filesystem::path &actual)
		{
			const auto candidate = std::filesystem::path(PLAYGROUND_TEST_EXPECTED_CANDIDATE_DIR) /
				PaletteCategory / (std::string(name) + ".png");
			std::filesystem::create_directories(candidate.parent_path());
			std::filesystem::copy_file(actual, candidate, std::filesystem::copy_options::overwrite_existing);
		}
	}

	void capturePaletteMigrationCandidates()
	{
		const auto human = sparkle_test::resultImagePath(PaletteCategory, "human_palette_model_actual");
		const auto orc = sparkle_test::resultImagePath(PaletteCategory, "orc_palette_model_actual");
		const auto pair = sparkle_test::resultImagePath(PaletteCategory, "human_and_orc_palettes_actual");
		const auto world = sparkle_test::resultImagePath(PaletteCategory, "world_palette_chunk_actual");
		renderPaletteModel(false, human);
		renderPaletteModel(true, orc);
		renderPaletteModelPair(pair);
		renderWorldPaletteChunk(world);
		preservePaletteCandidate("human_palette_model", human);
		preservePaletteCandidate("orc_palette_model", orc);
		preservePaletteCandidate("human_and_orc_palettes", pair);
		preservePaletteCandidate("world_palette_chunk", world);
		const auto oldExpected = sparkle_test::expectedImagePath("voxel_mesher", "json_cross_statue");
		const auto difference = sparkle_test::resultImagePath(PaletteCategory, "textured_to_palette_difference");
		if (!std::filesystem::is_regular_file(oldExpected))
			throw spk::Exception("approved textured reference is missing");
		if (sparkle_test::compareImages(human, oldExpected, difference).matches)
			throw spk::Exception("Palette candidate unexpectedly matches the textured reference");
		if (!std::filesystem::is_regular_file(human) || !std::filesystem::is_regular_file(difference))
			throw spk::Exception("Palette migration evidence was not preserved");
	}
}
