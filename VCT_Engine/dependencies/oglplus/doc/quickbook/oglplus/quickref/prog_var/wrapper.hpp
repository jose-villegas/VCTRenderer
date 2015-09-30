/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_prog_var_loc_ops

template <typename __VarTag>
class ProgVarLocOps
{
public:
};

//]
//[oglplus_prog_var_common_ops

template <typename __VarTag>
class ProgVarCommonOps
 : public __ProgVarLoc<__VarTag>
{
public:
};

//]
//[oglplus_prog_var_get_set_ops

template <typename __OpsTag, typename __VarTag, typename T>
class ProgVarGetSetOps
 : public __ProgVarCommonOps<__VarTag>
{
public:
};

//]
//[oglplus_prog_var_typecheck

template <typename __ChkTag, typename __VarTag>
class ProgVarTypecheck
{
public:
	void CheckType(
		__ProgramName program,
		GLint location,
		__StrCRef identifier
	) const;
};

//]
//[oglplus_prog_var_wrapper

template <typename __OpsTag, typename __VarTag, typename __ChkTag, typename T>
class ProgVar
 : public __ProgVarGetSetOps<__OpsTag, __VarTag, T>
 , public __ProgVarTypecheck<__ChkTag, __VarTag>
{
public:
	ProgVar(void);
	ProgVar(const ProgVar&);

	ProgVar(__ProgVarLoc<__VarTag> pvloc); /*<
	Constructs a [^ProgVar] from the specified location.
	>*/

	ProgVar(__ProgramName program, GLuint location); /*<
	Constructs a [^ProgVar] from a reference to a program and a location.
	>*/

	ProgVar(__ProgramName program, __StrCRef identifier); /*<
	Constructs a [^ProgVar] from a reference to a program
	and an identifier of the variable. If no such variable
	is active, then this constructor throws __ProgVarError.
	>*/

	ProgVar(__ProgramName program, __StrCRef identifier, bool active_only); /*<
	Constructs a [^ProgVar] from a reference to a program
	and an identifier of the variable. If no such variable
	is active and [^active_only] is [^true] then this
	constructor throws __ProgVarError.
	>*/

	ProgVar(__ProgramName program, __StrCRef identifier, std::nothrow_t); /*<
	Constructs a [^ProgVar] from a reference to a program
	and an identifier of the variable. Does not throw
	if no such variable is active.
	>*/

	ProgVar& operator = (const ProgVar& var);
	ProgVar& operator = (__ProgVarLoc<__VarTag> pvloc);

	ProgVar& BindTo(__StrCRef identifier);

	ProgVar operator[](std::size_t offset) const; /*<
	Allows to access elements in array GPU program variables.
	>*/
};
// TODO

//]

