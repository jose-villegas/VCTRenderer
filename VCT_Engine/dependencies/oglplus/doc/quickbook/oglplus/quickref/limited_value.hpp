/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_limited_value

template <GLenum Query>
class LimitedCount
{
public:
	template <typename Type>
	explicit operator Type(void) const; /*<
	Explicit conversion of the internally stored value to a value
	of the specified numeric [^Type].
	>*/

	friend bool operator == (LimitedCount a, LimitedCount b);
	friend bool operator != (LimitedCount a, LimitedCount b);
	friend bool operator <= (LimitedCount a, LimitedCount b);
	friend bool operator <  (LimitedCount a, LimitedCount b);
	friend bool operator >= (LimitedCount a, LimitedCount b);
	friend bool operator >  (LimitedCount a, LimitedCount b);
};

template <GLenum Query>
GLuint LimitedCountMax(const __LimitedCount<Query>& lim_count); /*<
Returns the maximum for the specified limited value type.
>*/

//]

