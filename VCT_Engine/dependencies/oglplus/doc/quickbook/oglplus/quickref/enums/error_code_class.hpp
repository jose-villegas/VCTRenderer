//  File doc/quickbook/oglplus/quickref/enums/error_code_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_error_code_class
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

	Transform<ErrorCode::NoError>
		NoError;
	Transform<ErrorCode::OutOfMemory>
		OutOfMemory;
	Transform<ErrorCode::InvalidEnum>
		InvalidEnum;
	Transform<ErrorCode::InvalidValue>
		InvalidValue;
	Transform<ErrorCode::InvalidOperation>
		InvalidOperation;
	Transform<ErrorCode::InvalidFramebufferOperation>
		InvalidFramebufferOperation;
	Transform<ErrorCode::StackOverflow>
		StackOverflow;
	Transform<ErrorCode::StackUnderflow>
		StackUnderflow;
	Transform<ErrorCode::TableTooLarge>
		TableTooLarge;
	Transform<ErrorCode::ContextLost>
		ContextLost;
};

} // namespace enums
#endif
//]

