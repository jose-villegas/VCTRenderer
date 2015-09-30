/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_sampler_common

template <>
class __ObjCommonOps<__tag_Sampler>
 : public __SamplerName
{
public:
	typedef __TextureUnitSelector Target; /*<
	Sampler bind target.
	>*/

	static __SamplerName Binding(__TextureUnitSelector target); /*<
	Returns the sampler currently bound to the specified [^target].
	See [glfunc GetIntegerv].
	>*/
	static void Bind(__TextureUnitSelector target, __SamplerName sampler); /*<
	Binds the specified [^sampler] to the specified [^target].
	See [glfunc BindSampler].
	>*/

	void Bind(__TextureUnitSelector target) const; /*<
	Binds [^this] sampler to the specified [^target].
	See [glfunc BindSampler].
	>*/
};
//]
//[oglplus_sampler_1
template <>
class __ObjectOps<__tag_DirectState, __tag_Sampler>
 : public __ObjZeroOps<__tag_DirectState, __tag_Sampler> /*<
Indirectly inherits from __ObjCommonOps_Sampler<__tag_Sampler>.
>*/
{
public:
	__Vector<GLfloat, 4> BorderColor(__TypeTag<GLfloat>) const; /*<
	Returns the currently set border color of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	__Vector<GLuint, 4> BorderColor(__TypeTag<GLuint>) const;
	__Vector<GLint, 4> BorderColor(__TypeTag<GLint>) const;

	void BorderColor(__Vector<GLfloat, 4> color); /*<
	Sets the border color for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	void BorderColor(__Vector<GLuint, 4> color);
	void BorderColor(__Vector<GLint, 4> color);

	__TextureCompareMode CompareMode(void) const; /*<
	Returns the currently set texture compare mode of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/
	void CompareMode(__TextureCompareMode mode); /*<
	Sets the texture compare mode for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/

	__CompareFunction CompareFunc(void) const; /*<
	Returns the currently set texture compare function of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/
	void CompareFunc(__CompareFunction func); /*<
	Sets the texture compare mode for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/

	__TextureMinFilter MinFilter(void) const; /*<
	Returns the currently set texture minnification filter of this
	sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_MIN_FILTER].
	>*/
	void MinFilter(__TextureMinFilter filter); /*<
	Sets the texture minnification filter for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_MIN_FILTER].
	>*/

	__TextureMagFilter MagFilter(void) const; /*<
	Returns the currently set texture magnification filter of this
	sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_MAG_FILTER].
	>*/
	void MagFilter(__TextureMagFilter filter); /*<
	Sets the texture magnification filter for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_MAG_FILTER].
	>*/

	void Filter(__TextureFilter filter) const; /*<
	Sets both the minification and the magnification filter for
	this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_MIN_FILTER],
	[glconst TEXTURE_MAG_FILTER].
	>*/
//]
//[oglplus_sampler_2

	__TextureWrap Wrap(__TextureWrapCoord coord) const; /*<
	Returns the currently set texture wrap mode on the specified
	[^coord] of this sampler object.
	See [glfunc GetSamplerParameter].
	>*/
	void Wrap(__TextureWrapCoord coord, __TextureWrap mode) /*<
	Sets the texture wrap mode on the specified [^coord]
	for this sampler object.
	See [glfunc SamplerParameter].
	>*/

	__TextureWrap WrapS(void) const; /*<
	Returns the currently set texture wrap mode on the [^S], [^T] and [^R]
	coordinates of this sampler object.
	See [glfunc GetSamplerParameter],
	[glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T],
	[glconst TEXTURE_WRAP_R].
	>*/
	TextureWrap WrapT(void) const;
	TextureWrap WrapR(void) const;

	void WrapS(__TextureWrap mode);  /*<
	Sets the texture wrap mode on the [^S], [^T] and [^R]
	coordinates for this sampler object.
	See [glfunc SamplerParameter],
	[glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T],
	[glconst TEXTURE_WRAP_R].
	>*/
	void WrapT(TextureWrap mode);
	void WrapR(TextureWrap mode);

	GLfloat LODBias(void) const; /*<
	Returns the currently set texture level-of-detail bias value
	of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_LOD_BIAS].
	>*/
	void LODBias(GLfloat value); /*<
	Sets the texture level-of-detail bias value for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_LOD_BIAS].
	>*/

	GLfloat MinLOD(void) const; /*<
	Returns the currently set minimum texture level-of-detail value
	of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_MIN_LOD].
	>*/
	void MinLOD(GLfloat value); /*<
	Sets the minimal texture level-of-detail value for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_MIN_LOD].
	>*/

	GLfloat MaxLOD(void) const; /*<
	Returns the currently set maximum texture level-of-detail value
	of this sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_MAX_LOD].
	>*/
	void MaxLOD(GLfloat value); /*<
	Sets the maximal texture level-of-detail value for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_MAX_LOD].
	>*/

#if GL_ARB_seamless_cubemap_per_texture
	__Boolean Seamless(void) const; /*<
	Returns the value of the seamless-cube map setting of this
	sampler object.
	See [glfunc GetSamplerParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/
	void Seamless(__Boolean enable); /*<
	Sets the seamless cubemap setting for this sampler object.
	See [glfunc SamplerParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/
#endif
};
//]
//[oglplus_sampler_def
typedef ObjectOps<__tag_DirectState, __tag_Sampler>
	SamplerOps;

typedef __Object<SamplerOps> Sampler;

typedef __ObjectZero<__ObjZeroOps<__tag_DirectState, __tag_Sampler>>
	NoSampler;
//]
//[oglplus_sampler_sugar

struct SamplerOpsAndSlot { };

SamplerOpsAndSlot operator | (
	SamplerOps& sam,
	GLuint slot
);

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureUnitSelector tus
); /*< Bind >*/

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureFilter filter
); /*< Filter >*/

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureMinFilter filter
); /*< MinFilter >*/

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureMagFilter filter
); /*< MagFilter >*/

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureCompareMode mode
); /*< CompareMode >*/

SamplerOps& operator << (
	SamplerOps& sam,
	__CompareFunction func
); /*< CompareFunc >*/
{
	sam.CompareFunc(func);
	return sam;
}

SamplerOps& operator << (
	SamplerOps& sam,
	__TextureWrap wrap
); /*< Wrap mode on all coordinates >*/

SamplerOps& operator << (
	SamplerOpsAndSlot sas,
	__TextureWrap wrap
); /*< Wrap mode on the i-th coordinate >*/

template <typename T>
SamplerOps& operator << (
	SamplerOps& sam,
	const Vector<T, 4>& col
); /*< Border color >*/

//]

