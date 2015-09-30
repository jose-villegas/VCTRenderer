/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_bound_texture_example_setup_1
using namespace oglplus;

__Texture tex; /*<
Create a texture object.
>*/

__TextureUnitSelector tex_unit = /* ... */; /*<
Choose a texture unit for the texture.
>*/
Texture::Active(tex_unit); /*<
Make the texture unit current (active).
>*/

__TextureTarget tex_tgt = Texture::Target::_2D; /*<
Choose binding point for the texture.
>*/
tex.Bind(tex_tgt); /*<
Bind the texture object to the texture target.
>*/

std::vector<GLubyte> image = /* ... */; /*<
Obtain the texture image data.
>*/
GLsizei width = /* ... */, height = /* ... */;

__Context gl;
gl.Bound<__Texture>(tex_tgt, tex) /*<
	Use the wrapper for GL context to create a reference to
	__BoundObjOps_Texture<__Texture> and use it to setup
	the texture parameters and image. Many member functions
	of the returned class return a reference to itself which
	allows to conveniently chain the calls to several functions,
	as seen here.
	>*/
	.MinFilter(__TextureMinFilter::Linear)
	.MagFilter(__TextureMagFilter::Nearest)
	.WrapS(__TextureWrap::Repeat)
	.WrapT(__TextureWrap::ClampToEdge)
	.Image2D(
		0,
		__PixelDataInternalFormat::RGBA,
		width, height,
		0,
		__PixelDataFormat::RGBA,
		__PixelDataType::UnsignedByte,
		image.data() /*<
		The buffer pointed-to by this parameter must hold enough data
		to be consumed, depending on the width, height, data type and other
		parameters.
		Optionally this pointer can be null, which causes a texture
		with blank image to be created.
		>*/
	);
//]

