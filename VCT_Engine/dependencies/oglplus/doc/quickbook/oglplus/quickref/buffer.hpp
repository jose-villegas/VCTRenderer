/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_common_1

template <>
class __ObjCommonOps<__tag_Buffer>
 : public __BufferName
{
public:
	typedef __BufferTarget Target; /*<
	Buffer bind targets.
	>*/
	typedef __BufferIndexedTarget IndexedTarget;

	static __BufferName Binding(__BufferTarget target); /*<
	Returns the buffer currently bound to the specified [^target].
	>*/
	static void Bind(__BufferTarget target, __BufferName buffer); /*<
	Binds the specified [^buffer] to the specified [^target].
	>*/

	static __BufferName Binding(__BufferIndexedTarget target, GLuint index); /*<
	Returns the current buffer bound to the specified [^index]ed [^target].
	>*/
	static void BindBase(
		__BufferIndexedTarget target,
		GLuint index,
		__BufferName buffer
	); /*<
	Binds the specified [^buffer] to the specified [^index]ed [^target].
	>*/
	static void BindRange(
		__BufferIndexedTarget target,
		GLuint index,
		__BufferName buffer,
		__BufferSize offset,
		__BufferSize size
	); /*<
	Bind a range starting at [^offset] with length specified by [^size]
	in a [^buffer] to the specified [^index]ed [^target].
	>*/

#if GL_VERSION_4_4 || GL_ARB_multi_bind
	static void BindBase(
		__BufferIndexedTarget target,
		GLuint first,
		const __Sequence<__BufferName>& buffers
	); /*<
	Sequentially binds all [^buffers] to indexed [^target]s starting at
	[^first] index.
	>*/

	static void BindRange(
		__BufferIndexedTarget target,
		GLuint first,
		const __Sequence<__BufferName>& buffers,
		const GLintptr* offsets,
		const GLsizeiptr* sizes
	); /*<
	Sequentially binds ranges (specified by [^offsets] and [^sizes] which must
	point to arrays of at least as much values as the [^buffers] sequence)
	of all [^buffers] to indexed [^target]s starting at [^first] index.
	>*/
#endif
//]
//[oglplus_buffer_common_2
	void Bind(__BufferTarget target) const; /*<
	Binds [^this] buffer to the specified [^target].
	>*/
	void Bind(__BufferIndexedTarget target, GLuint index) const; /*<
	Binds [^this] buffer to the specified indexed [^target].
	>*/
	void BindBase(__BufferIndexedTarget target, GLuint index) const;

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
	void BindBaseUniform(__UniformBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the uniform buffer binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_0 || GL_ARB_transform_feedback3
	void BindBaseTransformFeedback(__TransformFeedbackBufferBindingPoint index) const /*<
	Binds [^this] buffer to the transform feedback binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_2 || GL_ARB_shader_atomic_counters
	void BindBaseAtomicCounter(__AtomicCounterBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the atomic counter binding point with
	the specified [^index].
	>*/
#endif

#if GL_VERSION_4_3 || GL_ARB_shader_storage_buffer_object
	void BindBaseShaderStorage(__ShaderStorageBufferBindingPoint index) const; /*<
	Binds [^this] buffer to the shader storage binding point with
	the specified [^index].
	>*/
#endif

	void BindRange(
		__BufferIndexedTarget target,
		GLuint index,
		__BufferSize offset,
		__BufferSize size
	) const; /*<
	Binds a range (specified by [^offset] and [^size]) of [^this] buffer
	to the specified [^index]ed [^target].
	>*/
//]
//[oglplus_buffer_common_3
#if GL_VERSION_4_3 || GL_ARB_invalidate_subdata
	void InvalidateData(void); /*<
	Invalidate all of [^this] buffer's data.
	See [glfunc InvalidateBufferData].
	>*/
	void InvalidateSubData(__BufferSize offset, __BufferSize size); /*<
	Invalidate a subrange (specified by [^offset] and [^size])
	of [^this] buffer's data.
	See [glfunc InvalidateBufferSubData].
	>*/
#endif
};
//]
//[oglplus_buffer_1
template <>
class __ObjectOps<__tag_ExplicitSel, __tag_Buffer>
 : public __ObjZeroOps<__tag_ExplicitSel, __tag_Buffer> /*<
Indirectly inherits from __ObjCommonOps_Buffer<__tag_Buffer>.
>*/
{
public:
	struct Property
	{
		typedef __BufferUsage Usage;
		typedef __BufferMapAccess MapAccess;
	};

	typedef __BufferTypedMap<GLubyte> Map; /*<
	Mapping of the buffer to the client address space.
	>*/

	static __Boolean Mapped(__BufferTarget target); /*<
	Returns true if the buffer currently bound to the specified
	[^target] is mapped.
	See [glfunc GetBufferParameter], [glconst BUFFER_MAPPED].
	>*/

	static void Resize(
		__BufferTarget target,
		__BufferSize size,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Allocates or reallocates storage of a buffer currently bound
	to [^target] to the specified [^size] without uploading any data,
	taking [^usage] as a hint.
	See [glfunc BufferData].
	>*/

	static void RawData(
		__BufferTarget target,
		__BufferSize size,
		const GLvoid* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Uploads [^size] bytes from the location pointed to by [^data]
	to the buffer bound to the specified [^target], taking [^usage]
	as a hint.
	See [glfunc BufferData].
	>*/

	static void Data(
		__BufferTarget target,
		const __BufferData& data,
		__BufferUsage usage = BufferUsage::StaticDraw
	); /*<
	Uploads the specified [^data] to the buffer bound to the specified
	[^target] taking [^usage] as a hint.
	See [glfunc BufferData].
	>*/

	template <typename GLtype>
	static void Data(
		__BufferTarget target,
		__SizeType count,
		const GLtype* data,
		__BufferUsage usage = BufferUsage::StaticDraw
	);

	static void SubData(
		__BufferTarget target,
		__BufferSize offset,
		const __BufferData& data
	); /*< Uploads the specified [^data] to a sub-range (starting at
	[^offset]) of the buffer bound to the specified [^target].
	See [glfunc BufferSubData].
	>*/

	template <typename GLtype>
	static void SubData(
		__BufferTarget target,
		__BufferSize offset,
		__SizeType count,
		const GLtype* data
	);

#if GL_VERSION_3_1 || GL_ARB_copy_buffer
	static void CopySubData(
		__BufferTarget readtarget,
		__BufferTarget writetarget,
		__BufferSize readoffset,
		__BufferSize writeoffset,
		__BufferSize size
	); /*<
	Copies a block of data of the specified [^size] from a buffer
	bound to [^readtarget] starting at [^readoffset] to a buffer
	bound to [^writetarget] starting at [^writeoffset].
	See [glfunc CopyBufferSubData].
	>*/
#endif

#if GL_VERSION_4_3
	template <typename GLtype>
	static void ClearData(
		__BufferTarget target,
		__PixelDataInternalFormat internal_format,
		__PixelDataFormat format,
		const GLtype* value
	); /*<
	Clears the whole buffer bound to [^target] with the specified
	fixed [^value], in the specified [^format].
	See [glfunc ClearBufferSubData].
	>*/

	template <typename GLtype>
	static void ClearSubData(
		__BufferTarget target,
		__PixelDataInternalFormat internal_format,
		__BufferSize offset,
		__BufferSize size,
		__PixelDataFormat format,
		const GLtype* value
	); /*< Clears a sub-range (specified by [^offset] and [^size])
	of a buffer bound to [^target] with the specified fixed [^value],
	using the specified [^internal_format].
	>*/
#endif

#if GL_VERSION_4_4 || GL_ARB_buffer_storage
	static void Storage(
		__BufferTarget target,
		const __BufferData& data,
		__Bitfield<__BufferStorageBit> flags
	); /*<
	Creates a data store for buffer currently bound to
	[^target], and uploads the specified [^data].
	See [glfunc BufferStorage].
	>*/
	static void Storage(
		__BufferTarget target,
		__BufferSize size,
		const void* data,
		__Bitfield<__BufferStorageBit> flags
	);

	static __Boolean ImmutableStorage(__BufferTarget target); /*<
	Returns true if the storage of the buffer currently bound
	to [^target] is immutable.
	See [glfunc GetBufferParameter], [glconst BUFFER_IMMUTABLE_STORAGE].
	>*/

	static __Bitfield<__BufferStorageBit> StorageFlags(__BufferTarget target); /*<
	Returns the storage flags of a buffer currently bound to [^target].
	See [glfunc GetBufferParameter], [glconst BUFFER_STORAGE_FLAGS].
	>*/
#endif
//]
//[oglplus_buffer_2

	static __SizeType Size(__BufferTarget target); /*<
	Returns the size of a buffer currently bound to the specified [^target].
	See [glfunc Get], [glconst BUFFER_SIZE].
	>*/
	static __BufferUsage Usage(__BufferTarget target); /*<
	Returns the usage hint of a buffer currently bound to the specified [^target].
	See [glfunc Get], [glconst BUFFER_USAGE].
	>*/
	static __Bitfield<__BufferMapAccess> Access(__BufferTarget target); /*<
	Returns the access bits of a buffer currently bound to the specified [^target].
	See [glfunc Get], [glconst BUFFER_ACCESS].
	>*/

#if GL_ARB_sparse_buffer
	static void PageCommitment(
		__BufferTarget target,
		__BufferSize offset,
		__BufferSize size,
		__Boolean commit
	); /*<
	Commits and de-commits regions (specified by [^offset] and [^size])
	of sparse buffer currently bound to the specified [^target].
	See [glfunc BufferPageCommitmentARB].
	>*/

	static __SizeType PageSize(void); /*<
	Returns the implementation-dependent sparse buffer storage page size.
	See [glfunc Get], [glconst SPARSE_BUFFER_PAGE_SIZE_ARB].
	>*/
#endif

#if GL_NV_shader_buffer_load
	static void MakeResident(__BufferTarget target, __AccessSpecifier access); /*<
	Makes buffer currently bound to [^target] accessible to GLSL shaders.
	See [glfunc MakeBufferResidentNV]
	>*/
	static void MakeNonResident(__BufferTarget target); /*<
	Makes buffer currently bound to [^target] inaccessible to GLSL shaders.
	See [glfunc MakeBufferNonResidentNV]
	>*/
	static __BufferGPUAddress GPUAddress(__BufferTarget target); /*<
	Returns the GPU address of the buffer currently bound to [^target].
	See [glfunc GetBufferParameter], [glconst BUFFER_GPU_ADDRESS_NV].
	>*/
#endif
};
//]
//[oglplus_buffer_def

typedef ObjectOps<__tag_ExplicitSel, __tag_Buffer>
	BufferOps;

typedef __Object<BufferOps> Buffer;

typedef __ObjectZero<__ObjZeroOps<__tag_ExplicitSel, __tag_Buffer>>
	NoBuffer;
//]
//[oglplus_buffer_sugar

struct BufferTargetAndUsage { }; /*<
Helper class for syntax sugar operators. Binds together the buffer target
and usage hint.
>*/

BufferTargetAndUsage operator << (
	__BufferTarget target,
	__BufferUsage usage
); /*<
Ties together a buffer [^target] and [^usage] hint.
>*/


struct BufferOpsAndIdxTgt { }; /*<
Helper class for syntax sugar operators. Binds together a reference
to a buffer and an indexed target.
>*/

BufferOpsAndIdxTgt operator << (
	const BufferOps& buffer,
	__BufferIndexedTarget target
); /*<
Ties together a reference to a [^buffer] and an indexed [^target].
>*/


struct BufferTargetAndOffset { }; /*<
Helper class for syntax sugar operators. Binds together a buffer target
and offset value.
>*/

BufferTargetAndOffset operator + (
	__BufferTarget target,
	__BufferSize offset
); /*<
Ties together a buffer target and offset value.
>*/

// Bind
__BufferTarget operator << (
	const BufferOps& buf,
	__BufferTarget target
); /*<
Equivalent to [^buf.Bind(target)].
>*/

const BufferOps& operator << (
	const BufferOpsAndIdxTgt& bat,
	GLuint index
); /*<
Equivalent to [^BufferOps::BindBase(target, index)].
>*/

__BufferTarget operator << (
	__BufferTarget target,
	const __BufferData& data
); /*<
Equivalent to [^BufferOps::Data(target, data)].
>*/

BufferTarget operator << (
	BufferTargetAndUsage&& tau,
	const BufferData& data
); /*<
Equivalent to [^BufferOps::Data(target, data, usage)].
>*/

__BufferTarget operator << (
	BufferTargetAndOffset&& tao,
	const __BufferData& data
); /*<
Equivalent to [^BufferOps::SubData(target, offset, data)].
>*/

//]

