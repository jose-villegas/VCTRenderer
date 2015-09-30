/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_prog_var_typechecked

template <typename __ProgVar>
class Typechecked
 : public __ProgVar
{
public:
	using __ProgVar::ProgVar; /*<
	Inherits constructors from __ProgVar, but also enables
	typechecking for those program variable types where it
	does make sense.
	>*/
};

//]

