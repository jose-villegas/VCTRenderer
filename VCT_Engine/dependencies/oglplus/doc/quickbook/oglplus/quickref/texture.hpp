/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_texture_subtype

template <>
struct __ObjectSubtype<__tag_Texture>
{
	typedef __TextureTarget Type;
};

//]
//[oglplus_texture_common

template <>
class __ObjCommonOps<__tag_Texture>
 : public __TextureName
{
public:
	typedef __TextureTarget Target; /*<
	Texture bind target.
	>*/

	static __TextureName Binding(__TextureTarget target); /*<
	Returns the texture currently bound to the specified [^target].
	See [glfunc GetIntegerv].
	>*/
	static void Bind(__TextureTarget target, __TextureName texture); /*<
	Binds the specified [^texture] to the specified [^target].
	See [glfunc BindTexture].
	>*/

	void Bind(__TextureTarget target) const; /*<
	Binds [^this] texture to the specified [^target].
	See [glfunc BindTexture].
	>*/

#if GL_VERSION_4_2 ||GL_ARB_shader_image_load_store
	static void BindImage(
		__ImageUnitSelector unit,
		__TextureName texture,
		GLint level,
		__Boolean layered,
		GLint layer,
		__AccessSpecifier access,
		__ImageUnitFormat format
	); /*<
	Binds a [^level] of [^texture] to an image [^unit].
	See [glfunc BindTextureImage].
	>*/
#endif

#if GL_VERSION_4_4 || GL_ARB_multi_bind
	static void Bind(
		GLuint first,
		const __Sequence<__TextureName>& textures
	); /*<
	Sequentially binds [^textures] to texture units starting with [^first].
	See [glfunc BindTextures].
	>*/

	static void BindImage(
		GLuint first,
		const __Sequence<__TextureName>& textures
	); /*<
	Sequentially binds [^textures] to image units starting with [^first].
	See [glfunc BindImageTextures].
	>*/
#endif

	static void Active(__TextureUnitSelector index); /*<
	Specifies active texture unit for subsequent commands.
	See [glfunc ActiveTexture].
	>*/

	static GLint Active(void); /*<
	Returns the currently active texture unit.
	See [glfunc Get], [glconst ACTIVE_TEXTURE].
	>*/

	static __TextureTarget CubeMapFace(GLuint face); /*<
	Returns one of the values for cube map faces
	from the __TextureTarget enumeration. The value of [^face] must
	be between 0 and 5 with the following meaning:
	[itemized_list
	[0 = [^__TextureTarget::CubeMapPositiveX]]
	[1 = [^__TextureTarget::CubeMapNegativeX]]
	[2 = [^__TextureTarget::CubeMapPositiveY]]
	[3 = [^__TextureTarget::CubeMapNegativeY]]
	[4 = [^__TextureTarget::CubeMapPositiveZ]]
	[5 = [^__TextureTarget::CubeMapNegativeZ]]
	]
	>*/

#if GL_VERSION_4_3
	void InvalidateImage(__SizeType level); /*<
	Invalidates the specified [^level] of texture image.
	See [glfunc InvalidateTexImage].
	>*/

	void InvalidateSubImage(
		__SizeType level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth
	); /*<
	Invalidates the specified part of texture image.
	See [glfunc InvalidateTexSubImage].
	>*/
#endif

#if GL_VERSION_4_4
	template <typename GLtype>
	void ClearImage(
		__SizeType level,
		__PixelDataFormat format,
		const GLtype* data
	); /*<
	Clears the specified [^level] of texture image.
	See [glfunc ClearTexImage].
	>*/

	template <typename GLtype>
	void ClearSubImage(
		__SizeType level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__PixelDataFormat format,
		const GLtype* data
	); /*<
	Clears the specified part of texture image.
	See [glfunc ClearTexSubImage].
	>*/
#endif

#if GL_VERSION_4_3 || GL_ARB_texture_view
	void View(
		__TextureTarget target,
		__TextureName orig_texture,
		__PixelDataInternalFormat internal_format,
		GLuint min_level,
		GLuint num_levels,
		GLuint min_layer,
		GLuint num_layers
	); /*<
	References and reinteprets a subset of the data of another texture.
	See [glfunc TextureView].
	>*/
#endif
};
//]
//[oglplus_texture_1
template <>
class __ObjectOps<__tag_ExplicitSel, __tag_Texture>
 : public __ObjZeroOps<__tag_ExplicitSel, __tag_Texture> /*<
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

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
		typedef __TextureSwizzleTuple SwizzleTuple;
#endif
		typedef __TextureWrapCoord WrapCoord;

		typedef __TextureWrap Wrap;

		typedef __OneOf<
			__DataType,
			__PixelDataType
		> PixDataType;
	};

#if GL_VERSION_3_0
	static
	__SizeType Width(__TextureTarget target, GLint level = 0); /*<
	Returns the width of the texture image on the specified [^level].
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_WIDTH].
	>*/

	static
	__SizeType Height(__TextureTarget target, GLint level = 0); /*<
	Returns the height of the texture image on the specified [^level].
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_HEIGHT].
	>*/

	static
	__SizeType Depth(__TextureTarget target, GLint level = 0); /*<
	Returns the depth of the texture image on the specified [^level].
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_DEPTH].
	>*/

	static
	__PixelDataType RedType(__TextureTarget target, GLint level = 0); /*<
	Returns the data type used to store the RED component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_RED_TYPE].
	>*/

	static
	__PixelDataType GreenType(__TextureTarget target, GLint level = 0); /*<
	Returns the data type used to store the GREEN component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_GREEN_TYPE].
	>*/

	static
	__PixelDataType BlueType(__TextureTarget target, GLint level = 0); /*<
	Returns the data type used to store the BLUE component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_BLUE_TYPE].
	>*/

	static
	__PixelDataType AlphaType(__TextureTarget target, GLint level = 0); /*<
	Returns the data type used to store the ALPHA component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_ALPHA_TYPE].
	>*/

	static
	__PixelDataType DepthType(__TextureTarget target, GLint level = 0); /*<
	Returns the data type used to store the DEPTH component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_DEPTH_TYPE].
	>*/

	static
	__SizeType RedSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the RED component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_RED_SIZE].
	>*/

	static
	__SizeType GreenSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the GREEN component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_GREEN_SIZE].
	>*/

	static
	__SizeType BlueSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the BLUE component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_BLUE_SIZE].
	>*/

	static
	__SizeType AlphaSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the ALPHA component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_ALPHA_SIZE].
	>*/

	static
	__SizeType DepthSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the DEPTH component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_DEPTH_SIZE].
	>*/

	static
	__SizeType StencilSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of the STENCIL component.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_STENCIL_SIZE].
	>*/

	static
	__SizeType SharedSize(__TextureTarget target, GLint level = 0); /*<
	Returns the actual resolution of all texture components.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_SHARED_SIZE].
	>*/
//]
//[oglplus_texture_2
	static
	__SizeType CompressedImageSize(__TextureTarget target, GLint level = 0); /*<
	Returns the size (in bytes) of the image array if it is compressed.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_COMPRESSED_IMAGE_SIZE].
	>*/

	static
	__PixelDataInternalFormat InternalFormat(__TextureTarget target, GLint level = 0); /*<
	Returns the internal data format of the image array.
	See [glfunc GetTexLevelParameter], [glconst TEXTURE_INTERNAL_FORMAT].
	>*/

	static void GetImage(
		__TextureTarget target,
		GLint level,
		__PixelDataFormat format,
		const __OutputData& dest
	); /*<
	Stores the image of the texture bound to
	the specified texture [^target] with the specified [^level]
	of detail in uncompressed form into the [^dest] buffer.
	See [glfunc GetTexImage].
	[note This function, unlike [^GetCompressedImage], does NOT
	automatically resize the destination buffer so that
	it can accomodate the texture data. The caller is responsible
	for keeping track or querying the type of the texture, its
	dimensions and current pixel transfer settings and resize
	the [^dest] buffer accordingly.]
	>*/
	static void GetImage(
		__TextureTarget target,
		GLint level,
		__PixelDataFormat format,
		Property::PixDataType type,
		__SizeType size,
		GLvoid* buffer
	);

	static void GetCompressedImage(
		__TextureTarget target,
		GLint level,
		const __OutputData& dest
	); /*<
	Stores the image of the texture bound to
	the specified texture [^target] with the specified [^level]
	of detail in compressed form into the [^dest] buffer.
	See [glfunc GetCompressedTexImage].
	This function automatically resizes the buffer so that
	it can accomodate the texture data.
	>*/
	static void GetCompressedImage(
		__TextureTarget target,
		GLint level,
		std::vector<GLubyte>& dest
	);
	static void GetCompressedImage(
		__TextureTarget target,
		GLint level,
		__SizeType size,
		GLubyte* buffer
	);
#endif
	static void Image3D(
		__TextureTarget target,
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
	See [glfunc TexImage3D].
	>*/

	static void Image3D(
		__TextureTarget target,
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	static void SubImage3D(
		__TextureTarget target,
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
	See [glfunc TexSubImage3D].
	>*/

	static void SubImage3D(
		__TextureTarget target,
		const __images_Image& image,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint level = 0
	);

	static void Image2D(
		__TextureTarget target,
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
	See [glfunc TexImage2D].
	>*/

	static void Image2D(
		__TextureTarget target,
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	static void ImageCM(
		GLuint face,
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		GLint border,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies the image of the selected cube-map face.
	See [glfunc TexImage2D].
	>*/

	static void ImageCM(
		GLuint face,
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	static void SubImage2D(
		__TextureTarget target,
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
	See [glfunc TexSubImage2D].
	>*/

	static void SubImage2D(
		__TextureTarget target,
		const __images_Image& image,
		GLint xoffs,
		GLint yoffs,
		GLint level = 0
	);

#if GL_VERSION_3_0
	static void Image1D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		GLint border,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	); /*<
	Specifies a one dimensional texture image.
	See [glfunc TexImage1D].
	>*/

	static void Image1D(
		__TextureTarget target,
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	);

	static void SubImage1D(
		__TextureTarget target,
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		Property::PixDataType type,
		const void* data
	);  /*<
	Specifies a one dimensional texture subimage.
	See [glfunc TexSubImage1D].
	>*/

	static void SubImage1D(
		__TextureTarget target,
		const __images_Image& image,
		GLint xoffs,
		GLint level = 0
	);
#endif

	static void Image(
		__TextureTarget target,
		const __images_Image& image,
		GLint level = 0,
		GLint border = 0
	); /*<
	Specifies one, two or three dimensional texture image.
	>*/

	static void Image(
		__TextureTarget target,
		const __images_ImageSpec& image_spec,
		GLint level = 0,
		GLint border = 0
	);
//]
//[oglplus_texture_3

	static void CopyImage2D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height,
		GLint border
	); /*<
	Copies a two dimensional texture image from the current framebuffer.
	See [glfunc CopyTexImage2D].
	>*/

#if GL_VERSION_3_0
	static void CopyImage1D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		__SizeType width,
		GLint border
	); /*<
	Copies a one dimensional texture image from the current framebuffer.
	See [glfunc CopyTexImage1D].
	>*/
#endif

	static void CopySubImage3D(
		__TextureTarget target,
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
	See [glfunc CopyTexSubImage3D].
	>*/

	static void CopySubImage2D(
		__TextureTarget target,
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	); /*<
	Copies a two dimensional texture subimage from the current framebuffer.
	See [glfunc CopyTexSubImage2D].
	>*/

#if GL_VERSION_3_0
	static void CopySubImage1D(
		__TextureTarget target,
		GLint level,
		GLint xoffs,
		GLint x,
		GLint y,
		__SizeType width
	); /*<
	Copies a one dimensional texture subimage from the current framebuffer.
	See [glfunc CopyTexSubImage2D].
	>*/
#endif

	static void CompressedImage3D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		GLint border,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a three dimensional compressed texture image.
	See [glfunc CompressedTexImage3D].
	>*/

	static void CompressedImage2D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		GLint border,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a two dimensional compressed texture image.
	See [glfunc CompressedTexImage2D].
	>*/

#if GL_VERSION_3_0
	static void CompressedImage1D(
		__TextureTarget target,
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		GLint border,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a one dimensional compressed texture image.
	See [glfunc CompressedTexImage1D].
	>*/
#endif

	static void CompressedSubImage3D(
		__TextureTarget target,
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
	See [glfunc CompressedTexSubImage3D].
	>*/

	static void CompressedSubImage2D(
		__TextureTarget target,
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
	See [glfunc CompressedTexSubImage2D].
	>*/

#if GL_VERSION_3_0
	static void CompressedSubImage1D(
		__TextureTarget target,
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		__SizeType image_size,
		const void* data
	); /*<
	Specifies a one dimensional compressed texture subimage.
	See [glfunc CompressedTexSubImage1D].
	>*/
#endif
//]
//[oglplus_texture_4

#if GL_VERSION_3_2 || GL_ARB_texture_multisample
	static void Image3DMultisample(
		__TextureTarget target,
		__SizeType samples,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__Boolean fixed_sample_locations
	); /*<
	Sets-up a three dimensional multisample texture.
	See [glfunc TexImage3DMultisample].
	>*/

	static void Image2DMultisample(
		__TextureTarget target,
		__SizeType samples,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__Boolean fixed_sample_locations
	); /*<
	Sets-up a two dimensional multisample texture.
	See [glfunc TexImage3DMultisample].
	>*/

#endif

#if GL_VERSION_3_1
	static void Buffer(
		__TextureTarget target,
		__PixelDataInternalFormat internal_format,
		__BufferName buffer
	); /*<
	Assigns a buffer storing the texel data to the texture.
	See [glfunc TexBuffer].
	>*/
#endif

#if GL_VERSION_4_3
	static void BufferRange(
		__TextureTarget target,
		__PixelDataInternalFormat internal_format,
		__BufferName buffer,
		GLintptr offset,
		__BigSizeType size
	); /*<
	Assigns a buffer range storing the texel data to the texture.
	See [glfunc TexBufferRange].
	>*/
#endif

#if GL_VERSION_4_2 || GL_ARB_texture_storage
	static void Storage3D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth
	); /*<
	Specifies all levels of 3D texture at the same time.
	See [glfunc TexStorage3D].
	>*/

	static void Storage2D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height
	); /*<
	Specifies all levels of 2D texture at the same time.
	See [glfunc TexStorage2D].
	>*/

	static void Storage1D(
		__TextureTarget target,
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width
	); /*<
	Specifies all levels of 1D texture at the same time.
	See [glfunc TexStorage1D].
	>*/
#endif
	static GLuint BaseLevel(__TextureTarget target); /*<
	Returns the texture base level.
	See [glfunc GetTexParameter], [glconst TEXTURE_BASE_LEVEL].
	>*/

	static void BaseLevel(__TextureTarget target, GLuint level); /*<
	Sets the texture base level.
	See [glfunc TexParameter], [glconst TEXTURE_BASE_LEVEL].
	>*/

#if GL_VERSION_3_0
	static Vector<GLfloat, 4> BorderColor(
		__TextureTarget target,
		__TypeTag<GLfloat>
	); /*<
	Gets the texture border color.
	See [glfunc GetTexParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	static Vector<GLint, 4> BorderColor(
		__TextureTarget target,
		__TypeTag<GLint>
	);
	static Vector<GLuint, 4> BorderColor(
		__TextureTarget target,
		__TypeTag<GLuint>
	);

	static void BorderColor(
		__TextureTarget target,
		Vector<GLfloat, 4> color
	); /*<
	Sets the texture border color.
	See [glfunc TexParameter], [glconst TEXTURE_BORDER_COLOR].
	>*/
	static void BorderColor(
		__TextureTarget target,
		Vector<GLint, 4> color
	);
	static void BorderColor(
		__TextureTarget target,
		Vector<GLuint, 4> color
	);
#endif
	static __TextureCompareMode CompareMode(__TextureTarget target); /*<
	Gets the texture compare mode.
	See [glfunc GetTexParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/

	static void CompareMode(
		__TextureTarget target,
		__TextureCompareMode mode
	); /*<
	Sets the texture compare mode.
	See [glfunc TexParameter], [glconst TEXTURE_COMPARE_MODE].
	>*/

	static __CompareFunction CompareFunc(__TextureTarget target); /*<
	Gets the texture compare function.
	See [glfunc GetTexParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/

	static void CompareFunc(
		__TextureTarget target,
		__CompareFunction func
	); /*<
	Sets the texture compare function.
	See [glfunc TexParameter], [glconst TEXTURE_COMPARE_FUNC].
	>*/
//]
//[oglplus_texture_5

#if GL_VERSION_3_0
	static GLfloat LODBias(__TextureTarget target); /*<
	Gets the level-of-detail bias value.
	See [glfunc GetTexParameter], [glconst TEXTURE_LOD_BIAS].
	>*/

	static void LODBias(__TextureTarget target, GLfloat value); /*<
	Sets the level-of-detail bias value.
	See [glfunc TexParameter], [glconst TEXTURE_LOD_BIAS].
	>*/
#endif // GL_VERSION_3_0

	static void Filter(
		__TextureTarget target,
		__TextureFilter filter
	); /*<
	Sets both the minification and magnification filter.
	See [glfunc TexParameter], [glconst TEXTURE_MIN_FILTER], [glconst TEXTURE_MAG_FILTER].
	>*/

	static __TextureMagFilter MagFilter(__TextureTarget target); /*<
	Gets the magnification filter.
	See [glfunc GetTexParameter], [glconst TEXTURE_MAG_FILTER].
	>*/

	static void MagFilter(
		__TextureTarget target,
		__TextureMagFilter filter
	); /*<
	Sets the magnification filter.
	See [glfunc TexParameter], [glconst TEXTURE_MAG_FILTER].
	>*/

	static __TextureMinFilter MinFilter(__TextureTarget target); /*<
	Gets the minification filter.
	See [glfunc GetTexParameter], [glconst TEXTURE_MIN_FILTER].
	>*/

	static void MinFilter(
		__TextureTarget target,
		__TextureMinFilter filter
	); /*<
	Sets the minification filter.
	See [glfunc TexParameter], [glconst TEXTURE_MIN_FILTER].
	>*/

	static GLfloat MinLOD(__TextureTarget target); /*<
	Gets minimal level-of-detail value.
	See [glfunc GetTexParameter], [glconst TEXTURE_MIN_LOD].
	>*/

	static void MinLOD(__TextureTarget target, GLfloat value); /*<
	Sets minimal level-of-detail value.
	See [glfunc TexParameter], [glconst TEXTURE_MIN_LOD].
	>*/

	static GLfloat MaxLOD(__TextureTarget target); /*<
	Gets maximal level-of-detail value.
	See [glfunc GetTexParameter], [glconst TEXTURE_MAX_LOD].
	>*/

	static void MaxLOD(__TextureTarget target, GLfloat value); /*<
	Sets maximal level-of-detail value.
	See [glfunc TexParameter], [glconst TEXTURE_MAX_LOD].
	>*/

	static GLint MaxLevel(__TextureTarget target); /*<
	Gets the maximum level value.
	See [glfunc GetTexParameter], [glconst TEXTURE_MAX_LEVEL].
	>*/

	static void MaxLevel(__TextureTarget target, GLint value); /*<
	Sets the maximum level value.
	See [glfunc TexParameter], [glconst TEXTURE_MAX_LEVEL].
	>*/
//]
//[oglplus_texture_6

	static GLfloat MaxAnisotropy(__TextureTarget target); /*<
	Gets the maximum anisotropy value.
	See [glfunc TexParameter], [glconst MAX_TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

	static GLfloat Anisotropy(__TextureTarget target); /*<
	Gets the current anisotropy level.
	See [glfunc GetTexParameter], [glconst TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

	static void Anisotropy(__TextureTarget target, GLfloat value); /*<
	Sets the anisotropy level.
	See [glfunc TexParameter], [glconst TEXTURE_MAX_ANISOTROPY_EXT].
	>*/

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	static __TextureSwizzle Swizzle(
		__TextureTarget target,
		__TextureSwizzleCoord coord
	); /*<
	Gets the swizzle parameter.
	See [glfunc GetTexParameter], [glconst TEXTURE_SWIZZLE_R],
	[glconst TEXTURE_SWIZZLE_G], [glconst TEXTURE_SWIZZLE_B],
	[glconst TEXTURE_SWIZZLE_A], [glconst TEXTURE_SWIZZLE_RGBA].
	>*/
	static __TextureSwizzle SwizzleR(__TextureTarget target);
	static __TextureSwizzle SwizzleG(__TextureTarget target);
	static __TextureSwizzle SwizzleB(__TextureTarget target);
	static __TextureSwizzle SwizzleA(__TextureTarget target);
	static __TextureSwizzleTuple SwizzleRGBA(__TextureTarget target);

	static void Swizzle(
		__TextureTarget target,
		__TextureSwizzleCoord coord,
		__TextureSwizzle mode
	); /*<
	Sets the swizzle parameter.
	See [glfunc TexParameter], [glconst TEXTURE_SWIZZLE_R],
	[glconst TEXTURE_SWIZZLE_G], [glconst TEXTURE_SWIZZLE_B],
	[glconst TEXTURE_SWIZZLE_A], [glconst TEXTURE_SWIZZLE_RGBA].
	>*/
	static void SwizzleR(__TextureTarget target, __TextureSwizzle mode);
	static void SwizzleG(__TextureTarget target, __TextureSwizzle mode);
	static void SwizzleB(__TextureTarget target, __TextureSwizzle mode);
	static void SwizzleA(__TextureTarget target, __TextureSwizzle mode);
	static void SwizzleRGBA(
		__TextureTarget target,
		__TextureSwizzle mode
	);
	static void SwizzleRGBA(
		__TextureTarget target,
		__TextureSwizzle mode_r,
		__TextureSwizzle mode_g,
		__TextureSwizzle mode_b,
		__TextureSwizzle mode_a
	);
	static void SwizzleRGBA(
		__TextureTarget target,
		const __TextureSwizzleTuple& modes
	);
#endif

	static __TextureWrap Wrap(
		__TextureTarget target,
		__TextureWrapCoord coord
	); /*<
	Gets the texture wrap parameter.
	See [glfunc GetTexParameter], [glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T], [glconst TEXTURE_WRAP_R].
	>*/
	static __TextureWrap WrapS(__TextureTarget target);
	static __TextureWrap WrapT(__TextureTarget target);
	static __TextureWrap WrapR(__TextureTarget target);


	static void Wrap(
		__TextureTarget target,
		__TextureWrapCoord coord,
		__TextureWrap mode
	); /*<
	Sets the texture wrap [^mode] on the specified coordinate.
	See [glfunc TexParameter], [glconst TEXTURE_WRAP_S],
	[glconst TEXTURE_WRAP_T], [glconst TEXTURE_WRAP_R].
	>*/
	static void WrapS(__TextureTarget target, __TextureWrap mode);
	static void WrapT(__TextureTarget target, __TextureWrap mode);
	static void WrapR(__TextureTarget target, __TextureWrap mode);

	static void Wrap(__TextureTarget target, __TextureWrap mode); /*<
	Sets the texture wrap [^mode] on all coordinates meaningful for
	the specified texture [^target].
	>*/

#if GL_VERSION_4_3
	static __PixelDataFormat DepthStencilMode(__TextureTarget target); /*<
	Gets the depth stencil mode parameter.
	See [glfunc GetTexParameter], [glconst DEPTH_STENCIL_TEXTURE_MODE].
	>*/

	static void DepthStencilMode(
		__TextureTarget target,
		__PixelDataFormat mode
	); /*<
	Sets the depth stencil mode parameter.
	See [glfunc TexParameter], [glconst DEPTH_STENCIL_TEXTURE_MODE].
	>*/
#endif

#if GL_ARB_seamless_cubemap_per_texture
	static __Boolean Seamless(__TextureTarget target); /*<
	Gets the seamless cubemap setting value.
	See [glfunc GetTexParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/

	static void Seamless(__TextureTarget target, __Boolean enable); /*<
	Changes the seamless cubemap setting value.
	See [glfunc TexParameter], [glconst TEXTURE_CUBE_MAP_SEAMLESS].
	>*/
#endif

#if GL_VERSION_4_5 || GL_ARB_texture_barrier || GL_NV_texture_barrier
	static void Barrier(void); /*<
	Ensures that texture writes have been completed.
	See [glfunc TextureBarrier].
	>*/
#endif

	static void GenerateMipmap(__TextureTarget target) /*<
	Generates mipmap for the texture bound to the specified target.
	See [glfunc GenerateMipmap].
	>*/
};

//]
//[oglplus_texture_def
typedef ObjectOps<__tag_ExplicitSel, __tag_Texture>
	TextureOps;

typedef __Object<TextureOps> Texture;

typedef __ObjectZero<__ObjZeroOps<__tag_ExplicitSel, __tag_Texture>> /*<
Indirectly inherits from __ObjCommonOps_Texture<__tag_Texture>.
>*/
	DefaultTexture;
//]
//[oglplus_texture_sugar1

struct TextureTargetAndSlot { }; /*<
Helper class used with syntax-sugar operators.
Stores a texture target and an integer ['slot] number.
Depending on the context the slot may be interpreted as
the swizzle or wrap coordinate or image level number.
>*/

TextureTargetAndSlot operator | (
	__TextureTarget target,
	GLuint slot
);

struct TextureUnitAndTarget { }; /*<
Helper class used with syntax-sugar operators.
Stores a texture unit number and a texture target.
>*/

TextureUnitAndTarget operator | (
	__TextureUnitSelector unit,
	__TextureTarget tex
);

__TextureTarget operator << (
	const TextureOps& tex,
	__TextureTarget target
); /*<
Binds a texture object to the specified [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	__TextureFilter filter
); /*<
Sets texture minification and magnification filter
on the texture currently bound to [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	__TextureMinFilter filter
); /*<
Sets a texture minification filter
on the texture currently bound to [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	__TextureMagFilter filter
); /*<
Sets a texture magification filter
on the texture currently bound to [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	__TextureCompareMode mode
); /*<
Sets a texture compare mode
on the texture currently bound to [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	__CompareFunction func
); /*<
Sets a texture compare function
on the texture currently bound to [^target].
>*/

//]
//[oglplus_texture_sugar2

__TextureTarget operator << (
	__TextureTarget target,
	__TextureWrap wrap
); /*<
Sets a texture wrap mode
on the texture currently bound to [^target]
on all texture dimensions.
>*/

TextureTargetAndSlot operator << (
	TextureTargetAndSlot tas,
	__TextureWrap wrap
); /*<
Sets a texture wrap mode
on the texture currently bound to target
on the dimension specified by the slot number in [^tas].
>*/

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
__TextureTarget operator << (
	__TextureTarget target,
	__TextureSwizzle swizzle
); /*<
Sets a texture swizzle mode
on the texture currently bound to [^target]
on all texture dimensions.
>*/

TextureTargetAndSlot operator << (
	TextureTargetAndSlot tas,
	__TextureSwizzle swizzle
); /*<
Sets a texture swizzle mode
on the texture currently bound to target
on the dimension specified by the slot number in [^tas].
>*/

#endif

#if GL_VERSION_3_0
template <typename T>
__TextureTarget operator << (
	TextureTarget target,
	const Vector<T, 4>& col
); /*<
Sets a texture border color
on the texture currently bound to [^target].
>*/
#endif

__TextureTarget operator << (
	__TextureTarget target,
	const __images_Image& image
); /*<
Specifies the level 0 texture image
on the texture currently bound to [^target].
>*/

__TextureTarget operator << (
	__TextureTarget target,
	const __images_ImageSpec& image_spec
);

__TextureTarget operator << (
	TextureTargetAndSlot tas,
	const __images_Image& image
); /*<
Specifies the texture image on the level
specified by the slot number in [^tas]
of the texture currently bound to target.
>*/

__TextureTarget operator << (
	TextureTargetAndSlot tas,
	const __images_ImageSpec& image_spec
);

struct TextureMipmap { }; /*<
Helper class used with syntax-sugar operators.
Selects the GenerateMipmap member function.
>*/

__TextureTarget operator << (
	__TextureTarget target,
	TextureMipmap
); /*<
Generates mipmap on the texture currently bound to [^target].
>*/

//]

