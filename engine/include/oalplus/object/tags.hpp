/**
 *  @file oalplus/object/tags.hpp
 *  @brief AL Object tag types.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_OBJECT_TAGS_1405011014_HPP
#define OALPLUS_OBJECT_TAGS_1405011014_HPP

#include <type_traits>

namespace oalplus {
namespace tag {

#define OALPLUS_DEFINE_OBJECT_TAG(ID, OBJECT) \
struct OBJECT \
 : std::integral_constant<int, ID> \
{ \
	typedef ALuint NameType; \
};

OALPLUS_DEFINE_OBJECT_TAG( 101, Source)
OALPLUS_DEFINE_OBJECT_TAG( 102, Buffer)

#undef OALPLUS_DEFINE_OBJECT_TAG

} // namespace tag
} // namespace oalplus

#endif // include guard
