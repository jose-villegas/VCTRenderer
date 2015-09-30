/**
 *  @file oalplus/error/al.hpp
 *  @brief Declaration of OALplus' OpenAL exceptions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ERROR_AL_1107121317_HPP
#define OALPLUS_ERROR_AL_1107121317_HPP

#include <oalplus/error/basic.hpp>
#include <oalplus/enums/al_error_code.hpp>

namespace oalplus {

class ErrorAL
 : public Error
{
public:
	static const char* Message(ALenum error_code)
	{
		return ::alGetString(error_code);
	}

	ErrorAL(const char* message)
	 : Error(message)
	{ }

	ALErrorCode Code(void) const
	OALPLUS_NOEXCEPT(true)
	{
		return ALErrorCode(_code);
	}
};

#define OALPLUS_ALFUNC_CHECK(FUNC_NAME, ERROR, ERROR_INFO)\
	OALPLUS_HANDLE_ERROR_IF(\
		error_code != AL_NO_ERROR,\
		alGetError(),\
		ERROR::Message(error_code),\
		ERROR,\
		ERROR_INFO.\
		ALFunc(FUNC_NAME)\
	)

#define OALPLUS_CHECK(ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_ALFUNC_CHECK(#ALFUNC, ERROR, ERROR_INFO)

#define OALPLUS_CHECK_SIMPLE(ALFUNC) \
	OALPLUS_CHECK(ALFUNC, ErrorAL, NoInfo())

#if !OALPLUS_LOW_PROFILE
#define OALPLUS_VERIFY(ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_CHECK(ALFUNC, ERROR, ERROR_INFO)
#else
#define OALPLUS_VERIFY(ALFUNC, ERROR, ERROR_INFO)
#endif

#define OALPLUS_VERIFY_SIMPLE(ALFUNC) \
	OALPLUS_CHECK(ALFUNC, ErrorAL, NoInfo())

#define OALPLUS_IGNORE(ALLIB, PARAM) ::ALLIB ## GetError();

} // namespace oalplus

#endif // include guard
