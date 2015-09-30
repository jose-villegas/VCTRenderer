/**
 *  @example eglplus/002_configurations.cpp
 *  @brief Shows basic usage of EGLplus by printing configuration information
 *
 *  Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <eglplus/egl.hpp>
#include <eglplus/all.hpp>

#include <iostream>
#include <iomanip>

int main(void)
{
	// open the default display
	eglplus::Display display;
	// initialize the library
	eglplus::LibEGL egl(display);
	// get the list of all available configurations
	eglplus::Configs configs(display);
	//
	int w = 20;
	for(auto cr=configs.All(); !cr.Empty(); cr.Next())
	{
		auto cfg = cr.Front();
		std::cout << "Config ID = " << cfg.ConfigId() << std::endl;
		std::cout << "\tRed size:          " << std::setw(w) <<
			cfg.RedSize() << std::endl;
		std::cout << "\tGreen size:        " << std::setw(w) <<
			cfg.GreenSize() << std::endl;
		std::cout << "\tBlue size:         " << std::setw(w) <<
			cfg.BlueSize() << std::endl;
		std::cout << "\tAlpha size:        " << std::setw(w) <<
			cfg.AlphaSize() << std::endl;
		std::cout << "\tDepth size:        " << std::setw(w) <<
			cfg.DepthSize() << std::endl;
		std::cout << "\tStencil size:      " << std::setw(w) <<
			cfg.StencilSize() << std::endl;
		std::cout << "\tMax Pbuffer width: " << std::setw(w) <<
			cfg.MaxPbufferWidth() << std::endl;
		std::cout << "\tMax Pbuffer height:" << std::setw(w) <<
			cfg.MaxPbufferHeight() << std::endl;
		std::cout << "\tMax Pbuffer pixels:" << std::setw(w) <<
			cfg.MaxPbufferPixels() << std::endl;

		std::cout << "\tConfig caveat:     " << std::setw(w) <<
			eglplus::EnumValueName(cfg.ConfigCaveat()).c_str() << std::endl;
		std::cout << "\tColor buffer type: " << std::setw(w) <<
			eglplus::EnumValueName(cfg.ColorBufferType()).c_str() << std::endl;
		std::cout << "\tTransparent type:  " << std::setw(w) <<
			eglplus::EnumValueName(cfg.TransparentType()).c_str() << std::endl;

		std::cout << "\tRenderable types:  " << std::setw(w);
		auto rtr=eglplus::EnumValueRange<eglplus::RenderableTypeBit>();
		while(!rtr.Empty())
		{
			if(cfg.HasRenderableType(rtr.Front()))
			{
				std::cout << eglplus::EnumValueName(rtr.Front()).c_str();
				std::cout << '|';
			}
			rtr.Next();
		}
		std::cout << std::endl;

		std::cout << "\tSurface types:     " << std::setw(w);
		auto str=eglplus::EnumValueRange<eglplus::SurfaceTypeBit>();
		while(!str.Empty())
		{
			if(cfg.HasSurfaceType(str.Front()))
			{
				std::cout << eglplus::EnumValueName(str.Front()).c_str();
				std::cout << '|';
			}
			str.Next();
		}
		std::cout << std::endl;

		std::cout << "\tConformant to:     " << std::setw(w);
		auto ctr=eglplus::EnumValueRange<eglplus::RenderableTypeBit>();
		while(!ctr.Empty())
		{
			if(cfg.IsConformantTo(ctr.Front()))
			{
				std::cout << eglplus::EnumValueName(ctr.Front()).c_str();
				std::cout << '|';
			}
			ctr.Next();
		}
		std::cout << std::endl;

		std::cout << "-----------------------------------------------" << std::endl;
	}
	//
	return 0;
}
