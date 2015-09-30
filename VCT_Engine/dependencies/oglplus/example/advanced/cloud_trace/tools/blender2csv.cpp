/**
 *  @file advanced/cloud_trace/tools/blender2csv.cpp
 *  @brief Generates a set of cross-linked html describing a .blend file
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include <oglplus/imports/blend_file.hpp>
#include <oglplus/math/matrix.hpp>

bool find_blend_obj_prop(
	oglplus::imports::BlendFile& blend_file,
	oglplus::imports::BlendFilePointer prop_ptr,
	const char* name,
	double& value
)
{
	using namespace oglplus::imports;

	if(prop_ptr)
	{
		auto prop = blend_file[prop_ptr];

		if(std::strcmp(prop.Field<std::string>("name").Get().c_str(), name) == 0)
		{
			if(prop.Field<char>("type").Get() == 0x8)
			{
				try
				{
					int data[2] = {
						prop.Field<int>("data.val").Get(),
						prop.Field<int>("data.val2").Get()
					};
					value = *(reinterpret_cast<double*>(data));

					return true;
				}
				catch(std::runtime_error&) { }
			}
		}
		const char* child_names[] = {"data.group.first", "next", nullptr};
		const char** child_name = child_names;

		while(*child_name)
		{
			prop_ptr = prop.Field<void*>(*child_name).Get();
			if(prop_ptr)
			{
				if(find_blend_obj_prop(
					blend_file,
					prop_ptr,
					name,
					value
				)) return true;
			}
			++child_name;
		}
	}
	return false;
}


void blend_to_csv(const std::string& input_path, const std::string& output_path)
{
	using namespace oglplus::imports;

	std::ifstream input(input_path);
	if(!input.good())
	{
		throw std::runtime_error(
			"Error opening input file "
			"'"+input_path+"'."
		);
	}

	std::ofstream output_file;
	bool output_to_cout = (output_path == "-");
	if(!output_to_cout)
	{
		output_file.open(output_path);
		if(!output_file.good())
		{
			throw std::runtime_error(
				"Error opening file output file "
				"'"+output_path+"'."
			);
		}
	}
	std::ostream& output = output_to_cout?std::cout:output_file;

	// the header containing list of columns
	output << "pos_x|pos_y|pos_z|";
	output << "rotxx|rotxy|rotxz|";
	output << "rotyx|rotyy|rotyz|";
	output << "rotzx|rotzy|rotzz|";
	output << "csize|doffs|dmult|";
	output << "c_age|";

	output << "name";
	output << std::endl;

	//
	BlendFile blend_file(input);
	auto glob_block = blend_file.StructuredGlobalBlock();
	auto scene_data = blend_file[glob_block.curscene];

	auto object_link_ptr = scene_data.Field<void*>("base.first").Get();

	while(object_link_ptr)
	{
		auto object_link_data = blend_file[object_link_ptr];
		auto object_ptr = object_link_data.Field<void*>("object").Get();

		if(object_ptr)
		{
			auto object = blend_file[object_ptr];
			short type = object.Field<short>("type").Get();

			if(type == 1) // TODO: other types?
			{
				double pos_x = 0, pos_y = 0, pos_z = 0;
				double rotxx = 1, rotxy = 0, rotxz = 0;
				double rotyx = 0, rotyy = 1, rotyz = 0;
				double rotzx = 0, rotzy = 0, rotzz = 1;
				double csize = 1, doffs = 0, dmult = 1;
				double c_age = 0;

				try
				{
					using oglplus::Radians;
					using oglplus::ModelMatrix;

					typedef ModelMatrix<double> MMd;

					auto object_rot = object.Field<float>("rot");
					auto rot_m =
						MMd::RotationY(Radians(-object_rot.Get(0, 1)))*
						MMd::RotationZ(Radians(+object_rot.Get(0, 2)))*
						MMd::RotationX(Radians(+object_rot.Get(0, 0)));

					rotxx = rot_m.At(0, 0);
					rotxy = rot_m.At(0, 1);
					rotxz = rot_m.At(0, 2);
					rotyx = rot_m.At(1, 0);
					rotyy = rot_m.At(1, 1);
					rotyz = rot_m.At(1, 2);
					rotzx = rot_m.At(2, 0);
					rotzy = rot_m.At(2, 1);
					rotzz = rot_m.At(2, 2);
				}
				catch(std::runtime_error&) { }

				try
				{
					auto object_pos = object.Field<float>("loc");
					pos_x = +object_pos.Get(0, 0);
					pos_y = +object_pos.Get(0, 2);
					pos_z = -object_pos.Get(0, 1);
				}
				catch(std::runtime_error&) { }

				try
				{
					auto object_size = object.Field<float>("size");
					csize  = object_size.Get(0, 0);
					csize += object_size.Get(0, 1);
					csize += object_size.Get(0, 2);
					csize /= 3.0f;
				}
				catch(std::runtime_error&) { }

				const char* prop_names[] = {"c_age", "doffs", "dmult"};
				double*    prop_values[] = {&c_age , &doffs , &dmult };

				for(int p=0; p!=3; ++p)
				{
					try
					{
						find_blend_obj_prop(
							blend_file,
							object.Field<void*>("id.properties").Get(),
							prop_names[p], *prop_values[p]
						);
					}
					catch(std::runtime_error&) { }
				}

				output << pos_x << "|" << pos_y << "|" << pos_z << "|";
				output << rotxx << "|" << rotxy << "|" << rotxz << "|";
				output << rotyx << "|" << rotyy << "|" << rotyz << "|";
				output << rotzx << "|" << rotzy << "|" << rotzz << "|";
				output << csize << "|" << doffs << "|" << dmult << "|";
				output << c_age << "|";

				output << object.Field<std::string>("id.name").Get().c_str();
				output << std::endl;
			}
		}

		object_link_ptr = object_link_data.Field<void*>("next").Get();
	}
	output_file.close();
}

void print_usage(std::ostream& output)
{
	output
		<< "Usage: blender2csv "
		<< "[input-file.blend] [output-file.csv]"
		<< std::endl;
	output	<< std::endl;
}

int main(int argc, const char* argv[])
{
	try
	{
		if((argc>1) && (std::strcmp(argv[1], "--help") == 0))
		{
			print_usage(std::cout);
		}
		else blend_to_csv(
			argc>1?argv[1]:"input.blend", // input file path
			argc>2?argv[2]:"output.csv"   //output file path
		);
	}
	catch(std::exception& error)
	{
		std::cerr
			<< "Error: "
			<< error.what()
			<< std::endl;
		print_usage(std::cerr);
		return 1;
	}
	return 0;
}


