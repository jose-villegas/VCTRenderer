#!/bin/bash
# Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
exe_path=${1}
lib_name=${2:-libGL.so}
lib_hdr=${3:-$(dirname ${0})/../third_party/include/GL/glcorearb.h}

if [ ${#exe_path} -eq 0 ]
then echo "Usage: $(basename $0) <path-to-executable>" && exit 1
fi

if [ ! -x ${exe_path} ]
then echo "Error: ${exe_path} is not an executable" && exit 1
fi

lib_path=$(
	ldd ${exe_path} |
	grep -e "${lib_name}" |
	while read name sep path rest
	do echo ${path}
	done
)

if [ ${#lib_path} -eq 0 ]
then echo "Error: Executable does not link to ${lib_name}" && exit 1
fi

if [ ! -f "${lib_path}" ]
then echo "Error: File ${lib_path} not found" && exit 1
fi

if [ -h "${lib_path}" ]
then lib_path="$(readlink -e ${lib_path})"
fi

tmp_file=$(mktemp)
ltrace --demangle --library "${lib_path}"  "${exe_path}" 2> "${tmp_file}" > /dev/null &
exe_pid=$!
sleep 3
kill ${exe_pid}

cut -d'(' -f1 "${tmp_file}" |
uniq | sort | uniq |
grep -e '^gl' |
while read gl_sym
do $(dirname $0)/_get_gl_sym_ver.sh "${gl_sym}" ${lib_hdr} #| grep "${gl_sym}:"
done #|
#sed -n 's/^.*GL_VERSION_\([1-9]_[0-9]\).*$/\1/p' |
#uniq | sort | uniq |
#tail -1

rm -f ${tmp_file}
