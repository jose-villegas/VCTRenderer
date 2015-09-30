#!/bin/bash
# Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
rootdir=${1:-${PWD}}
#
find ${rootdir}/include -type f -name '*.hpp' -exec grep \
	-e '\<GL_[A-Z0-9_]\+\>' \
 {} \; |
grep -v -e '^\s*#define' -e '^\s*///' |
sed 's/.*\<GL_\([A-Z0-9_]\+\).*/\1/g' |
sort | uniq |
grep -v -e '^\s*$'
