/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_NumericQueries
namespace context {

class NumericQueries
{
public:
	static GLint MajorVersion(void); /*<
	Returns the GL major version.
	See [glfunc Get], [glconst MAJOR_VERSION].
	>*/
	static GLint MinorVersion(void); /*<
	Returns the GL minor version.
	See [glfunc Get], [glconst MINOR_VERSION].
	>*/

#if GL_VERSION_3_3 || GL_ARB_timer_query
	static GLint64 Timestamp(void); /*<
	Queries the current GL timestamp.
	See [glfunc Get], [glconst TIMESTAMP].
	>*/
#endif

#if GL_VERSION_3_2
	static __Bitfield<__ContextProfileBit> ProfileMask(void); /*<
	Returns the GL context profile mask.
	See [glfunc Get], [glconst CONTEXT_PROFILE_MASK].
	>*/
	static __Bitfield<__ContextFlagBit> Flags(void); /*<
	Returns the current context flags.
	See [glfunc Get], [glconst CONTEXT_FLAGS].
	>*/
#endif

#if GL_VERSION_4_5 || GL_KHR_context_flush_control
	static __ContextReleaseBehavior ReleaseBehavior(void); /*<
	Returns the current context flush control reset behavior.
	See [glfunc Get], [glconst CONTEXT_RELEASE_BEHAVIOR].
	>*/
#endif

#if GL_VERSION_4_5
	static __ResetNotificationStrategy ResetNotificationStrategy(void); /*<
	Returns the currently active context reset notification strategy.
	See [glfunc Get], [glconst RESET_NOTIFICATION_STRATEGY].
	>*/
	static __GraphicsResetStatus GraphicsResetStatus(void); /*<
	Returns the current GL context reset status.
	See [glfunc GetGraphicsResetStatus].
	>*/
#endif
};

} // namespace context
//]

