//  File implement/oalplus/enums/alc_error_code_class.ipp
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
namespace enums {
template <typename Base, template<ALCErrorCode> class Transform>
class EnumToClass<Base, ALCErrorCode, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_NO_ERROR
# if defined NoError
#  pragma push_macro("NoError")
#  undef NoError
	Transform<ALCErrorCode::NoError> NoError;
#  pragma pop_macro("NoError")
# else
	Transform<ALCErrorCode::NoError> NoError;
# endif
#endif
#if defined AL_INVALID_DEVICE
# if defined InvalidDevice
#  pragma push_macro("InvalidDevice")
#  undef InvalidDevice
	Transform<ALCErrorCode::InvalidDevice> InvalidDevice;
#  pragma pop_macro("InvalidDevice")
# else
	Transform<ALCErrorCode::InvalidDevice> InvalidDevice;
# endif
#endif
#if defined AL_INVALID_CONTEXT
# if defined InvalidContext
#  pragma push_macro("InvalidContext")
#  undef InvalidContext
	Transform<ALCErrorCode::InvalidContext> InvalidContext;
#  pragma pop_macro("InvalidContext")
# else
	Transform<ALCErrorCode::InvalidContext> InvalidContext;
# endif
#endif
#if defined AL_INVALID_ENUM
# if defined InvalidEnum
#  pragma push_macro("InvalidEnum")
#  undef InvalidEnum
	Transform<ALCErrorCode::InvalidEnum> InvalidEnum;
#  pragma pop_macro("InvalidEnum")
# else
	Transform<ALCErrorCode::InvalidEnum> InvalidEnum;
# endif
#endif
#if defined AL_INVALID_VALUE
# if defined InvalidValue
#  pragma push_macro("InvalidValue")
#  undef InvalidValue
	Transform<ALCErrorCode::InvalidValue> InvalidValue;
#  pragma pop_macro("InvalidValue")
# else
	Transform<ALCErrorCode::InvalidValue> InvalidValue;
# endif
#endif
#if defined AL_OUT_OF_MEMORY
# if defined OutOfMemory
#  pragma push_macro("OutOfMemory")
#  undef OutOfMemory
	Transform<ALCErrorCode::OutOfMemory> OutOfMemory;
#  pragma pop_macro("OutOfMemory")
# else
	Transform<ALCErrorCode::OutOfMemory> OutOfMemory;
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
#if defined AL_INVALID_DEVICE
# if defined InvalidDevice
#  pragma push_macro("InvalidDevice")
#  undef InvalidDevice
	 , InvalidDevice(_base())
#  pragma pop_macro("InvalidDevice")
# else
	 , InvalidDevice(_base())
# endif
#endif
#if defined AL_INVALID_CONTEXT
# if defined InvalidContext
#  pragma push_macro("InvalidContext")
#  undef InvalidContext
	 , InvalidContext(_base())
#  pragma pop_macro("InvalidContext")
# else
	 , InvalidContext(_base())
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

