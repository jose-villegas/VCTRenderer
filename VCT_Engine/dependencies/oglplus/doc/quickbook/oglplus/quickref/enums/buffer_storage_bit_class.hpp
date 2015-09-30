//  File doc/quickbook/oglplus/quickref/enums/buffer_storage_bit_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_storage_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_storage_bit_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BufferStorageBit> class Transform>
class __EnumToClass<Base, __BufferStorageBit, Transform> /*<
Specialization of __EnumToClass for the __BufferStorageBit enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BufferStorageBit::MapRead>
		MapRead;
	Transform<BufferStorageBit::MapWrite>
		MapWrite;
	Transform<BufferStorageBit::MapPersistent>
		MapPersistent;
	Transform<BufferStorageBit::MapCoherent>
		MapCoherent;
	Transform<BufferStorageBit::DynamicStorage>
		DynamicStorage;
	Transform<BufferStorageBit::ClientStorage>
		ClientStorage;
	Transform<BufferStorageBit::SparseStorage>
		SparseStorage;
};

} // namespace enums
#endif
//]

