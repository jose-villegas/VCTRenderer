/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_bitfield

template <typename Bits>
class Bitfield
{
public:
	Bitfield(void); /*<
	Constructs an empty (zero) bitfield.
	>*/
	Bitfield(Bits _bit); /*<
	Construct a bitfield from a single value of [^Bits].
	>*/
	Bitfield(Bits _bit_a, Bits _bit_b); /*<
	Construct a bitfield from a pair of [^Bits].
	>*/

#if !__OGLPLUS_NO_INITIALIZER_LISTS
	Bitfield(const std::initializer_list<Bits>& bits); /*<
	Construct a bitfield from an initializer list of [^Bits].
	>*/
#endif
	template <typename Iter>
	Bitfield(Iter pos, Iter end); /*<
	Construction from a pair of iterators through [^Bits].
	>*/

	friend Bitfield operator | (Bitfield bf, Bits b); /*<
	Bitwise-or operator for combining [^Bits] into a bitfield.
	>*/
	Bitfield& operator |= (Bits b); /*<
	Bitwise-or operator for combining [^Bits] into a bitfield.
	>*/

	bool Test(Bits b) const; /*<
	These functions test if a specified bit is set.
	>*/
	bool Has(Bits b) const;
};

template <typename Bits>
__Bitfield<Bits> operator | (Bits b1, Bits b2);

//]

