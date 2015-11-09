/**
 *  @file eglplus/lib.hpp
 *  @brief All-in-one Include file for the separatelly-built library
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_LIB_1306020632_HPP
#define EGLPLUS_LIB_1306020632_HPP

#ifndef EGLPLUS_IMPLEMENTING_LIBRARY
#define EGLPLUS_IMPLEMENTING_LIBRARY 1
#endif

#include <eglplus/config.hpp>
#include <eglplus/string.hpp>
#include <eglplus/fwd.hpp>

#include <eglplus/error/basic.hpp>
#include <eglplus/configs.hpp>

#include <eglplus/detail/base_range.hpp>
#include <eglplus/detail/enum_class.hpp>

namespace eglplus {

// EnumValueName implementations
#include <eglplus/lib/enum_value_name.ipp>
// EnumValueRange implementations
#include <eglplus/lib/enum_value_range.ipp>

} // namespace eglplus

#undef EGLPLUS_IMPLEMENTING_LIBRARY

#endif // include guard
