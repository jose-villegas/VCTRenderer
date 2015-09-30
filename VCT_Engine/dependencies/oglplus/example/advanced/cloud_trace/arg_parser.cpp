/**
 *  @file advanced/cloud_trace/arg_parser.cpp
 *  @brief Implementation of argument parser class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include "arg_parser.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

namespace oglplus {
namespace cloud_trace {

ArgInfoBase::ArgInfoBase(std::string&& sn, std::string&& ln)
 : parse_count(0)
 , short_name(std::move(sn))
 , long_name(std::move(ln))
 , description()
 , has_min(false)
 , has_max(false)
{ }

ArgInfoBase::ArgInfoBase(ArgInfoBase&& tmp)
 : parse_count(tmp.parse_count)
 , short_name(std::move(tmp.short_name))
 , long_name(std::move(tmp.long_name))
 , description(std::move(tmp.description))
 , has_min(tmp.has_min)
 , has_max(tmp.has_max)
{ }

ArgInfoBase& ArgInfoBase::AddDesc(std::string&& desc)
{
	description = std::move(desc);
	return *this;
}

template <typename T>
ArgInfo<T>::ArgInfo(std::string&& sn, std::string&& ln, T& var)
 : ArgInfoBase(std::move(sn), std::move(ln))
 , vars(1, &var)
{ }

template <typename T>
ArgInfo<T>::ArgInfo(ArgInfo&& tmp)
 : ArgInfoBase(static_cast<ArgInfoBase&&>(tmp))
 , vars(std::move(tmp.vars))
 , min(std::move(tmp.min))
 , max(std::move(tmp.max))
{ }

template <>
void ArgInfo<bool>::PrintPlaceholder(std::ostream&)
{ }

template <>
void ArgInfo<unsigned>::PrintPlaceholder(std::ostream& out)
{
	out << "UINT";
}

template <>
void ArgInfo<float>::PrintPlaceholder(std::ostream& out)
{
	out << "FLOAT";
}

template <>
void ArgInfo<std::string>::PrintPlaceholder(std::ostream& out)
{
	out << "STRING";
}

template <>
void ArgInfo<std::vector<std::string>>::PrintPlaceholder(std::ostream& out)
{
	out << "STRING";
}

template <typename T>
void PrintValue(std::ostream& out, T* ptr)
{
	assert(ptr != nullptr);
	out << *ptr;
}

template <>
void PrintValue(std::ostream&, bool*)
{
}

template <>
void PrintValue(std::ostream& out, std::string* ptr)
{
	assert(ptr != nullptr);
	out << "'" << *ptr << "'";
}

template <typename T>
void PrintValue(std::ostream& out, std::vector<T>* ptr)
{
	assert(ptr != nullptr);
	if(!ptr->empty())
	{
		PrintValue(out, &ptr->front());
	}
}

template <typename T>
void ArgInfo<T>::PrintDefault(std::ostream& out)
{
	if(!vars.empty())
	{
		PrintValue(out, vars.front());
	}
}

template <typename T>
void ArgInfo<T>::PrintMin(std::ostream& out)
{
	if(has_min)
	{
		PrintValue(out, &min);
	}
}

template <typename T>
void ArgInfo<T>::PrintMax(std::ostream& out)
{
	if(has_max)
	{
		PrintValue(out, &max);
	}
}

template <typename T>
void ParseValue(const char* name, const char* str, T& value)
{
	std::stringstream ss(str);

	ss >> value;
	if(ss.fail())
	{
		std::string message;
		message.append("Invalid value '");
		message.append(str);
		message.append("' after '");
		message.append(name);
		message.append("'");

		throw std::runtime_error(message);
	}
}

template <typename T>
void ParseAndSetValue(
	const char* name,
	const char* str,
	std::vector<T*> vars
)
{
	T value;
	ParseValue(name, str, value);

	for(T* ptr : vars)
	{
		assert(ptr != nullptr);
		*ptr = value;
	}
}

template <typename T>
void ParseAndSetValue(
	const char* name,
	const char* str,
	std::vector<std::vector<T>*>& vars
)
{
	T value;
	ParseValue(name, str, value);

	for(std::vector<T>* ptr : vars)
	{
		assert(ptr);
		ptr->push_back(std::move(value));
	}
}

template <typename T>
bool ArgInfo<T>::Parse(int& arg, int argc, char** argv)
{
	const char* name = nullptr;

	std::size_t len = short_name.size();
	if(std::strncmp(argv[arg], short_name.c_str(), len) == 0)
	{
		name = short_name.c_str();
	}
	else
	{
		len = long_name.size();
		if(std::strncmp(argv[arg], long_name.c_str(), len) == 0)
		{
			name = long_name.c_str();
		}
		else len = 0;
	}

	if(name && len)
	{
		if(argv[arg][len] == '=')
		{
			ParseAndSetValue(name, argv[arg]+len+1, vars);
			arg += 1;
			++parse_count;
			return true;
		}
		else if(argv[arg][len] == '\0')
		{
			if(arg+1 < argc)
			{
				ParseAndSetValue(name, argv[arg+1], vars);
				arg += 2;
				++parse_count;
				return true;
			}
			else
			{
				std::string message;
				message.append("Missing value after '");
				message.append(argv[arg]);
				message.append("'");

				throw std::runtime_error(message);
			}
		}
	}
	return false;
}

template <>
bool ArgInfo<bool>::Parse(int& arg, int, char** argv)
{
	bool result =
		(std::strcmp(argv[arg], long_name.c_str()) == 0) ||
		(std::strcmp(argv[arg], short_name.c_str()) == 0);
	if(result)
	{
		++arg;
		for(auto ptr : vars)
		{
			assert(ptr != nullptr);
			*ptr = true;
		}
		++parse_count;
	}

	return result;
}

ArgParser::ArgParser(void)
 : print_help(false)
{
	AddOpt("-h", "--help", print_help)
		.AddDesc("Print the help screen and exit.");
}

ArgInfo<bool>& ArgParser::AddOpt(
	std::string&& short_name,
	std::string&& long_name,
	bool& var
)
{
	options.push_back(ArgInfo<bool>(
		std::move(short_name),
		std::move(long_name),
		var
	));
	all_arg_refs.push_back(&options.back());
	return options.back();
}

ArgInfo<unsigned>& ArgParser::AddArg(
	std::string&& short_name,
	std::string&& long_name,
	unsigned& var
)
{
	uint_args.push_back(ArgInfo<unsigned>(
		std::move(short_name),
		std::move(long_name),
		var
	));
	all_arg_refs.push_back(&uint_args.back());
	return uint_args.back();
}

ArgInfo<float>& ArgParser::AddArg(
	std::string&& short_name,
	std::string&& long_name,
	float& var
)
{
	float_args.push_back(ArgInfo<float>(
		std::move(short_name),
		std::move(long_name),
		var
	));
	all_arg_refs.push_back(&float_args.back());
	return float_args.back();
}

ArgInfo<std::string>& ArgParser::AddArg(
	std::string&& short_name,
	std::string&& long_name,
	std::string& var
)
{
	str_args.push_back(ArgInfo<std::string>(
		std::move(short_name),
		std::move(long_name),
		var
	));
	all_arg_refs.push_back(&str_args.back());
	return str_args.back();
}

ArgInfo<std::vector<std::string>>& ArgParser::AddArg(
	std::string&& short_name,
	std::string&& long_name,
	std::vector<std::string>& var
)
{
	str_list_args.push_back(ArgInfo<std::vector<std::string>>(
		std::move(short_name),
		std::move(long_name),
		var
	));
	all_arg_refs.push_back(&str_list_args.back());
	return str_list_args.back();
}

template <typename T>
bool ArgParser::ParseOne(
	int& arg,
	int argc,
	char** argv,
	std::list<ArgInfo<T>>& arg_infos
)
{
	for(auto& arg_info : arg_infos)
	{
		if(arg_info.Parse(arg, argc, argv))
			return true;
	}
	return false;
}

bool ArgParser::ParseOne(int& arg, int argc, char** argv)
{
	if(ParseOne(arg, argc, argv, options))
		return true;
	if(ParseOne(arg, argc, argv, float_args))
		return true;
	if(ParseOne(arg, argc, argv, uint_args))
		return true;
	if(ParseOne(arg, argc, argv, str_args))
		return true;
	if(ParseOne(arg, argc, argv, str_list_args))
		return true;

	return false;
}

bool ArgParser::Parse(int argc, char** argv)
{
	int arg = 1;
	while(arg < argc)
	{
		if(!ParseOne(arg, argc, argv))
		{
			std::string message("Unknown command-line option: '");
			message.append(argv[arg]);
			message.append("'");

			throw std::runtime_error(message);
		}
	}
	if(print_help)
	{
		PrintHelp(std::cout);
		return false;
	}
	return true;
}

void ArgParser::PrintHelp(std::ostream& out)
{
	out << "Usage: cloud_trace [--help | -h] | [options...]" << std::endl;
	out << std::endl;
	out << "Optional arguments:" << std::endl;

	const std::size_t indent = 25;
	const std::size_t columns = 80;
	std::string indent_str(indent, ' ');

	for(ArgInfoBase* aib : all_arg_refs)
	{
		assert(aib != nullptr);

		std::stringstream placeholder;
		aib->PrintPlaceholder(placeholder);

		std::size_t len = 0;

		out << "  " << aib->long_name;
		len += 2 + aib->long_name.size();

		if(!placeholder.str().empty())
		{
			out << " "  << placeholder.str();
			len += 1 + placeholder.str().size();
		}

		out << ", " << aib->short_name;
		len += 2 + aib->short_name.size();

		if(!placeholder.str().empty())
		{
			out << " "  << placeholder.str();
			len += 1 + placeholder.str().size();
		}

		if(indent > len)
		{
			out << std::string(indent - len, ' ');
		}
		else
		{
			out << std::endl;
			out << indent_str;
		}

		len = 0;
		std::size_t p0 = 0, p1 = 0;
		while(true)
		{
			if(indent + len >= columns)
			{
				out << std::endl;
				out << indent_str;
				len = 0;
			}
			p1 = aib->description.find(" ", p0);
			if(p1 == std::string::npos)
			{
				out << aib->description.c_str()+p0;
				break;
			}
			else
			{
				out.write(aib->description.c_str()+p0, p1-p0+1);
			}
			len += p1-p0+1;
			p0 = p1+1;
		}

		std::stringstream value_str;

		aib->PrintDefault(value_str);
		if(!value_str.str().empty())
		{
			out << std::endl;
			out << indent_str;
			out << "Default: "  << value_str.str();
		}

		if(aib->has_min)
		{
			value_str.str(std::string());
			aib->PrintMin(value_str);
			out << std::endl;
			out << indent_str;
			out << "Minimum: " << value_str.str();
		}

		if(aib->has_max)
		{
			value_str.str(std::string());
			aib->PrintMax(value_str);
			out << std::endl;
			out << indent_str;
			out << "Maximum: " << value_str.str();
		}

		out << std::endl;
	}
	// TODO
}

} // namespace cloud_trace
} // namespace oglplus
