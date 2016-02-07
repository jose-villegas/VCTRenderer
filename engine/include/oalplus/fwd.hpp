/**
 *  @file oalplus/fwd.hpp
 *  @brief Forward declarations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_FWD_1303201602_HPP
#define OALPLUS_FWD_1303201602_HPP

#include <oalplus/config.hpp>

namespace oalplus {

namespace tag {

struct Source;
struct Buffer;

struct DirectState;

struct Playback;
struct Capture;

} // namespace tag

template <typename DevTag>
class DeviceOps;
class ContextOps;
class Context;

} // namespace oalplus

#endif // include guard
