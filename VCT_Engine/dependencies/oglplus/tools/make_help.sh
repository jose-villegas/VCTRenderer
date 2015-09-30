#!/bin/bash
# Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# make the manual for the configure script
(
	cd config &&
	groff -Tascii -man ./help.man > ./help.txt
)
git add ./config/help.txt
