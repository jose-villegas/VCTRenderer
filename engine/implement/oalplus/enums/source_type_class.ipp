//  File implement/oalplus/enums/source_type_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_type.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<SourceType> class Transform>
class EnumToClass<Base, SourceType, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_UNDETERMINED
# if defined Undetermined
#  pragma push_macro("Undetermined")
#  undef Undetermined
	Transform<SourceType::Undetermined> Undetermined;
#  pragma pop_macro("Undetermined")
# else
	Transform<SourceType::Undetermined> Undetermined;
# endif
#endif
#if defined AL_STATIC
# if defined Static
#  pragma push_macro("Static")
#  undef Static
	Transform<SourceType::Static> Static;
#  pragma pop_macro("Static")
# else
	Transform<SourceType::Static> Static;
# endif
#endif
#if defined AL_STREAMING
# if defined Streaming
#  pragma push_macro("Streaming")
#  undef Streaming
	Transform<SourceType::Streaming> Streaming;
#  pragma pop_macro("Streaming")
# else
	Transform<SourceType::Streaming> Streaming;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_UNDETERMINED
# if defined Undetermined
#  pragma push_macro("Undetermined")
#  undef Undetermined
	 , Undetermined(_base())
#  pragma pop_macro("Undetermined")
# else
	 , Undetermined(_base())
# endif
#endif
#if defined AL_STATIC
# if defined Static
#  pragma push_macro("Static")
#  undef Static
	 , Static(_base())
#  pragma pop_macro("Static")
# else
	 , Static(_base())
# endif
#endif
#if defined AL_STREAMING
# if defined Streaming
#  pragma push_macro("Streaming")
#  undef Streaming
	 , Streaming(_base())
#  pragma pop_macro("Streaming")
# else
	 , Streaming(_base())
# endif
#endif
	{ }
};

} // namespace enums

