/**
 *  .file tools/reshape_raw_cube.hpp
 *  .brief Tool for reshaping raw 3d texture date
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <cassert>

bool parse_dim(const char* str, int& w, int& h, int& d)
{
	std::stringstream ss(str);
	char x;
	if(!(ss >> w).good()) return false;
	if(!(ss >> x).good()) return false;
	if(!(ss >> h).good()) return false;
	if(!(ss >> x).good()) return false;
	if(!(ss >> d). eof()) return false;
	return true;
}

void parse_and_check_dim(
	int& arg,
	const int argc,
	const char* argv[],
	int& w,
	int& h,
	int& d
)
{
	if(arg+1 >= argc)
	{
		std::stringstream msg;
		msg << "Missing argument after '" << argv[arg] << "', ";
		msg << "expecting dimensions";
		throw std::runtime_error(msg.str());
	}
	else if(!parse_dim(argv[arg+1], w, h, d))
	{
		std::stringstream msg;
		msg << "Invalid argument " << arg+1 << ", ";
		msg << "error parsing dimensions: '";
		msg << argv[arg+1] << "'";
		throw std::runtime_error(msg.str());
	}
	else if(w <= 0)
	{
		std::stringstream msg;
		msg << "Invalid argument " << arg+1 << ", ";
		msg << "invalid width value '";
		msg << w << "' in specified dimensions.";
		throw std::runtime_error(msg.str());
	}
	else if(h <= 0)
	{
		std::stringstream msg;
		msg << "Invalid argument " << arg+1 << ", ";
		msg << "invalid height value '";
		msg << h << "' in specified dimensions.";
		throw std::runtime_error(msg.str());
	}
	else if(d <= 0)
	{
		std::stringstream msg;
		msg << "Invalid argument " << arg+1 << ", ";
		msg << "invalid depth value '";
		msg << d << "' in specified dimensions.";
		throw std::runtime_error(msg.str());
	}
	else
	{
		++arg;
	}
}

typedef unsigned char byte;

template <typename T>
void read_raw_data(std::istream& input, std::vector<T>& data)
{
	if(!input.good())
	{
		throw std::runtime_error("Failed reading input");
	}

	if(input.read((char*)data.data(), data.size()).gcount() < data.size())
	{
		throw std::runtime_error("Not enough data on input");
	}
}

template <typename T>
void write_raw_data(std::ostream& output, const std::vector<T>& data)
{
	if(!output.good())
	{
		throw std::runtime_error("Failed writing output");
	}

	output.write((const char*)data.data(), data.size());
}

struct reshape_info
{
	int off_x, off_y, off_z;
	int ori_w, ori_h, ori_d;
	int new_w, new_h, new_d;
	byte nullbyte;
};

byte translate_input(
	const reshape_info& r,
	byte value
)
{
	if(value == 0xFF) value = r.nullbyte;
	return value;
}

byte sample_input(
	int x, int y, int z,
	const reshape_info& r,
	const std::vector<byte>& input_data
)
{
	if(z < 0 || z >= r.ori_d) return r.nullbyte;
	if(y < 0 || y >= r.ori_h) return r.nullbyte;
	if(x < 0 || x >= r.ori_w) return r.nullbyte;

	int i = z*r.ori_w*r.ori_h + y*r.ori_w + x;

	return translate_input(r, input_data[i]);
}

void reshape_raw_cube(
	const reshape_info& r,
	const std::vector<byte>& input_data,
	std::vector<byte>& output_data
)
{
	auto o=output_data.begin();
	for(int oz=0; oz<r.new_d; ++oz)
	{
		int iz = oz-r.off_z;
		for(int oy=0; oy<r.new_h; ++oy)
		{
			int iy = oy-r.off_y;
			for(int ox=0; ox<r.new_w; ++ox)
			{
				int ix = ox-r.off_x;
				assert(o != output_data.end());
				*o++ = sample_input(ix, iy, iz, r, input_data);
			}
		}
	}
	assert(o == output_data.end());
}

int run(int argc, const char* argv[])
{
	reshape_info r = {
		0, 0, 0,
		0, 0, 0,
		512, 512, 512
	};
	const char* input_path = nullptr;
	const char* output_path = nullptr;

	for(int arg=1; arg<argc; ++arg)
	{
		if(std::strncmp(argv[arg], "-output", 7) == 0)
		{
			if(arg+1 >= argc)
			{
				throw std::runtime_error("Missing output path.");
			}
			output_path = argv[arg+1];
			++arg;
		}
		else if(std::strncmp(argv[arg], "-offset", 7) == 0)
		{
			parse_and_check_dim(arg, argc, argv, r.off_x, r.off_y, r.off_z);
		}
		else if(std::strncmp(argv[arg], "-ori-dim", 8) == 0)
		{
			parse_and_check_dim(arg, argc, argv, r.ori_w, r.ori_h, r.ori_d);
		}
		else if(std::strncmp(argv[arg], "-new-dim", 8) == 0)
		{
			parse_and_check_dim(arg, argc, argv, r.new_w, r.new_h, r.new_d);
		}
		else
		{
			input_path = argv[arg];
		}
	}

	if(r.ori_w*r.ori_h*r.ori_d == 0)
	{
		throw std::runtime_error("Missing input dimensions.");
	}

	if(r.off_x*r.off_y*r.off_z == 0)
	{
		r.off_x = (r.new_w-r.ori_w)/2;
		r.off_y = (r.new_h-r.ori_h)/2;
		r.off_z = (r.new_d-r.ori_d)/2;
	}

	std::vector<byte> input_data(r.ori_w*r.ori_h*r.ori_d);
	if(input_path)
	{
		std::ifstream input(input_path);
		read_raw_data(input, input_data);
	}
	else
	{
		read_raw_data(std::cin, input_data);
	}

	std::vector<byte> output_data(r.new_w*r.new_h*r.new_d);

	reshape_raw_cube(r, input_data, output_data);

	if(output_path)
	{
		std::ofstream output(output_path);
		write_raw_data(output, output_data);
	}
	else
	{
		write_raw_data(std::cout, output_data);
	}

	return 0;
}

int main(int argc, const char* argv[])
{
	try { return run(argc, argv); }
	catch(std::exception& error)
	{
		std::cerr << "Error: ";
		std::cerr << error.what();
		std::cerr << std::endl;
	}
}
