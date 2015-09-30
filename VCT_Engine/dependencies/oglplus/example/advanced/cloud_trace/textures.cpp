/**
 *  @file advanced/cloud_trace/textures.cpp
 *  @brief Implementation of texture classes
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "textures.hpp"

#include <oglplus/images/cloud.hpp>
#include <oglplus/images/image_spec.hpp>

#include <fstream>
#include <string>

namespace oglplus {
namespace cloud_trace {

CloudVolume::CloudVolume(const AppData& app_data)
 : image(GenOrLoad(app_data))
{
	if(app_data.dump_cloud_image)
	{
		Save(app_data);
	}
}

images::Image CloudVolume::GenOrLoad(const AppData& app_data)
{
	if(app_data.cloud_image_path.empty() || app_data.dump_cloud_image)
	{
		return Generate(app_data);
	}
	else
	{
		return Load(app_data);
	}
}

images::Image CloudVolume::Generate(const AppData& app_data)
{
	return images::Cloud(
			app_data.cloud_res,
			app_data.cloud_res,
			app_data.cloud_res,
			Vec3f(),
			0.45f,
			0.333f,
			0.55f,
			0.005f
	);
}

images::Image CloudVolume::Load(const AppData& app_data)
{
	const std::string& cip = app_data.cloud_image_path;

	GLsizei channels = 0;
	std::vector<GLubyte> storage;
	PixelDataFormat format;
	PixelDataInternalFormat iformat;

	if(cip.rfind(".r8") ==  cip.size()-3)
	{
		format = PixelDataFormat::Red;
		iformat = PixelDataInternalFormat::Red;
		channels = 1;

		std::ifstream input;
		input.exceptions(std::ifstream::failbit|std::ifstream::badbit);
		try
		{
			input.open(cip.c_str());
			storage.resize(
				app_data.cloud_res*
				app_data.cloud_res*
				app_data.cloud_res
			);
			input.read((char*)storage.data(), storage.size());
			if(input.gcount() < std::streamsize(storage.size()))
			{
				std::string message;
				message.append("Not enough data in file '");
				message.append(cip);
				message.append("' for specified image size");
				throw std::runtime_error(message);
			}
		}
		catch(std::ifstream::failure& f)
		{
			std::string message;
			message.append("Loading image file '");
			message.append(cip);
			message.append("' failed with: ");
			message.append(f.what());
			throw std::runtime_error(message);
		}
	}
	else
	{
		std::string message;
		message.append("Unsupported image file format '");
		message.append(cip);
		message.append("'");
		throw std::runtime_error(message);
	}

	return images::Image(
		app_data.cloud_res,
		app_data.cloud_res,
		app_data.cloud_res,
		channels,
		storage.data(),
		format,
		iformat
	);
}

void CloudVolume::Save(const AppData& app_data)
{
	const std::string& cip = app_data.cloud_image_path;

	std::ofstream output;
	output.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	if(cip.empty())
	{
		std::string message;
		message.append("No output image file specified");
		throw std::runtime_error(message);
	}

	try { output.open(cip.c_str()); }
	catch(std::ifstream::failure& f)
	{
		std::string message;
		message.append("Opening output image file '");
		message.append(cip);
		message.append("' failed with: ");
		message.append(f.what());
		throw std::runtime_error(message);
	}

	try { output.write((const char*)image.RawData(), image.DataSize()); }
	catch(std::ifstream::failure& f)
	{
		std::string message;
		message.append("Writing to output image file '");
		message.append(cip);
		message.append("' failed with: ");
		message.append(f.what());
		throw std::runtime_error(message);
	}
}

CloudTexture::CloudTexture(
	const AppData&,
	const CloudVolume& cloud_vol,
	ResourceAllocator& alloc
): tex_unit(alloc.GetNextTexUnit())
{
	Texture::Active(tex_unit);

	*this	<< TextureTarget::_3D
		<< TextureWrap::ClampToBorder
		<< TextureFilter::Linear
		<< cloud_vol.image;
}

void CloudTexture::Use(void)
{
	Texture::Active(tex_unit);
	*this	<< TextureTarget::_3D;
}

} // namespace cloud_trace
} // namespace oglplus
