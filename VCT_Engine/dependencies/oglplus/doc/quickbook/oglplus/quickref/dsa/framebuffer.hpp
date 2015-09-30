/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_dsa_framebuffer_1

template <>
class __ObjectOps<__tag_DirectState, __tag_Framebuffer>
 : public __ObjZeroOps<__tag_DirectState, __tag_Framebuffer> /*<
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

	void Bind(__FramebufferTarget target);

	__FramebufferStatus Status(__FramebufferTarget target) const; /*<
	Returns the status of [^this] framebuffer for the specified [^target].
	See [glfunc CheckNamedFramebufferStatus].
	>*/
	bool IsComplete(__FramebufferTarget target) const; /*<
	Returns true if [^this] framebuffer is complete for the specified [^target].
	See [glfunc CheckNamedFramebufferStatus].
	>*/
	void Complete(__FramebufferTarget target) const; /*<
	Throws an __IncompleteFramebuffer exception if [^this] framebuffer
	is not complete.
	>*/

	void AttachRenderbuffer(
		Property::Attachment attachment,
		__RenderbufferName renderbuffer
	); /*<
	Attaches a [^renderbuffer] object as an [^attachment] to [^this]
	framebuffer.
	See [glfunc NamedFramebufferRenderbuffer].
	>*/
	void AttachColorRenderbuffer(
		__FramebufferColorAttachmentNumber attachment_no,
		__RenderbufferName renderbuffer
	); /*<
	Attaches a [^renderbuffer] object as a color attachment with index
	specified by [^attachment_no] to [^this] framebuffer.
	>*/

	void AttachTexture(
		Property::Attachment attachment,
		__TextureName texture,
		GLint level
	); /*<
	Attaches the specified [^texture] [^level] as an attachment
	to [^this] framebuffer.
	See [glfunc NamedFramebufferTexture].
	>*/
	void AttachColorTexture(
		__FramebufferColorAttachmentNumber attachment_no,
		__TextureName texture,
		GLint level
	); /*<
	Attaches the specified [^texture] [^level] as a color attachment with
	index specified by [^attachment_no] to [^this] framebuffer.
	>*/

	void AttachTextureLayer(
		Property::Attachment attachment,
		__TextureName texture,
		GLint level,
		GLint layer
	); /*<
	Attaches the [^level] (or [^level]'s [^layer]) of a 1D, 2D or 3D
	[^texture] as an [^attachment] of [^this] framebuffer.
	See [glfunc NamedFramebufferTextureLayer].
	>*/
};
//]
//[oglplus_dsa_framebuffer_def

typedef ObjectOps<__tag_DirectState, __tag_Framebuffer>
	DSAFramebufferOps;

typedef __Object<DSAFramebufferOps> DSAFramebuffer;
//]
//[oglplus_dsa_framebuffer_sugar

struct DSAFramebufferOpsAndAttch { }; /*<
Helper class used with syntax-sugar operators.
Stores a framebuffer name and attachment.
>*/

DSAFramebufferOpsAndAttch operator << (
	DSAFramebufferOps& fbo,
	DSAFramebufferOps::Property::Attachment attch
);

DSAFramebufferOps& operator << (
	DSAFramebufferOps& fbo,
	__FramebufferTarget target
); /*<
Equivalent to [^fbo.Bind(target)].
>*/

DSAFramebufferOps& operator << (
	DSAFramebufferOpsAndAttch&& faa,
	__TextureName tex
); /*<
Attaches [^tex] to the attachment of the framebuffer
specified by [^faa].
>*/

DSAFramebufferOps& operator << (
	DSAFramebufferOpsAndAttch&& faa,
	__RenderbufferName rbo
); /*<
Attaches [^rbo] to the attachment of the framebuffer specified by [^faa].
>*/

//]

