//  File doc/quickbook/oglplus/quickref/enums/ext/compat_prim_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/compat_prim_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_compat_prim_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__CompatibilityPrimitiveType> class Transform>
class __EnumToClass<Base, __CompatibilityPrimitiveType, Transform> /*<
Specialization of __EnumToClass for the __CompatibilityPrimitiveType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<CompatibilityPrimitiveType::Points>
		Points;
	Transform<CompatibilityPrimitiveType::LineStrip>
		LineStrip;
	Transform<CompatibilityPrimitiveType::LineLoop>
		LineLoop;
	Transform<CompatibilityPrimitiveType::Lines>
		Lines;
	Transform<CompatibilityPrimitiveType::TriangleStrip>
		TriangleStrip;
	Transform<CompatibilityPrimitiveType::TriangleFan>
		TriangleFan;
	Transform<CompatibilityPrimitiveType::Triangles>
		Triangles;
	Transform<CompatibilityPrimitiveType::Quads>
		Quads;
	Transform<CompatibilityPrimitiveType::QuadStrip>
		QuadStrip;
	Transform<CompatibilityPrimitiveType::Polygon>
		Polygon;
};

} // namespace enums
#endif
//]

