/**
 *  @file oalplus/error/alc.hpp
 *  @brief Declaration of OALplus ALC exception
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ERROR_ALC_1107121317_HPP
#define OALPLUS_ERROR_ALC_1107121317_HPP

#include <oalplus/error/basic.hpp>
#include <oalplus/enums/alc_error_code.hpp>

namespace oalplus {

class ErrorALC
 : public Error
{
private:
	const ::ALCdevice* _device;
public:
	static const char* Message(::ALCdevice* device, ALenum error_code)
	{
		return ::alcGetString(device, error_code);
	}

	ErrorALC(const char* message)
	 : Error(message)
	 , _device(nullptr)
	{ }

#if !OGLPLUS_NO_DEFAULTED_FUNCTIONS
	ErrorALC(const ErrorALC&) = default;
#else
	ErrorALC(const ErrorALC& that)
	 : _device(that._device)
	{ }
#endif

	~ErrorALC(void)
	OGLPLUS_NOTHROW
	{ }

	ALCErrorCode Code(void) const
	OALPLUS_NOEXCEPT(true)
	{
		return ALCErrorCode(_code);
	}

	ErrorALC& Device(const ::ALCdevice* device)
	{
		_device = device;
		return *this;
	}

	const ::ALCdevice* Device(void) const
	{
		return _device;
	}
};

#define OALPLUS_CHECK_ALC(DEVICE, ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_HANDLE_ERROR_IF(\
		error_code != ALC_NO_ERROR,\
		alcGetError(DEVICE),\
		ERROR::Message(DEVICE, error_code),\
		ERROR,\
		ERROR_INFO.\
		Device(DEVICE).\
		ALLib("alc").\
		ALFunc(#ALFUNC)\
	)

#define OALPLUS_CHECK_SIMPLE_ALC(DEVICE, ALFUNC) \
	OALPLUS_CHECK_ALC(DEVICE, ALFUNC, ErrorALC, Device(DEVICE))

#if !OALPLUS_LOW_PROFILE
#define OALPLUS_VERIFY_ALC(DEVICE, ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_CHECK_ALC(DEVICE, ALFUNC, ERROR, ERROR_INFO)
#else
#define OALPLUS_VERIFY_ALC(DEVICE, ALFUNC, ERROR, ERROR_INFO)
#endif

#define OALPLUS_VERIFY_SIMPLE_ALC(DEVICE, ALFUNC) \
	OALPLUS_VERIFY_ALC(DEVICE, ALFUNC, ErrorALC, Device(DEVICE))

} // namespace oalplus

#endif // include guard
