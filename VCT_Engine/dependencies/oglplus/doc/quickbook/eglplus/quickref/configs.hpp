/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[eglplus_config_attribs

typedef __AttributeList<__ConfigAttrib, ...> ConfigAttribs; /*<
Attribute list type for config attributes.
>*/

typedef __FinishedAttributeList<__ConfigAttrib, ...> FinishedConfigAttribs; /*<
Finished attribute list type for config attributes.
>*/

//]
//[eglplus_config_1

class Config
{
public:
	EGLint GetAttrib(__ConfigAttrib attrib) const; /*<
	Returns the value of a specified attribute.
	See [eglfunc GetConfigAttrib].
	>*/

	EGLint ConfigId(void) const; /*<
	Returns the unique configuration identifier.
	See [eglfunc GetConfigAttrib], [eglconst CONFIG_ID].
	>*/
	EGLint Id(void) const;

	EGLint BufferSize(void) const; /*<
	Returns the total component bits of the color buffer.
	See [eglfunc GetConfigAttrib], [eglconst BUFFER_SIZE].
	>*/

	EGLint RedSize(void) const; /*<
	Returns the number of bits of the red color component.
	See [eglfunc GetConfigAttrib], [eglconst RED_SIZE].
	>*/
	EGLint GreenSize(void) const; /*<
	Returns the number of bits of the green color component.
	See [eglfunc GetConfigAttrib], [eglconst GREEN_SIZE].
	>*/
	EGLint BlueSize(void) const; /*<
	Returns the number of bits of the blue color component.
	See [eglfunc GetConfigAttrib], [eglconst BLUE_SIZE].
	>*/
	EGLint LuminanceSize(void) const; /*<
	Returns the number of bits of the luminance component.
	See [eglfunc GetConfigAttrib], [eglconst LUMINANCE_SIZE].
	>*/
	EGLint AlphaSize(void) const; /*<
	Returns the number of bits of the alpha component.
	See [eglfunc GetConfigAttrib], [eglconst ALPHA_SIZE].
	>*/

	EGLint DepthSize(void) const; /*<
	Returns the number of bits of the depth buffer.
	See [eglfunc GetConfigAttrib], [eglconst DEPTH_SIZE].
	>*/
	EGLint StencilSize(void) const; /*<
	Returns the number of bits of the stencil buffer.
	See [eglfunc GetConfigAttrib], [eglconst STENCIL_SIZE].
	>*/

	EGLint MaxPbufferWidth(void) const; /*<
	Returns the maximum width of Pbuffers in pixels.
	See [eglfunc GetConfigAttrib], [eglconst MAX_PBUFFER_WIDTH].
	>*/
	EGLint MaxPbufferHeight(void) const; /*<
	Returns the maximum height of Pbuffers in pixels.
	See [eglfunc GetConfigAttrib], [eglconst MAX_PBUFFER_HEIGHT].
	>*/
	EGLint MaxPbufferPixels(void) const; /*<
	Returns the maximum size (number of pixels) of Pbuffers.
	See [eglfunc GetConfigAttrib], [eglconst MAX_PBUFFER_PIXELS].
	>*/
//]
//[eglplus_config_2

	EGLint MinSwapInterval(void) const; /*<
	Returns the minimum swap interval.
	See [eglfunc GetConfigAttrib], [eglconst MIN_SWAP_INTERVAL].
	>*/
	EGLint MaxSwapInterval(void) const; /*<
	Returns the maximum swap interval.
	See [eglfunc GetConfigAttrib], [eglconst MAX_SWAP_INTERVAL].
	>*/

	__Boolean NativeRenderable(void) const; /*<
	Returns true if native rendering API can render to surface.
	See [eglfunc GetConfigAttrib], [eglconst NATIVE_RENDERABLE].
	>*/

	__ConfigCaveat ConfigCaveat(void) const; /*<
	Returns the caveat for this config.
	See [eglfunc GetConfigAttrib], [eglconst CONFIG_CAVEAT].
	>*/
	__ConfigCaveat Caveat(void) const;

	__ColorBufferType ColorBufferType(void) const; /*<
	Returns the color buffer type.
	See [eglfunc GetConfigAttrib], [eglconst COLOR_BUFFER_TYPE].
	>*/

	__TransparentType TransparentType(void) const; /*<
	Returns the transparency type.
	See [eglfunc GetConfigAttrib], [eglconst TRANSPARENT_TYPE].
	>*/

	__Bitfield<__RenderableTypeBit> RenderableTypes(void) const; /*<
	Returns the renderable type bits.
	See [eglfunc GetConfigAttrib], [eglconst RENDERABLE_TYPE].
	>*/
	bool HasRenderableType(__RenderableTypeBit type) const; /*<
	Returns true if a specified renderable type is supported.
	>*/

	__Bitfield<__SurfaceTypeBit> SurfaceTypes(void) const; /*<
	Returns the surface type bits.
	See [eglfunc GetConfigAttrib], [eglconst SURFACE_TYPE].
	>*/
	bool HasSurfaceType(__SurfaceTypeBit type) const; /*<
	Returns true if a specified surface type is supported.
	>*/

	__Bitfield<__RenderableTypeBit> Conformant(void) const; /*<
	Returns the conformant bits for this config.
	See [eglfunc GetConfigAttrib], [eglconst CONFORMANT].
	>*/
	bool IsConformantTo(__RenderableTypeBit type) const; /*<
	Returns true if the config is conformant to a renderable type.
	>*/
};

::EGLConfig GetEGLHandle(const __Config& config)
noexcept;

//]
//[eglplus_configs_1

class Configs
{
public:
	Configs(const __Display& display); /*<
	Gets the configurations for the specified display.
	>*/

	Configs(
		const __Display& display,
		const __FinishedConfigAttribs& attribs
	); /*<
	Gets configurations for the specified display matching
	the specified attribute values.
	>*/

	__Range<__Config> All(void) const; /*<
	Returns a range of all supported configurations.
	>*/

	bool Empty(void) const; /*<
	Returns true if there are no matching configs.
	>*/

	__Config First(void) const; /*<
	Returns the first config in this set of configurations if
	it is not empty.
	>*/
};

//]

