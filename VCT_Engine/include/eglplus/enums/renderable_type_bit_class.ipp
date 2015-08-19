//  File implement/eglplus/enums/renderable_type_bit_class.ipp
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
namespace enums {
template <typename Base, template<RenderableTypeBit> class Transform>
class EnumToClass<Base, RenderableTypeBit, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_OPENGL_BIT
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
	Transform<RenderableTypeBit::OpenGL> OpenGL;
#  pragma pop_macro("OpenGL")
# else
	Transform<RenderableTypeBit::OpenGL> OpenGL;
# endif
#endif
#if defined EGL_OPENGL_ES_BIT
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
	Transform<RenderableTypeBit::OpenGL_ES> OpenGL_ES;
#  pragma pop_macro("OpenGL_ES")
# else
	Transform<RenderableTypeBit::OpenGL_ES> OpenGL_ES;
# endif
#endif
#if defined EGL_OPENGL_ES2_BIT
# if defined OpenGL_ES2
#  pragma push_macro("OpenGL_ES2")
#  undef OpenGL_ES2
	Transform<RenderableTypeBit::OpenGL_ES2> OpenGL_ES2;
#  pragma pop_macro("OpenGL_ES2")
# else
	Transform<RenderableTypeBit::OpenGL_ES2> OpenGL_ES2;
# endif
#endif
#if defined EGL_OPENGL_ES3_BIT
# if defined OpenGL_ES3
#  pragma push_macro("OpenGL_ES3")
#  undef OpenGL_ES3
	Transform<RenderableTypeBit::OpenGL_ES3> OpenGL_ES3;
#  pragma pop_macro("OpenGL_ES3")
# else
	Transform<RenderableTypeBit::OpenGL_ES3> OpenGL_ES3;
# endif
#endif
#if defined EGL_OPENVG_BIT
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
	Transform<RenderableTypeBit::OpenVG> OpenVG;
#  pragma pop_macro("OpenVG")
# else
	Transform<RenderableTypeBit::OpenVG> OpenVG;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_OPENGL_BIT
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
	 , OpenGL(_base())
#  pragma pop_macro("OpenGL")
# else
	 , OpenGL(_base())
# endif
#endif
#if defined EGL_OPENGL_ES_BIT
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
	 , OpenGL_ES(_base())
#  pragma pop_macro("OpenGL_ES")
# else
	 , OpenGL_ES(_base())
# endif
#endif
#if defined EGL_OPENGL_ES2_BIT
# if defined OpenGL_ES2
#  pragma push_macro("OpenGL_ES2")
#  undef OpenGL_ES2
	 , OpenGL_ES2(_base())
#  pragma pop_macro("OpenGL_ES2")
# else
	 , OpenGL_ES2(_base())
# endif
#endif
#if defined EGL_OPENGL_ES3_BIT
# if defined OpenGL_ES3
#  pragma push_macro("OpenGL_ES3")
#  undef OpenGL_ES3
	 , OpenGL_ES3(_base())
#  pragma pop_macro("OpenGL_ES3")
# else
	 , OpenGL_ES3(_base())
# endif
#endif
#if defined EGL_OPENVG_BIT
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
	 , OpenVG(_base())
#  pragma pop_macro("OpenVG")
# else
	 , OpenVG(_base())
# endif
#endif
	{ }
};

} // namespace enums

