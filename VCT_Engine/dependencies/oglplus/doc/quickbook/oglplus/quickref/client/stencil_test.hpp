/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_client_StencilTestState
namespace client {

class StencilTestState
{
private:
	template <__SingleFace Value>
	class StencilFuncStack /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__context_StencilFuncArgs, ...>
	{ };
public:
	__enums_EnumToClass_SingleFace<
		__Nothing,
		__SingleFace,
		StencilFuncStack
	> StencilFunc; /*<
	Stack managing the setting of stencil function arguments.
	>*/

private:
	template <__SingleFace Value>
	class StencilOpStack /*<
	This class template is here for documentation purposes only
	and is [*not] part of the public interface.
	>*/
	 : __SettingStack<__context_StencilOperations, ...>
	{ };
public:
	__enums_EnumToClass_SingleFace<
		__Nothing,
		__SingleFace,
		StencilOpStack
	> StencilOp; /*<
	Stack managing the setting of stencil operations.
	>*/
};

} // namespace client
//]

