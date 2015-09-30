/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_buffer_data

class BufferData
{
public:
	BufferData(__BufferSize size, const GLvoid* data); /*<
	Constructs [^BufferData] from a raw pointer to a [^data] block
	and its [^size].
	>*/

	/// Construction from @p count of instances of type @c T at @p data
	template <typename T>
	BufferData(__SizeType count, const T* data); /*<
	Constructs [^BufferData] from a pointer to [^count] instances
	of type [^T].
	>*/

	template <typename T, std::size_t N>
	BufferData(const T (&data)[N]); /*<
	Constructs [^BufferData] from an array of [^N] elements of type [^T].
	>*/

	template <typename T, std::size_t N>
	BufferData(const std::array<T, N>& array); /*<
	Constructs [^BufferData] from an [^array].
	>*/

	template <typename T>
	BufferData(const std::vector<T>& vector); /*<
	Constructs [^BufferData] from a [^vector].
	>*/
};

//]

