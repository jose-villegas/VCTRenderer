/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_BoundObjOps
template <typename __ObjTag>
class BoundObjOps
{
private:
	typedef  ExplicitOps;
public:
	typedef __ObjectOps<__tag_ExplicitSel, __ObjTag>::Target Target;
	Target target;

	BoundObjOps(void);

	BoundObjOps(Target init_tgt);
};
//]

//[oglplus_object_ObjectOps_CurrentBound
template <typename __ObjTag>
class ObjectOps<__tag_CurrentBound, ObjTag>
 : public __ObjZeroOps<__tag_CurrentBound, ObjTag>
 , public __BoundObjOps<ObjTag>
{
public:
	typedef typename __BoundObjOps<__ObjTag>::Target Target;

	ObjectOps(ObjectOps&&);
	ObjectOps(const ObjectOps&);
	ObjectOps& operator = (ObjectOps&&);
	ObjectOps& operator = (const ObjectOps&);
};
//]

//[oglplus_object_Reference_CurrentBound
template <typename __ObjTag>
class __Reference<__ObjectOps_Bound<__tag_CurrentBound, ObjTag>>
 : public __ObjectOps_Bound<__tag_CurrentBound, ObjTag>
{
public:
	Reference(void);

	Reference(typename __ObjectOps_Bound<__tag_CurrentBound, __ObjTag>::Target);
};
//]

