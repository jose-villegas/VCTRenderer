/**
 *  @file advanced/cloud_trace/resources.hpp
 *  @brief Declares resource-allocation-related classes and functions.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_TRACE_RESOURCES_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_TRACE_RESOURCES_1119071146_HPP

namespace oglplus {
namespace cloud_trace {

class ResourceAllocator
{
private:
	unsigned tex_unit;
	unsigned ub_index;
public:
	ResourceAllocator(void);

	unsigned GetNextTexUnit(void);
	unsigned GetNextUniformIndex(void);
};

} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
