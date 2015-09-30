/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_subroutine_loc_ops

template <>
class __ProgVarLocOps<__tag_Subroutine>
{
public:
	static GLint GetLocation(
		__ProgramName program,
		__ShaderType stage,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the subroutine specified by [^identifier]
	in the [^stage] of a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such subroutine exists or if it is not active.
	See [glfunc GetSubroutineIndex].
	>*/

	GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	) const;

	__ShaderType Stage(void) const; /*<
	Returns this subroutine's program stage.
	>*/
};

//]
//[oglplus_subroutine_uniform_loc_ops

template <>
class __ProgVarLocOps<__tag_SubroutineUniform>
{
public:
	static GLint GetLocation(
		__ProgramName program,
		__ShaderType stage,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the subroutine uniform specified by [^identifier]
	in the [^stage] of a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such uniform exists or if it is not active.
	See [glfunc GetSubroutineUniformLocation].
	>*/

	GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	) const;

	__ShaderType Stage(void) const; /*<
	Returns this subroutine uniforms's program stage.
	>*/
};

//]
//[oglplus_subroutine_prog_var

template <>
class __ProgVar<__tag_ImplicitSel, __tag_Subroutine, __tag_NoTypecheck, void>
 : public __ProgVarLoc<__tag_Subroutine>
{
public:
	ProgVar(__ProgVarLoc<__tag_Subroutine> pvloc);

	ProgVar(__ProgramName program, __ShaderType stage, __StrCRef identifier);

	ProgVar(
		__ProgramName program,
		__ShaderType stage,
		__StrCRef identifier,
		bool active_only
	);
};

//]
//[oglplus_subroutine_uniform_prog_var

template <>
class __ProgVar<__tag_ImplicitSel, __tag_SubroutineUniform, __tag_NoTypecheck, void>
 : public __ProgVarLoc<__tag_SubroutineUniform>
{
public:
	ProgVar(__ProgVarLoc<__tag_SubroutineUniform> pvloc);

	ProgVar(__ProgramName program, __ShaderType stage, __StrCRef identifier);

	ProgVar(
		__ProgramName program,
		__ShaderType stage,
		__StrCRef identifier,
		bool active_only
	);
};

//]
//[oglplus_subroutine_def

typedef __ProgVar_Subroutine<
	__tag_ImplicitSel,
	__tag_Subroutine,
	__tag_NoTypecheck,
	void
> Subroutine;

//]
//[oglplus_subroutine_uniform_def

typedef __ProgVar_SubroutineUniform<
	__tag_ImplicitSel,
	__tag_SubroutineUniform,
	__tag_NoTypecheck,
	void
> SubroutineUniform;

//]
//[oglplus_uniform_subroutines

class UniformSubroutines
{
public:
	UniformSubroutines(__ProgramName program, __ShaderType stage); /*<
	Constructs a uniform subroutine manager for a [^stage] of a [^program].
	>*/

	UniformSubroutines& Assign(
		const __SubroutineUniform& uniform,
		const __Subroutine& subroutine
	); /*<
	Remembers the assignment of a [^subroutine] to the specified
	subroutine [^uniform].
	[note This function does not immediately apply the changes
	to the actual uniform variables in the managed stage
	of a GPU program. Use the member [^Apply] function to do this
	after the subroutines are assigned to subroutine uniforms.]
	>*/

	void Apply(void); /*<
	Applies all changes made by [^Assign].
	>*/

	class Preset /*<
	[^Preset] stores a setting of the whole set of subroutine uniforms.
	which can be later applied or loaded.
	Applications should treat this type as opaque and use it
	only with the [^Save], [^Load] and [^Apply] functions.
	>*/
	{
	public:
		Preset(Preset&&);
	};

	Preset Save(void); /*<
	Saves the current setting of subroutine uniforms into a preset.
	>*/
	void Load(const Preset& preset); /*<
	Loads the setting of subroutine uniforms from a [^preset].
	[note Only presets from the same instance of [^UniformSubroutines]
	that saved them can be loaded or applied.]
	>*/

	void Apply(const Preset& preset); /*<
	Applies the setting from a preset without changing the current setting
	of [^this] instance of [^UniformSubroutines].
	[note Only presets from the same instance of [^UniformSubroutines]
	that saved them can be loaded or applied.]
	>*/
};

//]

