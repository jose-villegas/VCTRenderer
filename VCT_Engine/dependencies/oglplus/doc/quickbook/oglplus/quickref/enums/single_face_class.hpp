//  File doc/quickbook/oglplus/quickref/enums/single_face_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/single_face.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_single_face_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SingleFace> class Transform>
class __EnumToClass<Base, __SingleFace, Transform> /*<
Specialization of __EnumToClass for the __SingleFace enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SingleFace::Front>
		Front;
	Transform<SingleFace::Back>
		Back;
};

} // namespace enums
#endif
//]

