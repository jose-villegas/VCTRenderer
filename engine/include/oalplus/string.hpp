/**
 *  @file oalplus/string.hpp
 *  @brief String-related typedefs
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_STRING_1303291951_HPP
#define OALPLUS_STRING_1303291951_HPP

#include <oglplus/string/utf8.hpp>
#include <oglplus/string/ref_tpl.hpp>
#include <oglplus/string/empty.hpp>

namespace oalplus {

/// String class
/**
 *  @ingroup oalplus_strings
 */
typedef ::std::basic_string<ALchar> String;

/// String const reference wrapper
/**
 *  @ingroup oalplus_strings
 */
typedef ::oglplus::StrCRefTpl<ALchar> StrCRef;

using oglplus::EmptyStdString;

} // namespace oalplus

#endif // include guard
