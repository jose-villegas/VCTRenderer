//  File implement/eglplus/enums/rendering_api_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/rendering_api.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<RenderingAPI> class Transform>
class EnumToClass<Base, RenderingAPI, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_OPENGL_API
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
	Transform<RenderingAPI::OpenGL> OpenGL;
#  pragma pop_macro("OpenGL")
# else
	Transform<RenderingAPI::OpenGL> OpenGL;
# endif
#endif
#if defined EGL_OPENGL_ES_API
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
	Transform<RenderingAPI::OpenGL_ES> OpenGL_ES;
#  pragma pop_macro("OpenGL_ES")
# else
	Transform<RenderingAPI::OpenGL_ES> OpenGL_ES;
# endif
#endif
#if defined EGL_OPENVG_API
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
	Transform<RenderingAPI::OpenVG> OpenVG;
#  pragma pop_macro("OpenVG")
# else
	Transform<RenderingAPI::OpenVG> OpenVG;
# endif
#endif
#if defined EGL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	Transform<RenderingAPI::None> None;
#  pragma pop_macro("None")
# else
	Transform<RenderingAPI::None> None;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_OPENGL_API
# if defined OpenGL
#  pragma push_macro("OpenGL")
#  undef OpenGL
	 , OpenGL(_base())
#  pragma pop_macro("OpenGL")
# else
	 , OpenGL(_base())
# endif
#endif
#if defined EGL_OPENGL_ES_API
# if defined OpenGL_ES
#  pragma push_macro("OpenGL_ES")
#  undef OpenGL_ES
	 , OpenGL_ES(_base())
#  pragma pop_macro("OpenGL_ES")
# else
	 , OpenGL_ES(_base())
# endif
#endif
#if defined EGL_OPENVG_API
# if defined OpenVG
#  pragma push_macro("OpenVG")
#  undef OpenVG
	 , OpenVG(_base())
#  pragma pop_macro("OpenVG")
# else
	 , OpenVG(_base())
# endif
#endif
#if defined EGL_NONE
# if defined None
#  pragma push_macro("None")
#  undef None
	 , None(_base())
#  pragma pop_macro("None")
# else
	 , None(_base())
# endif
#endif
	{ }
};

} // namespace enums

