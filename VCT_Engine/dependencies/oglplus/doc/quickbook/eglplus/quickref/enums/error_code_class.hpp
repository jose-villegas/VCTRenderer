//  File doc/quickbook/eglplus/quickref/enums/error_code_class.hpp
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
//[eglplus_enums_error_code_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ErrorCode> class Transform>
class __EnumToClass<Base, __ErrorCode, Transform> /*<
Specialization of __EnumToClass for the __ErrorCode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ErrorCode::NotInitialized>
		NotInitialized;
	Transform<ErrorCode::BadAccess>
		BadAccess;
	Transform<ErrorCode::BadAlloc>
		BadAlloc;
	Transform<ErrorCode::BadAttribute>
		BadAttribute;
	Transform<ErrorCode::BadContext>
		BadContext;
	Transform<ErrorCode::BadConfig>
		BadConfig;
	Transform<ErrorCode::BadCurrentSurface>
		BadCurrentSurface;
	Transform<ErrorCode::BadDisplay>
		BadDisplay;
	Transform<ErrorCode::BadSurface>
		BadSurface;
	Transform<ErrorCode::BadMatch>
		BadMatch;
	Transform<ErrorCode::BadParameter>
		BadParameter;
	Transform<ErrorCode::BadNativePixmap>
		BadNativePixmap;
	Transform<ErrorCode::BadNativeWindow>
		BadNativeWindow;
	Transform<ErrorCode::ContextLost>
		ContextLost;
};

} // namespace enums
#endif
//]

