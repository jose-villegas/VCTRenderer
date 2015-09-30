/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_context_Hints
namespace context {

class Hints
{
public:
	static void Hint(__HintTarget target, __HintOption option); /*<
	Sets a hint [^option] for a hint [^target].
	See [glfunc Hint].
	>*/
	static __HintOption Hint(__HintTarget target); /*<
	Queries the currently set hint option for a hint [^target].
	>*/
};

} // namespace context
//]

