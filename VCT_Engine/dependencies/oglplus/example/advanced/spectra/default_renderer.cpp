/*
 *  .file advanced/spectra/default_renderer.cpp
 *  .brief Implements the document default renderer class.
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#include <oglplus/gl.hpp>
#include <oglplus/vertex_array.hpp>
#include <oglplus/context.hpp>
#include <oglplus/prog_var/optional.hpp>

#include "default_renderer.hpp"

#include <wx/settings.h>

class SpectraDefaultRenderer
 : public SpectraRenderer
{
private:
	GLfloat clear_r, clear_g, clear_b;
	GLfloat color_r, color_g, color_b;

	oglplus::Program doc_vis_prog;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> doc_vis_projection_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> doc_vis_camera_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> doc_vis_stretch_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> doc_vis_transf_matrix;
	oglplus::Optional<oglplus::Uniform<GLint>> doc_vis_spectrum_tex;
	oglplus::Optional<oglplus::Uniform<GLint>> doc_vis_spectrum_size;
	oglplus::Optional<oglplus::Uniform<GLint>> doc_vis_samples_per_unit;
	oglplus::Optional<oglplus::Uniform<GLfloat>> doc_vis_selected_time;
	oglplus::Optional<oglplus::Uniform<GLfloat>> doc_vis_selection_begin;
	oglplus::Optional<oglplus::Uniform<GLfloat>> doc_vis_selection_end;

	const oglplus::shapes::ShapeWrapper& spectrum_plane_wrap;
	oglplus::VertexArray spectrum_plane_vao;

	oglplus::Program vis_cue_prog;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> vis_cue_projection_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> vis_cue_camera_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> vis_cue_stretch_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> vis_cue_transf_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Vec3f>> vis_cue_color;
	oglplus::Optional<oglplus::Uniform<GLfloat>> vis_cue_alpha;

	oglplus::VertexArray ground_grid_vao, wall_grid_vao;

	void CacheBgColor(void);
	void RenderSpectrum(SpectraDocumentView& view);
	void RenderVisualCues(SpectraDocumentView& view);
public:
	SpectraDefaultRenderer(
		SpectraApp& app,
		const std::shared_ptr<SpectraSharedObjects>& sh_obj,
		const std::shared_ptr<SpectraVisualisation>& doc_vis,
		wxGLCanvas* canvas
	);

	void ReinitStyle(void);

	bool Interactive(void);

	void Render(SpectraDocumentView& view, wxGLCanvas* canvas);
};

std::shared_ptr<SpectraRenderer> SpectraMakeDefaultRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
)
{
	return std::make_shared<SpectraDefaultRenderer>(
		app,
		sh_obj,
		doc_vis,
		canvas
	);
}

SpectraDefaultRenderer::SpectraDefaultRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
): SpectraRenderer(app, sh_obj, doc_vis, canvas)
 , doc_vis_prog(Common().BuildProgram("default_doc_vis.prog"))
 , doc_vis_projection_matrix(doc_vis_prog, "ProjectionMatrix")
 , doc_vis_camera_matrix(doc_vis_prog, "CameraMatrix")
 , doc_vis_stretch_matrix(doc_vis_prog, "StretchMatrix")
 , doc_vis_transf_matrix(doc_vis_prog, "TransfMatrix")
 , doc_vis_spectrum_tex(doc_vis_prog, "SpectrumTex")
 , doc_vis_spectrum_size(doc_vis_prog, "SpectrumSize")
 , doc_vis_samples_per_unit(doc_vis_prog, "SamplesPerUnit")
 , doc_vis_selected_time(doc_vis_prog, "SelectedTime")
 , doc_vis_selection_begin(doc_vis_prog, "SelectionBegin")
 , doc_vis_selection_end(doc_vis_prog, "SelectionEnd")
 , spectrum_plane_wrap(
	Common().SpectrumPlane(
		GLuint(DocVis().GridSamples()),
		GLuint(DocVis().SignalSpectrumSize())
	)
), spectrum_plane_vao(spectrum_plane_wrap.VAOForProgram(doc_vis_prog))
 , vis_cue_prog(Common().BuildProgram("default_vis_cue.prog"))
 , vis_cue_projection_matrix(vis_cue_prog, "ProjectionMatrix")
 , vis_cue_camera_matrix(vis_cue_prog, "CameraMatrix")
 , vis_cue_stretch_matrix(vis_cue_prog, "StretchMatrix")
 , vis_cue_transf_matrix(vis_cue_prog, "TransfMatrix")
 , vis_cue_color(vis_cue_prog, "Color")
 , vis_cue_alpha(vis_cue_prog, "Alpha")
 , ground_grid_vao(Common().GroundUnitGrid().VAOForProgram(vis_cue_prog))
 , wall_grid_vao(Common().WallUnitGrid().VAOForProgram(vis_cue_prog))
{
	CacheBgColor();
}

void SpectraDefaultRenderer::CacheBgColor(void)
{
	GLfloat s = 1.0f/255.f;

	wxColor bg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
	clear_r = bg.Red()*s;
	clear_g = bg.Green()*s;
	clear_b = bg.Blue()*s;

	wxColor lc = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
	color_r = lc.Red()*s;
	color_g = lc.Green()*s;
	color_b = lc.Blue()*s;
}

void SpectraDefaultRenderer::RenderSpectrum(SpectraDocumentView& view)
{
	doc_vis_prog.Use();
	doc_vis_projection_matrix.TrySet(view.ProjectionMatrix());
	doc_vis_camera_matrix.TrySet(view.CameraMatrix());
	doc_vis_stretch_matrix.TrySet(view.StretchMatrix());
	doc_vis_transf_matrix.TrySet(view.TransfMatrix());

	oglplus::Texture::Active(0);
	DocVis().SpectrumTex().Bind(oglplus::Texture::Target::Buffer);
	doc_vis_spectrum_tex.TrySet(0);
	doc_vis_spectrum_size.TrySet(int(DocVis().SignalSpectrumSize()));
	doc_vis_samples_per_unit.TrySet(int(DocVis().SignalSamplesPerGrid()));
	doc_vis_selected_time.TrySet(DocVis().SelectedTime());
	doc_vis_selection_begin.TrySet(DocVis().SelectionBegin());
	doc_vis_selection_end.TrySet(DocVis().SelectionEnd());


	spectrum_plane_vao.Bind();
	spectrum_plane_wrap.Draw(view.SegmentCount());
}

void SpectraDefaultRenderer::RenderVisualCues(SpectraDocumentView& view)
{
	oglplus::Context gl;

	gl.Enable(oglplus::Capability::LineSmooth);
	gl.Enable(oglplus::Capability::Blend);
	gl.BlendFunc(
		oglplus::BlendFn::SrcAlpha,
		oglplus::BlendFn::OneMinusSrcAlpha
	);

	vis_cue_prog.Use();
	vis_cue_projection_matrix.TrySet(view.ProjectionMatrix());
	vis_cue_camera_matrix.TrySet(view.CameraMatrix());
	vis_cue_stretch_matrix.TrySet(view.StretchMatrix());
	vis_cue_transf_matrix.TrySet(view.TransfMatrix());
	vis_cue_color.TrySet(oglplus::Vec3f(color_r, color_g, color_b));
	vis_cue_alpha.TrySet(0.5f);

	gl.DepthMask(false);

	ground_grid_vao.Bind();
	Common().GroundUnitGrid().Draw(view.SegmentCount());

	wall_grid_vao.Bind();
	Common().WallUnitGrid().Draw(view.SegmentCount());

	gl.DepthMask(true);

	gl.Disable(oglplus::Capability::Blend);
	gl.Disable(oglplus::Capability::LineSmooth);
}

void SpectraDefaultRenderer::ReinitStyle(void)
{
	CacheBgColor();
}

bool SpectraDefaultRenderer::Interactive(void)
{
	return true;
}

void SpectraDefaultRenderer::Render(
	SpectraDocumentView& view,
	wxGLCanvas* canvas
)
{
	assert(canvas);
	canvas->SetCurrent(DocVis().GLContext());

	oglplus::Context gl;

	gl.Viewport(view.Width(), view.Height());

	gl.ClearColor(clear_r, clear_g, clear_b, 0.0);
	gl.ClearDepth(1.0f);

	gl.Clear().ColorBuffer().DepthBuffer();

	gl.Enable(oglplus::Capability::DepthTest);

	RenderSpectrum(view);
	RenderVisualCues(view);
}
