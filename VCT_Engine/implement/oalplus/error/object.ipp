/**
 *  @file oalplus/error/object.ipp
 *  @brief Implementation of ObjectError
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oalplus {

OALPLUS_LIB_FUNC
ObjectError::ObjectError(const char* message)
 : ErrorAL(message)
#if !OALPLUS_ERROR_NO_OBJECT_TYPE
 , _obj_type(0)
#endif
 , _obj_typeid(0)
 , _obj_name(0)
{ }

OALPLUS_LIB_FUNC
ALenum ObjectError::ObjectType(void) const
{
#if !OALPLUS_ERROR_NO_OBJECT_TYPE
	return _obj_type;
#else
	return ALenum(0);
#endif
}

OALPLUS_LIB_FUNC
ALint ObjectError::ObjectName(void) const
{
	return ALint(_obj_name);
}

OALPLUS_LIB_FUNC
const String& ObjectError::ObjectDesc(void) const
{
	return aux::ObjectDescRegistry::_get_desc(
		_obj_typeid,
		_obj_name
	);
}

} // namespace oalplus

