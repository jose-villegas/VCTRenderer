/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_object_Array_example_1

__Array<__Buffer> vbos(3); /*<
Array of three buffer objects.
>*/

std::size_t n = /* ... */

std::vector<GLfloat> pos(n * 3); /*<
Position data.
>*/
std::vector<GLfloat> nms(n * 3); /*<
Normal vector data.
>*/
std::vector<GLfloat> tcs(n * 2); /*<
Texture coordinate data.
>*/

/* ... */

vbos[0].Bind(__BufferTarget::Array); /*<
Bind and setup the buffer object on index 0.
>*/
{
	__Buffer::Data(__BufferTarget::Array, pos);

	__VertexArrayAttrib attr(prog, "Position");
	attr.Setup<__Vec3f>();
	attr.Enable();
}

vbos[1].Bind(__BufferTarget::Array); /*<
Bind and setup the buffer object on index 1.
>*/
{
	__Buffer::Data(__BufferTarget::Array, nms);

	__VertexArrayAttrib attr(prog, "Normal");
	attr.Setup<__Vec3f>();
	attr.Enable();
}

vbos[2].Bind(__BufferTarget::Array); /*<
Bind and setup the buffer object on index 2.
>*/
{
	__Buffer::Data(__BufferTarget::Array, tcs);

	__VertexArrayAttrib attr(prog, "TexCoord");
	attr.Setup<__Vec2f>();
	attr.Enable();
}


//]

