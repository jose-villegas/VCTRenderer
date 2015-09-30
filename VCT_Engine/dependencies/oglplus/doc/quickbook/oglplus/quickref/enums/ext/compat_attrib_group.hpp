//  File doc/quickbook/oglplus/quickref/enums/ext/compat_attrib_group.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_attrib_group.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_attrib_group

enum class CompatibilityAttributeGroup : GLbitfield
{
	AccumBuffer    = GL_ACCUM_BUFFER_BIT,
	ColorBuffer    = GL_COLOR_BUFFER_BIT,
	Current        = GL_CURRENT_BIT,
	DepthBuffer    = GL_DEPTH_BUFFER_BIT,
	Enable         = GL_ENABLE_BIT,
	Eval           = GL_EVAL_BIT,
	Fog            = GL_FOG_BIT,
	Hint           = GL_HINT_BIT,
	Lighting       = GL_LIGHTING_BIT,
	Line           = GL_LINE_BIT,
	List           = GL_LIST_BIT,
	Multisample    = GL_MULTISAMPLE_BIT,
	PixelMode      = GL_PIXEL_MODE_BIT,
	Point          = GL_POINT_BIT,
	Polygon        = GL_POLYGON_BIT,
	PolygonStipple = GL_POLYGON_STIPPLE_BIT,
	Scissor        = GL_SCISSOR_BIT,
	StencilBuffer  = GL_STENCIL_BUFFER_BIT,
	Texture        = GL_TEXTURE_BIT,
	Transform      = GL_TRANSFORM_BIT,
	Viewport       = GL_VIEWPORT_BIT,
	AllAttribs     = GL_ALL_ATTRIB_BITS
};

template <>
__Range<CompatibilityAttributeGroup> __EnumValueRange<CompatibilityAttributeGroup>(void);

__StrCRef __EnumValueName(CompatibilityAttributeGroup);

__Bitfield<CompatibilityAttributeGroup> operator | (CompatibilityAttributeGroup b1, CompatibilityAttributeGroup b2);

//]
