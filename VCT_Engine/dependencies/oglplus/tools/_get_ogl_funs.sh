#!/bin/bash
# Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
rootdir=${1:-${PWD}}
#
find ${rootdir}/include -type f -name '*.hpp' -exec grep \
	-e '::gl[A-Za-z0-9_]\+\>' \
	-e 'OGLPLUS_AUX_VARPARA_FNS(' \
 {} \; |
grep -v -e '^\s*#define' |
sed 's/.*::gl\([A-Za-z0-9_]\+\).*/\1/g' |
sed 's/.*OGLPLUS_AUX_VARPARA_FNS(\([A-Za-z0-9_]\+\)\s*,\s*\([A-Za-z0-9_]\+\)\s*,.*/\11\2\n\12\2\n\13\2\n\14\2\n/g' |
sort | uniq |
grep -v -e '^\s*$'
