//  File doc/quickbook/oalplus/quickref/enums/al_error_code_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/al_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_al_error_code_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ALErrorCode> class Transform>
class __EnumToClass<Base, __ALErrorCode, Transform> /*<
Specialization of __EnumToClass for the __ALErrorCode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ALErrorCode::NoError>
		NoError;
	Transform<ALErrorCode::InvalidName>
		InvalidName;
	Transform<ALErrorCode::InvalidEnum>
		InvalidEnum;
	Transform<ALErrorCode::InvalidValue>
		InvalidValue;
	Transform<ALErrorCode::InvalidOperation>
		InvalidOperation;
	Transform<ALErrorCode::OutOfMemory>
		OutOfMemory;
};

} // namespace enums
#endif
//]

