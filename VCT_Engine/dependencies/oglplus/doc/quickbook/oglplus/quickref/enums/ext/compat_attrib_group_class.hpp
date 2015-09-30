//  File doc/quickbook/oglplus/quickref/enums/ext/compat_attrib_group_class.hpp
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
//[oglplus_enums_ext_compat_attrib_group_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__CompatibilityAttributeGroup> class Transform>
class __EnumToClass<Base, __CompatibilityAttributeGroup, Transform> /*<
Specialization of __EnumToClass for the __CompatibilityAttributeGroup enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<CompatibilityAttributeGroup::AccumBuffer>
		AccumBuffer;
	Transform<CompatibilityAttributeGroup::ColorBuffer>
		ColorBuffer;
	Transform<CompatibilityAttributeGroup::Current>
		Current;
	Transform<CompatibilityAttributeGroup::DepthBuffer>
		DepthBuffer;
	Transform<CompatibilityAttributeGroup::Enable>
		Enable;
	Transform<CompatibilityAttributeGroup::Eval>
		Eval;
	Transform<CompatibilityAttributeGroup::Fog>
		Fog;
	Transform<CompatibilityAttributeGroup::Hint>
		Hint;
	Transform<CompatibilityAttributeGroup::Lighting>
		Lighting;
	Transform<CompatibilityAttributeGroup::Line>
		Line;
	Transform<CompatibilityAttributeGroup::List>
		List;
	Transform<CompatibilityAttributeGroup::Multisample>
		Multisample;
	Transform<CompatibilityAttributeGroup::PixelMode>
		PixelMode;
	Transform<CompatibilityAttributeGroup::Point>
		Point;
	Transform<CompatibilityAttributeGroup::Polygon>
		Polygon;
	Transform<CompatibilityAttributeGroup::PolygonStipple>
		PolygonStipple;
	Transform<CompatibilityAttributeGroup::Scissor>
		Scissor;
	Transform<CompatibilityAttributeGroup::StencilBuffer>
		StencilBuffer;
	Transform<CompatibilityAttributeGroup::Texture>
		Texture;
	Transform<CompatibilityAttributeGroup::Transform>
		Transform;
	Transform<CompatibilityAttributeGroup::Viewport>
		Viewport;
	Transform<CompatibilityAttributeGroup::AllAttribs>
		AllAttribs;
};

} // namespace enums
#endif
//]

