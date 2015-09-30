/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_texture_handle

#if GL_ARB_bindless_texture

class TextureHandle
{
public:
	TextureHandle(__TextureName texture); /*<
	Constructs a texture handle from a texture.
	See [glfunc GetTextureHandleARB].
	>*/

	TextureHandle(__TextureName texture, __SamplerName sampler); /*<
	Constructs a texture handle from a texture and a sampler.
	See [glfunc GetTextureSamplerHandleARB].
	>*/

	void MakeResident(void); /*<
	Make the texture resident.
	See [glfunc MakeTextureHandleResidentARB].
	>*/

	void MakeNonResident(void); /*<
	Make the texture non-resident.
	See [glfunc MakeTextureHandleNonResidentARB].
	>*/

	__Boolean IsResident(void) const; /*<
	Returns true if the texture is resident.
	See [glfunc IsTextureHandleResidentARB].
	>*/
};

GLuint64 GetGLHandle(__TextureHandle th)
noexcept; /*<
Returns the GL handle value from a __TextureHandle.
>*/

class ImageHandle
{
public:
	ImageHandle(
		__TextureName texture,
		GLint level,
		__Boolean layered,
		GLint layer,
		__ImageUnitFormat format
	); /*<
	Construction from a texture and additional parameters.
	See [glfunc GetImageHandleARB].
	>*/

	void MakeResident(void); /*<
	Make the image resident.
	See [glfunc MakeImageHandleResidentARB].
	>*/

	void MakeNonResident(void); /*<
	Make the image non-resident.
	See [glfunc MakeImageHandleNonResidentARB].
	>*/

	__Boolean IsResident(void) const; /*<
	Returns true if the image is resident.
	See [glfunc IsImageHandleResidentARB].
	>*/
};

GLuint64 GetGLHandle(__ImageHandle ih)
noexcept; /*<
Returns the GL handle value from an __ImageHandle.
>*/

#endif

//]

