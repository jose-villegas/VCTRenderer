//  File doc/quickbook/oglplus/quickref/enums/data_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/data_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_data_type

enum class DataType : GLenum
{
	Byte          = GL_BYTE,
	Short         = GL_SHORT,
	Int           = GL_INT,
	Fixed         = GL_FIXED,
	Float         = GL_FLOAT,
	HalfFloat     = GL_HALF_FLOAT,
	Double        = GL_DOUBLE,
	UnsignedByte  = GL_UNSIGNED_BYTE,
	UnsignedShort = GL_UNSIGNED_SHORT,
	UnsignedInt   = GL_UNSIGNED_INT
};

template <>
__Range<DataType> __EnumValueRange<DataType>(void);

__StrCRef __EnumValueName(DataType);

//]
