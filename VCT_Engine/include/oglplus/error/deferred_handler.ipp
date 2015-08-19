/**
 *  @file oglplus/error/deferred_handler.ipp
 *  @brief Implementation of DeferredHandler
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <oglplus/assert.hpp>

namespace oglplus {

OGLPLUS_LIB_FUNC
void
DeferredHandler::
_abort_handler::
execute(bool destroying)
{
	if(!destroying)
	{
		OGLPLUS_ABORT("Handler already executed");
	}
}

OGLPLUS_LIB_FUNC
std::unique_ptr<DeferredHandler::_handler_intf>
DeferredHandler::
_release_handler(void)
{
	std::unique_ptr<_handler_intf> tmp;

	if(_handler)
	{
		tmp.reset(new _abort_handler());
	}
	std::swap(tmp, _handler);
	return tmp;
}

} // namespace oglplus

