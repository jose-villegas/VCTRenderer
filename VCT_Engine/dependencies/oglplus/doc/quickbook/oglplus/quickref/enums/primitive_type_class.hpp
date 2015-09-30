//  File doc/quickbook/oglplus/quickref/enums/primitive_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_primitive_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PrimitiveType> class Transform>
class __EnumToClass<Base, __PrimitiveType, Transform> /*<
Specialization of __EnumToClass for the __PrimitiveType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PrimitiveType::Points>
		Points;
	Transform<PrimitiveType::LineStrip>
		LineStrip;
	Transform<PrimitiveType::LineLoop>
		LineLoop;
	Transform<PrimitiveType::Lines>
		Lines;
	Transform<PrimitiveType::TriangleStrip>
		TriangleStrip;
	Transform<PrimitiveType::TriangleFan>
		TriangleFan;
	Transform<PrimitiveType::Triangles>
		Triangles;
	Transform<PrimitiveType::LinesAdjacency>
		LinesAdjacency;
	Transform<PrimitiveType::LineStripAdjacency>
		LineStripAdjacency;
	Transform<PrimitiveType::TrianglesAdjacency>
		TrianglesAdjacency;
	Transform<PrimitiveType::TriangleStripAdjacency>
		TriangleStripAdjacency;
	Transform<PrimitiveType::Patches>
		Patches;
};

} // namespace enums
#endif
//]

