/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_framebuffer_common

template <>
class __ObjCommonOps<__tag_Framebuffer>
 : public __FramebufferName
{
public:
	typedef __FramebufferTarget Target; /*<
	Framebuffer bind target.
	>*/

	static __FramebufferName Binding(__FramebufferTarget target); /*<
	Returns the framebuffer currently bound to the specified [^target].
	See [glfunc GetIntegerv].
	>*/
	static void Bind(__FramebufferTarget target, __FramebufferName framebuffer); /*<
	Binds the specified [^framebuffer] to the specified [^target].
	See [glfunc BindFramebuffer].
	>*/

	void Bind(__FramebufferTarget target) const; /*<
	Binds [^this] framebuffer to the specified [^target].
	See [glfunc BindFramebuffer].
	>*/
};
//]
//[oglplus_framebuffer_1
template <>
class __ObjectOps<__tag_ExplicitSel, __tag_Framebuffer>
 : public __ObjZeroOps<__tag_ExplicitSel, __tag_Framebuffer> /*<
Indirectly inherits from __ObjCommonOps_Framebuffer<__tag_Framebuffer>.
>*/
{
public:
	struct Property
	{
		typedef __OneOf<
			__FramebufferBuffer,
			__FramebufferAttachment,
			__FramebufferColorAttachment
		> Buffer; /*<
		Enumerations specifying framebuffer output buffer.
		>*/

		typedef __OneOf<
			__FramebufferAttachment,
			__FramebufferColorAttachment
		> Attachment; /*<
		Enumerations specifying framebuffer attachments.
		>*/

		typedef __FramebufferStatus Status;
	};

	static __FramebufferStatus Status(__FramebufferTarget target); /*<
	Returns the status of the framebuffer currently bound
	to the specified [^target].
	See [glfunc CheckFramebufferStatus].
	>*/
	static bool IsComplete(__FramebufferTarget target); /*<
	Returns true if the framebuffer currently bound to the specified
	[^target] is complete.
	See [glfunc CheckFramebufferStatus].
	>*/
	static void Complete(__FramebufferTarget target); /*<
	Throws an __IncompleteFramebuffer exception if the framebuffer
	currently bound to the specified [^target] is not complete.
	>*/

	static void AttachRenderbuffer(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__RenderbufferName renderbuffer
	); /*<
	Attaches a [^renderbuffer] object as an [^attachment] to the framebuffer
	currently bound to the specified [^target].
	See [glfunc FramebufferRenderbuffer].
	>*/
	static void AttachColorRenderbuffer(
		__FramebufferTarget target,
		__FramebufferColorAttachmentNumber attachment_no,
		__RenderbufferName renderbuffer
	); /*<
	Attaches a [^renderbuffer] object as a color attachment with index
	specified by [^attachment_no] to the framebuffer currently bound
	to the specified [^target].
	>*/

#if GL_VERSION_3_2
	static void AttachTexture(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__TextureName texture,
		GLint level
	); /*<
	Attaches the specified [^texture] [^level] as an attachment
	to the framebuffer currently bound to the specified [^target].
	See [glfunc FramebufferTexture].
	>*/
	static void AttachColorTexture(
		__FramebufferTarget target,
		__FramebufferColorAttachmentNumber attachment_no,
		__TextureName texture,
		GLint level
	); /*<
	Attaches the specified [^texture] [^level] as a color attachment with
	index specified by [^attachment_no] to the framebuffer currently
	bound to the specified [^target].
	>*/
#endif

	static void AttachTexture1D(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__TextureTarget textarget,
		__TextureName texture,
		GLint level
	); /*<
	Attaches the [^level] (or [^level]'s [^layer]) of a 1D, 2D or 3D
	[^texture] with type specified by [^textarget], as an [^attachment]
	of the framebuffer currently bound to the specified [^target].
	See [glfunc FramebufferTexture1D], [glfunc FramebufferTexture2D],
	[glfunc FramebufferTexture3D].
	>*/
	static void AttachTexture2D(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__TextureTarget textarget,
		__TextureName texture,
		GLint level
	);
	static void AttachTexture3D(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__TextureTarget textarget,
		__TextureName texture,
		GLint level,
		GLint layer
	);

	static void AttachTextureLayer(
		__FramebufferTarget target,
		Property::Attachment attachment,
		__TextureName texture,
		GLint level,
		GLint layer
	); /*<
	Attaches the [^level] (or [^level]'s [^layer]) of a 1D, 2D or 3D
	[^texture] as an [^attachment] of the framebuffer currently bound
	to the specified [^target].
	See [glfunc FramebufferTextureLayer].
	>*/
//]
//[oglplus_framebuffer_2

#if GL_VERSION_4_3 || GL_ARB_invalidate_subdata
	static void Invalidate(
		__FramebufferTarget target,
		const __EnumArray<Property::Buffer>& buffers
	); /*<
	Invalidates the specified attachments or [^buffers] of the framebuffer
	currently bound to the specified [^target].
	See [glfunc InvalidateFramebuffer].
	>*/
	static void Invalidate(
		__FramebufferTarget target,
		__SizeType count,
		const Property::Buffer* buffers
	);

	static void Invalidate(
		__FramebufferTarget target,
		const __EnumArray<Property::Buffer>& buffers,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	); /*<
	Invalidates parts (specified by [^x], [^y], [^width] and [^height])
	of attachments or [^buffers] of the framebuffer currently bound to
	the specified [^target].
	See [glfunc InvalidateSubFramebuffer].
	>*/
	static void Invalidate(
		__FramebufferTarget target,
		__SizeType count,
		const Property::Buffer* buffers,
		GLint x,
		GLint y,
		__SizeType width,
		__SizeType height
	);
#endif
};
//]
//[oglplus_framebuffer_def

typedef ObjectOps<__tag_ExplicitSel, __tag_Framebuffer>
	FramebufferOps;

typedef __Object<FramebufferOps> Framebuffer;

typedef __ObjectZero<__ObjZeroOps<__tag_ExplicitSel, __tag_Framebuffer>>
	DefaultFramebuffer;
//]
//[oglplus_framebuffer_sugar


struct FramebufferComplete { }; /*<
Helper class used with syntax-sugar operators.
Selects the Complete member function.
>*/
struct FramebufferTargetAndAttch { };  /*<
Helper class used with syntax-sugar operators.
Stores a framebuffer target and attachment.
>*/

FramebufferTargetAndAttch operator | (
	__FramebufferTarget target,
	FramebufferOps::Property::Attachment attachment
); /*<
These two operators both tie a framebuffer target and attachment
into a single object for use with subsequent operators.
>*/
FramebufferTargetAndAttch operator << (
	__FramebufferTarget target,
	FramebufferOps::Property::Attachment attachment
);

__FramebufferTarget operator << (
	FramebufferOps fbo,
	__FramebufferTarget target
); /*<
Equivalent to [^fbo.Bind(target)].
>*/
FramebufferTarget operator << (
	DefaultFramebuffer dfb,
	FramebufferTarget target
);

#if GL_VERSION_3_2
__FramebufferTarget operator << (
	FramebufferTargetAndAttch taa,
	__TextureName tex
); /*<
Attaches [^tex] to the attachment of a framebuffer bound to a target
specified by [^taa].
>*/
#endif

// AttachRenderbuffer
__FramebufferTarget operator << (
	FramebufferTargetAndAttch taa,
	__RenderbufferName rbo
); /*<
Attaches [^rbo] to the attachment of a framebuffer bound to a target
specified by [^taa].
>*/

__FramebufferTarget operator << (
	__FramebufferTarget target,
	FramebufferComplete
); /*<
Equivalent to [^FramebufferOps::Complete(target)].
>*/

//]

