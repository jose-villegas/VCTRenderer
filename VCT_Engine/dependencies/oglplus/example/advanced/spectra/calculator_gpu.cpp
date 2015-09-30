/*
 *  .file advanced/spectra/calculator_gpu.cpp
 *  .brief Implements default signal spectrum calculators running on GPU
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2012-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/buffer.hpp>
#include <oglplus/program.hpp>
#include <oglplus/transform_feedback.hpp>
#include <oglplus/query.hpp>
#include <oglplus/texture.hpp>
#include <oglplus/context.hpp>

#include "calculator.hpp"
#include "shared_objects.hpp"

#include <vector>
#include <cassert>
#include <complex>
#include <functional>

class SpectraDefaultGPUMatrixTransf
 : public SpectraCalculator
{
private:
	wxGLContext* gl_context;
	wxGLCanvas* gl_canvas;

	GLuint max_transforms, current_transform;
	const std::size_t in_size, out_size;
	std::string name;

	oglplus::Program transf_prog;
	oglplus::Uniform<GLint> prog_input_offs;
	oglplus::Uniform<GLint> prog_input_size;
	oglplus::Uniform<GLint> prog_input_data;
	oglplus::Uniform<GLint> prog_matrix_data;

	oglplus::VertexArray vao;
	oglplus::Array<oglplus::Buffer> output_bufs;
	oglplus::Array<oglplus::TransformFeedback> xfbs;
	oglplus::Array<oglplus::Query> queries;
	oglplus::Buffer matrix_buf, input_buf, init_buf;
	oglplus::Texture matrix_tex, input_tex;

public:
	SpectraDefaultGPUMatrixTransf(
		SpectraSharedObjects&,
		std::size_t in_sz,
		std::size_t out_sz,
		const std::string& transf_name,
		const std::function<
			std::complex<double>(
				std::size_t,
				std::size_t,
				std::size_t,
				std::size_t
			)
		>& gen
	);

	std::size_t InputSize(void) const;

	std::size_t OutputSize(void) const;

	const char* Name(void) const;

	unsigned MaxConcurrentTransforms(void) const;

	void BeginBatch(void);

	void FinishBatch(void);

	unsigned BeginTransform(
		const float* input,
		std::size_t inbufsize,
		float* output,
		std::size_t outbufsize
	);

	void FinishTransform(
		unsigned tid,
		float* output,
		std::size_t outbufsize
	);
};

SpectraDefaultGPUMatrixTransf::SpectraDefaultGPUMatrixTransf(
	SpectraSharedObjects& shared_objects,
	std::size_t in_sz,
	std::size_t out_sz,
	const std::string& transf_name,
	const std::function<
		std::complex<double>(
			std::size_t,
			std::size_t,
			std::size_t,
			std::size_t
		)
	>& gen
): gl_context(shared_objects.GLContext())
 , gl_canvas(shared_objects.GLCanvas())
 , max_transforms(4)
 , current_transform(0)
 , in_size(in_sz)
 , out_size(out_sz)
 , name(transf_name)
 , transf_prog(shared_objects.BuildProgramWithXFB("xfb_matrix_transf.prog", "Output"))
 , prog_input_offs(transf_prog, "InputOffs")
 , prog_input_size(transf_prog, "InputSize")
 , prog_input_data(transf_prog, "InputData")
 , prog_matrix_data(transf_prog, "MatrixData")
 , output_bufs(max_transforms)
 , xfbs(max_transforms)
 , queries(max_transforms)
{
	assert(gl_context);
	assert(gl_canvas);

	std::vector<GLfloat> mat_data(in_size*out_size*2);
	auto m = mat_data.begin();
	for(std::size_t row=0; row!=out_size; ++row)
	{
		for(std::size_t col=0; col!=in_size; ++col)
		{
			std::complex<double> v = gen(col, row, in_size, out_size);
			*m = GLfloat(v.real()); ++m;
			*m = GLfloat(v.imag()); ++m;
		}
	}
	assert(m == mat_data.end());

	using namespace oglplus;

	matrix_buf.Bind(Buffer::Target::Texture);
	Buffer::Data(Buffer::Target::Texture, mat_data);
	Texture::Active(0);
	matrix_tex.Bind(Texture::Target::Buffer);
	Texture::Buffer(
		Texture::Target::Buffer,
		PixelDataInternalFormat::RG32F,
		matrix_buf
	);

	std::vector<GLfloat> init_data(in_size*max_transforms, 0.0f);

	input_buf.Bind(Buffer::Target::Texture);
	Buffer::Data(Buffer::Target::Texture, init_data);
	Texture::Active(1);
	input_tex.Bind(Texture::Target::Buffer);
	Texture::Buffer(
		Texture::Target::Buffer,
		PixelDataInternalFormat::R32F,
		input_buf
	);

	init_data.resize(out_size, 0.0f);

	vao.Bind();
	init_buf.Bind(Buffer::Target::Array);
	Buffer::Data(Buffer::Target::Array, init_data);
	(transf_prog|"Initial").Setup<GLfloat>().Enable();

	NoVertexArray().Bind();

	for(GLuint t=0; t!=max_transforms; ++t)
	{
		xfbs[t].Bind();

		output_bufs[t].Bind(Buffer::Target::TransformFeedback);
		Buffer::Data(
			Buffer::Target::TransformFeedback,
			init_data,
			Buffer::Property::Usage::DynamicCopy
		);
		output_bufs[t].BindBase(
			Buffer::IndexedTarget::TransformFeedback,
			0
		);
	}
}

std::size_t SpectraDefaultGPUMatrixTransf::InputSize(void) const
{
	return in_size;
}

std::size_t SpectraDefaultGPUMatrixTransf::OutputSize(void) const
{
	return out_size;
}

const char* SpectraDefaultGPUMatrixTransf::Name(void) const
{
	return name.c_str();
}

unsigned SpectraDefaultGPUMatrixTransf::MaxConcurrentTransforms(void) const
{
	return max_transforms;
}

void SpectraDefaultGPUMatrixTransf::BeginBatch(void)
{
	using namespace oglplus;
	gl_canvas->SetCurrent(*gl_context);
	transf_prog.Use();

	Texture::Active(0);
	matrix_tex.Bind(Texture::Target::Buffer);
	prog_matrix_data.Set(0);

	Texture::Active(1);
	input_tex.Bind(Texture::Target::Buffer);
	prog_input_data.Set(1);

	prog_input_size.Set(int(in_size));

	vao.Bind();

	Context gl;
	gl.Enable(Capability::RasterizerDiscard);
}

void SpectraDefaultGPUMatrixTransf::FinishBatch(void)
{
	using namespace oglplus;
	Context gl;
	gl.Disable(Capability::RasterizerDiscard);
	NoVertexArray().Bind();
}

unsigned SpectraDefaultGPUMatrixTransf::BeginTransform(
	const float* input,
	std::size_t inbufsize,
	float* /*output*/,
	std::size_t /*outbufsize*/
)
{
	using namespace oglplus;

	assert(inbufsize >= in_size);

	if(current_transform >= max_transforms)
		current_transform = 0;

	input_buf.Bind(Buffer::Target::Texture);
	Buffer::SubData(
		BufferTarget::Texture,
		BufferSize::Of<float>(current_transform*in_size),
		in_size,
		input
	);

	prog_input_offs.Set(int(current_transform*in_size));

	xfbs[current_transform].Bind();

	Query::Activator qrya(
		queries[current_transform],
		Query::Target::TransformFeedbackPrimitivesWritten
	);
	TransformFeedback::Activator xfba(
		TransformFeedbackPrimitiveType::Points
	);

	Context gl;
	gl.DrawArrays(PrimitiveType::Points, 0, out_size);

	xfba.Finish();
	qrya.Finish();

	DefaultTransformFeedback().Bind();

	return current_transform++;
}

void SpectraDefaultGPUMatrixTransf::FinishTransform(
	unsigned tid,
	float* output,
	std::size_t outbufsize
)
{
	using namespace oglplus;

	assert(tid < MaxConcurrentTransforms());
	assert(outbufsize >= out_size);
	GLuint count = 0;
	queries[tid].WaitForResult(count);
	assert(count <= out_size);

	output_bufs[tid].Bind(Buffer::Target::TransformFeedback);
	BufferTypedMap<GLfloat> map(
		BufferTarget::TransformFeedback,
		BufferMapAccess::Read
	);
	std::copy(map.Data(), map.Data()+count, output);
}

std::shared_ptr<SpectraCalculator>
SpectraGetDefaultGPUFourierTransf(
	SpectraSharedObjects& shared_objects,
	std::size_t spectrum_size
)
{
	std::size_t frame_size = spectrum_size*2-1;
	shared_objects.GLCanvas()->SetCurrent(*shared_objects.GLContext());
	assert(spectrum_size > 2);
	return std::make_shared<SpectraDefaultGPUMatrixTransf>(
		shared_objects,
		frame_size,
		spectrum_size,
		"Discrete Complex Fourier Transform (GPU)",
		SpectraFourierMatrixGen(frame_size, spectrum_size)
	);
}

