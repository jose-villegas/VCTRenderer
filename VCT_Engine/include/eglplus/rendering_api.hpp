/**
 *  @file eglplus/rendering_api.hpp
 *  @brief EGL rendering API enumeration
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef EGLPLUS_RENDERING_API_1303292057_HPP
#define EGLPLUS_RENDERING_API_1303292057_HPP

#include <eglplus/enums/rendering_api.hpp>
#include <eglplus/boolean.hpp>
#include <eglplus/eglfunc.hpp>
#include <eglplus/error/basic.hpp>

namespace eglplus {

/// Bind the specified rendering API
/**
 *  @eglsymbols
 *  @eglfunref{BindAPI}
 */
inline Boolean BindAPI(RenderingAPI api)
{
	Boolean result(
		EGLPLUS_EGLFUNC(BindAPI)(EGLenum(api)),
		std::nothrow
	);
	EGLPLUS_VERIFY_SIMPLE(BindAPI);
	return result;
}

/// Query the currently bound rendering API
/**
 *  @eglsymbols
 *  @eglfunref{QueryAPI}
 */
inline RenderingAPI QueryAPI(void)
{
	EGLenum result = EGLPLUS_EGLFUNC(QueryAPI)();
	EGLPLUS_VERIFY_SIMPLE(QueryAPI);
	return RenderingAPI(result);
}

} // namespace eglplus

#endif // include guard
