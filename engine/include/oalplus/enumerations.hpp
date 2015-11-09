/**
 *  @file oalplus/enumerations.hpp
 *  @brief Enumeration-related declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ENUMERATIONS_1303201759_HPP
#define OALPLUS_ENUMERATIONS_1303201759_HPP

#include <oalplus/config.hpp>
#include <oalplus/string.hpp>
#include <oalplus/detail/enum_class.hpp>
#include <oalplus/detail/base_range.hpp>
#include <oalplus/utils/type_tag.hpp>

namespace oalplus {
namespace enums {

template <typename Enum>
struct EnumBaseType
{
	typedef ALenum Type;
};

template <typename EnumType>
inline StrCRef EnumValueName(EnumType enum_value)
{
#if !OALPLUS_NO_ENUM_VALUE_NAMES
	typedef typename EnumBaseType<EnumType>::Type BaseType;
	return ValueName_(
		&TypeTag<EnumType>(),
		BaseType(enum_value)
	);
#else
	OGLPLUS_FAKE_USE(enum_value);
	return StrCRef();
#endif
}

template <typename EnumType>
inline aux::CastIterRange<
	const typename EnumBaseType<EnumType>::Type*,
	EnumType
> EnumValueRange(void)
{
#if !OALPLUS_NO_ENUM_VALUE_RANGES
	return ValueRange_(&TypeTag<EnumType>());
#else
	const typename EnumBaseType<EnumType>::Type* x = nullptr;
	return aux::CastIterRange<
		const typename EnumBaseType<EnumType>::Type*,
		EnumType
	>(x, x);

#endif
}

} // namespace enums

using enums::EnumValueName;
using enums::EnumValueRange;

} // namespace oalplus

#endif // include guard
