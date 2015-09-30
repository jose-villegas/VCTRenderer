@echo off
:: Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
:: Software License, Version 1.0. (See accompanying file
:: LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

:: First, reset the variables we are using
CALL :reset_environment

:: Path to the build directory
SET OGLPLUS_BUILD_DIR=_build
::
:: Use the environment variables if they are set
IF DEFINED OGLPLUS_DEP_INCLUDE_DIRS (
	SET "OGLPLUS_HEADER_SEARCH_PATHS=%OGLPLUS_DEP_INCLUDE_DIRS%"
)
IF DEFINED OGLPLUS_DEP_LIBRARY_DIRS (
	SET "OGLPLUS_LIBRARY_SEARCH_PATHS=%OGLPLUS_DEP_LIBRARY_DIRS%"
)
::
:: Parse the command line arguments
::
:parse_cmd_line_args

	::
	:: If there are no (more) arguments finish the parsing loop
	::
	IF [%1] == [] GOTO :done_parsing_cmd_line


	::
	:: Print help and quit if /? /h --help is passed as argument
	::
	IF "%~1" == "/h" CALL :print_usage && GOTO :eof
	IF "%~1" == "/?" CALL :print_usage && GOTO :eof
	IF "%~1" == "--help" CALL :print_usage && GOTO :eof


	::
	:: Set OGLPLUS_NO_BOOST_CONFIG if we don't want to use Boost.Config
	::
	IF "%~1" == "--no-boost-config" (
		SET /A OGLPLUS_NO_BOOST_CONFIG=1
		GOTO :shift_cmd_line_args
	)

	::
	:: Set OGLPLUS_NO_DOCS if we don't want to build docs
	::
	IF "%~1" == "--no-docs" (
		SET /A OGLPLUS_NO_DOCS=1
		GOTO :shift_cmd_line_args
	)

	::
	:: Set OGLPLUS_NO_EXAMPLES if we don't want to build examples
	::
	IF "%~1" == "--no-examples" (
		SET /A OGLPLUS_NO_EXAMPLES=1
		GOTO :shift_cmd_line_args
	)

	::
	:: Parse --prefix <path>
	::
	IF "%~1" == "--prefix" (
		IF [%2] == [] ECHO Missing path after '--prefix' ! && EXIT /B 2

		SET "OGLPLUS_PREFIX=%~2"

		SHIFT
		GOTO :shift_cmd_line_args
	)

	::
	:: Parse --include-dir <path>
	::
	IF "%~1" == "--include-dir" (
		IF [%2] == [] ECHO Missing path after '--include-dir' ! && EXIT /B 2
		IF NOT EXIST %2 ECHO Path '%2' does not exist! && EXIT /B 3

		IF DEFINED OGLPLUS_HEADER_SEARCH_PATHS (
			SET "OGLPLUS_HEADER_SEARCH_PATHS=%OGLPLUS_HEADER_SEARCH_PATHS%;%~2"
		) ELSE (SET "OGLPLUS_HEADER_SEARCH_PATHS=%~2")

		SHIFT
		GOTO :shift_cmd_line_args
	)


	::
	:: Parse --library-dir <path>
	::
	IF "%~1" == "--library-dir" (
		IF [%2] == [] ECHO Missing path after '--library-dir' ! && EXIT /B 2
		IF NOT EXIST %2 ECHO Path '%2' does not exist! && EXIT /B 3

		IF DEFINED OGLPLUS_LIBRARY_SEARCH_PATHS (
			SET "OGLPLUS_LIBRARY_SEARCH_PATHS=%OGLPLUS_LIBRARY_SEARCH_PATHS%;%~2"
		) ELSE (SET "OGLPLUS_LIBRARY_SEARCH_PATHS=%~2")

		SHIFT
		GOTO :shift_cmd_line_args
	)

	::
	:: Parse --use-gl-api-lib <NAME>
	::
	IF "%~1" == "--use-gl-api-lib" (
		IF [%2] == [] ECHO Missing name after '--use-gl-api-lib' ! && EXIT /B 2

		SET "OGLPLUS_GL_API_LIB=%~2"

		SHIFT
		GOTO :shift_cmd_line_args
	)

	::
	:: Parse --use-gl-init-lib <NAME>
	::
	IF "%~1" == "--use-gl-init-lib" (
		IF [%2] == [] ECHO Missing name after '--use-gl-init-lib' ! && EXIT /B 2

		SET "OGLPLUS_GL_INIT_LIB=%~2"

		SHIFT
		GOTO :shift_cmd_line_args
	)

	::
	:: If the --from-scratch option was passed remove the build directory
	::
	IF "%~1" == "--from-scratch" (
		IF EXIST %OGLPLUS_BUILD_DIR% (DEL /Q /F /S %OGLPLUS_BUILD_DIR%)
		GOTO :shift_cmd_line_args
	)

	::
	:: Set the cmake msvc generator
	::
	IF "%~1" == "--for-msvc" (
		IF [%2] == [] ECHO Missing MSVC version after '--for-msvc' ! && EXIT /B 2

		SET OGLPLUS_CMAKE_OPTIONS=%OGLPLUS_CMAKE_OPTIONS% -G "Visual Studio %~2"

		SHIFT
		GOTO :shift_cmd_line_args
	)

	::
	:: If --cmake option is found gather the rest of the command line
	::
	IF "%~1" == "--cmake" (
		SHIFT
		GOTO :append_cmake_option
	)

	::
	:: If we get here an invalid argument was supplied as argument
	::
	ECHO "Invalid command-line argument '%~1'"
	EXIT /B 1

::
:: Shift the argument index and restart the parsing loop
::
:shift_cmd_line_args
	SHIFT
GOTO :parse_cmd_line_args

::
:: Copy the rest of the arguments verbatim
:: to the OGLPLUS_CMAKE_OPTIONS variable
::
:append_cmake_option
	IF [%1] == [] GOTO :done_parsing_cmd_line ELSE (
		SET OGLPLUS_CMAKE_OPTIONS=%OGLPLUS_CMAKE_OPTIONS% %1
		SHIFT
		GOTO :append_cmake_option
	)

::
:: We've finished parsing the command line arguments
::
:done_parsing_cmd_line

::
:: Begin building the cmake command line ..
::
SET OGLPLUS_CMAKE_COMMAND=cmake ..

::
:: .. append the verbatim cmake options if any ..
::
IF DEFINED OGLPLUS_CMAKE_OPTIONS (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% %OGLPLUS_CMAKE_OPTIONS%
)

::
:: .. set the installation prefix if any
::
IF DEFINED OGLPLUS_PREFIX (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DCMAKE_INSTALL_PREFIX=%OGLPLUS_PREFIX%"
)


::
:: .. append the list of header file search paths ..
::
IF DEFINED OGLPLUS_HEADER_SEARCH_PATHS (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DHEADER_SEARCH_PATHS=%OGLPLUS_HEADER_SEARCH_PATHS%"
)

::
:: .. append the list of library search paths ..
::
IF DEFINED OGLPLUS_LIBRARY_SEARCH_PATHS (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DLIBRARY_SEARCH_PATHS=%OGLPLUS_LIBRARY_SEARCH_PATHS%"
)

::
:: .. append the forced api lib ..
::
IF DEFINED OGLPLUS_GL_API_LIB (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DOGLPLUS_FORCE_GL_API_LIB=%OGLPLUS_GL_API_LIB%"
)

::
:: .. append the forced init lib ..
::
IF DEFINED OGLPLUS_GL_INIT_LIB (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DOGLPLUS_FORCE_GL_INIT_LIB=%OGLPLUS_GL_INIT_LIB%"
)

::
:: .. define the OGLPLUS_NO_BOOST_CONFIG option ..
::
IF DEFINED OGLPLUS_NO_BOOST_CONFIG (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DOGLPLUS_NO_BOOST_CONFIG=On"
)

::
:: .. define the OGLPLUS_NO_DOCS option ..
::
IF DEFINED OGLPLUS_NO_DOCS (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DOGLPLUS_NO_DOCS=On"
)

::
:: .. define the OGLPLUS_NO_EXAMPLES option ..
::
IF DEFINED OGLPLUS_NO_EXAMPLES (
	SET OGLPLUS_CMAKE_COMMAND=%OGLPLUS_CMAKE_COMMAND% "-DOGLPLUS_NO_EXAMPLES=On"
)

::
:: .. and make the build dir, go into it and invoke cmake
::
IF NOT EXIST %OGLPLUS_BUILD_DIR% (MD %OGLPLUS_BUILD_DIR%)
CD %OGLPLUS_BUILD_DIR% && %OGLPLUS_CMAKE_COMMAND% && CD ..


::
:: cleanup and quit
::
CALL :reset_environment
GOTO :eof


::
:: Resets the variables that this script defines
::
:reset_environment
	SET OGLPLUS_HEADER_SEARCH_PATHS=
	SET OGLPLUS_LIBRARY_SEARCH_PATHS=
	SET OGLPLUS_BUILD_DIR=
	SET OGLPLUS_CMAKE_COMMAND=
	SET OGLPLUS_CMAKE_OPTIONS=
	SET OGLPLUS_NO_BOOST_CONFIG=
	SET OGLPLUS_NO_DOCS=
	SET OGLPLUS_NO_EXAMPLES=
	SET OGLPLUS_PREFIX=
GOTO :eof


::
:: Prints the usage screen
::
:print_usage
	ECHO:configure: OGLplus cmake configuration script
	ECHO.
	ECHO:NOTE: Consider using the configure.py Python script if possible.
	ECHO:      configure.py is more versatile, robust and up to date.
	ECHO:      Run `configure.py --help' to display full help screen.
	ECHO.
	ECHO:Synopsis
	ECHO:configure.bat [config-options] [^<--cmake^> [cmake-options]]
	ECHO.
	ECHO:All options after '-' or '--cmake' are passed verbatim to cmake
	ECHO.
	ECHO:config-options:
	ECHO.
	ECHO:  --help ^| /h ^| /?:      Print this help screen.
	ECHO.
	ECHO:  --prefix PREFIX:       Specifies the installation prefix. The path
	ECHO:                         must be absolute or relative to the current
	ECHO:                         working directory from which configure is
	ECHO:                         invoked.
	ECHO.
	ECHO:  --include-dir PATH:    Specifies additional directory to search
	ECHO:                         when looking for external headers like
	ECHO:                         GL/glew.h or GL3/gl3.h.  The specified path
	ECHO:                         must be absolute or relative to the current
	ECHO:                         working directory from which configure is
	ECHO:                         invoked.
	ECHO:                         This option may be specified multiple times
	ECHO:                         to add multiple directories to the search list.
	ECHO.
	ECHO:  --library-dir PATH:    Specifies additional directory to search
	ECHO:                         when looking for compiled libraries like
	ECHO:                         GL, GLEW, glut, png, etc. The specified path
	ECHO:                         must be absolute or relative to the current
	ECHO:                         working directory from which configure is
	ECHO:                         invoked.
	ECHO:                         This option may be specified multiple times
	ECHO:                         to add multiple directories to the search list.
	ECHO.
	ECHO:  --for-msvc VERSION:    Equivalent to --cmake -G "Visual Studio VERSION":
	ECHO:                         Uses the cmake generator for Microsoft Visual studio
	ECHO:                         with the specified VERSION.
	ECHO.
	ECHO:  --cmake [options]:     Everything following the --cmake option will
	ECHO:                         be passed to cmake verbatim.
	ECHO.
	ECHO:  --from-scratch         Remove any previous cached and intermediate files
	ECHO:                         and run the configuration process from scratch.
	ECHO.
	ECHO:  --no-examples          Do not build the examples and the textures.
	ECHO.
	ECHO:  --no-docs              Do not build and install the documentation.
	ECHO.

	CALL :reset_environment
GOTO :eof
