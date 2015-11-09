/**
 *  @file oalplus/config.hpp
 *  @brief Compile-time configuration options
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_CONFIG_1303201545_HPP
#define OALPLUS_CONFIG_1303201545_HPP

#ifndef OGLPLUS_NO_SITE_CONFIG
#include <oglplus/config/site.hpp>
#endif


#if OGLPLUS_USE_BOOST_CONFIG
#include <boost/config.hpp>
#endif

#include <oglplus/config/compiler.hpp>
#include <oglplus/config/basic.hpp>

#ifdef  OGLPLUS_DOCUMENTATION_ONLY
#define OALPLUS_DOCUMENTATION_ONLY OGLPLUS_DOCUMENTATION_ONLY
#else
#define OALPLUS_DOCUMENTATION_ONLY 0
#endif

#ifndef OALPLUS_LINK_LIBRARY
# define OALPLUS_LINK_LIBRARY OGLPLUS_LINK_LIBRARY
#endif

#if OALPLUS_LINK_LIBRARY
# define OALPLUS_LIB_FUNC
#else
# define OALPLUS_LIB_FUNC inline
#endif


#ifdef OGLPLUS_NO_OBJECT_DESC
#define OALPLUS_NO_OBJECT_DESC OGLPLUS_NO_OBJECT_DESC
#endif

#define OALPLUS_FAKE_USE(EXPR) (void)EXPR

#define OALPLUS_NOEXCEPT(...) OGLPLUS_NOEXCEPT(__VA_ARGS__)
#define OALPLUS_NOEXCEPT_IF(...) OALPLUS_NOEXCEPT_IF(__VA_ARGS__)

#define OALPLUS_OVERRIDE OGLPLUS_OVERRIDE

#ifndef OALPLUS_NO_ENUM_VALUE_NAMES
# define OALPLUS_NO_ENUM_VALUE_NAMES 0
#endif

#ifndef OALPLUS_NO_ENUM_VALUE_RANGES
# define OALPLUS_NO_ENUM_VALUE_RANGES 0
#endif

#endif // include guard
