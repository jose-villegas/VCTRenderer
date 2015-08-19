/**
 *  @file oalplus/context.ipp
 *  @brief Implementation of Context functions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oalplus {

OALPLUS_LIB_FUNC
ContextOps ContextOps::
Current(void)
{
	::ALCcontext* context = OALPLUS_ALFUNC(alc,GetCurrentContext)();
	OALPLUS_HANDLE_ERROR_IF(
		!context,
		ALC_INVALID_CONTEXT,
		"Failed to get current AL context",
		Error,
		ALLib("alc").
		ALFunc("GetCurrentContext")
	);

	::ALCdevice* device = OALPLUS_ALFUNC(alc,GetContextsDevice)(context);
	OALPLUS_HANDLE_ERROR_IF(
		!device,
		ALC_INVALID_DEVICE,
		"Failed to get AL context's device",
		Error,
		ALLib("alc").
		ALFunc("GetContextsDevice")
	);
	return ContextOps(device, context);
}

} // namespace oalplus

