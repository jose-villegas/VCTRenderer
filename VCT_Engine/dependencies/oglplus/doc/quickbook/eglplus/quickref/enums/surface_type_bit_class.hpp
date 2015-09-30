//  File doc/quickbook/eglplus/quickref/enums/surface_type_bit_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/surface_type_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[eglplus_enums_surface_type_bit_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__SurfaceTypeBit> class Transform>
class __EnumToClass<Base, __SurfaceTypeBit, Transform> /*<
Specialization of __EnumToClass for the __SurfaceTypeBit enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<SurfaceTypeBit::Window>
		Window;
	Transform<SurfaceTypeBit::Pixmap>
		Pixmap;
	Transform<SurfaceTypeBit::Pbuffer>
		Pbuffer;
	Transform<SurfaceTypeBit::MultisampleResolveBox>
		MultisampleResolveBox;
	Transform<SurfaceTypeBit::SwapBehaviorPreserved>
		SwapBehaviorPreserved;
	Transform<SurfaceTypeBit::VGColorspaceLinear>
		VGColorspaceLinear;
	Transform<SurfaceTypeBit::VGAlphaFormatPre>
		VGAlphaFormatPre;
};

} // namespace enums
#endif
//]

