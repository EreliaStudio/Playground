#pragma once

#include <cstddef>
#include <vector>

#include <sparkle>

namespace voxel
{
	class Palette final
	{
	public:
		struct Data
		{
			spk::Color color;
			bool operator==(const Data &) const noexcept = default;
		};

	private:
		static constexpr std::size_t BindingPoint = 2;
		using DataView = spk::ShaderStorageBuffer::View<void, Data>;

		spk::ShaderStorageBuffer _buffer;
		DataView _view{0, nullptr};

	public:
		explicit Palette(std::vector<Data> data);

		[[nodiscard]] Data &data(std::size_t index);
		[[nodiscard]] const Data &data(std::size_t index) const;
		[[nodiscard]] std::size_t size() const noexcept;
		[[nodiscard]] bool empty() const noexcept;
		[[nodiscard]] bool contains(std::size_t elementIndex) const noexcept;
		[[nodiscard]] spk::GPUResource::Identifier resourceIdentifier() const noexcept;

		void validate();
		void activate(spk::RenderContext &context) const;
	};
}
