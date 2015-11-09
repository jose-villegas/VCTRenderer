/**
 *  @file oalplus/attrib_list.hpp
 *  @brief Template for OpenAL context attribute list
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ATTRIB_LIST_1303292057_HPP
#define OALPLUS_ATTRIB_LIST_1303292057_HPP

#include <oglplus/detail/attrib_list.hpp>

#include <cassert>
#include <vector>

namespace oalplus {

/// Reused FinishedAttributeList from OGLplus
using oglplus::FinishedAttributeList;

/// Reused FinishedAttributeList from OGLplus
using oglplus::AttributeList;

struct AttributeListTraits
{
	typedef ALint Int;

	template <typename Enum>
	struct EnumBaseType
	{
		typedef typename oalplus::enums::EnumBaseType<Enum>::Type Type;
	};

	static ALint ListEnd(void) { return 0; }
	static ALint TrueValue(void) { return AL_TRUE; }
	static ALint FalseValue(void) { return AL_FALSE; }
	static ALint DontCare(void) { return 0; }
};

} // namespace oalplus

#endif // include guard
