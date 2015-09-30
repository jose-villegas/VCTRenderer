#!/bin/bash
# Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
script_dir=$(dirname ${0})
if [ "${script_dir:0:1}" == "/" ]
then root_dir="${script_dir}/.."
else root_dir="${PWD}/${script_dir}/.."
fi

function xml2tree()
{
	xsltpl="${root_dir}/xslt/ARB_d_o_xml2tree.xsl"
	if [ -f "${xsltpl}" ]
	then xsltproc --stringparam "width" "$(tput cols)" ${xsltpl} -
	else cat
	fi
}

cd ${root_dir}/_build/example/${2:-oglplus}/
for example in ./*${1}*
do
	make ${example} &&
	${example} | xml2tree
done
