/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_gpu_address

#if GL_NV_shader_buffer_load

class BufferGPUAddress
{
public:
	BufferGPUAddress(GLuint64EXT addr);
};

GLuint64EXT GetGLAddress(BufferGPUAddress bga); /*<
Returns the underlying buffer address value.
>*/

#endif

//]

