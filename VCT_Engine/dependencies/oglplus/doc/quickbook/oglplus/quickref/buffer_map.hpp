/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_raw_map

class BufferRawMap
{
public:
	BufferRawMap(
		__BufferTarget target,
		__BufferSize offset,
		__BufferSize size,
		__Bitfield<__BufferMapAccess> access
	); /*<
	Maps a range (specified by [^offset] and [^size]) of a buffer
	currently bound to [^target] with the specified [^access]
	to the client address space.
	See [glfunc MapBufferRange].
	>*/

	BufferRawMap(
		__BufferTarget target,
		__Bitfield<__BufferMapAccess> access
	); /*<
	Maps the whole buffer currently bound to the specified [^target]
	with the specified [^access] to the client address space.
	See [glfunc MapBuffer].
	>*/

	BufferRawMap(const BufferRawMap&) = delete; /*<
	Buffer maps are not copyable.
	>*/
	BufferRawMap(BufferRawMap&&); /*<
	Buffer maps are moveable.
	>*/

	~BufferRawMap(void); /*<
	Unmaps the buffer from client address space (if mapped).
	See [glfunc UnmapBuffer].
	>*/
	void Unmap(void);

	bool Mapped(void) const; /*<
	Returns true if the buffer is mapped.
	>*/
	GLsizeiptr Size(void) const; /*<
	Returns the size (in bytes) of the mapped buffer.
	>*/

	const GLvoid* RawData(void) const; /*<
	Returns a raw pointer to the mapped data.
	Note that the buffer has to be mapped or the result is undefined.
	>*/
	GLvoid* RawData(void);

	void FlushRange(__BufferSize offset, __BufferSize length); /*<
	Indicate modifications to a mapped buffer range.
	Note that the buffer has to be mapped or the result is undefined.
	See [glfunc FlushMappedBufferRange].
	>*/
};
//]
//[oglplus_buffer_typed_map

template <typename Type>
class BufferTypedMap
 : public __BufferRawMap
{
public:
	BufferTypedMap(
		__BufferTarget target,
		__BufferTypedSize<Type> offset,
		__BufferTypedSize<Type> size,
		__Bitfield<__BufferMapAccess> access
	) /*<
	Maps a range (specified by [^offset] and [^size]) of a buffer
	currently bound to [^target] with the specified [^access]
	to the client address space.
	See [glfunc MapBufferRange].
	>*/

	BufferTypedMap(
		__BufferTarget target,
		__Bitfield<__BufferMapAccess> access
	); /*<
	Maps the whole buffer currently bound to the specified [^target]
	with the specified [^access] to the client address space.
	See [glfunc MapBuffer].
	>*/

	GLsizeiptr Count(void) const; /*<
	Returns the count of elements of [^Type] in the mapped buffer.
	>*/

	const Type* Data(void) const; /*<
	Returns a typed pointer to the mapped data.
	Note that the buffer has to be mapped or the result is undefined.
	>*/
	Type* Data(void);

	const Type& At(GLuint index) const; /*<
	Returns a reference to the element at the specified [^index].
	Note that the buffer has to be mapped or the result is undefined.
	>*/
	Type& At(GLuint index);

	void FlushElements(
		__BufferTypedSize<Type> start,
		__BufferTypedSize<Type> count
	); /*<
	Indicate modifications to a mapped range of elements of [^Type].
	Note that the buffer has to be mapped or the result is undefined.
	See [glfunc FlushMappedBufferRange].
	>*/
};

//]

