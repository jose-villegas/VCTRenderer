/**
 *  @example standalone/001_text2d.cpp
 *  @brief Shows the basic usage of low-level 2D text rendering
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#include <cassert>
#include <iostream>

#include <oglplus/opt/resources.hpp>
#include <oglplus/text/font2d.hpp>

#include <iostream>
#include <vector>

void print_buffer(
	const unsigned char* buffer,
	const std::size_t width,
	const std::size_t height
)
{
	for(std::size_t j=0; j!=height; ++j)
	{
		for(std::size_t i=0; i!=width; ++i)
		{
			std::cout << " .:ioVM@"[buffer[j*width+i]>>5];
		}
		std::cout << std::endl;
	}
}

void render_text_1(const oglplus::text::Font2D& font)
{
	oglplus::text::CodePoints code_points = oglplus::text::UTF8ToCodePoints("OGLplus");
	oglplus::text::Font2D::Layout layout = font.MakeLayout(code_points);

	std::size_t width = 90;
	std::size_t height = 26;
	std::size_t font_size = 25;
	std::vector<unsigned char> buffer(width*height, 0x00);

	font.Render(font_size, layout, buffer.data(), width, height);

	print_buffer(buffer.data(), width, height);
}

void render_text_2(const oglplus::text::Font2D& font)
{
	oglplus::text::CodePoints cps = oglplus::text::UTF8ToCodePoints("OGLPLUS");

	std::size_t width = 72;
	std::size_t height = 72;
	std::vector<unsigned char> buffer(width*height, 0x00);

	font.Render(48, font.MakeLayout(cps.data()+0, 1), buffer.data(), width, height,  4,  0);
	font.Render(48, font.MakeLayout(cps.data()+1, 1), buffer.data(), width, height,  4, 32);
	font.Render(48, font.MakeLayout(cps.data()+2, 1), buffer.data(), width, height, 50, 32);

	font.Render(22, font.MakeLayout(cps.data()+3, 1), buffer.data(), width, height, 40,  5);
	font.Render(22, font.MakeLayout(cps.data()+4, 1), buffer.data(), width, height, 60,  5);
	font.Render(22, font.MakeLayout(cps.data()+5, 1), buffer.data(), width, height, 40, 22);
	font.Render(22, font.MakeLayout(cps.data()+6, 1), buffer.data(), width, height, 60, 22);

	print_buffer(buffer.data(), width, height);
}

int main(int argc, char* argv[])
{
	oglplus::ResourceFile font_file("fonts", (argc>1)?argv[1]:"FreeSans", ".ttf");
	oglplus::text::Font2D font(font_file);

	render_text_1(font);
	render_text_2(font);

	return 0;
}
