/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_output_data

class OutputData
{
public:
	typedef __OneOf<__DataType, __PixelDataType> PixDataType;

	OutputData(BufferSize size, GLvoid* addr); /*<
	Construction from [^size] in bytes and pointer to [^addr].
	>*/

	OutputData(PixDataType type, __BufferSize size, GLvoid* addr); /*<
	Construction from [^type], [^size] in bytes and pointer to [^addr].
	>*/

	template <typename T>
	OutputData(__SizeType count, T* addr); /*<
	Construction from [^count] of instances of type [^T] located at [^addr].
	>*/

	template <typename T, std::size_t N>
	OutputData(T (&addr)[N]); /*<
	Construction from an array with known size.
	>*/

	template <typename T, std::size_t N>
	OutputData(std::array<T, N>& a); /*<
	Construction from a std::array.
	>*/

	template <typename T>
	OutputData(std::vector<T>& v); /*<
	Construction from a std::vector.
	>*/

	PixDataType Type(void) const; /*<
	The pixel data type.
	>*/

	BigSizeType Size(void) const; /*<
	Geturns size in bytes.
	>*/

	GLvoid* Addr(void) const; /*<
	Returns the location of the data.
	>*/
};

//]

