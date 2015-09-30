/**
 *  @example standalone/025_bitmap_font_text.cpp
 *  @brief Shows rendering of text using a font stored in a texture+metrics
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "glut_glew_example.hpp"

#include <oglplus/all.hpp>
#include <oglplus/images/png.hpp>
#include <oglplus/dsa/ext/texture.hpp>
#include <oglplus/dsa/uniform.hpp>

#include <fstream>
#include <sstream>
#include <vector>

class FontTexture
 : public oglplus::DSATextureEXT
{
public:
	FontTexture(const char* png_path)
	{
		using namespace oglplus;

		std::ifstream png_stream(png_path);

		auto image = images::PNGImage(png_stream);

		this->target = Texture::Target::_2D;
		this->Bind();
		this->MinFilter(TextureMinFilter::LinearMipmapLinear);
		this->MagFilter(TextureMagFilter::Linear);
		this->WrapS(TextureWrap::ClampToBorder);
		this->WrapT(TextureWrap::ClampToBorder);
		this->SwizzleB(TextureSwizzle::Red);
		this->SwizzleG(TextureSwizzle::Red);
		this->Image2D(image);
		this->GenerateMipmap();
	}
};

// Class containing metric data for a range of glyphs
class GlyphMetrics
{
private:
	// For each glyph the following values are stored
	// 4 values * 3
	//
	// x - logical rectangle left bearing
	// y - logical rectangle right bearing
	// z - logical rectangle ascent
	// w - logical rectangle descent
	//
	// x - ink rectangle left bearing
	// y - ink rectangle right bearing
	// z - ink rectangle ascent
	// w - ink rectangle descent
	//
	// x - Glyph origin x in normalized texture space
	// y - Glyph origin y in normalized texture space
	// z - Glyph width in normalized texture space
	// w - Glyph height in normalized texture space

	const GLuint n_glyphs;
	std::vector<GLfloat> log_data, ink_data, tex_data;

	void LoadGlyph(
		std::istream& bfm,
		char* buf,
		const GLuint size
	)
	{
		// get the code point number from the stream
		GLuint glyph_cp;
		assert(bfm.good());
		bfm >> glyph_cp;
		assert(bfm.good());
		// eat the newline
		bfm.getline(buf, size);
		assert(bfm.good());
		// skip the hex code point line
		bfm.getline(buf, size);
		assert(bfm.good());
		// skip the code point's quoted utf-8 sequence
		char c;
		bfm.get(c);
		assert(bfm.good());
		assert(c == '\'');
		do { bfm.get(c); assert(bfm.good()); }
		while(c != '\'');
		bfm.getline(buf, size);
		assert(bfm.good());

		// now load the 12 metric values
		std::vector<GLfloat>* pdata[3] = {&log_data, &ink_data, &tex_data};
		for(GLuint d=0; d!=3; ++d)
		{
			for(GLuint v=0; v!=4; ++v)
			{
				// read the value
				GLfloat value;
				bfm >> value;
				assert(bfm.good());
				// assign the value to the data buffer
				(*pdata[d])[glyph_cp*4+v] = value;
				// eat the rest of the line
				bfm.getline(buf, size);
				assert(bfm.good());
			}
		}
		// skip the separating empty line
		bfm.getline(buf, size);
	}

public:
	GlyphMetrics(const char* bfm_path)
	 : n_glyphs(256)
	 , log_data(n_glyphs*4)
	 , ink_data(n_glyphs*4)
	 , tex_data(n_glyphs*4)
	{
		std::ifstream bfm_stream(bfm_path);
		const GLuint linelen = 63;
		char linebuf[linelen+1];
		for(GLuint cp=0; cp!=n_glyphs; ++cp)
			LoadGlyph(bfm_stream, linebuf, linelen);
	}

	const std::vector<GLfloat>& LogData(void) const
	{
		return log_data;
	}

	const std::vector<GLfloat>& InkData(void) const
	{
		return ink_data;
	}

	const std::vector<GLfloat>& TexData(void) const
	{
		return tex_data;
	}

	std::vector<GLfloat> XOffsets(const std::vector<GLubyte>& indices)
	{
		std::vector<GLfloat> result(indices.size()*4);
		auto ii=indices.begin(), ie=indices.end();
		if(ii != ie)
		{
			GLuint index = *ii;
			// left bearing
			GLfloat offs = log_data[index*4+0];

			auto ri=result.begin(), re=result.end();
			*ri++ = offs;
			*ri++ = 0.0f; *ri++ = 0.0f; *ri++ = 0.0f;
			while(true)
			{
				// right - left bearing
				offs += log_data[index*4+1]-log_data[index*4+0];
				++ii;
				if(ii != ie) index = *ii;
				else break;
				*ri++ = offs;
				*ri++ = 0.0f; *ri++ = 0.0f; *ri++ = 0.0f;
			}
			OGLPLUS_FAKE_USE(re);
			assert(ri == re);
		}
		return result;
	}
};

class BitmapFontProgram
 : public oglplus::Program
{
private:
	static oglplus::Program make(void)
	{
		using namespace oglplus;
		Program prog;
		prog.AttachShader(VertexShader(
			ObjectDesc("Glyph vertex"),
			"#version 330\n"

			"layout(location = 0) in vec4 InkData;"
			"layout(location = 1) in vec4 TexData;"

			"out vec4 vertInkData;"
			"out vec4 vertTexData;"

			"void main(void)"
			"{"
			"	vertInkData = InkData;"
			"	vertTexData = TexData;"
			"}"
		));
		/*
		 *        (5)    (4)
		 *         O-----O  <-- Ascender
		 *         |    /|      (InkD.z)
		 * Left<-->|   / |
		 * Bearing |  /  |
		 * (InkD.x)| /   |
		 *         |/    |
		 *  ===*===O=====O==<-- Baseline
		 *     .(3)|    /|(2)
		 *     .   |   / |
		 *     .   |  /  |
		 *     .   | /   |
		 *     .   |/    |
		 *     .   O-----O  <-- Descender
		 *     .  (1)    .(0)   (InkD.w)
		 *     .         .
		 *     <--------->Right Bearing
		 *                (InkD.y)
		 */
		prog.AttachShader(GeometryShader(
			ObjectDesc("Glyph geometry"),
			"#version 330\n"
			"layout(points) in;"
			"layout(triangle_strip, max_vertices = 6) out;"

			"layout(std140) uniform GlyphSpacing {"
			"	vec4 GlyphXOffsets[256];"
			"};"

			"uniform mat4 ProjectionMatrix, CameraMatrix, LayoutMatrix;"

			"mat4 Matrix = ProjectionMatrix*CameraMatrix*LayoutMatrix;"

			"in vec4 vertInkData[1], vertTexData[1];"

			"out vec2 geomTexCoord;"

			"void make_vertex(vec2 Pos, vec2 TexCoord)"
			"{"
			"	float xo = GlyphXOffsets[gl_PrimitiveIDIn].x;"
			"	gl_Position = Matrix*vec4(Pos.x+xo, Pos.y, 0.0, 1.0);"
			"	geomTexCoord = TexCoord;"
			"	EmitVertex();"
			"}"

			"void main(void)"
			"{"
			//	left bearing
			"	float lb = vertInkData[0].x;"
			//	right bearing
			"	float rb = vertInkData[0].y;"
			//	ascender
			"	float as = vertInkData[0].z;"
			//	descender
			"	float ds = vertInkData[0].w;"
			//	height
			"	float ht = as + ds;"
			//	glyph origin in texture space
			"	vec2  to = vertTexData[0].xy;"
			//	glyph width in texture space
			"	float tw = vertTexData[0].z;"
			//	glyph height in texture space
			"	float th = vertTexData[0].w;"
			//	glyph ascent in texture space
			"	float ta = th * (as / ht);"
			//	glyph descent in texture space
			"	float td = th * (ds / ht);"

			"	make_vertex(vec2( rb, -ds), to+vec2( tw, -td));"
			"	make_vertex(vec2( lb, -ds), to+vec2(0.0, -td));"
			"	make_vertex(vec2( rb, 0.0), to+vec2( tw, 0.0));"
			"	make_vertex(vec2( lb, 0.0), to+vec2(0.0, 0.0));"
			"	make_vertex(vec2( rb,  as), to+vec2( tw,  ta));"
			"	make_vertex(vec2( lb,  as), to+vec2(0.0,  ta));"
			"	EndPrimitive();"
			"}"
		));
		prog.AttachShader(FragmentShader(
			ObjectDesc("Glyph fragment"),
			"#version 330\n"
			"uniform sampler2D FontTexture;"

			"in vec2 geomTexCoord;"

			"out vec4 fragColor;"

			"void main(void)"
			"{"
			"	fragColor = texture(FontTexture, geomTexCoord);"
			"}"
		));

		prog.Link();
		prog.Use();
		return prog;
	}

	oglplus::Program& prog(void) { return *this; }

	oglplus::Buffer glyph_spacing_buf;
	oglplus::UniformBlock glyph_spacing;
public:
	oglplus::ProgramUniform<oglplus::Mat4f> projection_matrix;
	oglplus::ProgramUniform<oglplus::Mat4f> camera_matrix;
	oglplus::ProgramUniform<oglplus::Mat4f> layout_matrix;
	oglplus::ProgramUniform<GLint> font_texture;

	BitmapFontProgram(void)
	 : oglplus::Program(make())
	 , glyph_spacing(prog(), "GlyphSpacing")
	 , projection_matrix(prog(), "ProjectionMatrix")
	 , camera_matrix(prog(), "CameraMatrix")
	 , layout_matrix(prog(), "LayoutMatrix")
	 , font_texture(prog(), "FontTexture")
	{
	}

	void Render(
		const std::vector<GLfloat>& x_offsets,
		const std::vector<GLubyte>& indices
	)
	{
		using namespace oglplus;
		glyph_spacing_buf.Bind(Buffer::Target::Uniform);
		glyph_spacing_buf.BindBaseUniform(0);
		glyph_spacing.Binding(0);
		Buffer::Data(Buffer::Target::Uniform, x_offsets);

		Context().DrawElements(
			PrimitiveType::Points,
			indices.size(),
			indices.data()
		);
	}
};

// Wrapper around the bitmap font data
class BitmapFont
{
private:
	// The texture storing the bitmap containing font glyphs
	FontTexture texture;

	// The glyph metrics
	GlyphMetrics metrics;

	// The VAO and VBO for font glyph metrics
	oglplus::VertexArray vao;
	oglplus::Buffer ink_data, tex_data;
public:
	BitmapFont(const char* png_path, const char* bfm_path)
	 : texture(png_path)
	 , metrics(bfm_path)
	{
		using namespace oglplus;
		vao.Bind();

		ink_data.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, metrics.InkData());
		VertexArrayAttrib ink_attr(VertexAttribSlot(0));
		ink_attr.Setup<GLfloat>(4);
		ink_attr.Enable();

		tex_data.Bind(Buffer::Target::Array);
		Buffer::Data(Buffer::Target::Array, metrics.TexData());
		VertexArrayAttrib tex_attr(VertexAttribSlot(1));
		tex_attr.Setup<GLfloat>(4);
		tex_attr.Enable();
	}

	void Render(BitmapFontProgram& prog, const std::string& str)
	{
		std::vector<GLubyte> indices(str.begin(), str.end());
		std::vector<GLfloat> x_offsets(metrics.XOffsets(indices));
		prog.Render(x_offsets, indices);
	}
};

class BitmapFontExample
 : public oglplus::StandaloneExample
{
private:
	oglplus::Context gl;

	BitmapFontProgram font_prog;
	BitmapFont font;

	GLuint frame_no;
	std::stringstream txt;
public:
	BitmapFontExample(int argc, const char** argv)
	 : gl()
	 , font_prog()
	 , font(
		((argc>1) ? argv[1] : "font.png"),
		((argc>2) ? argv[2] : "font.bfm")
	), frame_no(0)
	{
		using namespace oglplus;

		gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.Enable(Capability::Blend);
		gl.BlendFunc(
			BlendFunction::SrcAlpha,
			BlendFunction::DstAlpha
		);
	}

	void Reshape(void)
	{
		using namespace oglplus;

		gl.Viewport(Width(), Height());
		font_prog.projection_matrix.Set(
			CamMatrixf::PerspectiveX(
				Degrees(60),
				Width()/Height(),
				1, 60
			)
		);
	}

	void Render(void)
	{
		using namespace oglplus;

		double t = FrameTime();
		gl.Clear().ColorBuffer().DoIt();
		font_prog.camera_matrix.Set(
			CamMatrixf::Orbiting(
				Vec3f(),
				GLfloat(11.0 + SineWave(t / 7.0)*3.0),
				FullCircles(t / 17.0),
				Degrees(SineWave(t / 21.0) * 35)
			)
		);

		// the "OGLplus" string
		font_prog.layout_matrix.Set(
			ModelMatrixf::Translation(
				-4.0f,
				GLfloat(3.0+SineWave(t/0.5)*0.1),
				0.1f
			)
		);
		font.Render(font_prog, "OGLplus");

		// the description string
		font_prog.layout_matrix.Set(
			ModelMatrixf::RotationY(Degrees(31+CosineWave(t/0.9)*2))*
			ModelMatrixf::Translation(-7.0f, 2.0f,-0.1f)*
			ModelMatrixf::Scale(0.707f, 0.707f, 1.0f)
		);
		font.Render(font_prog, "a C++ wrapper around the OpenGL C-API");

		// the frame number string
		font_prog.layout_matrix.Set(
			ModelMatrixf::RotationY(Degrees(-19))*
			ModelMatrixf::Translation(
				-2.0f,
				GLfloat(1.0+SineWave(t/0.7)*0.2),
				0.0f
			)*ModelMatrixf::Scale(0.6f, 0.6f, 1.0f)
		);
		txt << "Frame number: " << frame_no++;
		font.Render(font_prog, txt.str());
		txt.str(std::string());

		// the frame time string
		font_prog.layout_matrix.Set(
			ModelMatrixf::RotationY(Degrees(17))*
			ModelMatrixf::Translation(-3.0f, 0.0f, 0.1f)*
			ModelMatrixf::Scale(
				0.6f,
				GLfloat(0.6+CosineWave(t/0.6)*0.2),
				1.0f
			)
		);
		txt << "Frame time: " << t << " [s]";
		font.Render(font_prog, txt.str());
		txt.str(std::string());

		// the FPS string
		font_prog.layout_matrix.Set(
			ModelMatrixf::RotationY(Degrees(-7))*
			ModelMatrixf::Translation(-2.5f,-1.0f, 0.0f)*
			ModelMatrixf::Scale(
				GLfloat(0.6+SineWave(t/0.8)*0.1),
				0.6f,
				1.0f
			)
		);
		txt << "Frames per second : " << frame_no/t;
		font.Render(font_prog, txt.str());
		txt.str(std::string());
	}
};

int main(int argc, char* argv[])
{
	return oglplus::GlutGlewMain<BitmapFontExample>(
		"Simple example of bitmap font text rendering",
		argc, argv
	);
}

