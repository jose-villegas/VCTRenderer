//  File doc/quickbook/oglplus/quickref/enums/transform_feedback_primitive_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/transform_feedback_primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_transform_feedback_primitive_type

enum class TransformFeedbackPrimitiveType : GLenum
{
	Triangles = GL_TRIANGLES,
	Lines     = GL_LINES,
	Points    = GL_POINTS
};

template <>
__Range<TransformFeedbackPrimitiveType> __EnumValueRange<TransformFeedbackPrimitiveType>(void);

__StrCRef __EnumValueName(TransformFeedbackPrimitiveType);

//]
