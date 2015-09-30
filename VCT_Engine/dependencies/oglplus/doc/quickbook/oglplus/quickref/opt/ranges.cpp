/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_opt_ranges_ex_for_cat_tfm
using namespace oglplus::ranges;

ForEach(
	Concatenate<__StrCRef>(
		Transform(__EnumValueRange<__ObjectType>(), __EnumValueName<ObjectType>),
		Transform(__EnumValueRange<__ShaderType>(), __EnumValueName<ShaderType>)
	),
	[](__StrCRef str)
	{
		std::cout << str << std::endl;
	}
);
//]

//[oglplus_opt_ranges_ex_ext_dsa
using namespace oglplus::ranges;

__Context gl;

if(Contains(gl.Extensions(), "GL_EXT_direct_state_access"))
{
	std::cout << "Yay! We have DSA." << std::endl;
}
//]

//[oglplus_opt_ranges_ex_cnt_arb_ext
using namespace oglplus::ranges;

__Context gl;

std::cout << CountIf(
	gl.Extensions(),
	[](const std::string& ext) -> bool
	{
		return ext.find("_ARB_") != std::string::npos;
	}
) << std::endl;
//]

//[oglplus_opt_ranges_ex_max_ext_name
using namespace oglplus::ranges;

typedef std::pair<std::size_t, std::string> length_and_name;

__Context gl;

std::cout << Fold(
	gl.Extensions(),
	length_and_name(0, "N/A"),
	[](length_and_name state, std::string ext) -> length_and_name
	{
		if(state.first < ext.length())
		{
			state.first = ext.length();
			state.second = std::move(ext);
		}
		return state;
	}
).second << std::endl;
//]
