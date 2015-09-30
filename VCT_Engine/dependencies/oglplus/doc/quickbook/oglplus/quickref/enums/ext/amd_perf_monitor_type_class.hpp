//  File doc/quickbook/oglplus/quickref/enums/ext/amd_perf_monitor_type_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oglplus/ext/amd_perf_monitor_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oglplus_enums_ext_amd_perf_monitor_type_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__PerfMonitorAMDType> class Transform>
class __EnumToClass<Base, __PerfMonitorAMDType, Transform> /*<
Specialization of __EnumToClass for the __PerfMonitorAMDType enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<PerfMonitorAMDType::UnsignedInt>
		UnsignedInt;
	Transform<PerfMonitorAMDType::Float>
		Float;
	Transform<PerfMonitorAMDType::UnsignedInt64>
		UnsignedInt64;
	Transform<PerfMonitorAMDType::Percentage>
		Percentage;
};

} // namespace enums
#endif
//]

