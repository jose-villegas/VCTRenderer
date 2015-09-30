//  File implement/eglplus/enums/error_code_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ErrorCode> class Transform>
class EnumToClass<Base, ErrorCode, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_NOT_INITIALIZED
# if defined NotInitialized
#  pragma push_macro("NotInitialized")
#  undef NotInitialized
	Transform<ErrorCode::NotInitialized> NotInitialized;
#  pragma pop_macro("NotInitialized")
# else
	Transform<ErrorCode::NotInitialized> NotInitialized;
# endif
#endif
#if defined EGL_BAD_ACCESS
# if defined BadAccess
#  pragma push_macro("BadAccess")
#  undef BadAccess
	Transform<ErrorCode::BadAccess> BadAccess;
#  pragma pop_macro("BadAccess")
# else
	Transform<ErrorCode::BadAccess> BadAccess;
# endif
#endif
#if defined EGL_BAD_ALLOC
# if defined BadAlloc
#  pragma push_macro("BadAlloc")
#  undef BadAlloc
	Transform<ErrorCode::BadAlloc> BadAlloc;
#  pragma pop_macro("BadAlloc")
# else
	Transform<ErrorCode::BadAlloc> BadAlloc;
# endif
#endif
#if defined EGL_BAD_ATTRIBUTE
# if defined BadAttribute
#  pragma push_macro("BadAttribute")
#  undef BadAttribute
	Transform<ErrorCode::BadAttribute> BadAttribute;
#  pragma pop_macro("BadAttribute")
# else
	Transform<ErrorCode::BadAttribute> BadAttribute;
# endif
#endif
#if defined EGL_BAD_CONTEXT
# if defined BadContext
#  pragma push_macro("BadContext")
#  undef BadContext
	Transform<ErrorCode::BadContext> BadContext;
#  pragma pop_macro("BadContext")
# else
	Transform<ErrorCode::BadContext> BadContext;
# endif
#endif
#if defined EGL_BAD_CONFIG
# if defined BadConfig
#  pragma push_macro("BadConfig")
#  undef BadConfig
	Transform<ErrorCode::BadConfig> BadConfig;
#  pragma pop_macro("BadConfig")
# else
	Transform<ErrorCode::BadConfig> BadConfig;
# endif
#endif
#if defined EGL_BAD_CURRENT_SURFACE
# if defined BadCurrentSurface
#  pragma push_macro("BadCurrentSurface")
#  undef BadCurrentSurface
	Transform<ErrorCode::BadCurrentSurface> BadCurrentSurface;
#  pragma pop_macro("BadCurrentSurface")
# else
	Transform<ErrorCode::BadCurrentSurface> BadCurrentSurface;
# endif
#endif
#if defined EGL_BAD_DISPLAY
# if defined BadDisplay
#  pragma push_macro("BadDisplay")
#  undef BadDisplay
	Transform<ErrorCode::BadDisplay> BadDisplay;
#  pragma pop_macro("BadDisplay")
# else
	Transform<ErrorCode::BadDisplay> BadDisplay;
# endif
#endif
#if defined EGL_BAD_SURFACE
# if defined BadSurface
#  pragma push_macro("BadSurface")
#  undef BadSurface
	Transform<ErrorCode::BadSurface> BadSurface;
#  pragma pop_macro("BadSurface")
# else
	Transform<ErrorCode::BadSurface> BadSurface;
# endif
#endif
#if defined EGL_BAD_MATCH
# if defined BadMatch
#  pragma push_macro("BadMatch")
#  undef BadMatch
	Transform<ErrorCode::BadMatch> BadMatch;
#  pragma pop_macro("BadMatch")
# else
	Transform<ErrorCode::BadMatch> BadMatch;
# endif
#endif
#if defined EGL_BAD_PARAMETER
# if defined BadParameter
#  pragma push_macro("BadParameter")
#  undef BadParameter
	Transform<ErrorCode::BadParameter> BadParameter;
#  pragma pop_macro("BadParameter")
# else
	Transform<ErrorCode::BadParameter> BadParameter;
# endif
#endif
#if defined EGL_BAD_NATIVE_PIXMAP
# if defined BadNativePixmap
#  pragma push_macro("BadNativePixmap")
#  undef BadNativePixmap
	Transform<ErrorCode::BadNativePixmap> BadNativePixmap;
#  pragma pop_macro("BadNativePixmap")
# else
	Transform<ErrorCode::BadNativePixmap> BadNativePixmap;
# endif
#endif
#if defined EGL_BAD_NATIVE_WINDOW
# if defined BadNativeWindow
#  pragma push_macro("BadNativeWindow")
#  undef BadNativeWindow
	Transform<ErrorCode::BadNativeWindow> BadNativeWindow;
#  pragma pop_macro("BadNativeWindow")
# else
	Transform<ErrorCode::BadNativeWindow> BadNativeWindow;
# endif
#endif
#if defined EGL_CONTEXT_LOST
# if defined ContextLost
#  pragma push_macro("ContextLost")
#  undef ContextLost
	Transform<ErrorCode::ContextLost> ContextLost;
#  pragma pop_macro("ContextLost")
# else
	Transform<ErrorCode::ContextLost> ContextLost;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_NOT_INITIALIZED
# if defined NotInitialized
#  pragma push_macro("NotInitialized")
#  undef NotInitialized
	 , NotInitialized(_base())
#  pragma pop_macro("NotInitialized")
# else
	 , NotInitialized(_base())
# endif
#endif
#if defined EGL_BAD_ACCESS
# if defined BadAccess
#  pragma push_macro("BadAccess")
#  undef BadAccess
	 , BadAccess(_base())
#  pragma pop_macro("BadAccess")
# else
	 , BadAccess(_base())
# endif
#endif
#if defined EGL_BAD_ALLOC
# if defined BadAlloc
#  pragma push_macro("BadAlloc")
#  undef BadAlloc
	 , BadAlloc(_base())
#  pragma pop_macro("BadAlloc")
# else
	 , BadAlloc(_base())
# endif
#endif
#if defined EGL_BAD_ATTRIBUTE
# if defined BadAttribute
#  pragma push_macro("BadAttribute")
#  undef BadAttribute
	 , BadAttribute(_base())
#  pragma pop_macro("BadAttribute")
# else
	 , BadAttribute(_base())
# endif
#endif
#if defined EGL_BAD_CONTEXT
# if defined BadContext
#  pragma push_macro("BadContext")
#  undef BadContext
	 , BadContext(_base())
#  pragma pop_macro("BadContext")
# else
	 , BadContext(_base())
# endif
#endif
#if defined EGL_BAD_CONFIG
# if defined BadConfig
#  pragma push_macro("BadConfig")
#  undef BadConfig
	 , BadConfig(_base())
#  pragma pop_macro("BadConfig")
# else
	 , BadConfig(_base())
# endif
#endif
#if defined EGL_BAD_CURRENT_SURFACE
# if defined BadCurrentSurface
#  pragma push_macro("BadCurrentSurface")
#  undef BadCurrentSurface
	 , BadCurrentSurface(_base())
#  pragma pop_macro("BadCurrentSurface")
# else
	 , BadCurrentSurface(_base())
# endif
#endif
#if defined EGL_BAD_DISPLAY
# if defined BadDisplay
#  pragma push_macro("BadDisplay")
#  undef BadDisplay
	 , BadDisplay(_base())
#  pragma pop_macro("BadDisplay")
# else
	 , BadDisplay(_base())
# endif
#endif
#if defined EGL_BAD_SURFACE
# if defined BadSurface
#  pragma push_macro("BadSurface")
#  undef BadSurface
	 , BadSurface(_base())
#  pragma pop_macro("BadSurface")
# else
	 , BadSurface(_base())
# endif
#endif
#if defined EGL_BAD_MATCH
# if defined BadMatch
#  pragma push_macro("BadMatch")
#  undef BadMatch
	 , BadMatch(_base())
#  pragma pop_macro("BadMatch")
# else
	 , BadMatch(_base())
# endif
#endif
#if defined EGL_BAD_PARAMETER
# if defined BadParameter
#  pragma push_macro("BadParameter")
#  undef BadParameter
	 , BadParameter(_base())
#  pragma pop_macro("BadParameter")
# else
	 , BadParameter(_base())
# endif
#endif
#if defined EGL_BAD_NATIVE_PIXMAP
# if defined BadNativePixmap
#  pragma push_macro("BadNativePixmap")
#  undef BadNativePixmap
	 , BadNativePixmap(_base())
#  pragma pop_macro("BadNativePixmap")
# else
	 , BadNativePixmap(_base())
# endif
#endif
#if defined EGL_BAD_NATIVE_WINDOW
# if defined BadNativeWindow
#  pragma push_macro("BadNativeWindow")
#  undef BadNativeWindow
	 , BadNativeWindow(_base())
#  pragma pop_macro("BadNativeWindow")
# else
	 , BadNativeWindow(_base())
# endif
#endif
#if defined EGL_CONTEXT_LOST
# if defined ContextLost
#  pragma push_macro("ContextLost")
#  undef ContextLost
	 , ContextLost(_base())
#  pragma pop_macro("ContextLost")
# else
	 , ContextLost(_base())
# endif
#endif
	{ }
};

} // namespace enums

