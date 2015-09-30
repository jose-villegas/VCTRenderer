//  File doc/quickbook/oglplus/quickref/enums/face_orientation_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/face_orientation.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_face_orientation_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__FaceOrientation> class Transform>
class __EnumToClass<Base, __FaceOrientation, Transform> /*<
Specialization of __EnumToClass for the __FaceOrientation enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<FaceOrientation::CW>
		CW;
	Transform<FaceOrientation::CCW>
		CCW;
};

} // namespace enums
#endif
//]

