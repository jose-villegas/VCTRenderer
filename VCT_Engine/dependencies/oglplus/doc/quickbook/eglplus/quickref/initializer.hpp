/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[eglplus_initializer

class EGLInitializer
{
public:
	EGLInitializer(void); /*<
	Initializes EGL on the default display.
	See [eglfunc GetDisplay], [eglfunc Initialize],
	[eglconst DEFAULT_DISPLAY].
	>*/

	EGLInitializer(const __Display& display); /*<
	Initializes EGL on the specified display.
	See [eglfunc Initialize].
	>*/

	EGLInitializer(const EGLInitializer&) = delete;
	EGLInitializer(EGLInitializer&&);

	~EGLInitializer(void); /*<
	See [eglfunc Terminate].
	>*/

	const __Display& Disp(void) const; /*<
	Returns a reference to the associated display.
	>*/
	const __Display& AssociatedDisplay(void) const;

	EGLint VersionMajor(void) const; /*<
	Returns the major version number.
	>*/
	EGLint VersionMinor(void) const; /*<
	Returns the minor version number.
	>*/

	const char* QueryString(__StringQuery query) const; /*<
	Queries the specified EGL string.
	See [eglfunc QueryString].
	>*/

	const char* Vendor(void) const; /*<
	Returns the vendor name.
	See [eglfunc QueryString], [eglconst EGL_VENDOR].
	>*/

	const char* Version(void) const; /*<
	Returns the version string.
	See [eglfunc QueryString], [eglconst EGL_VERSION].
	>*/

	__Range<__String> ClientAPIs(void) const; /*<
	Returns a range of supported API name strings.
	See [eglfunc QueryString], [eglconst EGL_CLIENT_APIS].
	>*/

	__Range<__String> Extensions(void) const; /*<
	Returns a range of extension strings.
	See [eglfunc QueryString], [eglconst EGL_EXTENSIONS].
	>*/

	static bool HasClientExtensions(void); /*<
	Returns true if the EGL implementation supports client extensions.
	See [eglfunc QueryString], [eglconst EGL_EXTENSIONS].
	>*/

	static __Range<__String> ClientExtensions(void); /*<
	Returns a range of client extension strings.
	See [eglfunc QueryString], [eglconst EGL_EXTENSIONS].
	>*/

	static __Boolean ReleaseThread(void); /*<
	Releases the current thread state.
	See [eglfunc ReleaseThread].
	>*/
};

//]
//[eglplus_initializer_alias

typedef __EGLInitializer LibEGL;

//]

