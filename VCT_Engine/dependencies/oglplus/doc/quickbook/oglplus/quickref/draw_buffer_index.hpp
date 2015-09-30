/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_draw_buffer_index

class DrawBufferIndex
 : public __LimitedCount<GL_MAX_DRAW_BUFFERS>
{
public:
	DrawBufferIndex(GLuint number); /*<
	Throws __LimitError if [^number] is greater than
	the implementation-dependent maximum number of vertex attributes.
	See [glconst MAX_DRAW_BUFFERS].
	>*/
};

//]

