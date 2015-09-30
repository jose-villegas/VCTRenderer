/**
 *  @example standalone/010_blender2html.cpp
 *  @brief Generates a set of cross-linked html describing a .blend file
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

#include <oglplus/imports/blend_file.hpp>

using namespace oglplus::imports;

// Prints the atomic values from a .blend file
struct value_visitor : BlendFileVisitor
{
	std::ostream& _output;
	BlendFile& _file;

	value_visitor(std::ostream& out, BlendFile& bf)
	 : _output(out)
	 , _file(bf)
	{ }

	// string
	void VisitStr(const std::string& value)
	{
		_output << "<em>'";
		for(char c : value)
		{
			if(c == '\0') break;
			else if(std::isprint(c))
			{
				_output << c;
			}
			else
			{
				_output << "\\"
					<< std::setw(3)
					<< std::setfill('0')
					<< std::oct
					<< (c & 0xFF)
					<< std::dec;
			}
		}
		_output << "'</em>";
	}

	// pointer
	void VisitPtr(BlendFilePointer ptr)
	{
		BlendFilePointer::ValueType pv = ptr.Value();
		bool found_block = false;
		try
		{
			pv = _file.BlockByPointer(ptr, true).Pointer().Value();
			found_block = true;
		}
		catch(...){ }

		if(found_block)
		{
			_output << "<a href='block-0x";
			_output << std::hex << pv << std::dec;
			_output << ".html'>";
		}
		_output << "0x";
		_output << std::hex << pv << std::dec;
		if(pv != ptr.Value())
		{
			_output
				<< "+"
				<< std::hex
				<< (ptr.Value() - pv)
				<< std::dec;
		}
		if(found_block)
		{
			_output << "</a>";
		}
	}

	// pointer to pointer
	void VisitPPtr(BlendFilePointerToPointer ptr)
	{
		_output << "<a href='block-0x";
		_output << std::hex << ptr.Value() << std::dec;
		_output << ".html'>0x";
		_output << std::hex << ptr.Value() << std::dec;
		_output << "</a> (pointer to pointer)";
	}

	// floating point
	void VisitDbl(double value)
	{
		_output << value;
	}

	// unsigned int
	void VisitU64(uint64_t value)
	{
		_output << value;
	}

	// signed int
	void VisitI64(int64_t value)
	{
		_output << value;
	}

	// character / byte
	void VisitChr(char c)
	{
		_output
			<< "0x"
			<< std::hex
			<< (c & 0xFF)
			<< std::dec;
	}

	void VisitRaw(const char*, std::size_t)
	{
		_output << "<a href='block-0x0'>NULL</a>";
	}
};

void blend_to_html(const std::string& filename, const std::string& work_dir)
{
	std::ifstream input(filename);
	if(!input.good())
	{
		throw std::runtime_error(
			"Error opening input file "
			"'"+filename+"'."
		);
	}

	BlendFile file(input);
	auto fi = file.Info();

	std::ofstream index(work_dir+"/index.html");
	if(!index.good())
	{
		throw std::runtime_error(
			"Error opening file 'index.html' "
			"in '"+work_dir+"'."
		);
	}

	index	<< "<html>" << std::endl;
	index	<< "<head>" << std::endl;
	index	<< "<link rel='stylesheet' type='text/css' href='blend.css'/>";
	index	<< "</head>" << std::endl;
	index	<< "<body>" << std::endl;
	index	<< "<h2>File: '"<< filename << "'</h2>" << std::endl;
	index	<< "<table>" << std::endl;

	index
		<<"<tr><th>Property</th><th>Value</th></tr>"
		<< std::endl;
	index
		<< "<tr><th>Pointer size</th><td>"
		<< fi.PointerSize()
		<< "</td></tr>"
		<< std::endl;
	index
		<< "<tr><th>Byte order</th><td>"
		<< (fi.ByteOrder() == Endian::Big ? "Big" : "Little")
		<< " endian</td></tr>"
		<< std::endl;
	index
		<< "<tr><th>Version</th><td>"
		<< fi.VersionMajor()
		<< "."
		<< fi.VersionMinor()
		<< "</td></tr>"
		<< std::endl;

	auto sr = file.Structures();
	while(!sr.Empty())
	{
		auto s = sr.Front();

		std::stringstream filepath;
		filepath
			<< work_dir
			<< "/struct-"
			<< s.Name()
			<< ".html";

		std::ofstream output(filepath.str());
		if(!output.good())
		{
			throw std::runtime_error(
				"Error opening output file "
				"'"+filepath.str()+"'."
			);
		}

		output	<< "<html>" << std::endl;
		output	<< "<head>" << std::endl;
		output	<< "<link rel='stylesheet' type='text/css' href='blend.css'/>";
		output	<< "</head>" << std::endl;
		output	<< "<body>" << std::endl;

		output
			<< "<h2>Structure <code>"
			<< s.Name()
			<< "</code></h2>"
			<< std::endl;

		output << "<table>"  << std::endl;

		output
			<< "<tr><th>Property</th><th>Value</th></tr>"
			<< std::endl;
		output
			<< "<tr><th>Size</th><td>"
			<< s.Size()
			<< "</td></tr>"
			<< std::endl;

		output	<< "</table>"  << std::endl;

		output	<< "<h2>Fields</h2>" << std::endl;

		output	<< "<table>"  << std::endl;
		output
			<< "<tr><th>Type</th><th>Definition</th></tr>"
			<< std::endl;

		auto fr = s.Fields();
		while(!fr.Empty())
		{
			auto f = fr.Front();

			auto bt = f.BaseType();
			if(bt.IsStructure())
			{
				output
					<< "<tr><td><a href='struct-"
					<< bt.Name()
					<< ".html'>"
					<< bt.Name()
					<< "</a></td><td>"
					<< f.Definition()
					<< "</td></tr>"
					<< std::endl;
			}
			else
			{
				output
					<< "<tr><td>"
					<< bt.Name()
					<< "</td><td>"
					<< f.Definition()
					<< "</td></tr>"
					<< std::endl;
			}
			fr.Next();
		}
		output << "</table>"  << std::endl;
		output << "</body>" << std::endl;
		output << "</html>" << std::endl;
		sr.Next();
	}

	auto br = file.Blocks();
	while(!br.Empty())
	{
		auto b = br.Front();

		std::stringstream filepath;
		filepath
			<< work_dir
			<< "/block-0x"
			<< std::hex
			<< b.Pointer().Value()
			<< ".html";

		std::ofstream output(filepath.str());
		if(!output.good())
		{
			throw std::runtime_error(
				"Error opening output file "
				"'"+filepath.str()+"'."
			);
		}

		value_visitor vis(output, file);

		auto bs = file.BlockStructure(b);

		output	<< "<html>" << std::endl;
		output	<< "<head>" << std::endl;
		output	<< "<link rel='stylesheet' type='text/css' href='blend.css'/>";
		output	<< "</head>" << std::endl;
		output	<< "<body>" << std::endl;
		output	<< "<h2>Block information</h2>" << std::endl;
		output	<< "<table>"  << std::endl;

		output
			<< "<tr><th>Property</th><th>Value</th></tr>"
			<< std::endl;
		output
			<< "<tr><th>Code</th><td>"
			<< b.Code().c_str()
			<< "</td></tr>"
			<< std::endl;
		output
			<< "<tr><th>Structure</th><td><a href='struct-"
			<< bs.Name()
			<< ".html'>"
			<< bs.Name()
			<< "</a></td></tr>"
			<< std::endl;
		output
			<< "<tr><th>Element size</th><td>"
			<< bs.Size()
			<< "</td></tr>"
			<< std::endl;
		output
			<< "<tr><th>Element count</th><td>"
			<< b.ElementCount()
			<< "</td></tr>"
			<< std::endl;
		output
			<< "<tr><th>Size</th><td>"
			<< b.Size()
			<< "</td></tr>"
			<< std::endl;

		output << "</table>"  << std::endl;

		if(b.Code() == "GLOB")
		{
			index << "<tr><th>File Glob Block</th><td><a href='block-0x";
			index << std::hex << b.Pointer().Value() << std::dec;
			index << ".html'>0x";
			index << std::hex << b.Pointer().Value() << std::dec;
			index << "</a></td></tr>" << std::endl;
		}

		if((b.Size() != 0) && (b.Size() == bs.Size() * b.ElementCount()))
		{
			auto bd = file.BlockData(b);
			auto bfs = bs.Flattened();

			output << "<h2>Fields</h2>" << std::endl;

			for(std::size_t be=0; be != b.ElementCount(); ++be)
			{
				output << "<table>" << std::endl;
				output
					<< "<tr>"
					<< "<th>Type</th>"
					<< "<th>Definition</th>"
					<< "<th>ID</th>"
					<< "<th>Value</th>"
					<< "</tr>"
					<< std::endl;

				auto bfsfr = bfs.Fields();
				while(!bfsfr.Empty())
				{
					auto ff = bfsfr.Front();
					auto sf = ff.Field();

					auto bt = sf.BaseType();

					output << "<tr>" << std::endl;
					if(bt.IsStructure())
					{
						output
							<< "<td><a href='struct-"
							<< bt.Name()
							<< ".html'>"
							<< bt.Name()
							<< "</a></td>"
							<< std::endl;
					}
					else
					{
						output
							<< "<td>"
							<< bt.Name()
							<< "</td>"
							<< std::endl;
					}
					output
						<< "<td>"
						<< sf.Definition()
						<< "</td>"
						<< std::endl;
					output
						<< "<td>"
						<< ff.Name()
						<< "</td>"
						<< std::endl;
					output << "<td>";

					if(bt.IsNative<char>() && sf.ElementCount() > 1)
					{
						bd.ValueVisit(vis, ff, be);
					}
					else
					{
						for(std::size_t e=0; e != sf.ElementCount(); ++e)
						{
							if(e) output << ", ";
							bd.ValueVisit(vis, ff, be, e);
						}
					}
					output << "</td>";

					output << "</tr>" << std::endl;

					bfsfr.Next();
				}
				output << "</table>"  << std::endl;
			}
		}
		else if(b.Code() == "DATA")
		{
			output << "<h2>Raw data</h2>" << std::endl;
			output << "<table>"  << std::endl;
			output << "<tr><th>Bytes</th><tr>" << std::endl;

			auto bd = file.BlockData(b);

			output << "<tr><td>" << std::endl;
			for(std::size_t i=0, n=bd.DataSize(); i!=n; ++i)
			{
				char c = bd.RawByte(i);
				if(std::isprint(c))
				{
					output << c;
				}
				else
				{
					output << "\\"
						<< std::setw(3)
						<< std::setfill('0')
						<< std::oct
						<< (c & 0xFF)
						<< std::dec;
				}
			}
			output << "</td></tr>" << std::endl;
			output << "</table>"  << std::endl;

			if(b.Size() % file.Info().PointerSize() == 0)
			{
				output << "<h2>Pointers</h2>" << std::endl;
				output << "<table>"  << std::endl;
				output << "<tr><th>Pointer</th></tr>" << std::endl;

				auto vt = file.Type<void>();
				std::size_t n = b.Size() / file.Info().PointerSize();
				for(std::size_t i=0; i!=n; ++i)
				{
					auto ptr = bd.AsPointerTo(vt, i);
					output << "<tr><td><a href='block-0x";
					output << std::hex << ptr.Value() << std::dec;
					output << ".html'>0x";
					output << std::hex << ptr.Value() << std::dec;
					output << "</a></td></tr>";
				}
				output << "</table>"  << std::endl;
			}
		}
		output << "</body>" << std::endl;
		output << "</html>" << std::endl;
		br.Next();
	}
	index << "</table>" << std::endl;
	index << "</body>" << std::endl;
	index << "</html>" << std::endl;

	index.close();

	std::ofstream stylesheet(work_dir+"/blend.css");
	if(!stylesheet.good())
	{
		throw std::runtime_error(
			"Error opening file 'blend.css' "
			"in '"+work_dir+"'."
		);
	}

	stylesheet
		<< "body { font-family: 'Courier New', Courier, monospace; }"
		<< std::endl;
	stylesheet
		<< "table { border-collapse: collapse; }"
		<< std::endl;
	stylesheet
		<< "table, th, td { border: 1px solid black; }"
		<< std::endl;
	stylesheet
		<< "th { background-color: navy; color: white; }"
		<< std::endl;

	stylesheet.close();
}

void print_usage(std::ostream& output)
{
	output
		<< "Usage: 010_blender2html "
		<< "[input-file.blend] [output-directory]"
		<< std::endl;
	output
		<< "Note: The output directory must exist and must be writable"
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
		else blend_to_html(
			argc>1?argv[1]:"test.blend",     // input file path
			argc>2?argv[2]:"oglp-blend-file" // output dir path
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


