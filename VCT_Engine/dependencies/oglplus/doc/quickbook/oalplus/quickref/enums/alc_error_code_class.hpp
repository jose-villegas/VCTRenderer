//  File doc/quickbook/oalplus/quickref/enums/alc_error_code_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/alc_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_alc_error_code_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ALCErrorCode> class Transform>
class __EnumToClass<Base, __ALCErrorCode, Transform> /*<
Specialization of __EnumToClass for the __ALCErrorCode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ALCErrorCode::NoError>
		NoError;
	Transform<ALCErrorCode::InvalidDevice>
		InvalidDevice;
	Transform<ALCErrorCode::InvalidContext>
		InvalidContext;
	Transform<ALCErrorCode::InvalidEnum>
		InvalidEnum;
	Transform<ALCErrorCode::InvalidValue>
		InvalidValue;
	Transform<ALCErrorCode::OutOfMemory>
		OutOfMemory;
};

} // namespace enums
#endif
//]

