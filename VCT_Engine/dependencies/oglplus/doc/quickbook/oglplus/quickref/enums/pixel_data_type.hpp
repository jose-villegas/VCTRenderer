//  File doc/quickbook/oglplus/quickref/enums/pixel_data_type.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/pixel_data_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_pixel_data_type

enum class PixelDataType : GLenum
{
	UnsignedByte                 = GL_UNSIGNED_BYTE,
	Byte                         = GL_BYTE,
	UnsignedShort                = GL_UNSIGNED_SHORT,
	Short                        = GL_SHORT,
	UnsignedInt                  = GL_UNSIGNED_INT,
	Int                          = GL_INT,
	HalfFloat                    = GL_HALF_FLOAT,
	Float                        = GL_FLOAT,
	UnsignedByte_3_3_2           = GL_UNSIGNED_BYTE_3_3_2,
	UnsignedByte_2_3_3_Rev       = GL_UNSIGNED_BYTE_2_3_3_REV,
	UnsignedShort_5_6_5          = GL_UNSIGNED_SHORT_5_6_5,
	UnsignedShort_5_6_5_Rev      = GL_UNSIGNED_SHORT_5_6_5_REV,
	UnsignedShort_4_4_4_4        = GL_UNSIGNED_SHORT_4_4_4_4,
	UnsignedShort_4_4_4_4_Rev    = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	UnsignedShort_5_5_5_1        = GL_UNSIGNED_SHORT_5_5_5_1,
	UnsignedShort_1_5_5_5_Rev    = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	UnsignedInt_8_8_8_8          = GL_UNSIGNED_INT_8_8_8_8,
	UnsignedInt_8_8_8_8_Rev      = GL_UNSIGNED_INT_8_8_8_8_REV,
	UnsignedInt_10_10_10_2       = GL_UNSIGNED_INT_10_10_10_2,
	UnsignedInt_2_10_10_10_Rev   = GL_UNSIGNED_INT_2_10_10_10_REV,
	UnsignedInt_24_8             = GL_UNSIGNED_INT_24_8,
	UnsignedInt_10f_11f_11f_Rev  = GL_UNSIGNED_INT_10F_11F_11F_REV,
	UnsignedInt_5_9_9_9_Rev      = GL_UNSIGNED_INT_5_9_9_9_REV,
	Float_32UnsignedInt_24_8_Rev = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
};

template <>
__Range<PixelDataType> __EnumValueRange<PixelDataType>(void);

__StrCRef __EnumValueName(PixelDataType);

//]
