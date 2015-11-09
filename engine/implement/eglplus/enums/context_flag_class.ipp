//  File implement/eglplus/enums/context_flag_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_flag.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ContextFlag> class Transform>
class EnumToClass<Base, ContextFlag, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_CONTEXT_OPENGL_DEBUG_BIT
# if defined OpenGLDebug
#  pragma push_macro("OpenGLDebug")
#  undef OpenGLDebug
	Transform<ContextFlag::OpenGLDebug> OpenGLDebug;
#  pragma pop_macro("OpenGLDebug")
# else
	Transform<ContextFlag::OpenGLDebug> OpenGLDebug;
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT
# if defined OpenGLForwardCompatible
#  pragma push_macro("OpenGLForwardCompatible")
#  undef OpenGLForwardCompatible
	Transform<ContextFlag::OpenGLForwardCompatible> OpenGLForwardCompatible;
#  pragma pop_macro("OpenGLForwardCompatible")
# else
	Transform<ContextFlag::OpenGLForwardCompatible> OpenGLForwardCompatible;
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
# if defined OpenGLRobustAccess
#  pragma push_macro("OpenGLRobustAccess")
#  undef OpenGLRobustAccess
	Transform<ContextFlag::OpenGLRobustAccess> OpenGLRobustAccess;
#  pragma pop_macro("OpenGLRobustAccess")
# else
	Transform<ContextFlag::OpenGLRobustAccess> OpenGLRobustAccess;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_CONTEXT_OPENGL_DEBUG_BIT
# if defined OpenGLDebug
#  pragma push_macro("OpenGLDebug")
#  undef OpenGLDebug
	 , OpenGLDebug(_base())
#  pragma pop_macro("OpenGLDebug")
# else
	 , OpenGLDebug(_base())
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT
# if defined OpenGLForwardCompatible
#  pragma push_macro("OpenGLForwardCompatible")
#  undef OpenGLForwardCompatible
	 , OpenGLForwardCompatible(_base())
#  pragma pop_macro("OpenGLForwardCompatible")
# else
	 , OpenGLForwardCompatible(_base())
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT
# if defined OpenGLRobustAccess
#  pragma push_macro("OpenGLRobustAccess")
#  undef OpenGLRobustAccess
	 , OpenGLRobustAccess(_base())
#  pragma pop_macro("OpenGLRobustAccess")
# else
	 , OpenGLRobustAccess(_base())
# endif
#endif
	{ }
};

} // namespace enums

