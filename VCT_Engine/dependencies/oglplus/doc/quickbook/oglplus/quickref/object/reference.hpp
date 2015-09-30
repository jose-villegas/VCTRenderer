/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_Reference

template <typename __Object>
class Reference
 : public __ObjectOps<__OpsTag, __ObjTag>
{
public:
	Reference(__ObjectName<__ObjTag> name);
};

//]

