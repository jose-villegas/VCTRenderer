/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_ObjGenDelOps

template <typename __ObjTag>
class ObjGenDelOps
{
protected:
	static void Gen(__GenTag gen_tag, GLsizei count, GLuint* names); /*<
	Generates [^count] objects of the type specified by [^__ObjTag],
	using the method specified by [^gen_tag] and stores them in [^names].
	Note that [^names] must provide enough space for at least [^count]
	instances of [^GLuint].
	>*/
	static void Delete(GLsizei count, GLuint* names); /*<
	Deletes [^count] objects of the type specified by __ObjTag
	stored in names.
	>*/
	static GLboolean IsA(GLuint name); /*<
	Determines if [^name] is a valid name currently assigned by GL
	to some object of the type specified by __ObjTag.
	>*/
};

//]
//[oglplus_object_ObjBindingOps

template <typename __ObjTag>
class ObjBindingOps
{
public:
	typedef __Unspecified Target; /*<
	Strongly typed enumeration specifying binding points for object
	type associated with __ObjTag.
	This typedef is available only if the object type has named,
	explicit binding points.
	>*/

	static __ObjectName<__ObjTag> Binding(void); /*<
	Returns the object currently bound to the implicit binding point
	for the object type specified by __ObjTag.
	This overload is available only if the object type has only one
	implicit binding point.
	>*/

	static __ObjectName<__ObjTag> Binding(Target target); /*<
	Returns the object currently bound to the explicit binding point
	[^target] for the object type specified by __ObjTag.
	This overload is available only if the object type has named,
	explicit binding points.
	>*/

	static void Bind(__ObjectName<__ObjTag> object) /*<
	Binds the specified [^object] to the implicit binding point
	for the object type specified by __ObjTag.
	This overload is available only if the object type has only one
	implicit binding point.
	>*/

	static void Bind(Target target, __ObjectName<__ObjTag> object) /*<
	Binds the specified [^object] to the explicit binding point
	[^target] for the object type specified by __ObjTag.
	This overload is available only if the object type has named,
	explicit binding points.
	>*/
};

//]
//[oglplus_object_ObjCommonOps

template <typename __ObjTag>
class ObjCommonOps
 : public __ObjectName<__ObjTag>
{
public: /*<
The public interface depends on the __ObjTag.
>*/
};

//]
//[oglplus_object_ObjZeroOps

template <typename __OpsTag, typename __ObjTag>
class ObjZeroOps
 : public __ObjCommonOps<__ObjTag>
{
public: /*<
The public interface depends on the __OpsTag and __ObjTag.
If there are no operations that can work explicitly on
object zero besides binding, then this class just inherits from
the common object operations wrapper.
>*/
};

//]
//[oglplus_object_ObjectOps

template <typename __OpsTag, typename __ObjTag>
class ObjectOps
 : public __ObjZeroOps<__OpsTag, __ObjTag>
{
public: /*<
The public interface depends on the __OpsTag and __ObjTag.
>*/
};

//]
//[oglplus_object_ObjectSubtype

template <typename __ObjTag>
struct ObjectSubtype : __Nothing /*<
Most object types don't have any ['subtypes], which is indicated by
returning __Nothing by default.
>*/
{ };

//]
//[oglplus_object_ObjHandle

template <typename __ObjTag>
class ObjHandle
 : public __ObjGenDelOps<__ObjTag>
 , public __ObjectName<__ObjTag>
{
public:
	ObjHandle(__GenTag method); /*<
	Constructs an [^ObjHandle] with a specific [^method] of object creation.
	>*/

	ObjHandle(__GenTag method, __ObjectDesc&& description); /*<
	Constructs an [^ObjHandle] with a specific [^method] of object creation,
	and attaches the specified [^description] to it.
	>*/

	typedef typename __ObjectSubtype<__ObjTag>::Type Subtype; /*<
	The subtype of the object. If this object type does not have
	any subtypes, then [^Subtype] is defined as __Nothing.
	>*/

	template <typename __GenTag>
	ObjHandle(__GenTag method, Subtype subtype); /*<
	Constructs a handle for object of the specified [^subtype].
	If [^subtype] is __Nothing then this constructor is equivalent
	to the constructor taking only the [^method].
	>*/

	ObjHandle(ObjHandle&& temp);
	ObjHandle& operator = (ObjHandle&& temp);

	const std::string& Description(void) const /*<
	Returns the textual description of this object handle.
	>*/

	operator __Sequence<__ObjectName<__ObjTag>> (void) const; /*<
	[^ObjHandle]s are implicitly convertible to a sequence of object names.
	Note that the returned sequence must not be used after
	this object handle has been destroyed.
	>*/
};
//]
//[oglplus_object_Object

template <typename __OpsTag, typename __ObjTag>
class Object<ObjectOps<__OpsTag, __ObjTag>>
 : public __ObjGenDelOps<__ObjTag>
 , public __ObjectOps<__OpsTag, __ObjTag>
{
public:
	Object(const Object&) = delete; /*< [^Object]s are non-copyable. >*/
	Object(Object&& temp) noexcept; /*<
	[^Object]s are move-constructible and move assignable.
	>*/
	Object& operator = (Object&& temp);

	Object(void); /*<
	Default-constructs an [^Object]. This function creates a new GL object
	by calling the appropriate [^glGen*] or [^glCreate*] function.
	Note that object types with a [^Subtype] may not support default
	construction and may require that the subtype is specified.
	>*/
	Object(__GenTag gen_tag); /*<
	Constructs an [^object] with [^gen_tag] explicitly specifying
	the GL [link oglplus.quickref.gen_tags object creation method].
	>*/
	Object(__ObjectDesc&& description); /*<
	Constructs an [^Object] and attaches the specified [^description] to it.
	>*/
	Object(__GenTag gen_tag, __ObjectDesc&& description);

	typedef typename __ObjectSubtype<__ObjTag>::Type Subtype; /*<
	The subtype of the object. If this object type does not have
	any subtypes, then [^Subtype] is defined as __Nothing.
	>*/

	Object(Subtype subtype); /*<
	Constructs an object of the specified [^subtype].
	If [^subtype] is __Nothing then this constructor is equivalent
	to the default constructor.
	>*/
	Object(__GenTag gen_tag, Subtype subtype);
	Object(Subtype subtype, __ObjectDesc&& description); /*<
	Constructs an object of the specified [^subtype] and attaches
	the specified [^description] to it.
	>*/
	Object(__GenTag gen_tag, Subtype subtype, __ObjectDesc&& description);

	~Object(void) noexcept; /*<
	Cleans up the wrapped GL object by calling
	the appropriate [^glDelete*] function.
	>*/

	static Object FromRawName(__ObjectName<__ObjTag> name); /*<
	Wraps an existing GL object with the specified [^name]. This
	function does not create a new GL object, instead it adopts
	the one passed as argument.
	>*/

	const std::string& Description(void) const /*<
	Returns the textual description of this object.
	>*/

	operator __Sequence<__ObjectName<__ObjTag>> (void) const; /*<
	[^Object]s are implicitly convertible to a sequence of object names.
	Note that the returned sequence must not be used after
	this object has been destroyed.
	>*/
};
//]
//[oglplus_object_ObjectZero
template <typename __OpsTag, typename __ObjTag>
class ObjectZero<__ObjZeroOps<__OpsTag, __ObjTag>>
 : public __ObjZeroOps<__OpsTag, __ObjTag>
{
public:
	ObjectZero(void); /*<
	ObjectZero is default constructible.
	>*/
	ObjectZero(const ObjectZero&);
};

//]

