/**
 *  @file oalplus/object/sequence.hpp
 *  @brief Sequence of Object names
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OALPLUS_OBJECT_SEQUENCE_1405011014_HPP
#define OALPLUS_OBJECT_SEQUENCE_1405011014_HPP

#include <oglplus/object/seq_tpl.hpp>

namespace oalplus {

using oglplus::SeqIterator;
using oglplus::Sequence;

/// Returns a pointer to array of names stored in a @p sequence
template <typename ObjName>
inline const ALuint*
GetALNames(const Sequence<ObjName>& sequence)
OALPLUS_NOEXCEPT(true)
{
	return GetNames(sequence);
}

} // namespace oalplus

#endif // include guard
