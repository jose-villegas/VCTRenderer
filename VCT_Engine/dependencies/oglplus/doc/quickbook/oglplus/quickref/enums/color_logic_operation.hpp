//  File doc/quickbook/oglplus/quickref/enums/color_logic_operation.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/color_logic_operation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_color_logic_operation

enum class ColorLogicOperation : GLenum
{
	Clear        = GL_CLEAR,
	And          = GL_AND,
	AndReverse   = GL_AND_REVERSE,
	Copy         = GL_COPY,
	AndInverted  = GL_AND_INVERTED,
	Noop         = GL_NOOP,
	Xor          = GL_XOR,
	Or           = GL_OR,
	Nor          = GL_NOR,
	Equiv        = GL_EQUIV,
	Invert       = GL_INVERT,
	OrReverse    = GL_OR_REVERSE,
	CopyInverted = GL_COPY_INVERTED,
	OrInverted   = GL_OR_INVERTED,
	Nand         = GL_NAND,
	Set          = GL_SET
};

template <>
__Range<ColorLogicOperation> __EnumValueRange<ColorLogicOperation>(void);

__StrCRef __EnumValueName(ColorLogicOperation);

//]
