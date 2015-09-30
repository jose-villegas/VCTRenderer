#  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
set(wxWidgets_USE_UNICODE Off)

find_package(wxWidgets COMPONENTS gl core base adv)

if(wxWidgets_FOUND)
	set(WXGL_FOUND True)
endif()

