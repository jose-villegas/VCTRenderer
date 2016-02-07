//  File implement/oalplus/enums/data_format_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/data_format.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<DataFormat> class Transform>
class EnumToClass<Base, DataFormat, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_FORMAT_MONO8
# if defined Mono8
#  pragma push_macro("Mono8")
#  undef Mono8
	Transform<DataFormat::Mono8> Mono8;
#  pragma pop_macro("Mono8")
# else
	Transform<DataFormat::Mono8> Mono8;
# endif
#endif
#if defined AL_FORMAT_MONO16
# if defined Mono16
#  pragma push_macro("Mono16")
#  undef Mono16
	Transform<DataFormat::Mono16> Mono16;
#  pragma pop_macro("Mono16")
# else
	Transform<DataFormat::Mono16> Mono16;
# endif
#endif
#if defined AL_FORMAT_STEREO8
# if defined Stereo8
#  pragma push_macro("Stereo8")
#  undef Stereo8
	Transform<DataFormat::Stereo8> Stereo8;
#  pragma pop_macro("Stereo8")
# else
	Transform<DataFormat::Stereo8> Stereo8;
# endif
#endif
#if defined AL_FORMAT_STEREO16
# if defined Stereo16
#  pragma push_macro("Stereo16")
#  undef Stereo16
	Transform<DataFormat::Stereo16> Stereo16;
#  pragma pop_macro("Stereo16")
# else
	Transform<DataFormat::Stereo16> Stereo16;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_FORMAT_MONO8
# if defined Mono8
#  pragma push_macro("Mono8")
#  undef Mono8
	 , Mono8(_base())
#  pragma pop_macro("Mono8")
# else
	 , Mono8(_base())
# endif
#endif
#if defined AL_FORMAT_MONO16
# if defined Mono16
#  pragma push_macro("Mono16")
#  undef Mono16
	 , Mono16(_base())
#  pragma pop_macro("Mono16")
# else
	 , Mono16(_base())
# endif
#endif
#if defined AL_FORMAT_STEREO8
# if defined Stereo8
#  pragma push_macro("Stereo8")
#  undef Stereo8
	 , Stereo8(_base())
#  pragma pop_macro("Stereo8")
# else
	 , Stereo8(_base())
# endif
#endif
#if defined AL_FORMAT_STEREO16
# if defined Stereo16
#  pragma push_macro("Stereo16")
#  undef Stereo16
	 , Stereo16(_base())
#  pragma pop_macro("Stereo16")
# else
	 , Stereo16(_base())
# endif
#endif
	{ }
};

} // namespace enums

