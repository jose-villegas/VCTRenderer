/**
 *  .file lib/oalplus/enums.cpp
 *  .brief Enumeration-related functions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "prologue.ipp"
#include <oalplus/string.hpp>
#include "implement.ipp"

#include <oalplus/detail/base_range.hpp>
#include <oalplus/detail/enum_class.hpp>

namespace oalplus {
#include <oalplus/lib/enum_value_name.ipp>
#include <oalplus/lib/enum_value_range.ipp>
} // namespace oalplus

#include "epilogue.ipp"
