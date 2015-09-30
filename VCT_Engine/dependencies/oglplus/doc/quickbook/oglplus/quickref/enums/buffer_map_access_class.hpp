//  File doc/quickbook/oglplus/quickref/enums/buffer_map_access_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_map_access.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_map_access_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BufferMapAccess> class Transform>
class __EnumToClass<Base, __BufferMapAccess, Transform> /*<
Specialization of __EnumToClass for the __BufferMapAccess enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BufferMapAccess::Read>
		Read;
	Transform<BufferMapAccess::Write>
		Write;
	Transform<BufferMapAccess::Persistent>
		Persistent;
	Transform<BufferMapAccess::Coherent>
		Coherent;
	Transform<BufferMapAccess::InvalidateRange>
		InvalidateRange;
	Transform<BufferMapAccess::InvalidateBuffer>
		InvalidateBuffer;
	Transform<BufferMapAccess::FlushExplicit>
		FlushExplicit;
	Transform<BufferMapAccess::Unsynchronized>
		Unsynchronized;
};

} // namespace enums
#endif
//]

