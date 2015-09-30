/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oalplus_object_name

using __oglplus_ObjectName;

template <typename ObjTag>
const ALuint GetName(ObjectName<ObjTag> named)
noexcept;

//]
//[oalplus_object_name_typedefs

typedef __ObjectName<__tag_Buffer> BufferName;
typedef ObjectName<__tag_Source> SourceName;

//]

