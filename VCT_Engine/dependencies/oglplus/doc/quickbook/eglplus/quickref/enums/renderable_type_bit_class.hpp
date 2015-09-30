//  File doc/quickbook/eglplus/quickref/enums/renderable_type_bit_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/renderable_type_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_renderable_type_bit_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__RenderableTypeBit> class Transform>
class __EnumToClass<Base, __RenderableTypeBit, Transform> /*<
Specialization of __EnumToClass for the __RenderableTypeBit enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<RenderableTypeBit::OpenGL>
		OpenGL;
	Transform<RenderableTypeBit::OpenGL_ES>
		OpenGL_ES;
	Transform<RenderableTypeBit::OpenGL_ES2>
		OpenGL_ES2;
	Transform<RenderableTypeBit::OpenGL_ES3>
		OpenGL_ES3;
	Transform<RenderableTypeBit::OpenVG>
		OpenVG;
};

} // namespace enums
#endif
//]

