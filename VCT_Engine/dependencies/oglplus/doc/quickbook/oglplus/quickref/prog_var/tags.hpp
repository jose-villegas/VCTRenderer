/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_prog_var_tags
namespace tag {

struct VertexAttrib; /*< See __VertexAttrib >*/
struct Uniform; /*< See __Uniform and __ProgramUniform >*/
struct UniformBlock; /*< See __UniformBlock >*/
struct ShaderStorageBlock; /*< See __ShaderStorageBlock >*/
struct Subroutine; /*< See __Subroutine >*/
struct SubroutineUniform; /*< See __SubroutineUniform >*/
struct FragData; /*< See __FragData >*/

//]
//[oglplus_prog_var_typecheck_tags

struct Typecheck; /*<
Enables GPU program variable typechecking if possible.
>*/
struct NoTypecheck; /*<
Disables GPU program variable typechecking.
>*/

} // namespace tag
//]

