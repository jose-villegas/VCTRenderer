//  File doc/quickbook/oglplus/quickref/enums/buffer_target_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/buffer_target.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_buffer_target_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__BufferTarget> class Transform>
class __EnumToClass<Base, __BufferTarget, Transform> /*<
Specialization of __EnumToClass for the __BufferTarget enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<BufferTarget::Array>
		Array;
	Transform<BufferTarget::AtomicCounter>
		AtomicCounter;
	Transform<BufferTarget::CopyRead>
		CopyRead;
	Transform<BufferTarget::CopyWrite>
		CopyWrite;
	Transform<BufferTarget::DispatchIndirect>
		DispatchIndirect;
	Transform<BufferTarget::DrawIndirect>
		DrawIndirect;
	Transform<BufferTarget::ElementArray>
		ElementArray;
	Transform<BufferTarget::PixelPack>
		PixelPack;
	Transform<BufferTarget::PixelUnpack>
		PixelUnpack;
	Transform<BufferTarget::ShaderStorage>
		ShaderStorage;
	Transform<BufferTarget::Texture>
		Texture;
	Transform<BufferTarget::TransformFeedback>
		TransformFeedback;
	Transform<BufferTarget::Uniform>
		Uniform;
	Transform<BufferTarget::Query>
		Query;
	Transform<BufferTarget::Parameter>
		Parameter;
	Transform<BufferTarget::ExternalVirtualMemory>
		ExternalVirtualMemory;
};

} // namespace enums
#endif
//]

