/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_buffer_1

#if GL_VERSION_4_5 || GL_ARB_direct_state_access

template <>
class __ObjectOps<__tag_DirectState, __tag_Buffer>
 : public __ObjZeroOps<__tag_DirectState, __tag_Buffer> /*<
Indirectly inherits from __ObjCommonOps_Buffer<__tag_Buffer>.
>*/
{
public:
	struct Property
	{
		typedef __BufferUsage Usage;
		typedef __BufferMapAccess MapAccess;
	};

	typedef __DSABufferTypedMap<GLubyte> Map; /*<
	Mapping of the buffer to the client address space.
	>*/

	__Boolean Mapped(void) const; /*<
	Returns true if [^this] buffer is mapped.
	See [glfunc GetNamedBufferParameter], [glconst BUFFER_MAPPED].
	>*/

	void Resize(
		__BufferSize size,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const; /*<
	Allocates or reallocates storage of [^this] buffer
	to the specified [^size] without uploading any data,
	taking [^usage] as a hint.
	See [glfunc NamedBufferData].
	>*/

	void RawData(
		__BufferSize size,
		const GLvoid* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const; /*<
	Uploads [^size] bytes from the location pointed to by [^data]
	to [^this] buffer taking [^usage] as a hint.
	See [glfunc NamedBufferData].
	>*/

	void Data(
		const __BufferData& data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const; /*<
	Uploads the specified [^data] to [^this] buffer
	taking [^usage] as a hint.
	See [glfunc NamedBufferData].
	>*/

	template <typename GLtype>
	void Data(
		__SizeType count,
		const GLtype* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	) const;

	void SubData(
		__BufferSize offset,
		const __BufferData& data
	) const; /*< Uploads the specified [^data] to a sub-range (starting at
	[^offset]) of [^this] buffer.
	See [glfunc NamedBufferSubData].
	>*/

	template <typename GLtype>
	static void SubData(
		__BufferSize offset,
		__SizeType count,
		const GLtype* data
	);

	static void CopySubData(
		__BufferName readbuffer,
		__BufferName writebuffer,
		__BufferSize readoffset,
		__BufferSize writeoffset,
		__BufferSize size
	); /*<
	Copies a block of data of the specified [^size] from [^readbuffer]
	starting at [^readoffset] to [^writebuffer] starting at [^writeoffset].
	See [glfunc CopyNamedBufferSubData].
	>*/

	template <typename GLtype>
	void ClearData(
		__PixelDataInternalFormat internal_format,
		__PixelDataFormat format,
		const GLtype* value
	) const; /*<
	Clears [^this] with the specified
	fixed [^value], in the specified [^format].
	See [glfunc ClearNamedBufferSubData].
	>*/

	template <typename GLtype>
	void ClearSubData(
		__BufferTarget target,
		__PixelDataInternalFormat internal_format,
		__BufferSize offset,
		__BufferSize size,
		__PixelDataFormat format,
		const GLtype* value
	) const; /*< Clears a sub-range (specified by [^offset] and [^size])
	of [^this] buffer with the specified fixed [^value],
	using the specified [^internal_format].
	>*/

	void Storage(
		const __BufferData& data,
		__Bitfield<__BufferStorageBit> flags
	) const; /*<
	Creates a data store for [^this] buffer
	and uploads the specified [^data].
	See [glfunc NamedBufferStorage].
	>*/
	void Storage(
		__BufferSize size,
		const void* data,
		__Bitfield<__BufferStorageBit> flags
	) const;
//]
//[oglplus_dsa_buffer_2

	__SizeType Size(void) const; /*<
	Returns the size of [^this] buffer.
	See [glfunc Get], [glconst BUFFER_SIZE].
	>*/
	__BufferUsage Usage(void) const; /*<
	Returns the usage hint of [^this] buffer.
	See [glfunc Get], [glconst BUFFER_USAGE].
	>*/
	__Bitfield<__BufferMapAccess> Access(void) const; /*<
	Returns the access bits of [^this] buffer.
	See [glfunc Get], [glconst BUFFER_ACCESS].
	>*/

#if GL_NV_shader_buffer_load
	void MakeResident(__AccessSpecifier access); /*<
	Makes [^this] accessible to GLSL shaders.
	See [glfunc MakeBufferResidentNV]
	>*/
	void MakeNonResident(void); /*<
	Makes [^this] buffer inaccessible to GLSL shaders.
	See [glfunc MakeBufferNonResidentNV]
	>*/
	__BufferGPUAddress GPUAddress(void) const; /*<
	Returns the GPU address of [^this] buffer.
	See [glfunc GetBufferParameter], [glconst BUFFER_GPU_ADDRESS_NV].
	>*/
#endif
};
//]
//[oglplus_dsa_buffer_def

typedef ObjectOps<__tag_DirectState, __tag_Buffer>
	DSABufferOps;

typedef __Object<DSABufferOps> DSABuffer;

//]
//[oglplus_dsa_buffer_sugar

struct DSABufferOpsAndUsage { }; /*<
Helper class for syntax sugar operators. Binds together the buffer name
and usage hint.
>*/

DSABufferOpsAndUsage operator << (
	__DSABufferOps& buffer,
	__BufferUsage usage
); /*<
Ties together a [^buffer] and [^usage] hint.
>*/


struct DSABufferOpsAndIdxTgt { }; /*<
Helper class for syntax sugar operators. Binds together a reference
to a buffer and an indexed target.
>*/

DSABufferOpsAndIdxTgt operator << (
	__DSABufferOps& buffer,
	__BufferIndexedTarget target
); /*<
Ties together a reference to a [^buffer] and an indexed [^target].
>*/

struct DSABufferOpsAndOffset { }; /*<
Helper class for syntax sugar operators. Binds together a buffer target
and offset value.
>*/

DSABufferOpsAndOffset operator + (
	__DSABufferOps& buffer,
	__BufferSize offset
); /*<
Ties together a [^buffer] and [^offset] value.
>*/

const DSABufferOps& operator << (
	__DSABufferOps& buffer,
	__BufferTarget target
); /*<
Equivalent to [^buf.Bind(target)].
>*/

DSABufferOps& operator << (
	const DSABufferOpsAndIdxTgt& bat,
	GLuint index
); /*<
Equivalent to [^buffer.BindBase(target, index)].
>*/

DSABufferOps& operator << (
	__DSABufferOps& buffer,
	const __BufferData& data
); /*<
Equivalent to [^buffer.Data(data)].
>*/

DSABufferOps& operator << (
	BufferTargetAndUsage&& tau,
	const __BufferData& data
); /*<
Equivalent to [^buffer.Data(data, usage)].
>*/

DSABufferOps& operator << (
	BufferTargetAndOffset&& tao,
	const __BufferData& data
); /*<
Equivalent to [^buffer.SubData(offset, data)].
>*/

#endif // GL_VERSION_4_5 || GL_ARB_direct_state_access

//]

