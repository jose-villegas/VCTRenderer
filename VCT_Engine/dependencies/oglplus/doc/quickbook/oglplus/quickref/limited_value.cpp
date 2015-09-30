/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_limited_value_example_1

__Texture::Active(0); /*<
Implicit conversion of numeric value to __TextureUnitSelector (derived from __LimitedCount).
>*/

__TextureUnitSelector tex_unit = 2;

__Texture::Active(tex_unit);

__Texture::Active(std::numeric_limits<GLuint>::max()); /*<
Very probably will throw __LimitError.
>*/

//]

