/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_Capabilities
namespace context {

class Capabilities
{
public:
	static void Enable(__Capability capability); /*<
	Enables the specified [^capability].
	See [glfunc Enable].
	>*/
	static void Enable(__Functionality functionality, GLuint number); /*<
	Enables the specified [^functionality].
	See [glfunc Enable].
	>*/

	static void Disable(__Capability capability); /*<
	Disables the specified [^capability].
	See [glfunc Disable].
	>*/
	static void Disable(__Functionality functionality, GLuint number); /*<
	Disables the specified [^functionality].
	See [glfunc Disable].
	>*/

	static __Boolean IsEnabled(__Capability capability); /*<
	Checks is the specified [^capability] is enabled.
	See [glfunc IsEnabled].
	>*/
	static __Boolean IsEnabled(__Functionality functionality, GLuint number); /*<
	Checks is the specified [^functionality] is enabled.
	See [glfunc IsEnabled].
	>*/

#if GL_VERSION_3_0
	static void Enable(Capability capability, GLuint index); /*<
	Enables the specified [^capability] on an [^index]ed target.
	See [glfunc Enablei].
	>*/
	static void Disable(Capability capability, GLuint index); /*<
	Disables the specified [^capability] on an [^index]ed target.
	See [glfunc Disablei].
	>*/
	static __Boolean IsEnabled(Capability capability, GLuint index); /*<
	Checks if the specified [^capability] is enabled on an [^index]ed target.
	See [glfunc IsEnabledi].
	>*/
#endif
};

} // namespace context
//]

