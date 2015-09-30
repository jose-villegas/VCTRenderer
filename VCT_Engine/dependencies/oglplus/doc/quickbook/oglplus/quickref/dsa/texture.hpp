/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_texture_1

#if GL_VERSION_4_5 || GL_ARB_direct_state_access

template <>
class __ObjectOps<__tag_DirectState, __tag_Texture>
 : public __ObjZeroOps<__tag_DirectState, __tag_Texture> /*<
Indirectly inherits from __ObjCommonOps_Texture<__tag_Texture>.
>*/
{
public:
	struct Property
	{
		typedef __TextureCompareMode CompareMode;

		typedef __TextureFilter Filter;

		typedef __TextureMagFilter MagFilter;

		typedef __TextureMinFilter MinFilter;

		typedef __TextureSwizzleCoord SwizzleCoord;

		typedef __TextureSwizzle Swizzle;

#if GL_ARB_texture_swizzle
		typedef __TextureSwizzleTuple SwizzleTuple;
#endif
		typedef __TextureWrapCoord WrapCoord;

		typedef __TextureWrap Wrap;

		typedef __OneOf<
			__DataType,
			__PixelDataType
		> PixDataType;
	};

	__SizeType Width(GLint level = 0) const; /*<
	Returns the width of the texture image on the specified [^level].
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_WIDTH].
	>*/

	__SizeType Height(GLint level = 0) const; /*<
	Returns the height of the texture image on the specified [^level].
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_HEIGHT].
	>*/

	__SizeType Depth(GLint level = 0) const; /*<
	Returns the depth of the texture image on the specified [^level].
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_DEPTH].
	>*/

	__PixelDataType RedType(GLint level = 0) const; /*<
	Returns the data type used to store the RED component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_RED_TYPE].
	>*/

	__PixelDataType GreenType(GLint level = 0) const; /*<
	Returns the data type used to store the GREEN component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_GREEN_TYPE].
	>*/

	__PixelDataType BlueType(GLint level = 0) const; /*<
	Returns the data type used to store the BLUE component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_BLUE_TYPE].
	>*/

	__PixelDataType AlphaType(GLint level = 0) const; /*<
	Returns the data type used to store the ALPHA component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_ALPHA_TYPE].
	>*/

	__PixelDataType DepthType(GLint level = 0) const; /*<
	Returns the data type used to store the DEPTH component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_DEPTH_TYPE].
	>*/

	__SizeType RedSize(GLint level = 0) const; /*<
	Returns the actual resolution of the RED component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_RED_SIZE].
	>*/

	__SizeType GreenSize(GLint level = 0) const; /*<
	Returns the actual resolution of the GREEN component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_GREEN_SIZE].
	>*/

	__SizeType BlueSize(GLint level = 0) const; /*<
	Returns the actual resolution of the BLUE component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_BLUE_SIZE].
	>*/

	__SizeType AlphaSize(GLint level = 0) const; /*<
	Returns the actual resolution of the ALPHA component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_ALPHA_SIZE].
	>*/

	__SizeType DepthSize(GLint level = 0) const; /*<
	Returns the actual resolution of the DEPTH component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_DEPTH_SIZE].
	>*/

	__SizeType StencilSize(GLint level = 0) const; /*<
	Returns the actual resolution of the STENCIL component.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_STENCIL_SIZE].
	>*/

	__SizeType SharedSize(GLint level = 0) const; /*<
	Returns the actual resolution of all texture components.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_SHARED_SIZE].
	>*/
//]
//[oglplus_dsa_texture_2
	__SizeType CompressedImageSize(GLint level = 0) const; /*<
	Returns the size (in bytes) of the image array if it is compressed.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_COMPRESSED_IMAGE_SIZE].
	>*/

	__PixelDataInternalFormat InternalFormat(GLint level = 0) const; /*<
	Returns the internal data format of the image array.
	See [glfunc GetTextureLevelParameter], [glconst TEXTURE_INTERNAL_FORMAT].
	>*/

	void GetImage(
		GLint level,
		__PixelDataFormat format,
		const __OutputData& dest
	) const; /*<
	Stores the image of [^this] texture with the specified [^level]
	of detail in uncompressed form into the [^dest] buffer.
	See [glfunc GetTextureImage].
	[note This function, unlike [^GetCompressedImage], does NOT
	automatically resize the destination buffer so that
	it can accomodate the texture data. The caller is responsible
	for keeping track or querying the type of the texture, its
	dimensions and current pixel transfer settings and resize
	the [^dest] buffer accordingly.]
	>*/
	void GetImage(
		GLint level,
		__PixelDataFormat format,
		Property::PixDataType type,
		__SizeType size,
		GLvoid* buffer
	) const;

	void GetCompressedImage(
		GLint level,
		const __OutputData& dest
	) const; /*<
	Stores the image of [^this] texture with the specified [^level]
	of detail in compressed form into the [^dest] buffer.
	See [glfunc GetCompressedTextureImage].
	This function automatically resizes the buffer so that
	it can accomodate the texture data.
	>*/
	void GetCompressedImage(
		GLint level,
		std::vector<GLubyte>& dest
	) const;
	void GetCompressedImage(
		GLint level,
		__SizeType size,
		GLubyte* buffer
	) const;

	ObjectOps& Image3D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		GLint border,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a three dimensional texture image.
	See [glfunc TextureImage3D].
	>*/

	ObjectOps& Image3D(
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	ObjectOps& SubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a three dimensional texture subimage.
	See [glfunc TextureSubImage3D].
	>*/

	ObjectOps& SubImage3D(
		const __images_Image& image,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint level = 0
	);

	ObjectOps& Image2D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		GLint border,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a two dimensional texture image.
	See [glfunc TextureImage2D].
	>*/

	ObjectOps& Image2D(
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	ObjectOps& SubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		__SizeType width,
		__SizeType height,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a two dimensional texture subimage.
	See [glfunc TextureSubImage2D].
	>*/

	ObjectOps& SubImage2D(
		const __images_Image& image,
		GLint xoffs,
		GLint yoffs,
		GLint level = 0
	);

	ObjectOps& Image1D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		GLint border,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a one dimensional texture image.
	See [glfunc TextureImage1D].
	>*/

	ObjectOps& Image1D(
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	ObjectOps& SubImage1D(
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	);  /*<
	Specifies a one dimensional texture subimage.
	See [glfunc TextureSubImage1D].
>*/

	ObjectOps& SubImage1D(
		const __images_Image& image,
		GLint xoffs,
		GLint level = 0
	);

//]
//[oglplus_dsa_texture_3

	ObjectOps& CopySubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	); /*<
	Copies a three dimensional texture subimage from the current framebuffer.
	See [glfunc CopyTextureSubImage3D].
	>*/

	ObjectOps& CopySubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	); /*<
	Copies a two dimensional texture subimage from the current framebuffer.
	See [glfunc CopyTextureSubImage2D].
	>*/

	ObjectOps& CopySubImage1D(
		GLint level,
		GLint xoffs,
		GLint x,
		GLint y,
		__SizeType width
	); /*<
	Copies a one dimensional texture subimage from the current framebuffer.
	See [glfunc CopyTextureSubImage2D].
	>*/

	ObjectOps& CompressedSubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__PixelDataFormat format,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a three dimensional compressed texture subimage.
	See [glfunc CompressedTextureSubImage3D].
	>*/

	ObjectOps& CompressedSubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		__SizeType width,
		__SizeType height,
		__PixelDataFormat format,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a two dimensional compressed texture subimage.
	See [glfunc CompressedTextureSubImage2D].
	>*/

	ObjectOps& CompressedSubImage1D(
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a one dimensional compressed texture subimage.
	See [glfunc CompressedTextureSubImage1D].
>*/
//]
//[oglplus_dsa_texture_4

	ObjectOps& Buffer(
		__PixelDataInternalFormat internal_format,
		__BufferName buffer
	); /*<
	Assigns a buffer storing the texel data to the texture.
	See [glfunc TexureBuffer].
	>*/

#if GL_ARB_texture_storage
	static void Storage3D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth
	); /*<
	Specifies all levels of 3D texture at the same time.
	See [glfunc TextureStorage3D].
	>*/

	static void Storage2D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height
	); /*<
	Specifies all levels of 2D texture at the same time.
	See [glfunc TextureStorage2D].
	>*/

	static void Storage1D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width
	); /*<
	Specifies all levels of 1D texture at the same time.
	See [glfunc TextureStorage1D].
	>*/
#endif

	GLuint BaseLevel(void) const; /*<
	Returns the texture base level.
	See [glfunc GetTextureParameter], [glconst TEXTURE_BASE_LEVEL].
	>*/

	ObjectOps& BaseLevel(GLuint level); /*<
	Sets the texture base level.
	See [glfunc TextureParameter], [glconst TEXTURE_BASE_LEVEL].
	>*/

	Vector<GLfloat, 4> BorderColor(__TypeTag<GLfloat>) const; /*<
	Gets the texture border color.
	See [glfunc GetTextureParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	Vector<GLint, 4> BorderColor(__TypeTag<GLint>) const;
	Vector<GLuint, 4> BorderColor(__TypeTag<GLuint>) const;

	ObjectOps& BorderColor(Vector<GLfloat, 4> color); /*<
	Sets the texture border color.
	See [glfunc TextureParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	ObjectOps& BorderColor(Vector<GLint, 4> color);
	ObjectOps& BorderColor(Vector<GLuint, 4> color);

	__TextureCompareMode CompareMode(void) const; /*<
	Gets the texture compare mode.
	See [glfunc GetTextureParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/

	ObjectOps& CompareMode(__TextureCompareMode mode); /*<
	Sets the texture compare mode.
	See [glfunc TextureParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/

	__CompareFunction CompareFunc(void) const; /*<
	Gets the texture compare function.
	See [glfunc GetTextureParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/

	ObjectOps& CompareFunc(__CompareFunction func); /*<
	Sets the texture compare function.
	See [glfunc TextureParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/
//]
//[oglplus_dsa_texture_5

	GLfloat LODBias(void) const; /*<
	Gets the level-of-detail bias value.
	See [glfunc GetTextureParameter], [glconst TEXTURE_LOD_BIAS].
	>*/

	ObjectOps& LODBias(GLfloat value); /*<
	Sets the level-of-detail bias value.
	See [glfunc TextureParameter], [glconst TEXTURE_LOD_BIAS].
	>*/

	ObjectOps& Filter(__TextureFilter filter); /*<
	Sets both the minification and magnification filter.
	See [glfunc TextureParameter], [glconst TEXTURE_MIN_FILTER], [glconst TEXTURE_MAG_FILTER].
	>*/

	__TextureMagFilter MagFilter(void) const; /*<
	Gets the magnification filter.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MAG_FILTER].
	>*/

	ObjectOps& MagFilter(__TextureMagFilter filter); /*<
	Sets the magnification filter.
	See [glfunc TextureParameter], [glconst TEXTURE_MAG_FILTER].
	>*/

	__TextureMinFilter MinFilter(void) const; /*<
	Gets the minification filter.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MIN_FILTER].
	>*/

	ObjectOps& MinFilter(__TextureMinFilter filter); /*<
	Sets the minification filter.
	See [glfunc TextureParameter], [glconst TEXTURE_MIN_FILTER].
	>*/

	GLfloat MinLOD(void) const; /*<
	Gets minimal level-of-detail value.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MIN_LOD].
	>*/

	ObjectOps& MinLOD(GLfloat value); /*<
	Sets minimal level-of-detail value.
	See [glfunc TextureParameter], [glconst TEXTURE_MIN_LOD].
	>*/

	GLfloat MaxLOD(void) const; /*<
	Gets maximal level-of-detail value.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MAX_LOD].
	>*/

	ObjectOps& MaxLOD(GLfloat value); /*<
	Sets maximal level-of-detail value.
	See [glfunc TextureParameter], [glconst TEXTURE_MAX_LOD].
	>*/

	GLint MaxLevel(void) const; /*<
	Gets the maximum level value.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MAX_LEVEL].
	>*/

	ObjectOps& MaxLevel(GLint value); /*<
	Sets the maximum level value.
	See [glfunc TextureParameter], [glconst TEXTURE_MAX_LEVEL].
>*/
//]
//[oglplus_dsa_texture_6

	GLfloat MaxAnisotropy(void) const; /*<
	Gets the maximum anisotropy value.
	See [glfunc GetTextureParameter], [glconst MAX_TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

	GLfloat Anisotropy(void) const; /*<
	Gets the current anisotropy level.
	See [glfunc GetTextureParameter], [glconst TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

	ObjectOps& Anisotropy(GLfloat value); /*<
	Sets the anisotropy level.
	See [glfunc TextureParameter], [glconst TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

#if GL_ARB_texture_swizzle
	__TextureSwizzle Swizzle(__TextureSwizzleCoord coord) const; /*<
	Gets the swizzle parameter.
	See [glfunc GetTextureParameter], [glconst TEXTURE_SWIZZLE_R],
	[glconst TEXTURE_SWIZZLE_G], [glconst TEXTURE_SWIZZLE_B],
	[glconst TEXTURE_SWIZZLE_A], [glconst TEXTURE_SWIZZLE_RGBA].
	>*/
	__TextureSwizzle SwizzleR(void) const;
	__TextureSwizzle SwizzleG(void) const;
	__TextureSwizzle SwizzleB(void) const;
	__TextureSwizzle SwizzleA(void) const;
	__TextureSwizzleTuple SwizzleRGBA(void) const;

	ObjectOps& Swizzle(
		__TextureSwizzleCoord coord,
		__TextureSwizzle mode
	); /*<
	Sets the swizzle parameter.
	See [glfunc TextureParameter], [glconst TEXTURE_SWIZZLE_R],
	[glconst TEXTURE_SWIZZLE_G], [glconst TEXTURE_SWIZZLE_B],
	[glconst TEXTURE_SWIZZLE_A], [glconst TEXTURE_SWIZZLE_RGBA].
	>*/
	ObjectOps& SwizzleR(__TextureSwizzle mode);
	ObjectOps& SwizzleG(__TextureSwizzle mode);
	ObjectOps& SwizzleB(__TextureSwizzle mode);
	ObjectOps& SwizzleA(__TextureSwizzle mode);
	ObjectOps& SwizzleRGBA(__TextureSwizzle mode);
	ObjectOps& SwizzleRGBA(
		__TextureSwizzle mode_r,
		__TextureSwizzle mode_g,
		__TextureSwizzle mode_b,
		__TextureSwizzle mode_a
	);
	ObjectOps& SwizzleRGBA(const __TextureSwizzleTuple& modes);
#endif

	__TextureWrap Wrap(__TextureWrapCoord coord) const; /*<
	Gets the swizzle parameter.
	See [glfunc GetTextureParameter], [glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T], [glconst TEXTURE_WRAP_R].
	>*/
	__TextureWrap WrapS(void) const;
	__TextureWrap WrapT(void) const;
	__TextureWrap WrapR(void) const;


	ObjectOps& Wrap(
		__TextureWrapCoord coord,
		__TextureWrap mode
	); /*<
	Sets the swizzle parameter.
	See [glfunc TextureParameter], [glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T], [glconst TEXTURE_WRAP_R].
	>*/
	ObjectOps& WrapS(__TextureWrap mode);
	ObjectOps& WrapT(__TextureWrap mode);
	ObjectOps& WrapR(__TextureWrap mode);

	__PixelDataFormat DepthStencilMode(void) const; /*<
	Gets the depth stencil mode parameter.
	See [glfunc GetTextureParameter], [glconst DEPTH_STENCIL_TEXTURE_MODE].
	>*/

	ObjectOps& DepthStencilMode(__PixelDataFormat mode); /*<
	Sets the depth stencil mode parameter.
	See [glfunc TextureParameter], [glconst DEPTH_STENCIL_TEXTURE_MODE].
	>*/

#if GL_ARB_seamless_cubemap_per_texture
	__Boolean Seamless(void); /*<
	Gets the seamless cubemap setting value.
	See [glfunc GetTextureParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/

	ObjectOps& Seamless(__Boolean enable); /*<
	Changes the seamless cubemap setting value.
	See [glfunc TextureParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/
#endif

	ObjectOps& GenerateMipmap(void) /*<
	Generates mipmap for the texture bound to the specified target.
	See [glfunc GenerateTextureMipmap].
	>*/
};

//]
//[oglplus_dsa_texture_def
typedef ObjectOps<__tag_DirectState, __tag_Texture>
	DSATextureOps;

typedef __Object<DSATextureOps> Texture;

//]
//[oglplus_dsa_texture_sugar1

struct DSATextureOpsAndSlot { }; /*<
Helper class for syntax-sugar operators.
>*/

DSATextureOpsAndSlot operator | (
	DSATextureOps& tex,
	GLuint slot
);

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureTarget target
); /*< Bind >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureFilter filter
); /*< Filter >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureMinFilter filter
); /*< MinFilter >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureMagFilter filter
); /*< MagFilter >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureCompareMode mode
); /*< CompareMode >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__CompareFunction func
); /*< CompareFunc >*/

DSATextureOps& operator << (
	DSATextureOpsAndSlot tas,
	__TextureWrap wrap
); /*< Wrap >*/

DSATextureOps& operator << (
	DSATextureOps& tex,
	__TextureSwizzle swizzle
); /*< Swizzle >*/

DSATextureOps& operator << (
	DSATextureOpsAndSlot tas,
	__TextureSwizzle swizzle
); /*< Swizzle >*/

template <typename T>
DSATextureOps& operator << (
	DSATextureOps& tex,
	const Vector<T, 4>& col
); /*< BorderColor >*/

// TODO

#endif // GL_VERSION_4_5 || GL_ARB_direct_state_access

//]

