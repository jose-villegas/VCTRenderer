/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oglplus_boolean_example_1

__Context gl;

if(gl.IsEnabled(__Capability::DepthTest)) /*< Equal to [^GL_TRUE]. >*/
{
	/* ... */
}

if(!gl.IsEnabled(__Capability::StencilTest)) /*< Equal to [^GL_FALSE]. >*/
{
	/* ... */
}

if(~gl.IsEnabled(__Capability::DepthTest)) /*< Not equal to [^GL_FALSE]. >*/
{
	/* ... */
}

if(!~gl.IsEnabled(__Capability::StencilTest)) /*< Not equal to [^GL_TRUE]. >*/
{
	/* ... */
}

//]
