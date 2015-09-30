//  File doc/quickbook/oglplus/quickref/enums/face.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/face.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_face

enum class Face : GLenum
{
	Front        = GL_FRONT,
	Back         = GL_BACK,
	FrontAndBack = GL_FRONT_AND_BACK
};

template <>
__Range<Face> __EnumValueRange<Face>(void);

__StrCRef __EnumValueName(Face);

//]
