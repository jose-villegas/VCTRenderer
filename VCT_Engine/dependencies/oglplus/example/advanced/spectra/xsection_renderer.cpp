/*
 *  .file advanced/spectra/xsection_renderer.cpp
 *  .brief Implements the document cross-section renderer class.
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

#include "xsection_renderer.hpp"

#include <wx/settings.h>

class SpectraXSectionRenderer
 : public SpectraRenderer
{
private:
	GLfloat clear_r, clear_g, clear_b;
	GLfloat color_r, color_g, color_b;

	oglplus::Program xsection_prog;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> xsection_projection_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> xsection_camera_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> xsection_stretch_matrix;
	oglplus::Optional<oglplus::Uniform<oglplus::Mat4f>> xsection_transf_matrix;
	oglplus::Optional<oglplus::Uniform<GLint>> xsection_spectrum_tex;
	oglplus::Optional<oglplus::Uniform<GLint>> xsection_spectrum_size;
	oglplus::Optional<oglplus::Uniform<GLint>> xsection_samples_per_unit;
	oglplus::Optional<oglplus::Uniform<GLfloat>> xsection_selected_time;

	const oglplus::shapes::ShapeWrapper& spectrum_plane_wrap;
	oglplus::VertexArray spectrum_plane_vao;

	void CacheBgColor(void);
	void RenderSpectrum(SpectraDocumentView& view);
public:
	SpectraXSectionRenderer(
		SpectraApp& app,
		const std::shared_ptr<SpectraSharedObjects>& sh_obj,
		const std::shared_ptr<SpectraVisualisation>& doc_vis,
		wxGLCanvas* canvas
	);

	void ReinitStyle(void);

	bool Interactive(void);

	void Render(SpectraDocumentView& view, wxGLCanvas* canvas);
};

std::shared_ptr<SpectraRenderer> SpectraMakeXSectionRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
)
{
	return std::make_shared<SpectraXSectionRenderer>(
		app,
		sh_obj,
		doc_vis,
		canvas
	);
}

SpectraXSectionRenderer::SpectraXSectionRenderer(
	SpectraApp& app,
	const std::shared_ptr<SpectraSharedObjects>& sh_obj,
	const std::shared_ptr<SpectraVisualisation>& doc_vis,
	wxGLCanvas* canvas
): SpectraRenderer(app, sh_obj, doc_vis, canvas)
 , xsection_prog(Common().BuildProgram("xsection.prog"))
 , xsection_projection_matrix(xsection_prog, "ProjectionMatrix")
 , xsection_camera_matrix(xsection_prog, "CameraMatrix")
 , xsection_stretch_matrix(xsection_prog, "StretchMatrix")
 , xsection_transf_matrix(xsection_prog, "TransfMatrix")
 , xsection_spectrum_tex(xsection_prog, "SpectrumTex")
 , xsection_spectrum_size(xsection_prog, "SpectrumSize")
 , xsection_samples_per_unit(xsection_prog, "SamplesPerUnit")
 , xsection_selected_time(xsection_prog, "SelectedTime")
 , spectrum_plane_wrap(
	Common().SpectrumPlane(
		GLuint(DocVis().GridSamples()),
		GLuint(DocVis().SignalSpectrumSize())
	)
), spectrum_plane_vao(spectrum_plane_wrap.VAOForProgram(xsection_prog))
{
	CacheBgColor();
}

void SpectraXSectionRenderer::CacheBgColor(void)
{
	GLfloat s = 1.0f/255.f;

	wxColor bg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
	clear_r = bg.Red()*s;
	clear_g = bg.Green()*s;
	clear_b = bg.Blue()*s;

	wxColor lc = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
	color_r = lc.Red()*s;
	color_g = lc.Green()*s;
	color_b = lc.Blue()*s;
}

void SpectraXSectionRenderer::RenderSpectrum(SpectraDocumentView& view)
{
	float time = DocVis().SelectedTime();

	xsection_prog.Use();
	xsection_projection_matrix.TrySet(
		oglplus::CamMatrixf::Ortho(
			-0.5f, 0.5f,
			-0.5f, 0.5f,
			+1.0f, 1.01f
		)
	);
	xsection_camera_matrix.TrySet(
		oglplus::CamMatrixf::LookingAt(
			oglplus::Vec3f(time-1.0f, 0.5f, 0.5f),
			oglplus::Vec3f(time+0.0f, 0.5f, 0.5f)
		)
	);

	oglplus::Texture::Active(0);
	DocVis().SpectrumTex().Bind(oglplus::Texture::Target::Buffer);
	xsection_spectrum_tex.TrySet(0);
	xsection_spectrum_size.TrySet(int(DocVis().SignalSpectrumSize()));
	xsection_samples_per_unit.TrySet(int(DocVis().SignalSamplesPerGrid()));
	xsection_selected_time.TrySet(DocVis().SelectedTime());


	spectrum_plane_vao.Bind();
	spectrum_plane_wrap.Draw(view.SegmentCount());
}

void SpectraXSectionRenderer::ReinitStyle(void)
{
	CacheBgColor();
}

bool SpectraXSectionRenderer::Interactive(void)
{
	return false;
}

void SpectraXSectionRenderer::Render(
	SpectraDocumentView& view,
	wxGLCanvas* canvas
)
{
	assert(canvas);
	canvas->SetCurrent(DocVis().GLContext());

	oglplus::Context gl;

	gl.Viewport(view.Width(), view.Height());

	gl.ClearColor(clear_r, clear_g, clear_b, 0.0);

	gl.Clear().ColorBuffer();

	gl.Disable(oglplus::Capability::DepthTest);
	gl.Enable(oglplus::Capability::Blend);
	gl.BlendEquation(oglplus::BlendEq::Subtract);

	RenderSpectrum(view);

	gl.Disable(oglplus::Capability::Blend);
}
