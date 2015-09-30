/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_error_LimitError

class LimitError
 : public __Error
{
public:
	LimitError(const char* message);

	GLfloat Value(void) const; /*<
	Returns the value, which triggered the error.
	>*/
	GLfloat Limit(void) const; /*<
	Returns the limit value, which has been exceeded.
	>*/
};

//]

