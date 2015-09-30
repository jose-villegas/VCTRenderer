/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_IncompleteFramebuffer

class IncompleteFramebuffer
 : public __ObjectError
{
public:
	IncompleteFramebuffer(const char* message);

	__FramebufferStatus Status(void) const; /*<
	Returns the status of the __Framebuffer that raised this error.
	>*/
};

//]

