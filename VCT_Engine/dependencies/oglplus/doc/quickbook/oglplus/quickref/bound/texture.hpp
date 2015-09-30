
/*
 *  Automatically generated file, do not edit manually!
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oglplus_object_BoundObjOps_Texture

template <>
class __BoundObjOps<__tag_Texture>
{
private:
	typedef typename __ObjectOps_Explicit_Texture<__tag_ExplicitSel, __tag_Texture> ExplicitOps;
public:
	typedef typename ExplicitOps::Target Target;

	Target target;

	BoundObjOps(void);

	BoundObjOps(Target init_tgt);
	GLint GetIntParam(
		GLenum query
	) const;

	GLfloat GetFloatParam(
		GLenum query
	) const;

	GLint GetIntParam(
		GLint level,
		GLenum query
	) const;

	GLfloat GetFloatParam(
		GLint level,
		GLenum query
	) const;

	__SizeType Width(
		GLint level = 0
	) const;

	__SizeType Height(
		GLint level = 0
	) const;

	__SizeType Depth(
		GLint level = 0
	) const;

	__PixelDataType RedType(
		GLint level = 0
	) const;

	__PixelDataType GreenType(
		GLint level = 0
	) const;

	__PixelDataType BlueType(
		GLint level = 0
	) const;

	__PixelDataType AlphaType(
		GLint level = 0
	) const;

	__PixelDataType DepthType(
		GLint level = 0
	) const;

	__SizeType RedSize(
		GLint level = 0
	) const;

	__SizeType GreenSize(
		GLint level = 0
	) const;

	__SizeType BlueSize(
		GLint level = 0
	) const;

	__SizeType AlphaSize(
		GLint level = 0
	) const;

	__SizeType DepthSize(
		GLint level = 0
	) const;

	__SizeType StencilSize(
		GLint level = 0
	) const;

	__SizeType SharedSize(
		GLint level = 0
	) const;

	__SizeType CompressedImageSize(
		GLint level = 0
	) const;

	__PixelDataInternalFormat InternalFormat(
		GLint level = 0
	) const;

	const BoundObjOps& GetImage(
		GLint level,
		__PixelDataFormat format,
		const __OutputData & dest
	) const;

	const BoundObjOps& GetImage(
		GLint level,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		__SizeType size,
		GLvoid * buffer
	) const;

	const BoundObjOps& GetCompressedImage(
		GLint level,
		const __OutputData & dest
	) const;

	const BoundObjOps& GetCompressedImage(
		GLint level,
		__SizeType size,
		GLubyte * buffer
	) const;

	const BoundObjOps& GetCompressedImage(
		GLint level,
		std::vector< GLubyte > & dest
	) const;

	const BoundObjOps& Image3D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		GLint border,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& Image3D(
		const __images_Image & image,
		GLint level = 0,
		GLint border = 0
	) const;

	const BoundObjOps& SubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& SubImage3D(
		const __images_Image & image,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint level = 0
	) const;

	const BoundObjOps& Image2D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		GLint border,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& Image2D(
		const __images_Image & image,
		GLint level = 0,
		GLint border = 0
	) const;

	const BoundObjOps& SubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		__SizeType width,
		__SizeType height,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& SubImage2D(
		const __images_Image & image,
		GLint xoffs,
		GLint yoffs,
		GLint level = 0
	) const;

	const BoundObjOps& Image1D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		GLint border,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& Image1D(
		const __images_Image & image,
		GLint level = 0,
		GLint border = 0
	) const;

	const BoundObjOps& SubImage1D(
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		ExplicitOps::Property::PixDataType type,
		const void * data
	) const;

	const BoundObjOps& SubImage1D(
		const __images_Image & image,
		GLint xoffs,
		GLint level = 0
	) const;

	const BoundObjOps& Image(
		const __images_Image & image,
		GLint level = 0,
		GLint border = 0
	) const;

	const BoundObjOps& Image(
		const __images_ImageSpec & image_spec,
		GLint level = 0,
		GLint border = 0
	) const;

	const BoundObjOps& CopyImage2D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height,
		GLint border
	) const;

	const BoundObjOps& CopyImage1D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		GLint x,
		GLint y,
		__SizeType width,
		GLint border
	) const;

	const BoundObjOps& CopySubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	) const;

	const BoundObjOps& CopySubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	) const;

	const BoundObjOps& CopySubImage1D(
		GLint level,
		GLint xoffs,
		GLint x,
		GLint y,
		__SizeType width
	) const;

	const BoundObjOps& CompressedImage3D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		GLint border,
		__SizeType image_size,
		const void * data
	) const;

	const BoundObjOps& CompressedImage2D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		GLint border,
		__SizeType image_size,
		const void * data
	) const;

	const BoundObjOps& CompressedImage1D(
		GLint level,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		GLint border,
		__SizeType image_size,
		const void * data
	) const;

	const BoundObjOps& CompressedSubImage3D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		GLint zoffs,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__PixelDataFormat format,
		__SizeType image_size,
		const void * data
	) const;

	const BoundObjOps& CompressedSubImage2D(
		GLint level,
		GLint xoffs,
		GLint yoffs,
		__SizeType width,
		__SizeType height,
		__PixelDataFormat format,
		__SizeType image_size,
		const void * data
	) const;

	const BoundObjOps& CompressedSubImage1D(
		GLint level,
		GLint xoffs,
		__SizeType width,
		__PixelDataFormat format,
		__SizeType image_size,
		const void * data
	) const;

#if GL_VERSION_3_2 || GL_ARB_texture_multisample
	const BoundObjOps& Image3DMultisample(
		__SizeType samples,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth,
		__Boolean fixed_sample_locations
	) const;
#endif

#if GL_VERSION_3_2 || GL_ARB_texture_multisample
	const BoundObjOps& Image2DMultisample(
		__SizeType samples,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__Boolean fixed_sample_locations
	) const;
#endif

#if GL_VERSION_3_1
	const BoundObjOps& __Buffer(
		__PixelDataInternalFormat internal_format,
		__BufferName buffer
	) const;
#endif

#if GL_VERSION_4_3
	const BoundObjOps& BufferRange(
		__PixelDataInternalFormat internal_format,
		__BufferName buffer,
		GLintptr offset,
		__BigSizeType size
	) const;
#endif

#if GL_VERSION_4_2 || GL_ARB_texture_storage
	const BoundObjOps& Storage1D(
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width
	) const;
#endif

#if GL_VERSION_4_2 || GL_ARB_texture_storage
	const BoundObjOps& Storage2D(
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height
	) const;
#endif

#if GL_VERSION_4_2 || GL_ARB_texture_storage
	const BoundObjOps& Storage3D(
		__SizeType levels,
		__PixelDataInternalFormat internal_format,
		__SizeType width,
		__SizeType height,
		__SizeType depth
	) const;
#endif

	GLuint BaseLevel(void) const;

	const BoundObjOps& BaseLevel(
		GLint level
	) const;

	__Vector< GLfloat, 4 > BorderColor(
		__TypeTag< GLfloat >
	) const;

	const BoundObjOps& BorderColor(
		__Vector< GLfloat, 4 > color
	) const;

	__Vector< GLint, 4 > BorderColor(
		__TypeTag< GLint >
	) const;

	const BoundObjOps& BorderColor(
		__Vector< GLint, 4 > color
	) const;

	__Vector< GLuint, 4 > BorderColor(
		__TypeTag< GLuint >
	) const;

	const BoundObjOps& BorderColor(
		__Vector< GLuint, 4 > color
	) const;

	__TextureCompareMode CompareMode(void) const;

	const BoundObjOps& CompareMode(
		__TextureCompareMode mode
	) const;

	__CompareFunction CompareFunc(void) const;

	const BoundObjOps& CompareFunc(
		__CompareFunction func
	) const;

	GLfloat LODBias(void) const;

	const BoundObjOps& LODBias(
		GLfloat value
	) const;

	const BoundObjOps& Filter(
		__TextureFilter filter
	) const;

	__TextureMagFilter MagFilter(void) const;

	const BoundObjOps& MagFilter(
		__TextureMagFilter filter
	) const;

	__TextureMinFilter MinFilter(void) const;

	const BoundObjOps& MinFilter(
		__TextureMinFilter filter
	) const;

	GLfloat MinLOD(void) const;

	const BoundObjOps& MinLOD(
		GLfloat value
	) const;

	GLfloat MaxLOD(void) const;

	const BoundObjOps& MaxLOD(
		GLfloat value
	) const;

	GLint MaxLevel(void) const;

	const BoundObjOps& MaxLevel(
		GLint value
	) const;

	GLfloat MaxAnisotropy(void) const;

	GLfloat Anisotropy(void) const;

	const BoundObjOps& Anisotropy(
		GLfloat value
	) const;

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzle Swizzle(
		__TextureSwizzleCoord coord
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& Swizzle(
		__TextureSwizzleCoord coord,
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzle SwizzleR(void) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleR(
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzle SwizzleG(void) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleG(
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzle SwizzleB(void) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleB(
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzle SwizzleA(void) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleA(
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	__TextureSwizzleTuple SwizzleRGBA(void) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleRGBA(
		__TextureSwizzle mode
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleRGBA(
		__TextureSwizzle mode_r,
		__TextureSwizzle mode_g,
		__TextureSwizzle mode_b,
		__TextureSwizzle mode_a
	) const;
#endif

#if GL_VERSION_3_3 || GL_ARB_texture_swizzle
	const BoundObjOps& SwizzleRGBA(
		const __TextureSwizzleTuple & modes
	) const;
#endif

	__TextureWrap Wrap(
		__TextureWrapCoord coord
	) const;

	const BoundObjOps& Wrap(
		__TextureWrapCoord coord,
		__TextureWrap mode
	) const;

	__TextureWrap WrapS(void) const;

	const BoundObjOps& WrapS(
		__TextureWrap mode
	) const;

	__TextureWrap WrapT(void) const;

	const BoundObjOps& WrapT(
		__TextureWrap mode
	) const;

	__TextureWrap WrapR(void) const;

	const BoundObjOps& WrapR(
		__TextureWrap mode
	) const;

	const BoundObjOps& Wrap(
		__TextureWrap mode
	) const;

#if GL_VERSION_4_3
	__PixelDataFormat DepthStencilMode(void) const;
#endif

#if GL_VERSION_4_3
	const BoundObjOps& DepthStencilMode(
		__PixelDataFormat mode
	) const;
#endif

#if GL_ARB_seamless_cubemap_per_texture
	__Boolean Seamless(void) const;
#endif

#if GL_ARB_seamless_cubemap_per_texture
	const BoundObjOps& Seamless(
		__Boolean enable
	) const;
#endif

	const BoundObjOps& GenerateMipmap(void) const;


};
//]
