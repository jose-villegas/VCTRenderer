//  File doc/quickbook/oglplus/quickref/enums/transform_feedback_primitive_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/transform_feedback_primitive_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_transform_feedback_primitive_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TransformFeedbackPrimitiveType> class Transform>
class __EnumToClass<Base, __TransformFeedbackPrimitiveType, Transform> /*<
Specialization of __EnumToClass for the __TransformFeedbackPrimitiveType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TransformFeedbackPrimitiveType::Triangles>
		Triangles;
	Transform<TransformFeedbackPrimitiveType::Lines>
		Lines;
	Transform<TransformFeedbackPrimitiveType::Points>
		Points;
};

} // namespace enums
#endif
//]

