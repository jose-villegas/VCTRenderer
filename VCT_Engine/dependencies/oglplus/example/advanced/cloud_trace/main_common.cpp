/**
 *  @file advanced/cloud_trace/main_common.hpp
 *  @brief Implements common code shared by various main implementations
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2008-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "app_data.hpp"

#include <oglplus/gl.hpp>
#include <oglplus/config/fix_gl_version.hpp>
#include <oglplus/config/fix_gl_extension.hpp>
#include <oglplus/error/basic.hpp>

#include <stdexcept>
#include <system_error>
#include <iostream>
#include <cstring>

namespace oglplus {
namespace cloud_trace {

inline void print_std_error_common(
	std::exception& error,
	std::ostream& errstr
)
{
	errstr	<< "Message: '"
		<< error.what()
		<< "'"
		<< std::endl;
}

inline void print_error_common(
	Error& error,
	std::ostream& errstr
)
{
	if(error.SourceFile())
	{
		errstr	<< "Source file: '"
			<< error.SourceFile()
			<< "'"
			<< std::endl;
	}

	if(error.SourceLine())
	{
		errstr	<< "Source line: "
			<< error.SourceLine()
			<< std::endl;
	}

	if(error.SourceFunc())
	{
		errstr	<< "Source function: '"
			<< error.SourceFunc()
			<< "'"
			<< std::endl;
	}
	print_std_error_common(error, errstr);
	if(error.GLFunc())
	{
		errstr	<< "GL function: '"
			<< error.GLFunc()
			<< "'"
			<< std::endl;
	}

	if(error.EnumParam() || error.EnumParamName())
	{
		errstr	<< "GL constant: ";
		if(error.EnumParamName())
		{
			errstr	<< "'"
				<< error.EnumParamName()
				<< "'";
		}
		else
		{
			errstr	<< "(0x"
				<< std::hex
				<< error.EnumParam()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if(error.BindTarget() || error.TargetName())
	{
		errstr	<< "Binding point: ";
		if(error.TargetName())
		{
			errstr	<< "'"
				<< error.TargetName()
				<< "'";
		}
		else
		{
			errstr	<< "(0x"
				<< std::hex
				<< error.BindTarget()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if(error.ObjectTypeName() || error.ObjectType())
	{
		errstr	<< "Object type: ";
		if(error.ObjectTypeName())
		{
			errstr	<< "'"
				<< error.ObjectTypeName()
				<< "'";
		}
		else
		{
			errstr	<< "(0x"
				<< std::hex
				<< error.ObjectType()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if((!error.ObjectDesc().empty()) || (error.ObjectName() >= 0))
	{
		errstr	<< "Object: ";
		if(!error.ObjectDesc().empty())
		{
			errstr	<< "'"
				<< error.ObjectDesc()
				<< "'";
		}
		else
		{
			errstr	<< "("
				<< error.ObjectName()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if(error.SubjectTypeName() || error.SubjectType())
	{
		errstr	<< "Subject type: ";
		if(error.SubjectTypeName())
		{
			errstr	<< "'"
				<< error.SubjectTypeName()
				<< "'";
		}
		else
		{
			errstr	<< "(0x"
				<< std::hex
				<< error.SubjectType()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if((!error.SubjectDesc().empty()) || (error.SubjectName() >= 0))
	{
		errstr	<< "Subject: ";
		if(!error.SubjectDesc().empty())
		{
			errstr	<< "'"
				<< error.SubjectDesc()
				<< "'";
		}
		else
		{
			errstr	<< "("
				<< error.SubjectName()
				<< ")";
		}
		errstr	<< std::endl;
	}

	if(error.Index() >= 0)
	{
		errstr	<< "Index: ("
			<< error.Index()
			<< ")"
			<< std::endl;
	}

	if(!error.Log().empty())
	{
		errstr	<< "Log:"
			<< std::endl
			<< error.Log()
			<< std::endl;
	}
}


int do_run_main(int (*main_func)(AppData&), AppData& app_data)
{
	std::ostream& errstr = app_data.errstr();
	try
	{
		return main_func(app_data);
	}
	catch(Error& err)
	{
		errstr << "GL error";
		print_error_common(err, errstr);
	}
	catch(std::system_error& sye)
	{
		errstr << "System error";
		print_std_error_common(sye, errstr);
		errstr << "Error code: " << sye.code() << std::endl;
		errstr << std::endl;
	}
	catch(std::runtime_error& rte)
	{
		errstr << "Runtime error";
		print_std_error_common(rte, errstr);
		errstr << std::endl;
	}
	catch(std::exception& se)
	{
		errstr << "Error";
		print_std_error_common(se, errstr);
		errstr << std::endl;
	}
	return 1;
}

} // namespace cloud_trace
} // namespace oglplus
