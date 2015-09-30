#!/bin/bash
# Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
src_path=${1}

if [ ${#src_path} -eq 0 ]
then echo "Usage: $(basename $0) <path-to-c++-source>" && exit 1
fi

sed -n 's/^.*OGLPLUS_GLFUNC(\([^)]\+\)).*$/\1/p' < ${src_path} |
uniq | sort | uniq |
while read gl_sym
do $(dirname $0)/_get_gl_sym_ver.sh "${gl_sym}" ${2} | grep "${gl_sym}:"
done

