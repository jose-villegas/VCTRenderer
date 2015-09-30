//  File implement/oalplus/enums/context_attrib_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/context_attrib.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<ContextAttrib> class Transform>
class EnumToClass<Base, ContextAttrib, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined ALC_FREQUENCY
# if defined Frequency
#  pragma push_macro("Frequency")
#  undef Frequency
	Transform<ContextAttrib::Frequency> Frequency;
#  pragma pop_macro("Frequency")
# else
	Transform<ContextAttrib::Frequency> Frequency;
# endif
#endif
#if defined ALC_REFRESH
# if defined Refresh
#  pragma push_macro("Refresh")
#  undef Refresh
	Transform<ContextAttrib::Refresh> Refresh;
#  pragma pop_macro("Refresh")
# else
	Transform<ContextAttrib::Refresh> Refresh;
# endif
#endif
#if defined ALC_SYNC
# if defined Sync
#  pragma push_macro("Sync")
#  undef Sync
	Transform<ContextAttrib::Sync> Sync;
#  pragma pop_macro("Sync")
# else
	Transform<ContextAttrib::Sync> Sync;
# endif
#endif
#if defined ALC_MONO_SOURCES
# if defined MonoSources
#  pragma push_macro("MonoSources")
#  undef MonoSources
	Transform<ContextAttrib::MonoSources> MonoSources;
#  pragma pop_macro("MonoSources")
# else
	Transform<ContextAttrib::MonoSources> MonoSources;
# endif
#endif
#if defined ALC_STEREO_SOURCES
# if defined StereoSources
#  pragma push_macro("StereoSources")
#  undef StereoSources
	Transform<ContextAttrib::StereoSources> StereoSources;
#  pragma pop_macro("StereoSources")
# else
	Transform<ContextAttrib::StereoSources> StereoSources;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined ALC_FREQUENCY
# if defined Frequency
#  pragma push_macro("Frequency")
#  undef Frequency
	 , Frequency(_base())
#  pragma pop_macro("Frequency")
# else
	 , Frequency(_base())
# endif
#endif
#if defined ALC_REFRESH
# if defined Refresh
#  pragma push_macro("Refresh")
#  undef Refresh
	 , Refresh(_base())
#  pragma pop_macro("Refresh")
# else
	 , Refresh(_base())
# endif
#endif
#if defined ALC_SYNC
# if defined Sync
#  pragma push_macro("Sync")
#  undef Sync
	 , Sync(_base())
#  pragma pop_macro("Sync")
# else
	 , Sync(_base())
# endif
#endif
#if defined ALC_MONO_SOURCES
# if defined MonoSources
#  pragma push_macro("MonoSources")
#  undef MonoSources
	 , MonoSources(_base())
#  pragma pop_macro("MonoSources")
# else
	 , MonoSources(_base())
# endif
#endif
#if defined ALC_STEREO_SOURCES
# if defined StereoSources
#  pragma push_macro("StereoSources")
#  undef StereoSources
	 , StereoSources(_base())
#  pragma pop_macro("StereoSources")
# else
	 , StereoSources(_base())
# endif
#endif
	{ }
};

} // namespace enums

