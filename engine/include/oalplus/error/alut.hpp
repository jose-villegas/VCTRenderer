/**
 *  @file oalplus/error/alut.hpp
 *  @brief Declaration of OALplus ALUT exception
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_ERROR_ALUT_1107121317_HPP
#define OALPLUS_ERROR_ALUT_1107121317_HPP

#include <oalplus/error/basic.hpp>
#include <oalplus/enums/alut_error_code.hpp>

namespace oalplus {

class ErrorALUT
 : public Error
{
public:
	static const char* Message(ALenum error_code);

	ErrorALUT(const char* message)
	 : Error(message)
	{ }

#if !OGLPLUS_NO_DEFAULTED_FUNCTIONS
	ErrorALUT(const ErrorALUT&) = default;
#else
	ErrorALUT(const ErrorALUT& that)
	 : Error(static_cast<const Error&>(that))
	{ }
#endif

	~ErrorALUT(void)
	OGLPLUS_NOTHROW
	{ }

	ALUTErrorCode Code(void) const
	OALPLUS_NOEXCEPT(true)
	{
		return ALUTErrorCode(_code);
	}
};

#define OALPLUS_CHECK_ALUT(ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_HANDLE_ERROR_IF(\
		error_code != AL_NO_ERROR,\
		alutGetError(),\
		ERROR::Message(error_code),\
		ERROR,\
		ERROR_INFO.\
		ALLib("alut").\
		ALFunc(#ALFUNC)\
	)

#define OALPLUS_CHECK_SIMPLE_ALUT(ALFUNC) \
	OALPLUS_CHECK_ALUT(ALFUNC, ErrorALUT, NoInfo())

#if !OALPLUS_LOW_PROFILE
#define OALPLUS_VERIFY_ALUT(ALFUNC, ERROR, ERROR_INFO) \
	OALPLUS_CHECK_ALUT(ALFUNC, ERROR, ERROR_INFO)
#else
#define OALPLUS_VERIFY_ALUT(ALFUNC, ERROR, ERROR_INFO)
#endif

#define OALPLUS_VERIFY_SIMPLE_ALUT(ALFUNC) \
	OALPLUS_VERIFY_ALUT(ALFUNC, ErrorALUT, NoInfo())

} // namespace oalplus

#if !OALPLUS_LINK_LIBRARY || defined(OALPLUS_IMPLEMENTING_LIBRARY)
#include <oalplus/error/alut.ipp>
#endif

#endif // include guard
