
/*
 *  Automatically generated file, do not edit manually!
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oglplus_object_BoundObjOps_Renderbuffer

template <>
class __BoundObjOps<__tag_Renderbuffer>
{
private:
	typedef typename __ObjectOps_Explicit_Renderbuffer<__tag_ExplicitSel, __tag_Renderbuffer> ExplicitOps;
public:
	typedef typename ExplicitOps::Target Target;

	Target target;

	BoundObjOps(void);

	BoundObjOps(Target init_tgt);
	GLint GetIntParam(
		GLenum query
	) const;

	const BoundObjOps& Storage(
		__PixelDataInternalFormat internalformat,
		__SizeType width,
		__SizeType height
	) const;

	const BoundObjOps& Storage(
		const __images_ImageSpec & image_spec
	) const;

	const BoundObjOps& StorageMultisample(
		__SizeType samples,
		__PixelDataInternalFormat internalformat,
		__SizeType width,
		__SizeType height
	) const;

	__SizeType Width(void) const;

	__SizeType Height(void) const;

	__SizeType RedSize(void) const;

	__SizeType GreenSize(void) const;

	__SizeType BlueSize(void) const;

	__SizeType AlphaSize(void) const;

	__SizeType DepthSize(void) const;

	__SizeType StencilSize(void) const;

	__SizeType Samples(void) const;

	__PixelDataInternalFormat InternalFormat(void) const;


};
//]
