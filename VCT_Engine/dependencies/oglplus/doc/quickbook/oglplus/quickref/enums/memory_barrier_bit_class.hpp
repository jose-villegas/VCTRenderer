//  File doc/quickbook/oglplus/quickref/enums/memory_barrier_bit_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/memory_barrier_bit.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_memory_barrier_bit_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__MemoryBarrierBit> class Transform>
class __EnumToClass<Base, __MemoryBarrierBit, Transform> /*<
Specialization of __EnumToClass for the __MemoryBarrierBit enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<MemoryBarrierBit::VertexAttribArray>
		VertexAttribArray;
	Transform<MemoryBarrierBit::ElementArray>
		ElementArray;
	Transform<MemoryBarrierBit::Uniform>
		Uniform;
	Transform<MemoryBarrierBit::TextureFetch>
		TextureFetch;
	Transform<MemoryBarrierBit::ShaderImageAccess>
		ShaderImageAccess;
	Transform<MemoryBarrierBit::Command>
		Command;
	Transform<MemoryBarrierBit::PixelBuffer>
		PixelBuffer;
	Transform<MemoryBarrierBit::TextureUpdate>
		TextureUpdate;
	Transform<MemoryBarrierBit::BufferUpdate>
		BufferUpdate;
	Transform<MemoryBarrierBit::Framebuffer>
		Framebuffer;
	Transform<MemoryBarrierBit::TransformFeedback>
		TransformFeedback;
	Transform<MemoryBarrierBit::AtomicCounter>
		AtomicCounter;
	Transform<MemoryBarrierBit::ShaderStorage>
		ShaderStorage;
	Transform<MemoryBarrierBit::ClientMappedBuffer>
		ClientMappedBuffer;
	Transform<MemoryBarrierBit::All>
		All;
};

} // namespace enums
#endif
//]

