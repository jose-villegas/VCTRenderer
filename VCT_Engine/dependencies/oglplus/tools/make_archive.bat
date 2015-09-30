@echo off
:: Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
:: Software License, Version 1.0. (See accompanying file
:: LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SET oglplus_archive_items=^
	_prebuilt ^
	config ^
	doc ^
	etc ^
	example ^
	include ^
	implement ^
	utils ^
	source ^
	third_party ^
	test ^
	LICENSE_1_0.txt ^
	README.rst ^
	CHANGELOG ^
	VERSION ^
	CMakeLists.txt ^
	configure.py ^
	configure.bat

:: Get the current version
SET /P oglplus_version=<VERSION

:: make the zip file name
SET oglplus_archive_name=oglplus-%oglplus_version%.zip

zip -r -9 %oglplus_archive_name% %oglplus_archive_items% -x *.sdf *.swp
