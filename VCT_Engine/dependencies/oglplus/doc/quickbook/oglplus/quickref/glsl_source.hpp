/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_glsl_source

class GLSLSource
{
public:
	GLSLSource(GLSLSource&& tmp);

	explicit
	GLSLSource(const __StrCRef& source);

	GLSLSource(const std::vector<__StrCRef>& lits);

	template <size_t N>
	GLSLSource(const __StrCRef (&lits)[N]);

	GLSLSource(const std::vector<__String>& strs);

	GLSLSource(std::initializer_list<__StrCRef> lits);

	GLSLSource(std::initializer_list<__String> strs);

	explicit
	GLSLSource(const __String& source);

	explicit
	GLSLSource(__String&& source);

	static
	GLSLSource FromStream(std::istream& input); /*<
	Constructs a new [^GLSLSource] from the data read from
	the specified standard [^input] stream.
	>*/

	static
	GLSLSource FromFile(const char* path); /*<
	Constructs a new [^GLSLSource] from the data read from
	a file located at the specified [^path].
	>*/
	static
	GLSLSource FromFile(const __String& path);
};

//]

