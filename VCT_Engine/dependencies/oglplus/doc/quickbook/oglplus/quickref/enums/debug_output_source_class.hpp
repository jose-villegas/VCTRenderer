//  File doc/quickbook/oglplus/quickref/enums/debug_output_source_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/debug_output_source.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_debug_output_source_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__DebugOutputSource> class Transform>
class __EnumToClass<Base, __DebugOutputSource, Transform> /*<
Specialization of __EnumToClass for the __DebugOutputSource enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<DebugOutputSource::API>
		API;
	Transform<DebugOutputSource::WindowSystem>
		WindowSystem;
	Transform<DebugOutputSource::ShaderCompiler>
		ShaderCompiler;
	Transform<DebugOutputSource::ThirdParty>
		ThirdParty;
	Transform<DebugOutputSource::Application>
		Application;
	Transform<DebugOutputSource::Other>
		Other;
	Transform<DebugOutputSource::DontCare>
		DontCare;
};

} // namespace enums
#endif
//]

