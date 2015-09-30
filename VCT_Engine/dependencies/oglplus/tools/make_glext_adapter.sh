#!/bin/bash
# Copyright 2010-2011 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
GLDefPath=${1}
RootDir=${2:-${PWD}}
#
Exts="\(ARB\|EXT\|INTEL\|AMD\|ATI\|NV\|NVIDIA\|GLX\|SGI\|SGIX\|HP\|3Dlabs\|IBM\|PGI\|MS\|SUN\|Mesa\|3Dfx\|S3\)"

if [ "${GLDefPath}" == "" ]
then
	echo "Error: You must specify a path to a header file containing OpenGL symbol definitions !" >&2
	exit 1
fi

if [ ! -f ${GLDefPath} ]
then
	echo "Error: File '${GLDefPath}' not found!" >&2
	exit 2
fi

if [ ! -r ${GLDefPath} ]
then
	echo "Error: Unable to open file '${GLDefPath}' for reading!" >&2
	exit 3
fi

for SymType in def fun
do
	$(dirname $0)/_get_ogl_${SymType}s.sh ${RootDir} |
	while read RawSymbol
	do
		case ${SymType} in
		def) Symbol="GL_${RawSymbol}";;
		fun) Symbol="gl${RawSymbol}";;
		esac
		# filter input header, try to find Symbol and chop away everything else
		cat ${GLDefPath} |
		case ${SymType} in
		def) sed \
			-e "s/^\s*#define\s\+\(\<${Symbol}\(_${Exts}\)\?\>\).*$/\1/p;" \
			-n;;
		fun) sed \
			-e "s/^\s*#define\s\+\(\<${Symbol}${Exts}\?\>\).*$/\1/p;" \
			-e "s/.*APIENTRY\s\+\(\<${Symbol}\>\).*$/\1/p;" \
			-n;;
		esac |
		while read AltSymbol
		do
			# if we found the same symbol
			if [ ${Symbol} == ${AltSymbol} ]
			then
				echo "// Reuse ${Symbol}"
				exit 1
			# if we found an alternative extenstion
			else
				echo "#define ${Symbol} ${AltSymbol}"
				exit 1
			fi
		done &&
		echo "Error: Unable to adapt symbol '${Symbol}' !" >&2 && exit 4
	done
done

