/**
 *  @example standalone/001_shape2dot.cpp
 *  @brief Shows the basic usage of the mesh analyzer tool
 *
 *  Can be used with graphviz neato to render an image
 *  of the generated adjacency graph:
 *  @code
 *  ./001_shape2dot | neato -T png > 001_shape2dot.png
 *  @endcode
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>

#include <oglplus/shapes/analyzer.hpp>
#include <oglplus/shapes/sphere.hpp>

#include <oglplus/opt/resources.hpp>
#include <oglplus/opt/list_init.hpp>

#include <oglplus/math/matrix.hpp>

int main(void)
{
	try
	{
		using namespace oglplus;

		shapes::Sphere s(1.0, 3, 4);
		shapes::ShapeAnalyzer a(s);

		std::cout << "graph Shape {" << std::endl;

		std::cout << "\tsplines=true;" << std::endl;
		std::cout << "\tnode [shape=box];" << std::endl;

		auto tm =
			ModelMatrixd::Scale(5, 5, 5)*
			CamMatrixd::PerspectiveX(Angle<double>::Degrees(80), 1, 1, 20)*
			CamMatrixd::LookingAt(Vec3d(5, 5, 5), Vec3d());

		for(GLuint f=0; f!=a.FaceCount(); ++f)
		{
			auto face = a.Face(f);
			Vec4d v0 = face.Vert(0).MainAttrib();
			Vec4d v1 = face.Vert(1).MainAttrib();
			Vec4d v2 = face.Vert(2).MainAttrib();

			Vec4d v = tm*Vec4d((v0+v1+v2).xyz()/3.0, 1.0);
			std::cout
				<< "\tf" << f
				<< " [label=\"" << f << "\""
				<< ", pos=\"" << v.x() << "," << v.y() << "\""
				<< "];" << std::endl;
		}

		std::cout
			<< "\tnode ["
			<< "shape=circle,"
			<< "style=filled,"
			<< "fillcolor=\"#00FF00\""
			<< "];" << std::endl;
		for(GLuint f=0; f!=a.FaceCount(); ++f)
		{
			auto face = a.Face(f);
			for(GLuint e=0; e!=face.Arity(); ++e)
			{
				std::cout
					<< "\tf" << f
					<< "e" << e
					<< " [label=\"" << e
					<< "\"];" << std::endl;
				std::cout
					<< "\tf" << f
					<< " -- "
					<< "f" << f
					<< "e" << e
					<< ";" << std::endl;
			}
		}

		for(GLuint f=0; f!=a.FaceCount(); ++f)
		{
			auto face = a.Face(f);
			for(GLuint e=0; e!=face.Arity(); ++e)
			{
				auto edge = face.Edge(e);
				if(edge.HasAdjacentEdge())
				{
					auto adje = edge.AdjacentEdge();
					GLuint ae = adje.Index();
					GLuint af = adje.Face().Index();

					if(f < af)
					{
						std::cout
							<< "\tf" << f
							<< "e" << e
							<< " -- f" << af
							<< "e" << ae;
						if(edge.IsStripEdge())
						{
							std::cout << " [style=bold]";
						}
						else
						{
							std::cout << " [style=dotted]";
						}
						std::cout << std::endl;
					}
				}
			}
		}
		std::cout << "\toverlap=false;" << std::endl;
		std::cout << "}" << std::endl;

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
	return 1;
}
