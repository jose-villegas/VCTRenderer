#!/bin/bash
# Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
oglplus_lib=${1:-oglplus}
oglplus_example_dir=./$(dirname $0)/../example/${oglplus_lib}
oglplus_examples=${oglplus_example_dir}/[0-9][0-9][0-9]*.cpp

function make_example_req_list()
{
	dependency_dir="${1}"
	dependency_typ="${2}"
	dependency_tag="${3}"

	for example_path in ${oglplus_examples}
	do
		example_name=$(basename ${example_path} .cpp)

		echo ${example_name}

		dep_file=${oglplus_example_dir}/${dependency_dir}/${example_name}.${dependency_typ}.txt

		grep -e"${dependency_tag}" ${example_path} |
		sed -e's/.*{\([^}]\+\)}/\1/' |
		sort |
		uniq > ${dep_file}

		if [ -s ${dep_file} ]
		then git add ${dep_file}
		else git rm -f ${dep_file} 2> /dev/null || rm -f ${dep_file}
		fi
	done
}

function make_example_dep_lists()
{(
	temp_file=$(mktemp)
	#
	pushd ${PWD} > /dev/null
	cd $(dirname $0)/../
	#
	for example_path in example/${oglplus_lib}/[0-9][0-9][0-9]*.cpp
	do
		example_name=$(basename ${example_path} .cpp)
		echo ${example_name}
		dependency_file=example/${oglplus_lib}/dependencies/${example_name}.txt

		g++ --std=c++0x ${example_path} ${CXXFLAGS} -Iinclude -Iimplement -Ithird_party/include -DOGLPLUS_NO_SITE_CONFIG=1 -DOGLPLUS_NO_GL=1 -E -M -o ${temp_file}

		for dep_data
		do
			search_include=${dep_data%%:*}
			dependency_name=${dep_data##*:}
			if [ $(grep -c -e "${search_include}\>" < ${temp_file}) -gt 0 ]
			then echo ${dependency_name}
			fi
		done > ${dependency_file}

		if [ -s ${dependency_file} ]
		then git add ${dependency_file}
		else git rm -f ${dependency_file} 2> /dev/null || rm -f ${dependency_file}
		fi
	done
	#
	popd > /dev/null
	#
	rm -f ${temp_file}
)}

echo "----------------------|C++ 2011 features|---------------------"
make_example_req_list requirements cpp "@${oglplus_lib}_example_uses_cpp_feat"
echo "------------------------|OpenGL Version|----------------------"
make_example_req_list requirements gl "@${oglplus_lib}_example_uses_gl"
echo "--------------------------|Textures|--------------------------"
make_example_req_list resources tex "@${oglplus_lib}_example_uses_texture"
echo "---------------------------|Models|---------------------------"
make_example_req_list resources model "@${oglplus_lib}_example_uses_model"
echo "-------------------------|Libraries|--------------------------"
make_example_dep_lists png.h:PNG

