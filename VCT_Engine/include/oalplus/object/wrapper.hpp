/**
 *  @file oalplus/object/wrapper.hpp
 *  @brief Generic OpenAL object wrapper
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_OBJECT_WRAPPER_1107121519_HPP
#define OALPLUS_OBJECT_WRAPPER_1107121519_HPP

#include <oalplus/object/name.hpp>
#include <oglplus/object/wrap_tpl.hpp>

namespace oalplus {

/** @defgroup oalplus_objects OALplus objects
 *
 *  An @ref oalplus_object is a class wrapping around OpenAL objects
 *  like shaders, programs, textures, etc. It is using the RAII technique
 *  to provide automated resource management for these OpenAL resources.
 *  They also wrap operations related to these objects and provide
 *  additional type safety and more robust error handling. Look
 *  @ref oalplus_object "here" for more details.
 */

template <typename ObjTag>
class ObjGenDelOps;

template <typename OpsTag, typename ObjTag>
class ObjectOps;

using oglplus::Object;

namespace tag {

using oglplus::tag::Generate;

} // namespace tag
} // namespace oalplus

#endif // include guard
