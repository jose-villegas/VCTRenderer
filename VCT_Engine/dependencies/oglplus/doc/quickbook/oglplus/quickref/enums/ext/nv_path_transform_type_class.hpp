//  File doc/quickbook/oglplus/quickref/enums/ext/nv_path_transform_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/nv_path_transform_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_nv_path_transform_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PathNVTransformType> class Transform>
class __EnumToClass<Base, __PathNVTransformType, Transform> /*<
Specialization of __EnumToClass for the __PathNVTransformType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PathNVTransformType::None>
		None;
	Transform<PathNVTransformType::TranslateX>
		TranslateX;
	Transform<PathNVTransformType::TranslateY>
		TranslateY;
	Transform<PathNVTransformType::Translate2D>
		Translate2D;
	Transform<PathNVTransformType::Translate3D>
		Translate3D;
	Transform<PathNVTransformType::Affine2D>
		Affine2D;
	Transform<PathNVTransformType::Affine3D>
		Affine3D;
	Transform<PathNVTransformType::TransposeAffine2D>
		TransposeAffine2D;
	Transform<PathNVTransformType::TransposeAffine3D>
		TransposeAffine3D;
};

} // namespace enums
#endif
//]

