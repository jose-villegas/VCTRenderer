//  File doc/quickbook/oglplus/quickref/enums/precision_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/precision_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_precision_type

enum class PrecisionType : GLenum
{
	LowFloat    = GL_LOW_FLOAT,
	MediumFloat = GL_MEDIUM_FLOAT,
	HighFloat   = GL_HIGH_FLOAT,
	LowInt      = GL_LOW_INT,
	MediumInt   = GL_MEDIUM_INT,
	HighInt     = GL_HIGH_INT
};

template <>
__Range<PrecisionType> __EnumValueRange<PrecisionType>(void);

__StrCRef __EnumValueName(PrecisionType);

//]
