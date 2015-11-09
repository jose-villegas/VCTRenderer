//  File implement/eglplus/enums/gl_colorspace_class.ipp
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
namespace enums {
template <typename Base, template<GLColorspace> class Transform>
class EnumToClass<Base, GLColorspace, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_GL_COLORSPACE_sRGB
# if defined sRGB
#  pragma push_macro("sRGB")
#  undef sRGB
	Transform<GLColorspace::sRGB> sRGB;
#  pragma pop_macro("sRGB")
# else
	Transform<GLColorspace::sRGB> sRGB;
# endif
#endif
#if defined EGL_GL_COLORSPACE_LINEAR
# if defined Linear
#  pragma push_macro("Linear")
#  undef Linear
	Transform<GLColorspace::Linear> Linear;
#  pragma pop_macro("Linear")
# else
	Transform<GLColorspace::Linear> Linear;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_GL_COLORSPACE_sRGB
# if defined sRGB
#  pragma push_macro("sRGB")
#  undef sRGB
	 , sRGB(_base())
#  pragma pop_macro("sRGB")
# else
	 , sRGB(_base())
# endif
#endif
#if defined EGL_GL_COLORSPACE_LINEAR
# if defined Linear
#  pragma push_macro("Linear")
#  undef Linear
	 , Linear(_base())
#  pragma pop_macro("Linear")
# else
	 , Linear(_base())
# endif
#endif
	{ }
};

} // namespace enums

