//  File implement/eglplus/enums/context_attrib_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/context_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ContextAttrib> class Transform>
class EnumToClass<Base, ContextAttrib, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_CONTEXT_MAJOR_VERSION
# if defined MajorVersion
#  pragma push_macro("MajorVersion")
#  undef MajorVersion
	Transform<ContextAttrib::MajorVersion> MajorVersion;
#  pragma pop_macro("MajorVersion")
# else
	Transform<ContextAttrib::MajorVersion> MajorVersion;
# endif
#endif
#if defined EGL_CONTEXT_MINOR_VERSION
# if defined MinorVersion
#  pragma push_macro("MinorVersion")
#  undef MinorVersion
	Transform<ContextAttrib::MinorVersion> MinorVersion;
#  pragma pop_macro("MinorVersion")
# else
	Transform<ContextAttrib::MinorVersion> MinorVersion;
# endif
#endif
#if defined EGL_CONTEXT_FLAGS
# if defined Flags
#  pragma push_macro("Flags")
#  undef Flags
	Transform<ContextAttrib::Flags> Flags;
#  pragma pop_macro("Flags")
# else
	Transform<ContextAttrib::Flags> Flags;
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_PROFILE_MASK
# if defined OpenGLProfileMask
#  pragma push_macro("OpenGLProfileMask")
#  undef OpenGLProfileMask
	Transform<ContextAttrib::OpenGLProfileMask> OpenGLProfileMask;
#  pragma pop_macro("OpenGLProfileMask")
# else
	Transform<ContextAttrib::OpenGLProfileMask> OpenGLProfileMask;
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
# if defined OpenGLResetNotificationStrategy
#  pragma push_macro("OpenGLResetNotificationStrategy")
#  undef OpenGLResetNotificationStrategy
	Transform<ContextAttrib::OpenGLResetNotificationStrategy> OpenGLResetNotificationStrategy;
#  pragma pop_macro("OpenGLResetNotificationStrategy")
# else
	Transform<ContextAttrib::OpenGLResetNotificationStrategy> OpenGLResetNotificationStrategy;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_CONTEXT_MAJOR_VERSION
# if defined MajorVersion
#  pragma push_macro("MajorVersion")
#  undef MajorVersion
	 , MajorVersion(_base())
#  pragma pop_macro("MajorVersion")
# else
	 , MajorVersion(_base())
# endif
#endif
#if defined EGL_CONTEXT_MINOR_VERSION
# if defined MinorVersion
#  pragma push_macro("MinorVersion")
#  undef MinorVersion
	 , MinorVersion(_base())
#  pragma pop_macro("MinorVersion")
# else
	 , MinorVersion(_base())
# endif
#endif
#if defined EGL_CONTEXT_FLAGS
# if defined Flags
#  pragma push_macro("Flags")
#  undef Flags
	 , Flags(_base())
#  pragma pop_macro("Flags")
# else
	 , Flags(_base())
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_PROFILE_MASK
# if defined OpenGLProfileMask
#  pragma push_macro("OpenGLProfileMask")
#  undef OpenGLProfileMask
	 , OpenGLProfileMask(_base())
#  pragma pop_macro("OpenGLProfileMask")
# else
	 , OpenGLProfileMask(_base())
# endif
#endif
#if defined EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY
# if defined OpenGLResetNotificationStrategy
#  pragma push_macro("OpenGLResetNotificationStrategy")
#  undef OpenGLResetNotificationStrategy
	 , OpenGLResetNotificationStrategy(_base())
#  pragma pop_macro("OpenGLResetNotificationStrategy")
# else
	 , OpenGLResetNotificationStrategy(_base())
# endif
#endif
	{ }
};

} // namespace enums

