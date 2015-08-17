//  File implement/eglplus/enums/surface_type_bit_class.ipp
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
namespace enums {
template <typename Base, template<SurfaceTypeBit> class Transform>
class EnumToClass<Base, SurfaceTypeBit, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_WINDOW_BIT
# if defined Window
#  pragma push_macro("Window")
#  undef Window
	Transform<SurfaceTypeBit::Window> Window;
#  pragma pop_macro("Window")
# else
	Transform<SurfaceTypeBit::Window> Window;
# endif
#endif
#if defined EGL_PIXMAP_BIT
# if defined Pixmap
#  pragma push_macro("Pixmap")
#  undef Pixmap
	Transform<SurfaceTypeBit::Pixmap> Pixmap;
#  pragma pop_macro("Pixmap")
# else
	Transform<SurfaceTypeBit::Pixmap> Pixmap;
# endif
#endif
#if defined EGL_PBUFFER_BIT
# if defined Pbuffer
#  pragma push_macro("Pbuffer")
#  undef Pbuffer
	Transform<SurfaceTypeBit::Pbuffer> Pbuffer;
#  pragma pop_macro("Pbuffer")
# else
	Transform<SurfaceTypeBit::Pbuffer> Pbuffer;
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX_BIT
# if defined MultisampleResolveBox
#  pragma push_macro("MultisampleResolveBox")
#  undef MultisampleResolveBox
	Transform<SurfaceTypeBit::MultisampleResolveBox> MultisampleResolveBox;
#  pragma pop_macro("MultisampleResolveBox")
# else
	Transform<SurfaceTypeBit::MultisampleResolveBox> MultisampleResolveBox;
# endif
#endif
#if defined EGL_SWAP_BEHAVIOR_PRESERVED_BIT
# if defined SwapBehaviorPreserved
#  pragma push_macro("SwapBehaviorPreserved")
#  undef SwapBehaviorPreserved
	Transform<SurfaceTypeBit::SwapBehaviorPreserved> SwapBehaviorPreserved;
#  pragma pop_macro("SwapBehaviorPreserved")
# else
	Transform<SurfaceTypeBit::SwapBehaviorPreserved> SwapBehaviorPreserved;
# endif
#endif
#if defined EGL_VG_COLORSPACE_LINEAR_BIT
# if defined VGColorspaceLinear
#  pragma push_macro("VGColorspaceLinear")
#  undef VGColorspaceLinear
	Transform<SurfaceTypeBit::VGColorspaceLinear> VGColorspaceLinear;
#  pragma pop_macro("VGColorspaceLinear")
# else
	Transform<SurfaceTypeBit::VGColorspaceLinear> VGColorspaceLinear;
# endif
#endif
#if defined EGL_VG_ALPHA_FORMAT_PRE_BIT
# if defined VGAlphaFormatPre
#  pragma push_macro("VGAlphaFormatPre")
#  undef VGAlphaFormatPre
	Transform<SurfaceTypeBit::VGAlphaFormatPre> VGAlphaFormatPre;
#  pragma pop_macro("VGAlphaFormatPre")
# else
	Transform<SurfaceTypeBit::VGAlphaFormatPre> VGAlphaFormatPre;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_WINDOW_BIT
# if defined Window
#  pragma push_macro("Window")
#  undef Window
	 , Window(_base())
#  pragma pop_macro("Window")
# else
	 , Window(_base())
# endif
#endif
#if defined EGL_PIXMAP_BIT
# if defined Pixmap
#  pragma push_macro("Pixmap")
#  undef Pixmap
	 , Pixmap(_base())
#  pragma pop_macro("Pixmap")
# else
	 , Pixmap(_base())
# endif
#endif
#if defined EGL_PBUFFER_BIT
# if defined Pbuffer
#  pragma push_macro("Pbuffer")
#  undef Pbuffer
	 , Pbuffer(_base())
#  pragma pop_macro("Pbuffer")
# else
	 , Pbuffer(_base())
# endif
#endif
#if defined EGL_MULTISAMPLE_RESOLVE_BOX_BIT
# if defined MultisampleResolveBox
#  pragma push_macro("MultisampleResolveBox")
#  undef MultisampleResolveBox
	 , MultisampleResolveBox(_base())
#  pragma pop_macro("MultisampleResolveBox")
# else
	 , MultisampleResolveBox(_base())
# endif
#endif
#if defined EGL_SWAP_BEHAVIOR_PRESERVED_BIT
# if defined SwapBehaviorPreserved
#  pragma push_macro("SwapBehaviorPreserved")
#  undef SwapBehaviorPreserved
	 , SwapBehaviorPreserved(_base())
#  pragma pop_macro("SwapBehaviorPreserved")
# else
	 , SwapBehaviorPreserved(_base())
# endif
#endif
#if defined EGL_VG_COLORSPACE_LINEAR_BIT
# if defined VGColorspaceLinear
#  pragma push_macro("VGColorspaceLinear")
#  undef VGColorspaceLinear
	 , VGColorspaceLinear(_base())
#  pragma pop_macro("VGColorspaceLinear")
# else
	 , VGColorspaceLinear(_base())
# endif
#endif
#if defined EGL_VG_ALPHA_FORMAT_PRE_BIT
# if defined VGAlphaFormatPre
#  pragma push_macro("VGAlphaFormatPre")
#  undef VGAlphaFormatPre
	 , VGAlphaFormatPre(_base())
#  pragma pop_macro("VGAlphaFormatPre")
# else
	 , VGAlphaFormatPre(_base())
# endif
#endif
	{ }
};

} // namespace enums

