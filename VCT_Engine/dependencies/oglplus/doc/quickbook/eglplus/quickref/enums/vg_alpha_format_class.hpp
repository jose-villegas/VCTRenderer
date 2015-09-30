//  File doc/quickbook/eglplus/quickref/enums/vg_alpha_format_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/vg_alpha_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_vg_alpha_format_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__VGAlphaFormat> class Transform>
class __EnumToClass<Base, __VGAlphaFormat, Transform> /*<
Specialization of __EnumToClass for the __VGAlphaFormat enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<VGAlphaFormat::Pre>
		Pre;
	Transform<VGAlphaFormat::NonPre>
		NonPre;
};

} // namespace enums
#endif
//]

