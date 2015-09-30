/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_uniform_example_1

__Context gl;

__VertexShader vs;

vs.Source(
	"#version 150\n"
	"in vec3 Position;\n"
	"void main(void)\n"
	"{\n"
	"	gl_Position = Position;\n"
	"}\n"
).Compile();

__FragmentShader fs;

fs.Source(
	"#version 130\n"
	"uniform vec4 Color;\n"
	"void main(void)\n"
	"{\n"
	"	gl_FragColor = Color;\n"
	"}\n"
).Compile();

__Program prog;

prog.AttachShader(vs).AttachShader(fs).Link();
gl.Use(prog);

__UniformLoc color_loc1(prog, "Color"); /*<
Getting the location of a uniform in a GPU program, by its name.
Throws if no such uniform is active in the [*current] program.
To get the location of a program which is not currently active,
__ProgramUniform must be used.
>*/

assert(color_loc1.IsActive()); /*<
Check if the uniform is active.
>*/
assert(color_loc1);

__UniformLoc color_loc2(prog, "Color", false /*< Don't throw if inactive.>*/); /*<
Getting the location of a uniform in a GPU program, by its name.
Does not throw if the uniform is not active.
>*/

assert(color_loc1 == color_loc2); /*<
Compare two uniform locations for equality (in both the program name
and the location index).
>*/

assert(color_loc1.Location() == GetGLLocation(color_loc1)); /*<
Two ways to get the location index.
>*/

__UniformLoc color_loc3(0); /*<
Explicitly initializing a uniform location.
>*/

__Uniform<__Vec4f> color1(prog, "Color"); /*<
Initialize a typed reference to a uniform variable, from a program name
and identifier.
>*/

__Uniform<__Vec4f> color2(color_loc2); /*<
Initialize a typed reference to a uniform variable, from a uniform location.
>*/

__Uniform<__Vec4f> color3(prog); /*<
Initialize a uniform from a program name, the location is not obtained
in this case and must be bound later.
>*/

color3.BindTo("Color"); /*<
Late binding of a uniform to an identifier.
>*/

color1.Set(__Vec4f(1, 0, 0, 1)); /*<
Setting the value of a GPU program uniform variable.
This will throw if the uniform is not active.
>*/

__Typechecked_ProgVar<__Uniform<__Vec2f>> color4(prog, "Color"); /*<
The [^Typechecked] modifier enables uniform typechecking, which compares
the C++ template parameter type to the GLSL data type.
In this case the constructor would throw because [^__Vec2f] does not
match [^vec4] declared in the fragment shader.
>*/

//]
//[oglplus_uniform_example_2

__UntypedUniform color5(prog, "Color"); /*<
Untyped uniforms do not carry the type information on the C++ side 
and cannot be typechecked during construction.
>*/

color5.Set(__Vec4f(0,0,1,1));

(prog/"Color").Set(__Vec4f(0,0,1,1)); /*<
Syntax sugar operator for constructing untyped uniforms, equivalent
to [^__UntypedUniform(prog,"Color").Set(...)].
>*/

__Optional_ProgVar<__Uniform<__Vec4f>> color6(prog, "Blah"); /*<
The [^Optional] modifier allows to construct instances of [^Uniform]
without throwing even if the specified identifier does not refer to
an active uniform in the GPU program.
>*/

color6.TrySet(__Vec4f(0,1,0,1)); /*<
If the referenced uniform is active, then this sets its value,
but unlike [^Set], [^TrySet] does not throw if the uniform is inactive.
>*/

__Lazy_ProgVar<__Uniform<__Vec4f>> color7(prog, "Color"); /*<
The [^Lazy] modifier allows to postpone the querying of uniform location
until it is actually needed at the cost of having to store the identifier
of the shader unifom variable in the [^Lazy<Uniform>] instance.
This means that the referenced program does not have to be linked and current
when the [^Uniform] is constructed.
>*/

try { color7.Init(); /*<
The uniform location can be explicitly queried by calling the [^Init]
member function which throws in case of failure.
>*/ }
catch(...) { /* ... */ }

color7.TryInit(); /*<
The [^TryInit] function also queries the uniform location, but does not throw,
the instance just remains not active.
>*/

color7.Set(__Vec4f(0,0,0,1)); /*<
If the location is still not initialized yet, then it is queried before
setting the uniform value.
>*/

//]

