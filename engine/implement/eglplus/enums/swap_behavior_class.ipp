//  File implement/eglplus/enums/swap_behavior_class.ipp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/eglplus/swap_behavior.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
namespace enums {
template <typename Base, template<SwapBehavior> class Transform>
class EnumToClass<Base, SwapBehavior, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined EGL_BUFFER_PRESERVED
# if defined BufferPreserved
#  pragma push_macro("BufferPreserved")
#  undef BufferPreserved
	Transform<SwapBehavior::BufferPreserved> BufferPreserved;
#  pragma pop_macro("BufferPreserved")
# else
	Transform<SwapBehavior::BufferPreserved> BufferPreserved;
# endif
#endif
#if defined EGL_BUFFER_DESTROYED
# if defined BufferDestroyed
#  pragma push_macro("BufferDestroyed")
#  undef BufferDestroyed
	Transform<SwapBehavior::BufferDestroyed> BufferDestroyed;
#  pragma pop_macro("BufferDestroyed")
# else
	Transform<SwapBehavior::BufferDestroyed> BufferDestroyed;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined EGL_BUFFER_PRESERVED
# if defined BufferPreserved
#  pragma push_macro("BufferPreserved")
#  undef BufferPreserved
	 , BufferPreserved(_base())
#  pragma pop_macro("BufferPreserved")
# else
	 , BufferPreserved(_base())
# endif
#endif
#if defined EGL_BUFFER_DESTROYED
# if defined BufferDestroyed
#  pragma push_macro("BufferDestroyed")
#  undef BufferDestroyed
	 , BufferDestroyed(_base())
#  pragma pop_macro("BufferDestroyed")
# else
	 , BufferDestroyed(_base())
# endif
#endif
	{ }
};

} // namespace enums

