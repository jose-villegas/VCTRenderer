/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_string_def

class String
 : public std::basic_string<GLchar>
{
public:
	using std::basic_string<GLchar>::basic_string; /*<
	[^String] has the same constructors as a [^std::basic_string].
	except that the input strings must be UTF-8 encoded, otherwise
	the behavior is undefined.
	>*/
};

const String& EmptyString(void); /*<
Returns a const reference to an empty __String.
>*/

bool ValidString(const char* begin, const char* end); /*<
Checks if the character range between [^begin] and [^end] is a valid
UTF-8 sequence.
>*/

//]

