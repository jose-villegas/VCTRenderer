//  File implement/eglplus/enums/color_buffer_type_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/color_buffer_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ColorBufferType> class Transform>
class EnumToClass<Base, ColorBufferType, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_RGB_BUFFER
# if defined RGBBuffer
#  pragma push_macro("RGBBuffer")
#  undef RGBBuffer
	Transform<ColorBufferType::RGBBuffer> RGBBuffer;
#  pragma pop_macro("RGBBuffer")
# else
	Transform<ColorBufferType::RGBBuffer> RGBBuffer;
# endif
#endif
#if defined EGL_LUMINANCE_BUFFER
# if defined LuminanceBuffer
#  pragma push_macro("LuminanceBuffer")
#  undef LuminanceBuffer
	Transform<ColorBufferType::LuminanceBuffer> LuminanceBuffer;
#  pragma pop_macro("LuminanceBuffer")
# else
	Transform<ColorBufferType::LuminanceBuffer> LuminanceBuffer;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_RGB_BUFFER
# if defined RGBBuffer
#  pragma push_macro("RGBBuffer")
#  undef RGBBuffer
	 , RGBBuffer(_base())
#  pragma pop_macro("RGBBuffer")
# else
	 , RGBBuffer(_base())
# endif
#endif
#if defined EGL_LUMINANCE_BUFFER
# if defined LuminanceBuffer
#  pragma push_macro("LuminanceBuffer")
#  undef LuminanceBuffer
	 , LuminanceBuffer(_base())
#  pragma pop_macro("LuminanceBuffer")
# else
	 , LuminanceBuffer(_base())
# endif
#endif
	{ }
};

} // namespace enums

