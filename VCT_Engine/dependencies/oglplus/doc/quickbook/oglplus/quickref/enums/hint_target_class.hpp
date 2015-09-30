//  File doc/quickbook/oglplus/quickref/enums/hint_target_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/hint_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_hint_target_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__HintTarget> class Transform>
class __EnumToClass<Base, __HintTarget, Transform> /*<
Specialization of __EnumToClass for the __HintTarget enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<HintTarget::LineSmooth>
		LineSmooth;
	Transform<HintTarget::PolygonSmooth>
		PolygonSmooth;
	Transform<HintTarget::TextureCompression>
		TextureCompression;
	Transform<HintTarget::FragmentShaderDerivative>
		FragmentShaderDerivative;
};

} // namespace enums
#endif
//]

