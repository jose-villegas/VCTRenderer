/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_prog_var_optional

template <typename __ProgVar>
class Optional
 : public __ProgVar
{
public:
	OptionalImpl(__ProgramName program, __StrCRef identifier); /*<
	Tries to locate the variable identified by [^identifier] in
	the specified [^program], but does not throw if the variable
	is not active. The actual status can be determined by calling
	[^__ProgVarLoc::IsActive()].
	>*/
};

//]

