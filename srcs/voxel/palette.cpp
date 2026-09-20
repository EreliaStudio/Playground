#include "voxel/palette.hpp"

#include <type_traits>

#include <exception.hpp>

namespace voxel
{
	static_assert(std::is_trivially_copyable_v<Palette::Data>);
	static_assert(sizeof(Palette::Data) == sizeof(float) * 4);

	Palette::Palette(std::vector<Data> data) :
		_buffer(BindingPoint, 0, sizeof(Data))
	{
		_buffer.resize(data.size());
		_buffer.setDynamicData<Data>(std::span<const Data>(data));
		if (!data.empty()) _view = _buffer.cast<void, Data>();
		_buffer.validate();
	}

	Palette::Data &Palette::data(std::size_t index)
	{
		if (!contains(index)) throw spk::Exception("Palette data index is out of range");
		return _view.dynamicArray[index];
	}

	const Palette::Data &Palette::data(std::size_t index) const
	{
		if (!contains(index)) throw spk::Exception("Palette data index is out of range");
		return _view.dynamicArray[index];
	}

	std::size_t Palette::size() const noexcept
	{
		return _view.nbElement;
	}

	bool Palette::empty() const noexcept
	{
		return _elements.empty();
	}

	bool Palette::contains(std::size_t elementIndex) const noexcept
	{
		return elementIndex < size();
	}

	spk::GPUResource::Identifier Palette::resourceIdentifier() const noexcept
	{
		return _buffer.identifier();
	}

	void Palette::validate()
	{
		_buffer.validate();
	}

	void Palette::activate(spk::RenderContext &context) const
	{
		_buffer.activate(context);
	}
}
