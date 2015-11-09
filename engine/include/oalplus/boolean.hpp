/**
 *  @file oalplus/boolean.hpp
 *  @brief Wrapper for ALint (acting as boolean)
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_BOOLEAN_1501311633_HPP
#define OALPLUS_BOOLEAN_1501311633_HPP

#include <oglplus/detail/boolean.hpp>

namespace oalplus {

typedef oglplus::BoolImpl<ALboolean, ALint, AL_TRUE, AL_FALSE> Boolean;

} // namespace oalplus

#endif // include guard
