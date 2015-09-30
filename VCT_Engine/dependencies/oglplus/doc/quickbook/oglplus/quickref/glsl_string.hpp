/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_glsl_string

class GLSLString
{
public:
	GLSLString(const GLchar* str)
	noexcept;

	template <std::size_t N>
	GLSLString(const GLchar (&str)[N])
	noexcept;

	GLSLString(const __StrCRef& str)
	noexcept;

	GLSLString(const __String& str)
	noexcept;

	GLSLString(const std::vector<GLchar>& v);
	noexcept

	template <std::size_t N>
	GLSLString(const std::array<GLchar, N>& a);
	noexcept;
};
//]
//[oglplus_glsl_strings
class GLSLStrings
{
public:
	GLSLStrings(GLsizei count, const GLchar* const * strs)
	noexcept;

	GLSLStrings(GLsizei count, const GLchar* const* strs, const GLint* lens)
	noexcept;

	template <std::size_t N>
	GLSLStrings(const GLchar* (&strs)[N])
	noexcept;

	GLSLStrings(const std::vector<const GLchar*>& v)
	noexcept;

	template <std::size_t N>
	GLSLStrings(const std::array<const GLchar*, N>& a)
	noexcept;

	GLSLStrings(const std::initializer_list<const GLchar*>& l)
	noexcept;
};

//]

