/**
 *  @file advanced/cloud_trace/arg_parser.hpp
 *  @brief Declaration of argument parser class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_ADVANCED_CLOUD_ARG_PARSER_1119071146_HPP
#define OGLPLUS_ADVANCED_CLOUD_ARG_PARSER_1119071146_HPP

#include <string>
#include <iosfwd>
#include <vector>
#include <list>

namespace oglplus {
namespace cloud_trace {

struct ArgInfoBase
{
	unsigned parse_count;
	std::string short_name;
	std::string long_name;
	std::string description;

	bool has_min, has_max;

	ArgInfoBase(std::string&&, std::string&&);
	ArgInfoBase(ArgInfoBase&&);
	ArgInfoBase& AddDesc(std::string&& desc);

	virtual ~ArgInfoBase(void) { }
	virtual void PrintPlaceholder(std::ostream&) = 0;
	virtual void PrintDefault(std::ostream&) = 0;
	virtual void PrintMin(std::ostream&) = 0;
	virtual void PrintMax(std::ostream&) = 0;
};

template <typename T>
struct ArgInfo : ArgInfoBase
{
	std::vector<T*> vars;

	T min, max;

	ArgInfo(std::string&&, std::string&&, T&);
	ArgInfo(ArgInfo&&);

	void PrintPlaceholder(std::ostream&);
	void PrintDefault(std::ostream&);
	void PrintMin(std::ostream&);
	void PrintMax(std::ostream&);

	ArgInfo& AddVar(T& var)
	{
		vars.push_back(&var);
		return *this;
	}

	ArgInfo& SetMin(const T& val)
	{
		min = val;
		has_min = true;
		return *this;
	}

	ArgInfo& SetMax(const T& val)
	{
		max = val;
		has_max = true;
		return *this;
	}

	bool Parse(int& arg, int argc, char** argv);
};

class ArgParser
{
private:
	bool print_help;

	std::list<ArgInfo<bool>> options;
	std::list<ArgInfo<float>> float_args;
	std::list<ArgInfo<unsigned>> uint_args;
	std::list<ArgInfo<std::string>> str_args;
	std::list<ArgInfo<std::vector<std::string>>> str_list_args;

	std::list<ArgInfoBase*> all_arg_refs;

	template <typename T>
	bool ParseOne(int&, int, char**, std::list<ArgInfo<T>>& args);
	bool ParseOne(int&, int, char**);
public:
	ArgParser(void);

	ArgInfo<bool>& AddOpt(std::string&&, std::string&&, bool&);
	ArgInfo<float>& AddArg(std::string&&, std::string&&, float&);
	ArgInfo<unsigned>& AddArg(std::string&&, std::string&&, unsigned&);
	ArgInfo<std::string>& AddArg(std::string&&, std::string&&, std::string&);
	ArgInfo<std::vector<std::string>>&
	AddArg(std::string&&, std::string&&, std::vector<std::string>&);

	bool Parse(int argc, char** argv);
	void PrintHelp(std::ostream&);
};


} // namespace cloud_trace
} // namespace oglplus

#endif // include guard
