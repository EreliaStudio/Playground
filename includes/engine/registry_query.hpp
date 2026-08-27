#pragma once

#include "engine/query.hpp"

namespace spk
{
	template <typename TContextType, typename TRootType>
class RegistrySelection : public Selection<TRootType *>
{
public:
	explicit RegistrySelection(const TContextType &context) :
		Selection<TRootType *>(
			spk::Registry<TContextType, TRootType>::elements(context))
	{
	}
};

template<typename TContextType, typename TRootType, typename TData>
class RegistryQuery :
	public Query<TRootType*, TData>
{
private:
	using Base = Query<TRootType*, TData>;

public:
	using ObtainMode = typename Base::ObtainMode;

	explicit RegistryQuery(const TContextType& context) :
		Base(RegistrySelection<TContextType, TRootType>(context))
	{
	}

	template<typename TPredicate>
		requires std::predicate<TPredicate&, TRootType*, TData&>
	RegistryQuery& where(TPredicate&& predicate)
	{
		Base::where(std::forward<TPredicate>(predicate));
		return *this;
	}

	template<typename TMember, typename TProvider>
		requires std::invocable<TProvider&, TRootType*> &&
				 std::assignable_from<
					 TMember&,
					 std::invoke_result_t<TProvider&, TRootType*>>
	RegistryQuery& obtain(
		TMember TData::* member,
		TProvider&& provider,
		ObtainMode mode = ObtainMode::Mandatory)
	{
		Base::obtain(
			member,
			std::forward<TProvider>(provider),
			mode);

		return *this;
	}

	template<typename TProvider>
		requires std::invocable<TProvider&, TRootType*>
	RegistryQuery& obtain(
		TProvider&& provider,
		ObtainMode mode = ObtainMode::Mandatory)
	{
		Base::obtain(
			std::forward<TProvider>(provider),
			mode);

		return *this;
	}

	template<typename TOtherRoot>
		requires std::convertible_to<TOtherRoot, TRootType*>
	RegistryQuery& unionWith(const Selection<TOtherRoot>& selection)
	{
		Base::unionWith(selection);
		return *this;
	}

	template<typename TOtherRoot>
		requires std::convertible_to<TOtherRoot, TRootType*>
	RegistryQuery& intersectWith(const Selection<TOtherRoot>& selection)
	{
		Base::intersectWith(selection);
		return *this;
	}
};
}