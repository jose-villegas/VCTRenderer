//  File doc/quickbook/oglplus/quickref/enums/ext/compat_client_attrib_group.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_client_attrib_group.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_client_attrib_group

enum class CompatibilityClientAttributeGroup : GLbitfield
{
	VertexArray = GL_CLIENT_VERTEX_ARRAY_BIT,
	PixelStore  = GL_CLIENT_PIXEL_STORE_BIT,
	AllAttribs  = GL_CLIENT_ALL_ATTRIB_BITS
};

template <>
__Range<CompatibilityClientAttributeGroup> __EnumValueRange<CompatibilityClientAttributeGroup>(void);

__StrCRef __EnumValueName(CompatibilityClientAttributeGroup);

__Bitfield<CompatibilityClientAttributeGroup> operator | (CompatibilityClientAttributeGroup b1, CompatibilityClientAttributeGroup b2);

//]
