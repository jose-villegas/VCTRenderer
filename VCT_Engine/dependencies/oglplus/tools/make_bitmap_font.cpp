/**
 *  .file tools/make_bitmap_font.hpp
 *  .brief Tool for generating a font bitmap and metrics file using Pango/Cairo
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <pango/pangocairo.h>

#include <oglplus/string/utf8.hpp>

void render_glyph(
	cairo_t* cr,
	PangoFontDescription* font_desc,
	PangoFont* font,
	const guint code_point,
	const size_t cell_x,
	const size_t cell_y,
	const size_t cell_size,
	const double tex_size,
	const int ascent,
	const int descent,
	std::ostream& bgm_out
)
{
	PangoLayout *layout = pango_cairo_create_layout(cr);

	char str[6] = {'\0'};
	size_t len = 0;
	oglplus::aux::ConvertCodePointToUTF8(code_point, str, len);

	pango_layout_set_font_description(layout, font_desc);
	pango_layout_set_text(layout, str, len);

	const int baseline = pango_layout_get_baseline(layout);
	const double inv_ps = 1.0 / double(PANGO_SCALE);
	const double font_size = ascent+descent;

	PangoRectangle ink_rect, log_rect;
	pango_layout_get_extents(
		layout,
		&ink_rect,
		&log_rect
	);

	// code point number
	bgm_out << code_point << std::endl;
	// hex representation of the number
	bgm_out	<< std::hex
		<< "0x"
		<< code_point
		<< std::dec
		<< std::endl;
	// the utf-8 sequence
	bgm_out << "'" << str << "'" << std::endl;
	//
	// vertex[0] logical rectangle metrics
	//
	// Left bearing (x)
	bgm_out << PANGO_LBEARING(log_rect)/font_size << std::endl;
	// Right bearing (x+width)
	bgm_out << PANGO_RBEARING(log_rect)/font_size << std::endl;
	// Ascent
	bgm_out << (baseline-log_rect.y)/font_size << std::endl;
	// Descent
	bgm_out << (log_rect.height+log_rect.y-baseline)/font_size << std::endl;
	//
	// vertex[1] ink rectangle metrics
	//
	// Left bearing (x)
	bgm_out << PANGO_LBEARING(ink_rect)/font_size << std::endl;
	// Right bearing (x+width)
	bgm_out << PANGO_RBEARING(ink_rect)/font_size << std::endl;
	// Ascent
	bgm_out << (baseline-ink_rect.y)/font_size << std::endl;
	// Descent
	bgm_out << (ink_rect.y+ink_rect.height-baseline)/font_size << std::endl;
	//
	// vertex[2] texture coordinates
	//
	// Origin X
	bgm_out << (cell_x*cell_size+ink_rect.x*inv_ps)/tex_size << std::endl;
	// Origin Y
	bgm_out << 1.0-(cell_y*cell_size+baseline*inv_ps)/tex_size << std::endl;
	// Width
	bgm_out << ((ink_rect.width)*inv_ps)/tex_size << std::endl;
	// Height
	bgm_out << ((ink_rect.height)*inv_ps)/tex_size << std::endl;


	// separating newline
	bgm_out << std::endl;

	cairo_new_path(cr);
	cairo_move_to(cr, cell_x*cell_size, cell_y*cell_size);
	cairo_set_line_width(cr, 0.5);

	pango_cairo_update_layout(cr, layout);
	pango_cairo_layout_path(cr, layout);
	cairo_fill(cr);

	g_object_unref(layout);
}

int main(int argc, const char* argv[])
{
	size_t tex_side =(argc>1)? std::atoi(argv[1]) : 512;
	const char* font_desc_str = (argc>2)? argv[2] : "Sans 18";
	unsigned plane = (argc>3)? std::atoi(argv[3]) : 0;

	cairo_surface_t *surface = cairo_image_surface_create(
		CAIRO_FORMAT_A8,
		tex_side,
		tex_side
	);
	cairo_t *cr = cairo_create(surface);

	PangoFontDescription *font_desc = pango_font_description_from_string(
		font_desc_str
	);
	PangoFontMap* font_map = pango_cairo_font_map_get_default();
	PangoContext* context = pango_font_map_create_context(font_map);
	PangoFont* font = pango_font_map_load_font(
		font_map,
		context,
		font_desc
	);
	PangoFontMetrics* font_metrics = pango_font_get_metrics(font, nullptr);

	// The Bitmap Glyph Metrics file
	std::ofstream bgm((argc>5) ? argv[5] : "out.bgm");
	unsigned step = tex_side / 16;
	for(unsigned y=0; y!=16; ++y)
	{
		for(unsigned x=0; x!=16; ++x)
		{
			render_glyph(
				cr,
				font_desc,
				font,
				256*plane + y*16 + x,
				x, y,
				step,
				tex_side,
				pango_font_metrics_get_ascent(font_metrics),
				pango_font_metrics_get_descent(font_metrics),
				bgm
			);
		}
	}
	bgm.close();

	pango_font_metrics_unref(font_metrics);
	pango_font_description_free(font_desc);
	g_object_unref(context);


	cairo_destroy(cr);
	cairo_status_t status = cairo_surface_write_to_png(
		surface,
		(argc>4) ? argv[4] : "out.png"
	);
	cairo_surface_destroy(surface);

	return 0;
}
