/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[eglplus_display

class Display
{
public:
	Display(void); /*<
	Opens the default display.
	See [eglfunc GetDisplay], [eglconst DEFAULT_DISPLAY].
	>*/

	Display(::EGLNativeDisplayType display_id); /*<
	Opens the display specified by [^display_id].
	See [eglfunc GetDisplay].
	>*/

	__Boolean SwapInterval(EGLint interval); /*<
	Sets the SwapInterval for [^this] display.
	See [eglfunc SwapInterval].
	>*/

	__Boolean ReleaseContext(void); /*<
	Releases the current context without assigning a new one.
	See [eglfunc MakeCurrent].
	>*/
};

::EGLDisplay GetEGLHandle(const __Display& display)
noexcept; /*<
Returns the EGL display handle wrapped by a __Display.
>*/

//]

