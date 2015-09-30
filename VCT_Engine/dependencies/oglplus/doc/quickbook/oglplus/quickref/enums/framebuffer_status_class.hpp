//  File doc/quickbook/oglplus/quickref/enums/framebuffer_status_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/framebuffer_status.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_framebuffer_status_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__FramebufferStatus> class Transform>
class __EnumToClass<Base, __FramebufferStatus, Transform> /*<
Specialization of __EnumToClass for the __FramebufferStatus enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<FramebufferStatus::Complete>
		Complete;
	Transform<FramebufferStatus::Undefined>
		Undefined;
	Transform<FramebufferStatus::IncompleteAttachment>
		IncompleteAttachment;
	Transform<FramebufferStatus::IncompleteMissingAttachment>
		IncompleteMissingAttachment;
	Transform<FramebufferStatus::Unsupported>
		Unsupported;
	Transform<FramebufferStatus::IncompleteMultisample>
		IncompleteMultisample;
	Transform<FramebufferStatus::IncompleteLayerTargets>
		IncompleteLayerTargets;
};

} // namespace enums
#endif
//]

