/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_Synchronization
namespace context {

class Synchronization
{
public:
#if GL_VERSION_4_2 || GL_ARB_shader_image_load_store
	static void MemoryBarrier(__Bitfield<__MemoryBarrierBit> bits); /*<
	Defines a barrier ordering memory transactions.
	See [glfunc MemoryBarrier].
	>*/
#endif

	static void Flush(void); /*<
	Requests that all previous GL commands finish in finite time.
	See [glfunc Flush].
	>*/
	static void Finish(void); /*<
	Forces all previous GL commands to complete before returning.
	See [glfunc Finish].
	>*/
};

} // namespace context
//]

