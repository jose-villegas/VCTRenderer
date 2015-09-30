/*
 *  Copyright 2012-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330
//TODO: #extension GL_ARB_gpu_shader_fp64 : enable
#define complex vec2

uniform int InputOffs, InputSize;
uniform samplerBuffer InputData, MatrixData;
in float Initial;
out float Output;

complex Input(int j)
{
	return complex(texelFetch(InputData, InputOffs+j).r, 0.0);
}

complex Matrix(int i, int j)
{
	return complex(texelFetch(MatrixData, i*InputSize+j).rg);
}

complex Multiply(complex a, complex b)
{
	return complex(
		a.x*b.x-a.y*b.y,
		(a.x+a.y)*(b.x+b.y)-a.x*b.x-a.y*b.y
	);
}

void main(void)
{
	int i = gl_VertexID;
	complex sum = complex(Initial, 0.0);
	for(int j=0; j!=InputSize; ++j)
	{
		sum += Multiply(Matrix(i, j), Input(j));
	}
	Output = float(length(sum));
}

