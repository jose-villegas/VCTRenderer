/**
 *  @file eglplus/bitfield.hpp
 *  @brief EGL bitfield-related helpers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_BITFIELD_1107121519_HPP
#define EGLPLUS_BITFIELD_1107121519_HPP

#include <eglplus/config.hpp>
#include <oglplus/bitfield.hpp>


namespace eglplus {

/// This template serves as a wrapper for EGL bitfields
/**
 */
using ::oglplus::Bitfield;

// helper macro used to define bitfield-related functions
#define EGLPLUS_MAKE_BITFIELD(BITS) \
} namespace oglplus { \
namespace enums { \
template <> struct EnumBaseType<eglplus::BITS> { typedef EGLenum Type; }; \
template <> struct IsBitfieldBit<eglplus::BITS> { typedef std::true_type Type; }; \
} } namespace eglplus { \
inline eglplus::Bitfield<BITS> operator | (BITS b1, BITS b2) \
{ \
	return Bitfield<BITS>(b1, b2); \
}


} // namespace eglplus

#endif // include guard
