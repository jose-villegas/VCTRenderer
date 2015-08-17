//  File implement/oalplus/enums/al_error_code_class.ipp
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
namespace enums {
template <typename Base, template<ALErrorCode> class Transform>
class EnumToClass<Base, ALErrorCode, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_NO_ERROR
# if defined NoError
#  pragma push_macro("NoError")
#  undef NoError
	Transform<ALErrorCode::NoError> NoError;
#  pragma pop_macro("NoError")
# else
	Transform<ALErrorCode::NoError> NoError;
# endif
#endif
#if defined AL_INVALID_NAME
# if defined InvalidName
#  pragma push_macro("InvalidName")
#  undef InvalidName
	Transform<ALErrorCode::InvalidName> InvalidName;
#  pragma pop_macro("InvalidName")
# else
	Transform<ALErrorCode::InvalidName> InvalidName;
# endif
#endif
#if defined AL_INVALID_ENUM
# if defined InvalidEnum
#  pragma push_macro("InvalidEnum")
#  undef InvalidEnum
	Transform<ALErrorCode::InvalidEnum> InvalidEnum;
#  pragma pop_macro("InvalidEnum")
# else
	Transform<ALErrorCode::InvalidEnum> InvalidEnum;
# endif
#endif
#if defined AL_INVALID_VALUE
# if defined InvalidValue
#  pragma push_macro("InvalidValue")
#  undef InvalidValue
	Transform<ALErrorCode::InvalidValue> InvalidValue;
#  pragma pop_macro("InvalidValue")
# else
	Transform<ALErrorCode::InvalidValue> InvalidValue;
# endif
#endif
#if defined AL_INVALID_OPERATION
# if defined InvalidOperation
#  pragma push_macro("InvalidOperation")
#  undef InvalidOperation
	Transform<ALErrorCode::InvalidOperation> InvalidOperation;
#  pragma pop_macro("InvalidOperation")
# else
	Transform<ALErrorCode::InvalidOperation> InvalidOperation;
# endif
#endif
#if defined AL_OUT_OF_MEMORY
# if defined OutOfMemory
#  pragma push_macro("OutOfMemory")
#  undef OutOfMemory
	Transform<ALErrorCode::OutOfMemory> OutOfMemory;
#  pragma pop_macro("OutOfMemory")
# else
	Transform<ALErrorCode::OutOfMemory> OutOfMemory;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_NO_ERROR
# if defined NoError
#  pragma push_macro("NoError")
#  undef NoError
	 , NoError(_base())
#  pragma pop_macro("NoError")
# else
	 , NoError(_base())
# endif
#endif
#if defined AL_INVALID_NAME
# if defined InvalidName
#  pragma push_macro("InvalidName")
#  undef InvalidName
	 , InvalidName(_base())
#  pragma pop_macro("InvalidName")
# else
	 , InvalidName(_base())
# endif
#endif
#if defined AL_INVALID_ENUM
# if defined InvalidEnum
#  pragma push_macro("InvalidEnum")
#  undef InvalidEnum
	 , InvalidEnum(_base())
#  pragma pop_macro("InvalidEnum")
# else
	 , InvalidEnum(_base())
# endif
#endif
#if defined AL_INVALID_VALUE
# if defined InvalidValue
#  pragma push_macro("InvalidValue")
#  undef InvalidValue
	 , InvalidValue(_base())
#  pragma pop_macro("InvalidValue")
# else
	 , InvalidValue(_base())
# endif
#endif
#if defined AL_INVALID_OPERATION
# if defined InvalidOperation
#  pragma push_macro("InvalidOperation")
#  undef InvalidOperation
	 , InvalidOperation(_base())
#  pragma pop_macro("InvalidOperation")
# else
	 , InvalidOperation(_base())
# endif
#endif
#if defined AL_OUT_OF_MEMORY
# if defined OutOfMemory
#  pragma push_macro("OutOfMemory")
#  undef OutOfMemory
	 , OutOfMemory(_base())
#  pragma pop_macro("OutOfMemory")
# else
	 , OutOfMemory(_base())
# endif
#endif
	{ }
};

} // namespace enums

