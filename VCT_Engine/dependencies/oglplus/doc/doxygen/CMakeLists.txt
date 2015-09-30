#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
cmake_minimum_required(VERSION 2.8)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_subdirectory(oglplus)
	add_subdirectory(oalplus)
	add_subdirectory(eglplus)

	# docs may or may not be built by default
	# based on the user preferences
	if(OGLPLUS_NO_DOCS)
		add_custom_target(
			doc
			DEPENDS
				oglplus-doc
				oalplus-doc
				eglplus-doc
		)
	else()
		add_custom_target(
			doc ALL
			DEPENDS
				oglplus-doc
				oalplus-doc
				eglplus-doc
		)
	endif()
	add_custom_target(
		rebuild-doc
		DEPENDS
			rebuild-oglplus-doc
			rebuild-oalplus-doc
			rebuild-eglplus-doc
	)
	set_property(TARGET doc PROPERTY FOLDER "Documentation")
	set_property(TARGET rebuild-doc PROPERTY FOLDER "Documentation")
else()
	set(OGLPLUS_NO_DOCS On PARENT_SCOPE)
endif()

