/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_string_ref

class StrCRef
{
public:
	StrCRef(void) noexcept; /*<
	Constructs a reference to an empty string.
	>*/

	StrCRef(const GLchar* cstr) noexcept; /*<
	Constructs a reference to a C-string.
	>*/
	StrCRef(const GLchar* cstr, size_t size) noexcept; /*<
	Constructs a reference to a C-string with a specific size.
	>*/

	template <size_t N>
	StrCRef(const GLchar (&cary)[N]) noexcept; /*<
	Constructs a reference to a character array with a known size.
	>*/

	StrCRef(const __String& sstr) noexcept; /*<
	Constructs a reference to a string stored inside of a __String.
	>*/

	StrCRef(const std::vector<GLchar>& cvec) noexcept; /*<
	Constructs a reference to a string stored inside of a [^std::vector].
	>*/

	template <size_t N>
	StrCRef(const std::array<GLchar, N>& cvec) noexcept; /*<
	Constructs a reference to a string stored inside of a [^std::array].
	>*/

	size_t size(void) const noexcept; /*<
	Returns the lenght of the string, not counting any terminating
	characters.
	>*/
	bool empty(void) const noexcept; /*<
	Returns true if the referenced string is empty.
	>*/

	typedef const GLchar* iterator; /*<
	Iterator types.
	>*/
	typedef iterator const_iterator;

	const_iterator begin(void) const noexcept;
	const_iterator end(void) const noexcept;

	bool is_nts(void) const noexcept; /*<
	Returns true if the string is null-terminated
	>*/

	const GLchar* c_str(void) const noexcept; /*<
	Returns a const pointer to the referenced character string.
	This function may be called only if the string stored inside
	is null-terminated.
	>*/

	__String str(void) const; /*<
	Returns the referenced character string as a __String.
	>*/

	friend bool operator == (const StrCRef& a, const StrCRef& b); /*<
	Comparison operators.
	>*/
	friend bool operator == (const StrCRef& a, const Char* b);
	friend bool operator == (const Char* a, const StrCRef& b);
	friend bool operator != (const StrCRef& a, const StrCRef& b);
	friend bool operator != (const StrCRef& a, const Char* b);
	friend bool operator != (const Char* a, const StrCRef& b);
};

//]

