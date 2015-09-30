/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_program_uniform_def

template <typename T>
using ProgramUniform = __ProgVar<
	__tag_DirectState,
	__tag_Uniform,
	__tag_NoTypecheck,
	T
>; /*<
[^ProgramUniform] indirectly inherits from __ProgVarLocOps_Uniform and __ProgVarGetSetOps_Uniform.
>*/

typedef __ProgramUniform<GLint> ProgramUniformSampler;

//]

