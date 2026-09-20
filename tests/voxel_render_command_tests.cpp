#include "rendering/command/voxel_render_command.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

#include <exception.hpp>

namespace
{
	voxel::VoxelMesh triangle(voxel::VoxelVertex::PaletteElementIndex elementIndex)
	{
		voxel::VoxelMesh::Builder builder;
		const auto first = builder.addVertex({{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, elementIndex});
		const auto second = builder.addVertex({{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, elementIndex});
		const auto third = builder.addVertex({{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, elementIndex});
		builder.addTriangle(first, second, third);
		return std::move(builder).build();
	}

	voxel::Palette palette(std::size_t size)
	{
		return voxel::Palette(std::vector<voxel::Palette::Data>(size, {{1.0f, 1.0f, 1.0f, 1.0f}}));
	}
}

TEST(VoxelRenderCommandTest, AcceptsBothSupportedPaletteIndexBoundaries)
{
	const auto threeElements = palette(3);

	EXPECT_NO_THROW(voxel::VoxelRenderCommand(&threeElements, triangle(0), spk::Matrix4x4::identity()));
	EXPECT_NO_THROW(voxel::VoxelRenderCommand(&threeElements, triangle(2), spk::Matrix4x4::identity()));
}

TEST(VoxelRenderCommandTest, RejectsMissingEmptyAndOutOfRangePaletteBeforeDraw)
{
	const auto mesh = triangle(0);
	const voxel::Palette empty({});
	const auto threeElements = palette(3);

	EXPECT_THROW(voxel::VoxelRenderCommand(nullptr, mesh, spk::Matrix4x4::identity()), spk::Exception);
	EXPECT_THROW(voxel::VoxelRenderCommand(&empty, mesh, spk::Matrix4x4::identity()), spk::Exception);
	EXPECT_THROW(voxel::VoxelRenderCommand(&threeElements, triangle(3), spk::Matrix4x4::identity()), spk::Exception);
}

TEST(VoxelRenderCommandTest, RejectionLeavesThePaletteResourceAndMeshUnchanged)
{
	const auto twoElements = palette(2);
	const auto identifier = twoElements.resourceIdentifier();
	const auto first = twoElements.data(0);
	const auto second = twoElements.data(1);
	const auto mesh = triangle(2);
	const auto vertices = std::vector<voxel::VoxelVertex>(
		mesh.layout().vertexBuffer().cast<voxel::VoxelVertex>().begin(),
		mesh.layout().vertexBuffer().cast<voxel::VoxelVertex>().end());

	EXPECT_THROW(voxel::VoxelRenderCommand(&twoElements, mesh, spk::Matrix4x4::identity()), spk::Exception);
	EXPECT_EQ(twoElements.resourceIdentifier(), identifier);
	EXPECT_EQ(twoElements.data(0), first);
	EXPECT_EQ(twoElements.data(1), second);
	EXPECT_TRUE(std::ranges::equal(mesh.layout().vertexBuffer().cast<voxel::VoxelVertex>(), vertices));
}

TEST(VoxelRenderCommandTest, StoresACopyThatSharesThePaletteGpuResource)
{
	std::unique_ptr<voxel::VoxelRenderCommand> command;
	{
		const auto temporary = palette(1);
		command = std::make_unique<voxel::VoxelRenderCommand>(
			&temporary, triangle(0), spk::Matrix4x4::identity());
	}

	EXPECT_NE(command, nullptr);
}
