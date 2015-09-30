/**
 *  @example standalone/029_mandelbrot_glfw3.cpp
 *  @brief Shows the usage of OGLplus with GLFW3
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/screen.hpp>
#include <oglplus/shapes/wrapper.hpp>
#include <oglplus/opt/list_init.hpp>

#include <oglplus/images/gradient.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <sstream>
#include <iostream>
#include <cassert>
#include <ctime>
#include <map>

//------------------------------------------------------------------------------
class Example
{
private:
	static Example& self(void);

	static void HandleGLFWError(int error_code, const char* description);

	GLFWerrorfun glfw_error_handler;
	int glfw_init_ok;
	GLFWwindow* glfw_window;
	static GLFWwindow* InitWindow(void);

	oglplus::Context gl;

	static void GeneratePaletteImage(void);
	static oglplus::Texture MakePalette(void);
	oglplus::Texture palette;

	static oglplus::Program MakeProgram(void);
	oglplus::Program program;
	oglplus::Uniform<oglplus::Mat3f> view_matrix;

	oglplus::shapes::ShapeWrapper screen;

	unsigned screen_width, screen_height;

	GLfloat view_aspect, view_center_x, view_center_y, view_width, view_angle;
	void UpdateView(void);
	void CenterView(void);
	void TranslateView(float xcoef, float ycoef);
	void RotateView(float diff);
	void ScaleView(float coef);

	const unsigned view_divs;
	unsigned view_tile;
	std::vector<unsigned> view_tile_index;

	double mouse_x, mouse_y;
	bool mouse_left_down, mouse_middle_down;

	void MouseMove(double, double);

	static void OnKeyPress(GLFWwindow*, int, int, int, int);
	static void OnMouseMove(GLFWwindow*, double, double);
	static void OnMouseButton(GLFWwindow*, int, int, int);
	static void OnScroll(GLFWwindow*, double, double);

	void Resize(int width, int height);
	static void OnResize(GLFWwindow* window, int width, int height);

	unsigned long frame_no;
	void RenderFrame(double);
public:
	Example(void);
	~Example(void);
	void Run(void);
};
//------------------------------------------------------------------------------
int main(void)
{
	try
	{
		std::srand(std::time(0));
		Example().Run();
		return 0;
	}
	catch(oglplus::Error& err)
	{
		std::cerr
			<< "Error (in "
			<< err.GLFunc()
			<< "'): "
			<< err.what()
			<< " ["
			<< err.SourceFile()
			<< ":"
			<< err.SourceLine()
			<< "] "
			<< std::endl;
	}
	catch(std::exception& se)
	{
		std::cerr
			<< "Unspecified error: '"
			<< se.what()
			<< "'."
			<< std::endl;
	}
	return 0;
}
//------------------------------------------------------------------------------
Example* self_ptr(Example* ptr)
{
	static Example* _self = ptr;
	assert(_self);
	return _self;
}

Example& Example::self(void)
{
	return *self_ptr(nullptr);
}
//------------------------------------------------------------------------------
void Example::HandleGLFWError(int error_code, const char* description)
{
	std::stringstream msgs;
	msgs	<< "GLFW error (#"
		<< error_code
		<< ") "
		<< description;
	throw std::runtime_error(msgs.str());
}
//------------------------------------------------------------------------------
void Example::GeneratePaletteImage(void)
{
	std::map<GLfloat, oglplus::Vec3f> gradient_colors;

	GLuint tex_size = 2048;

	gradient_colors[0.0f] = oglplus::Vec3f(
		GLfloat(std::rand())/RAND_MAX,
		GLfloat(std::rand())/RAND_MAX,
		GLfloat(std::rand())/RAND_MAX
	);
	for(int i=0; i<21; ++i)
	{
		gradient_colors[std::pow(GLfloat(std::rand())/RAND_MAX, 2)] = oglplus::Vec3f(
			GLfloat(std::rand())/RAND_MAX,
			GLfloat(std::rand())/RAND_MAX,
			GLfloat(std::rand())/RAND_MAX
		);
	}

	oglplus::Texture::Image1D(
		oglplus::TextureTarget::_1D,
		oglplus::images::LinearGradient(
			tex_size,
			oglplus::Vec3f(0, 0, 0),
			gradient_colors
		)
	);
}
//------------------------------------------------------------------------------
oglplus::Texture Example::MakePalette(void)
{
	oglplus::Texture palette;

	oglplus::Texture::Active(0);
	palette.Bind(oglplus::TextureTarget::_1D);

	GeneratePaletteImage();

	oglplus::Texture::MinFilter(oglplus::TextureTarget::_1D, oglplus::TextureMinFilter::Linear);
	oglplus::Texture::MagFilter(oglplus::TextureTarget::_1D, oglplus::TextureMagFilter::Linear);
	oglplus::Texture::WrapS(oglplus::TextureTarget::_1D, oglplus::TextureWrap::Repeat);
	oglplus::Texture::WrapT(oglplus::TextureTarget::_1D, oglplus::TextureWrap::Repeat);

	return palette;
}
//------------------------------------------------------------------------------
oglplus::Program Example::MakeProgram(void)
{
	oglplus::Program prog;

	oglplus::VertexShader vs;
	vs.Source(
		"#version 140\n"
		"#define imag vec2\n"

		"uniform mat3 ViewMatrix;"

		"in vec4 Position;"
		"out imag vertCoord;"

		"void main(void)"
		"{"
		"	gl_Position = Position;"
		"	vertCoord = imag((ViewMatrix*vec3(Position.xy, 1.0)).xy);"
		"}"
	).Compile();

	oglplus::FragmentShader fs;
	fs.Source(
		"#version 140\n"
		"#define imag vec2\n"

		"uniform sampler1D Palette;"

		"in imag vertCoord;"
		"out vec3 fragColor;"

		"float mandelbrot(void)"
		"{"
		"	imag z = imag(0.0, 0.0);"
		"	imag c = vertCoord;"

		"	int i = 0, n = textureSize(Palette, 0);"
		"	while((i<n) && distance(z, c) < 2.0)"
		"	{"
		"		imag zn = imag("
		"			z.x*z.x - z.y*z.y + c.x,"
		"			2.0*z.x*z.y + c.y"
		"		);"
		"		z = zn;"
		"		++i;"
		"	}"
		"	return float(i)/float(n);"
		"}"

		"void main(void)"
		"{"
		"	fragColor = texture(Palette, mandelbrot()).rgb;"
		"}"
	).Compile();

	prog.AttachShader(vs).AttachShader(fs).Link().Use();

	return std::move(prog);
}
//------------------------------------------------------------------------------
GLFWwindow* Example::InitWindow(void)
{
	GLFWwindow* window = glfwCreateWindow(
		800, 600,
		"OGLplus example",
		0, 0
	);

	glfwSetKeyCallback(window, OnKeyPress);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetScrollCallback(window, OnScroll);
	glfwSetFramebufferSizeCallback(window, OnResize);

	glfwMakeContextCurrent(window);

	return window;
}
//------------------------------------------------------------------------------
Example::Example(void)
 : glfw_error_handler(glfwSetErrorCallback(HandleGLFWError))
 , glfw_init_ok(glfwInit())
 , glfw_window(InitWindow())
 , gl()
 , palette(MakePalette())
 , program(MakeProgram())
 , view_matrix(program, "ViewMatrix")
 , screen(
	oglplus::List("Position").Get(),
	oglplus::shapes::Screen(),
	program
), screen_width(800)
 , screen_height(600)
 , view_aspect(4.0/3.0)
 , view_divs(4)
 , view_tile(0)
 , view_tile_index(view_divs*view_divs)
 , mouse_left_down(false)
 , mouse_middle_down(false)
 , frame_no(0)
{
	oglplus::UniformSampler(program, "Palette").Set(0);


	for(unsigned i=0; i!=view_tile_index.size(); ++i)
		view_tile_index[i] = i;
	CenterView();
	UpdateView();

	screen.Use();
	self_ptr(this);

	gl.Enable(oglplus::Capability::ScissorTest);
}
//------------------------------------------------------------------------------
Example::~Example(void)
{
	if(glfw_window)
	{
		glfwDestroyWindow(glfw_window);
	}

	if(glfw_init_ok == GL_TRUE)
	{
		glfwTerminate();
	}
}
//------------------------------------------------------------------------------
void Example::Run(void)
{
	double start_time = glfwGetTime();
	glfwPollEvents();
	while(!glfwWindowShouldClose(glfw_window))
	{
		RenderFrame(glfwGetTime()-start_time);
		glfwSwapBuffers(glfw_window);
		glfwPollEvents();
	}
}
//------------------------------------------------------------------------------
void Example::UpdateView(void)
{
	float tx = view_center_x;
	float ty = view_center_y;
	float sa = oglplus::Sin(oglplus::Degrees(view_angle));
	float ca = oglplus::Cos(oglplus::Degrees(view_angle));
	float sx = view_width;
	float sy = view_width/view_aspect;

	oglplus::Mat3f t(
		1.0f, 0.0f,   tx,
		0.0f, 1.0f,   ty,
		0.0f, 0.0f, 1.0f
	);
	oglplus::Mat3f r(
		 ca,  sa, 0.0f,
		-sa,  ca, 0.0f,
		0.0, 0.0, 1.0f
	);
	oglplus::Mat3f s(
		  sx, 0.0f, 0.0f,
		0.0f,   sy, 0.0f,
		0.0f, 0.0f, 1.0f
	);
	view_matrix.Set(t*r*s);
}
//------------------------------------------------------------------------------
void Example::CenterView(void)
{
	view_center_x = 0.0f;
	view_center_y = 0.0f;
	view_width = 2.0f;
	view_angle = 0.0f;
}
//------------------------------------------------------------------------------
void Example::TranslateView(float xcoef, float ycoef)
{
	float sa = oglplus::Sin(oglplus::Degrees(view_angle));
	float ca = oglplus::Cos(oglplus::Degrees(view_angle));

	view_center_x += (ca*xcoef + sa*ycoef)*view_width;
	view_center_y += (ca*ycoef - sa*xcoef)*view_width;
}
//------------------------------------------------------------------------------
void Example::RotateView(float diff)
{
	view_angle += diff;
}
//------------------------------------------------------------------------------
void Example::ScaleView(float coef)
{
	assert(coef > 0.00001f);
	view_width *= coef;
}
//------------------------------------------------------------------------------
void Example::OnKeyPress(
	GLFWwindow* window,
	int key,
	int /*scancode*/,
	int action,
	int /*mods*/
)
{
	switch(key)
	{
		case GLFW_KEY_HOME:
		{
			self().CenterView();
			self().UpdateView();
			break;
		}
		case GLFW_KEY_LEFT:
		{
			self().TranslateView(-0.05f, 0.00f);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_RIGHT:
		{
			self().TranslateView(+0.05f, 0.00f);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_UP:
		{
			self().TranslateView( 0.00f,+0.05f);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_DOWN:
		{
			self().TranslateView( 0.00f,-0.05f);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_PAGE_UP:
		{
			self().RotateView(+1.0);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_PAGE_DOWN:
		{
			self().RotateView(-1.0);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_KP_ADD:
		{
			self().ScaleView(0.98);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_KP_SUBTRACT:
		{
			self().ScaleView(1.02);
			self().UpdateView();
			break;
		}
		case GLFW_KEY_ENTER:
		{
			if(action == GLFW_RELEASE)
			{
				self().view_tile = 0;
				self().GeneratePaletteImage();
			}
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			if(action == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(
					window,
					GL_TRUE
				);
			}
			break;
		}
		default:;
	}
}
//------------------------------------------------------------------------------
void Example::MouseMove(double x, double y)
{
	y = screen_height-y;
	if(mouse_left_down)
	{
		TranslateView(
			(mouse_x - x)/screen_width,
			(mouse_y - y)/screen_height
		);
		UpdateView();
	}
	if(mouse_middle_down)
	{
		double v1x = mouse_x - screen_width/2;
		double v1y = mouse_y - screen_height/2;
		double v2x = x - screen_width/2;
		double v2y = y - screen_height/2;
		double a1 = std::atan2(v1y, v1x);
		double a2 = std::atan2(v2y, v2x);

		RotateView((a2-a1)*180.0/3.1415);
		UpdateView();
	}
	mouse_x = x;
	mouse_y = y;

	std::map<double, unsigned> view_tile_dist;

	for(unsigned i=0, n=view_divs*view_divs; i!=n; ++i)
	{
		double tile_x = ((i % view_divs)+0.5)*screen_width/view_divs;
		double tile_y = ((i / view_divs)+0.5)*screen_height/view_divs;

		double dist_sq = std::pow(mouse_x-tile_x, 2)+std::pow(mouse_y-tile_y, 2);

		view_tile_dist[dist_sq] = i;
	}

	auto t=view_tile_dist.begin();
	unsigned i=0;

	while(t!=view_tile_dist.end())
	{
		view_tile_index[i] = t->second;
		++t;
		++i;
	}
}
//------------------------------------------------------------------------------
void Example::OnMouseMove(GLFWwindow*, double x, double y)
{
	self().MouseMove(x, y);
}
//------------------------------------------------------------------------------
void Example::OnMouseButton(GLFWwindow*, int button, int action, int)
{
	switch(button)
	{
		case GLFW_MOUSE_BUTTON_1:
		{
			switch(action)
			{
				case GLFW_PRESS:
				{
					self().mouse_left_down = true;
					break;
				}
				case GLFW_RELEASE:
				{
					self().mouse_left_down = false;
					break;
				}
				default:;
			}
			break;
		}
		case GLFW_MOUSE_BUTTON_3:
		{
			switch(action)
			{
				case GLFW_PRESS:
				{
					self().mouse_middle_down = true;
					break;
				}
				case GLFW_RELEASE:
				{
					self().mouse_middle_down = false;
					break;
				}
				default:;
			}
			break;
		}
		default:;
	}
}
//------------------------------------------------------------------------------
void Example::OnScroll(GLFWwindow*, double x_offs, double y_offs)
{
	self().ScaleView(std::exp(-(x_offs+y_offs)*0.1));
	self().UpdateView();
}
//------------------------------------------------------------------------------
void Example::Resize(int width, int height)
{
	if((width > 0) && (height > 0))
	{
		screen_width = width;
		screen_height = height;
		view_aspect = float(width)/float(height);
	}
	gl.Viewport(0, 0, width, height);
}
//------------------------------------------------------------------------------
void Example::OnResize(GLFWwindow* window, int width, int height)
{
	assert(self().glfw_window == window);
	self().Resize(width, height);
}
//------------------------------------------------------------------------------
void Example::RenderFrame(double /*time*/)
{
	bool dragging = mouse_left_down || mouse_middle_down;

	if(dragging && (frame_no % 2))
	{
		gl.Scissor(
			mouse_x-screen_width/(view_divs*2),
			mouse_y-screen_height/(view_divs*2),
			screen_width/view_divs,
			screen_height/view_divs
		);
	}
	else
	{
		unsigned this_tile = view_tile_index[view_tile];
		gl.Scissor(
			(this_tile % view_divs)*screen_width/view_divs,
			(this_tile / view_divs)*screen_height/view_divs,
			screen_width/view_divs+1,
			screen_height/view_divs+1
		);

		if(++view_tile >= (dragging?view_divs:view_divs*view_divs))
		{
			view_tile = 0;
		}
	}

	screen.Draw();
	++frame_no;
}
//------------------------------------------------------------------------------

