/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_object_desc

#ifndef OGLPLUS_NO_OBJECT_DESC /*<
Compile-time switch disabling textual object descriptions.
Setting this preprocessor option to a non-zero integer value
disables the OGLplus object description attached to
various instantiations of the __Object template (like __Program, __Shader,
__Texture, etc.) during construction, by the means of the __ObjectDesc
parameter in constructor of __Object.
By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. objects descriptions are enabled, when not in low-profile mode
and disabled otherwise.
>*/
# define OGLPLUS_NO_OBJECT_DESC __OGLPLUS_LOW_PROFILE
#endif

//]

