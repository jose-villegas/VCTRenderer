/**
 *  @example standalone/002_amd_perf_monitor.cpp
 *  @brief Shows simple usage of the AMD_performance_monitor extension
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <oglplus/all.hpp>
#include <oglplus/opt/smart_enums.hpp>
#include <oglplus/ext/AMD_performance_monitor.hpp>

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OGLplus+GLUT+GLEW");

	if(glewInit() == GLEW_OK) try
	{
		glGetError();
		namespace se = oglplus::smart_enums;
		oglplus::Context gl;
		oglplus::AMD_performance_monitor apm;

		const char* f1[2] = {"  +--", "  `--"};
		const char* f2[2] = {"  |", "   "};
		const char* f3[2] = {"  +--", "  `--"};
		const char* f4[2] = {"  |", ""};

		auto groups = apm.GetGroups();
		auto gi=groups.begin(), ge=groups.end();

		std::cout << "--+-{Performance monitor groups}" << std::endl;
		if(gi != ge) std::cout << "  |" << std::endl;

		while(gi != ge)
		{
			auto group = *gi;
			++gi;
			const int fgi = (gi == ge)?1:0;

			std::cout << f1[fgi] << "+-[";
			std::cout << group.GetString();
			std::cout << "]" << std::endl;
			std::cout << f2[fgi] << f4[0] << std::endl;

			GLint max;
			auto counters = group.GetCounters(max);
			auto ci=counters.begin(), ce=counters.end();

			while(ci != ce)
			{
				auto counter = *ci;
				++ci;
				const int fci = (ci == ce)?1:0;
				std::cout << f2[fgi] << f3[fci] << "(";
				std::cout << counter.GetString();
				std::cout << ") [";
				std::cout << EnumValueName(counter.Type());
				std::cout << "]" << std::endl;
				std::cout << f2[fgi] << f4[fci] << std::endl;
			}
			if(fgi != 0)
			{
				oglplus::PerfMonitorAMD mon;
				mon.SelectCounters(true, counters);
				mon.Begin();
				// Imagine some heavy duty rendering here
				gl.Clear().ColorBuffer();
				mon.End();

				if(mon.ResultAvailable())
				{
					std::vector<oglplus::PerfMonitorAMDResult>
						values;
					mon.Result(values);

					auto ri=values.begin(), re=values.end();
					while(ri != re)
					{
						auto counter = ri->Counter();
						auto type = counter.Type();

						std::cout << counter.GetString();
						std::cout << " [";
						std::cout << EnumValueName(type);
						std::cout << "] = ";
						if(type == se::UnsignedInt())
							std::cout << ri->Low();
						else if(type == se::Float())
							std::cout << ri->Float();
						else if(type == se::Percentage())
							std::cout << ri->Float() << "%";
						else if(type == se::UnsignedInt64())
							std::cout << "too big";
						std::cout << std::endl;
						++ri;
					}
				}
			}
		}
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

