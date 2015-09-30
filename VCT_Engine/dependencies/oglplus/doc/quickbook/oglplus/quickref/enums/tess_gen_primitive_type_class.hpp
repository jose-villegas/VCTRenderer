//  File doc/quickbook/oglplus/quickref/enums/tess_gen_primitive_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/tess_gen_primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_tess_gen_primitive_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TessGenPrimitiveType> class Transform>
class __EnumToClass<Base, __TessGenPrimitiveType, Transform> /*<
Specialization of __EnumToClass for the __TessGenPrimitiveType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TessGenPrimitiveType::Quads>
		Quads;
	Transform<TessGenPrimitiveType::Triangles>
		Triangles;
	Transform<TessGenPrimitiveType::Isolines>
		Isolines;
};

} // namespace enums
#endif
//]

