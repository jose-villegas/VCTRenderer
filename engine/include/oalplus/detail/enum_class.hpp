/**
 *  .file oalplus/detail/enum_class.hpp
 *  @brief Helper macros for declaring enumerations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_AUX_ENUM_CLASS_1303201651_HPP
#define OALPLUS_AUX_ENUM_CLASS_1303201651_HPP

#include <oglplus/detail/enum_class.hpp>

#define OALPLUS_ENUM_CLASS_FWD(NAME, TYPE) \
	OGLPLUS_ENUM_CLASS_FWD(NAME, TYPE)

#define OALPLUS_ENUM_CLASS_BEGIN(NAME, TYPE) \
	OGLPLUS_ENUM_CLASS_BEGIN(NAME, TYPE)

#define OALPLUS_ENUM_CLASS_VALUE(ITEM, VALUE) \
	OGLPLUS_ENUM_CLASS_VALUE(ITEM, VALUE)

#define OALPLUS_CONST_ENUM_VALUE(ENUM_ITEM) \
	OGLPLUS_CONST_ENUM_VALUE(ENUM_ITEM)

#define OALPLUS_ENUM_CLASS_COMMA \
	OGLPLUS_ENUM_CLASS_COMMA

#define OALPLUS_ENUM_CLASS_END(NAME) \
	OGLPLUS_ENUM_CLASS_END(NAME)

#endif // include guard
