
/*
 *  Automatically generated file, do not edit manually!
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oglplus_object_BoundObjOps_Buffer

template <>
class __BoundObjOps<__tag_Buffer>
{
private:
	typedef typename __ObjectOps_Explicit_Buffer<__tag_ExplicitSel, __tag_Buffer> ExplicitOps;
public:
	typedef typename ExplicitOps::Target Target;

	Target target;

	BoundObjOps(void);

	BoundObjOps(Target init_tgt);
	GLint GetIntParam(
		GLenum query
	) const;

	__Boolean Mapped(void) const;

	const BoundObjOps& Resize(
		__BufferSize size,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const;

	const BoundObjOps& Data(
		const __BufferData & data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const;

	const BoundObjOps& RawData(
		__BufferSize size,
		const GLvoid * data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const;

	template <typename GLtype>
	const BoundObjOps& Data(
		__SizeType count,
		const GLtype * data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const;

	const BoundObjOps& SubData(
		__BufferSize offset,
		const __BufferData & data
	) const;

	template <typename GLtype>
	const BoundObjOps& SubData(
		__BufferSize offset,
		__SizeType count,
		const GLtype * data
	) const;

#if GL_VERSION_4_3
	template <typename GLtype>
	const BoundObjOps& ClearData(
		__PixelDataInternalFormat internal_format,
		__PixelDataFormat format,
		const GLtype * data
	) const;
#endif

#if GL_VERSION_4_3
	template <typename GLtype>
	const BoundObjOps& ClearSubData(
		__PixelDataInternalFormat internal_format,
		__BufferSize offset,
		__BufferSize size,
		__PixelDataFormat format,
		const GLtype * data
	) const;
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	const BoundObjOps& Storage(
		const __BufferData & data,
		__Bitfield< __BufferStorageBit > flags
	) const;
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	const BoundObjOps& Storage(
		__BufferSize size,
		const void * data,
		__Bitfield< __BufferStorageBit > flags
	) const;
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	__Boolean ImmutableStorage(void) const;
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	__Bitfield< __BufferStorageBit > StorageFlags(void) const;
#endif

#if GL_ARB_sparse_buffer
	const BoundObjOps& PageCommitment(
		__BufferSize offset,
		__BufferSize size,
		__Boolean commit
	) const;
#endif

	__SizeType Size(void) const;

	__BufferUsage Usage(void) const;

	__Bitfield< __BufferMapAccess > __Access(void) const;

#if GL_NV_shader_buffer_load
	const BoundObjOps& MakeResident(
		__AccessSpecifier access
	) const;
#endif

#if GL_NV_shader_buffer_load
	const BoundObjOps& MakeNonResident(void) const;
#endif

#if GL_NV_shader_buffer_load
	__BufferGPUAddress GPUAddress(void) const;
#endif


};
//]
