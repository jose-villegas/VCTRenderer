/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_concept_Range

template <typename Element>
class Range
{
public:
	typedef const Element ValueType; /*<
	The type into which the value returned by [^Front]
	is implicitly convertible.
	>*/

	Range(const Range&); /*< Ranges are copyable.  >*/

	bool Empty(void) const; /*<
	Returns true if the range is empty.
	>*/
	size_t Size(void) const; /*<
	Returns the number of [^Element]s in the range.
	>*/
	void Next(void); /*<
	This function moves the front of the range one element ahead.
	The range must not be empty when calling [^Next], otherwise the
	result is undefined and the application may be aborted.
	>*/
	ValueType Front(void); /*<
	Returns the elements at the front of the range.
	The range must not be empty when calling Front, otherwise the
	result is undefined and the application may be aborted.
	>*/
};

//]

