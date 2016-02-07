//  File implement/oalplus/enums/source_state_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/source_state.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<SourceState> class Transform>
class EnumToClass<Base, SourceState, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_INITIAL
# if defined Initial
#  pragma push_macro("Initial")
#  undef Initial
	Transform<SourceState::Initial> Initial;
#  pragma pop_macro("Initial")
# else
	Transform<SourceState::Initial> Initial;
# endif
#endif
#if defined AL_PLAYING
# if defined Playing
#  pragma push_macro("Playing")
#  undef Playing
	Transform<SourceState::Playing> Playing;
#  pragma pop_macro("Playing")
# else
	Transform<SourceState::Playing> Playing;
# endif
#endif
#if defined AL_PAUSED
# if defined Paused
#  pragma push_macro("Paused")
#  undef Paused
	Transform<SourceState::Paused> Paused;
#  pragma pop_macro("Paused")
# else
	Transform<SourceState::Paused> Paused;
# endif
#endif
#if defined AL_STOPPED
# if defined Stopped
#  pragma push_macro("Stopped")
#  undef Stopped
	Transform<SourceState::Stopped> Stopped;
#  pragma pop_macro("Stopped")
# else
	Transform<SourceState::Stopped> Stopped;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_INITIAL
# if defined Initial
#  pragma push_macro("Initial")
#  undef Initial
	 , Initial(_base())
#  pragma pop_macro("Initial")
# else
	 , Initial(_base())
# endif
#endif
#if defined AL_PLAYING
# if defined Playing
#  pragma push_macro("Playing")
#  undef Playing
	 , Playing(_base())
#  pragma pop_macro("Playing")
# else
	 , Playing(_base())
# endif
#endif
#if defined AL_PAUSED
# if defined Paused
#  pragma push_macro("Paused")
#  undef Paused
	 , Paused(_base())
#  pragma pop_macro("Paused")
# else
	 , Paused(_base())
# endif
#endif
#if defined AL_STOPPED
# if defined Stopped
#  pragma push_macro("Stopped")
#  undef Stopped
	 , Stopped(_base())
#  pragma pop_macro("Stopped")
# else
	 , Stopped(_base())
# endif
#endif
	{ }
};

} // namespace enums

