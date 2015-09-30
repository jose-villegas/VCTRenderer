//  File doc/quickbook/oglplus/quickref/enums/transform_feedback_mode_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/transform_feedback_mode.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_transform_feedback_mode_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__TransformFeedbackMode> class Transform>
class __EnumToClass<Base, __TransformFeedbackMode, Transform> /*<
Specialization of __EnumToClass for the __TransformFeedbackMode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<TransformFeedbackMode::InterleavedAttribs>
		InterleavedAttribs;
	Transform<TransformFeedbackMode::SeparateAttribs>
		SeparateAttribs;
};

} // namespace enums
#endif
//]

