//  File implement/eglplus/enums/render_buffer_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/render_buffer.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<RenderBuffer> class Transform>
class EnumToClass<Base, RenderBuffer, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_SINGLE_BUFFER
# if defined SingleBuffer
#  pragma push_macro("SingleBuffer")
#  undef SingleBuffer
	Transform<RenderBuffer::SingleBuffer> SingleBuffer;
#  pragma pop_macro("SingleBuffer")
# else
	Transform<RenderBuffer::SingleBuffer> SingleBuffer;
# endif
#endif
#if defined EGL_BACK_BUFFER
# if defined BackBuffer
#  pragma push_macro("BackBuffer")
#  undef BackBuffer
	Transform<RenderBuffer::BackBuffer> BackBuffer;
#  pragma pop_macro("BackBuffer")
# else
	Transform<RenderBuffer::BackBuffer> BackBuffer;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_SINGLE_BUFFER
# if defined SingleBuffer
#  pragma push_macro("SingleBuffer")
#  undef SingleBuffer
	 , SingleBuffer(_base())
#  pragma pop_macro("SingleBuffer")
# else
	 , SingleBuffer(_base())
# endif
#endif
#if defined EGL_BACK_BUFFER
# if defined BackBuffer
#  pragma push_macro("BackBuffer")
#  undef BackBuffer
	 , BackBuffer(_base())
#  pragma pop_macro("BackBuffer")
# else
	 , BackBuffer(_base())
# endif
#endif
	{ }
};

} // namespace enums

