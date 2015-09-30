/**
 *  @file oglplus/example_main.hpp
 *  @brief Implements common code shared by examples
 *
 *  Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef OGLPLUS_EXAMPLE_EXAMPLE_MAIN_1119071146_HPP
#define OGLPLUS_EXAMPLE_EXAMPLE_MAIN_1119071146_HPP

#include <oglplus/error/prog_var.hpp>
#include <oglplus/error/program.hpp>
#include <oglplus/error/limit.hpp>
#include <oglplus/opt/application.hpp>

#include <oglplus/os/semaphore.hpp>

#include <stdexcept>
#include <system_error>
#include <iostream>
#include <iomanip>

namespace oglplus {

inline void example_print_std_error_common(
	std::exception& error,
	std::ostream& errstr
)
{
	errstr	<< "Message: '"
		<< error.what()
		<< "'"
		<< std::endl;
}

inline void example_print_error_common(
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
	example_print_std_error_common(error, errstr);
	if(error.GLFunc())
	{
		errstr	<< "GL function: '";

		if(error.GLLib())
		{
			errstr << error.GLLib();
		}

		errstr	<< error.GLFunc()
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

	if(error.Identifier())
	{
		errstr	<< "Identifier: '"
			<< error.Identifier()
			<< "'"
			<< std::endl;
	}

	if(error.Index() >= 0)
	{
		errstr	<< "Index: ("
			<< error.Index()
			<< ")"
			<< std::endl;
	}

	if(error.Value() != 0)
	{
		errstr	<< "Value: ("
			<< error.Value()
			<< ")"
			<< std::endl;
	}

	if(error.Limit() != 0)
	{
		errstr	<< "Limit: ("
			<< error.Limit()
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

template <typename Func>
inline int example_guarded_exec(Func func, std::ostream& errstr)
{
	try
	{
		return func();
	}
	catch(ProgVarError& pve)
	{
		errstr << "Program variable error" << std::endl;
		example_print_error_common(pve, errstr);
	}
	catch(ProgramBuildError& pbe)
	{
		errstr << "Program build error" << std::endl;
		example_print_error_common(pbe, errstr);
	}
	catch(LimitError& le)
	{
		errstr << "Limit error" << std::endl;
		example_print_error_common(le, errstr);
	}
	catch(ObjectError& oe)
	{
		errstr << "Object error" << std::endl;
		example_print_error_common(oe, errstr);
	}
	catch(Error& err)
	{
		errstr << "GL error" << std::endl;
		example_print_error_common(err, errstr);
	}
	catch(std::system_error& sye)
	{
		errstr << "System error" << std::endl;
		example_print_std_error_common(sye, errstr);
		errstr << "Error code: " << sye.code() << std::endl;
		errstr << std::endl;
	}
	catch(std::runtime_error& rte)
	{
		errstr << "Runtime error" << std::endl;
		example_print_std_error_common(rte, errstr);
		errstr << std::endl;
	}
	catch(std::exception& se)
	{
		errstr << "Error" << std::endl;
		example_print_std_error_common(se, errstr);
		errstr << std::endl;
	}
	return 1;
}

inline int example_main(
	int (*main_func)(int, char**),
	int argc,
	char ** argv
)
{
	struct main_wrapper
	{
		int (*main_func)(int, char**);
		int argc;
		char** argv;

		int operator()(void) const
		{
			os::CriticalSection cs("OGLplus example");
			Application::ParseCommandLineOptions(argc, argv);
			return main_func(argc, argv);
		}
	} wrapped_main = {main_func, argc, argv};

	return example_guarded_exec(wrapped_main, std::cerr);
}

} // namespace oglplus

#endif // include guard
