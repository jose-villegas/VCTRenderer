/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_texture_example_setup_1
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

Texture::MinFilter(tex_tgt, __TextureMinFilter::Linear); /*<
Setup various texture parameters for the texture currently bound
to the specified texture target on the currenly active texture unit.
>*/
Texture::MagFilter(tex_tgt, __TextureMagFilter::Nearest);
Texture::WrapS(tex_tgt, __TextureWrap::ClampToEdge);
Texture::WrapT(tex_tgt, __TextureWrap::ClampToEdge);
Texture::CompareMode(tex_tgt, __TextureCompareMode::CompareRefToTexture);

std::vector<GLfloat> image = /* ... */; /*<
Obtain the texture image data.
>*/
GLsizei width = /* ... */, height = /* ... */;

Texture::Image2D( /*<
Setup the texture image data.
>*/
	tex_tgt,
	0,
	__PixelDataInternalFormat::DepthComponent32,
	width, height,
	0,
	__PixelDataFormat::DepthComponent,
	__PixelDataType::Float,
	image.data() /*<
	The buffer pointed-to by this parameter must hold enough data
	to be consumed, depending on the width, height, data type and other
	parameters.
	Optionally this pointer can be null, which causes a blank texture
	to be created.
>*/
);
//]

//[oglplus_texture_example_setup_2
__Texture tex; /*<
Create a texture object.
>*/

__TextureUnitSelector tex_unit = /* ... */; /*<
Choose a texture unit for the texture and make it current.
>*/
Texture::Active(tex_unit);

std::vector<GLubyte> image = /* ... */;
GLsizei width = /* ... */, height = /* ... */;

tex	<< __TextureTarget::_2D /*<
	This expression creates a pair of a texture object and a texture
	target which allows to chain the other operands which in turn
	call various __Texture's functions.
	>*/
	<< __TextureMinFilter::Linear /*<
	[^__Texture::MinFilter(...)]
	>*/
	<< __TextureMagFilter::Nearest /*<
	[^Texture::MagFilter(...)]
	>*/
	<< __TextureWrap::ClampToEdge /*<
	[^Texture::Wrap(...)]
	>*/
	<< __images_ImageSpec( /*<
	[^Texture::Image(...)]
	>*/
		width, height,
		__PixelDataFormat::RGBA,
		image.data()
	);
//]

