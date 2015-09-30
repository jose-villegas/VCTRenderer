/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_Group

template <typename ObjectName>
class Group;

template <typename ObjTag>
class Group<__ObjectName<__ObjTag>>
{
public:
	Group(const Group&); /*< [^Group] is copyable and movable. >*/
	Group(Group&&);

	Group(void); /*< Constructs an empty group >*/

#if !__OGLPLUS_NO_INITIALIZER_LISTS
	Group(std::initializer_list<__ObjectName<__ObjTag>> names) /*<
	Constructs the Group from an initializer list.
	>*/
#endif

	Group& Add(__ObjectName<__ObjTag> name); /*<
	Add a new name to this group.
	>*/

	__Sequence<__ObjectName<__ObjTag>> seq(void) const;

	operator __Sequence<__ObjectName<__ObjTag>> (void) const; /*<
	Implicit conversion to a sequence of object names, which can
	be passed as an argument to various functions.
	Note that the returned sequence must not be used after
	this group has been destroyed.
	>*/
};

template <typename ObjTag>
__Group<__ObjectName<__ObjTag>>
operator , (__ObjectName<__ObjTag> a, __ObjectName<__ObjTag> b); /*<
This operator creates a [^Group] containing two object names.
>*/

template <typename ObjTag>
__Group<__ObjectName<__ObjTag>>&&
operator , (__Group<__ObjectName<__ObjTag>>&&, __ObjectName<__ObjTag>); /*<
This operator appends an object name to a group.
>*/
//]

//[oglplus_object_StaticGroup

template <typename ObjName, size_t N>
class StaticGroup;

template <typename ObjTag, std::size_t N>
class StaticGroup<__ObjectName<__ObjTag>, N>
{
public:
	StaticGroup(const __ObjectName<__ObjTag> (&names)[N]); /*<
	Construction from an array of object names with known size.
	>*/

	template <typename ... Tag>
	StaticGroup(__ObjectName<Tag>... names); /*<
	Construction from a pack of object names. All names must refer to objects
	of the same type.
	>*/

	__Sequence<__ObjectName<__ObjTag>> seq(void) const;

	operator __Sequence<__ObjectName<__ObjTag>> (void) const; /*<
	Implicit conversion to a sequence of object names, which can
	be passed as an argument to various functions.
	Note that the returned sequence must not be used after
	this static group has been destroyed.
	>*/
};

template <typename ... ObjTag>
__StaticGroup<__ObjectName<__ObjTag>, sizeof...(ObjTag)>
MakeGroup(__ObjectName<__ObjTag> ... names); /*<
Creates a static group from a pack of object names. All names must refer to objects
of the same type.
>*/

//]

