/**
 *  @file advanced/cloud_trace/cloud_data.cpp
 *  @brief Implements data structures storing cloud parameters
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "cloud_data.hpp"

#include <oglplus/math/matrix.hpp>
#include <oglplus/math/angle.hpp>

#include <map>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace oglplus {
namespace cloud_trace {

CloudData::CloudData(const AppData& app_data)
{
	if(app_data.cloud_data_path.empty())
	{
		Generate(app_data);
	}
	else Load(app_data);
}

void CloudData::LoadCSV(const AppData& app_data, std::istream& input)
{
	const char sep = '|';
	std::size_t ncol = 0;

	typedef std::pair<std::string, std::size_t> col_pos;
	std::map<std::string, std::size_t> col_map;

	std::stringbuf buf;

	if(input.get(buf).good())
	{
		input.ignore();
		std::istream header(&buf);
		while(header.good())
		{
			std::stringbuf col;
			header.get(col, sep);

			if(col.str().empty()) break;
			header.ignore();

			if(!col_map.insert(col_pos(col.str(), ncol++)).second)
			{
				std::string message;
				message.append("Duplicate column name '");
				message.append(col.str());
				message.append("' in CSV file header");
				throw std::runtime_error(message);
			}
		}
		buf.str(std::string());
	}

	std::default_random_engine re(app_data.rand_seed);
	std::uniform_real_distribution<float> r01( 0, 1);

	while(input.get(buf).good())
	{
		input.ignore();
		std::istream data(&buf);

		std::vector<std::stringbuf> row(ncol);

		for(std::size_t c=0; c!=ncol; ++c)
		{
			data.get(row[c], sep);
			if(data.good()) data.ignore();
		}

		const std::size_t nattr = 17;
		const char* attr_names[nattr] = {
			"rotxx", "rotxy", "rotxz",
			"rotyx", "rotyy", "rotyz",
			"rotzx", "rotzy", "rotzz",
			"pos_x", "pos_y", "pos_z",
			"doffs", "dmult",
			"c_age", "csize",
			"rot_r"
		};

		float attr_values[nattr] = {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
			0, 0, 0,
			0, 1,
			0, 1,
			0
		};

		for(std::size_t a=0; a!=nattr; ++a)
		{
			auto p = col_map.find(attr_names[a]);
			if(p == col_map.end()) continue;

			std::istream cell(&row[p->second]);
			cell >> attr_values[a];
		}

		Mat4f cloud;

		if(attr_values[16] != 0)
		{
			float rot = attr_values[16];

			if((rot - int(rot)) == 0)
				rot = r01(re);

			cloud = ModelMatrixf::RotationA(
				Vec3f(r01(re), r01(re), r01(re)),
				FullCircles(rot)
			);
		}
		else
		{
			// rotation
			cloud.Set(0, 0, attr_values[ 0]);
			cloud.Set(0, 1, attr_values[ 1]);
			cloud.Set(0, 2, attr_values[ 2]);
			cloud.Set(1, 0, attr_values[ 3]);
			cloud.Set(1, 1, attr_values[ 4]);
			cloud.Set(1, 2, attr_values[ 5]);
			cloud.Set(2, 0, attr_values[ 6]);
			cloud.Set(2, 1, attr_values[ 7]);
			cloud.Set(2, 2, attr_values[ 8]);
		}
		// position
		cloud.Set(0, 3, attr_values[ 9]);
		cloud.Set(1, 3, attr_values[10]);
		cloud.Set(2, 3, attr_values[11]);
		// other
		cloud.Set(3, 0, attr_values[12]);
		cloud.Set(3, 1, attr_values[13]);
		cloud.Set(3, 2, attr_values[14]);
		// size
		cloud.Set(3, 3, attr_values[15]);

		storage.push_back(cloud);

		buf.str(std::string());
	}
}

void CloudData::Load(const AppData& app_data)
{
	const std::string& cdp = app_data.cloud_data_path;

	std::ifstream input_file;
	input_file.exceptions(std::ifstream::badbit);
	try
	{
		input_file.open(cdp.c_str());
		std::istream& input = input_file;

		if(cdp.rfind(".csv") ==  cdp.size()-4)
		{
			LoadCSV(app_data, input);
		}
		else
		{
			std::string message;
			message.append("Unknown cloud data file format '");
			message.append(cdp);
			message.append("'");
			throw std::runtime_error(message);
		}
	}
	catch(std::ifstream::failure& f)
	{
		std::string message;
		message.append("Loading cloud data file '");
		message.append(cdp);
		message.append("' failed with: ");
		message.append(f.what());
		throw std::runtime_error(message);
	}
}

void CloudData::Generate(const AppData& app_data)
{
	storage.clear();
	storage.reserve(app_data.cloud_count);

	float radius = app_data.planet_radius;
	float cloud_alt = app_data.cloud_mean_alt;
	float cloud_alt_disp = app_data.cloud_alt_disp;

	float cloud_size = app_data.cloud_mean_size;
	float cloud_size_disp = app_data.cloud_size_disp;

	Anglef angle = Degrees(app_data.covered_angle);
	Vec3f center(0,-radius, 0);
	Mat4f cloud;

	std::size_t c = 0, count = app_data.cloud_count;

	if(radius > 0)
	{
		cloud = ModelMatrixf::Translation(center);

		cloud.Set(3, 3, radius-1);
		cloud.Set(3, 0,-100);
		cloud.Set(3, 1, 1);

		storage.push_back(cloud);
		++c;
	}

	std::default_random_engine re(app_data.rand_seed);
	std::uniform_real_distribution<float> r01( 0, 1);
	std::uniform_real_distribution<float> r11(-1, 1);
	std::uniform_int_distribution<unsigned> rcc( 3, 8);

	if(app_data.verbosity > 3)
	{
		app_data.logstr()
			<< "Generating "
			<< count
			<< " clouds"
			<< std::endl;
	}

	while(c < count)
	{
		auto lat = angle*r11(re)*0.5;
		auto lon= angle*r11(re)*0.5;

		Vec3f cpos = center +
			Vec3f(Cos(lat)*Sin(lon), Cos(lat)*Cos(lon), Sin(lat))*
			(radius + cloud_alt + cloud_alt_disp*r11(re));
		Vec3f pos = cpos;

		float size = cloud_size+cloud_size_disp*r11(re);

		unsigned cc = rcc(re);
		while((c < count) && (cc > 0))
		{
			cloud = ModelMatrixf::Translation(pos)*
				ModelMatrixf::RotationA(
					Vec3f(r01(re), r01(re), r01(re)),
					FullCircles(r01(re))
				);

			cloud.Set(3, 3, size);
			cloud.Set(3, 0, r01(re)*0.5f);
			cloud.Set(3, 1, 0.1f+0.9f*r01(re));

			storage.push_back(cloud);

			Vec3f offs(r11(re)*1.4f, r11(re)*0.1f, r11(re));
			offs.Normalize();

			pos = cpos + offs*size;

			++c;
			--cc;
		}
	}
}

CloudBuffer::CloudBuffer(
	const AppData&,
	const CloudData& cloud_data,
	ResourceAllocator& alloc
): count(cloud_data.storage.size())
 , ub_idx(alloc.GetNextUniformIndex())
{
	*this	<< BufferTarget::Uniform
		<< cloud_data.storage;
	*this	<< BufferIndexedTarget::Uniform << ub_idx;
}

void CloudBuffer::Use(void)
{
	*this	<< BufferIndexedTarget::Uniform << ub_idx;
}

} // namespace cloud_trace
} // namespace oglplus
