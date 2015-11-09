/**
 *  @file eglplus/config.hpp
 *  @brief Compile-time configuration options
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_CONFIG_1303201545_HPP
#define EGLPLUS_CONFIG_1303201545_HPP

#ifndef OGLPLUS_NO_SITE_CONFIG
#include <oglplus/config/site.hpp>
#endif


#if OGLPLUS_USE_BOOST_CONFIG
#include <boost/config.hpp>
#endif

#include <oglplus/config/compiler.hpp>

#ifdef  OGLPLUS_DOCUMENTATION_ONLY
#define EGLPLUS_DOCUMENTATION_ONLY OGLPLUS_DOCUMENTATION_ONLY
#else
#define EGLPLUS_DOCUMENTATION_ONLY 0
#endif

#ifndef EGLPLUS_LINK_LIBRARY
# define EGLPLUS_LINK_LIBRARY OGLPLUS_LINK_LIBRARY
#endif

#if EGLPLUS_LINK_LIBRARY
# define EGLPLUS_LIB_FUNC
#else
# define EGLPLUS_LIB_FUNC inline
#endif


#define EGLPLUS_FAKE_USE(EXPR) (void)EXPR

#define EGLPLUS_NOEXCEPT(...) OGLPLUS_NOEXCEPT(__VA_ARGS__)
#define EGLPLUS_NOEXCEPT_IF(...) EGLPLUS_NOEXCEPT_IF(__VA_ARGS__)

#ifndef EGLPLUS_NO_ENUM_VALUE_NAMES
# define EGLPLUS_NO_ENUM_VALUE_NAMES 0
#endif

#ifndef EGLPLUS_NO_ENUM_VALUE_RANGES
# define EGLPLUS_NO_ENUM_VALUE_RANGES 0
#endif

#endif // include guard
