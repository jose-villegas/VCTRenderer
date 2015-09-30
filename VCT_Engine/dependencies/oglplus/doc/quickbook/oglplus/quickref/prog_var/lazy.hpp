/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_prog_var_lazy

template <typename __ProgVar>
class Lazy
 : public __ProgVar
{
public:
	Lazy(__ProgramName program, __String&& identifier); /*<
	Internally stores the program name and the identifier of the
	program variable but does not search for the location.
	>*/

	Lazy& Init(void); /*<
	If the location has not been found yet, [^Init] tries to find
	it in the referenced GPU program by the stored identifier.
	If no such variable exists in the program or if it is not active
	then this function throws __ProgVarError.
	>*/

	Lazy& TryInit(void); /*<
	Similar to [^Init] tries to find the location of a variable
	in a GPU program, but does not throw if the variable is not active.
	The location just remains uninitialized. The state can be queried
	by using the [^__ProgVarLoc::IsActive] member function.
	>*/

	ProgVar operator[](std::size_t offset); /*<
	Wrapper around [^__ProgVar::operator []].
	Allows to access the elements of an array GPU program variable.
	Calls [^Init] before actually trying to access the program variable.
	>*/

	template <typename T>
	void Set(T&& value); /*<
	Wrapper around [^__ProgVar::Set].
	Calls [^Init] before actually trying to access the program variable.
	>*/
};

//]

