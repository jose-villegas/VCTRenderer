/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_frag_data_loc_ops

template <>
class __ProgVarLocOps<__tag_FragData>
{
public:
	static void BindLocation(
		__ProgramName program,
		__FragDataSlot location,
		__StrCRef identifier
	); /*<
	Binds the fragment data specified by [^identifier] to the [^location]
	in the specified [^program].
	See [glfunc BindFragDataLocation].
	>*/

	static GLint GetLocation(
		__ProgramName program,
		__StrCRef identifier,
		bool active_only
	); /*<
	Finds the location of the fragment data output specified
	by [^identifier] in a [^program]. If [^active_only] is [^true] then
	throws __ProgVarError if no such frag_data exists or if it is not active.
	See [glfunc GetFragDataLocation].
	>*/
};

//]
//[oglplus_frag_data_common_ops

template <>
class __ProgVarCommonOps<__tag_FragData>
 : public __ProgVarLoc<__tag_FragData>
{
public:
	void Bind(__StrCRef identifier); /*<
	Binds the fragment output specified by [^identifier]
	to [^this] fragment data location.
	See [glfunc BindFragDataLocation].
	>*/
};

//]
//[oglplus_frag_data_def

typedef __ProgVar<
	__tag_ImplicitSel,
	__tag_FragData,
	__tag_NoTypecheck,
	void
> FragData; /*<
Indirectly inherits from __ProgVarLocOps_FragData, __ProgVarCommonOps_FragData.
>*/

typedef FragData FragmentData;

//]

