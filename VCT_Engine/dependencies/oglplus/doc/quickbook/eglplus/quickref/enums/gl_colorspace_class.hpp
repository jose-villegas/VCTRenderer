//  File doc/quickbook/eglplus/quickref/enums/gl_colorspace_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/gl_colorspace.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_gl_colorspace_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__GLColorspace> class Transform>
class __EnumToClass<Base, __GLColorspace, Transform> /*<
Specialization of __EnumToClass for the __GLColorspace enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<GLColorspace::sRGB>
		sRGB;
	Transform<GLColorspace::Linear>
		Linear;
};

} // namespace enums
#endif
//]

