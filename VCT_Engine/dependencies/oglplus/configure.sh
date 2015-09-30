#!/bin/bash
#  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# defaults
oglplus_default_build_dir=_build
oglplus_forced_gl_header=""
oglplus_forced_gl_init_lib=""
oglplus_no_boost_config=false
oglplus_no_examples=false
oglplus_no_screenshots=true
oglplus_no_docs=false
oglplus_use_cxxflags=false
oglplus_use_ldflags=false

dry_run=false
from_scratch=false
quiet=false
build=false
install=false
num_jobs=""
#
function oglplus_make_temp_file()
{
	mktemp -t oglplus-configure.XXXXXXXX
}
#
function echoerror()
{
	echo "$@" 1>&2
}
# function for filesystem path normalization
function normalize_path()
{
	local normal_path="${1}"

	# make an absolute path
	case "${normal_path}" in
		/*);;
		~*) normal_path="${HOME}${normal_path:1}";;
		*)  normal_path="${PWD%/}/${normal_path}";;
	esac
	# translates '/./' -> '/'
	normal_path="${normal_path//\/.\///\/}"

	# removes 'dir/../'
	while [[ "${normal_path}" =~ ([^/][^/]*/\.\./) ]]
	do normal_path="${normal_path/${BASH_REMATCH[0]}/}"
	done

	# removes '/dir/..' from the end of the path
	if [[ "${normal_path}" =~ (/[^/][^/]*/\.\.) ]]
	then normal_path="${normal_path/${BASH_REMATCH[0]}/}"
	fi

	# removes '/.' from the end of the path
	echo ${normal_path%/.}
}
#
# function that finds the shortest path from $1 to $2
function shortest_path_from_to()
{
	local source_path="$(normalize_path ${1})"
	local target_path="$(normalize_path ${2})"

	# if the paths are equal after normalization
	if [ "${source_path}" == "${target_path}" ]
	then echo "." && exit
	fi

	local common_part="${source_path}/"
	local backtr_part=

	while [ "${target_path#${common_part}}" == "${target_path}" ]
	do
		common_part="$(dirname ${common_part})"
		backtr_part="../${backtr_part}"
	done

	local result_path="${backtr_part%/}${target_path#${common_part}}"

	if [ ${#result_path} -gt ${#target_path} ]
	then echo ${target_path%/}
	else echo ${result_path%/}
	fi
}

oglplus_src_root="$(normalize_path "${PWD%/}/$(dirname ${0})")"
#
unset oglplus_prefix
unset oglplus_build_dir
unset oglplus_cmake_options
unset header_search_paths
unset library_search_paths
#
# prints a short help screen
function print_short_help()
{
	echo "Unknown option '${1}'"
	echo "Use $(basename ${0}) --help to print a help screen."
	return 0
}
#
# prints a full help screen
function print_help()
{
	cat $(dirname $0)/config/help.txt
	return 0
}
#
# parses a path-specifying command-line option
function parse_path_spec_option()
{
	local option_tag=${1}
	local option=${2}
	local next_option=${3}

	unset option_path

	case "${option}" in
	${option_tag})
		option_path="$(normalize_path "${next_option}")"
		return 1;;

	${option_tag}=*)
		option_path="$(normalize_path "${option#${option_tag}=}")"
		return 0;;

	${option_tag}*)
		option_path="$(normalize_path "${option#${option_tag}}")"
		return 0;;

	*) echoerror "Parse error at ${option} ${next_option}" && exit 1
	esac
}

#
# parse the command line options
while true
do
	case "${1}" in
	"") break;;
	-|--cmake)
		shift
		for arg
		do oglplus_cmake_options="${oglplus_cmake_options} '${arg}'"
		done
		break;;
	--generator)
		shift
		oglplus_cmake_options="${oglplus_cmake_options} -G ${1// /\ }"
		break;;
	--prefix*)
		parse_path_spec_option "--prefix" "${1}" "${2}" || shift
		oglplus_prefix=${option_path}
		unset option_path;;

	--build-dir*)
		parse_path_spec_option "--build-dir" "${1}" "${2}" || shift
		oglplus_build_dir=${option_path}
		unset option_path;;

	--include-dir*)
		parse_path_spec_option "--include-dir" "${1}" "${2}" || shift
		header_search_paths="${header_search_paths}${option_path};"
		unset option_path;;

	--library-dir*)
		parse_path_spec_option "--library-dir" "${1}" "${2}" || shift
		library_search_paths="${library_search_paths}${option_path};"
		unset option_path;;

	--use-gl-api-lib)
		shift
		case "${1}" in
		GLCOREARB_H|GL3_H|GLEW|GL3W)oglplus_forced_gl_header=${1};;
		*) echoerror "Unknown GL header lib '${1}'" && exit 1;;
		esac
		;;
	--use-gl-api-lib=*)
		case "${1##*=}" in
		GLCOREARB_H|GL3_H|GLEW|GL3W)oglplus_forced_gl_header=${1##*=};;
		*) echoerror "Unknown GL header lib '${1##*=}'" && exit 1;;
		esac
		;;

	--use-glcorearb-h) oglplus_forced_gl_header=GLCOREARB_H;;
	--use-gl3-h) oglplus_forced_gl_header=GL3_H;;
	--use-glew) oglplus_forced_gl_header=GLEW;;
	--use-gl3w) oglplus_forced_gl_header=GL3W;;

	--use-gl-init-lib)
		shift
		case "${1}" in
		GLX|GLUT|GLFW3|GLFW|SDL|WXGL|QT4GL|QT5GL) oglplus_forced_gl_init_lib=${1};;
		*) echoerror "Unknown GL initialization lib '${1}'" && exit 1;;
		esac
		;;
	--use-gl-init-lib=*)
		case "${1##*=}" in
		GLX|GLUT|GLFW3|GLFW|SDL|WXGL|QT4GL|QT5GL) oglplus_forced_gl_init_lib=${1##*=};;
		*) echoerror "Unknown GL initialization lib '${1##*=}'" && exit 1;;
		esac
		;;

	--use-glx) oglplus_forced_gl_init_lib=GLX;;
	--use-glut) oglplus_forced_gl_init_lib=GLUT;;
	--use-glfw3) oglplus_forced_gl_init_lib=GLFW3;;
	--use-glfw) oglplus_forced_gl_init_lib=GLFW;;
	--use-sdl) oglplus_forced_gl_init_lib=SDL;;
	--use-wxgl) oglplus_forced_gl_init_lib=WXGL;;
	--use-qt4gl) oglplus_forced_gl_init_lib=QT4GL;;
	--use-qt5gl) oglplus_forced_gl_init_lib=QT5GL;;

	--no-boost-config) oglplus_no_boost_config=true;;

	--no-examples) oglplus_no_examples=true;;
	--screenshots) oglplus_no_screenshots=false;;
	--no-docs) oglplus_no_docs=true;;

	--use-cxxflags) oglplus_use_cxxflags=true;;
	--use-ldflags) oglplus_use_ldflags=true;;

	--dry-run) dry_run=true;;
	--from-scratch) from_scratch=true;;
	--quiet) quiet=true;;
	--build) build=true;;
	--install) build=true && install=true;;
	--jobs)
		shift
		if [[ "${1}" =~ ^[1-9][0-9]*$ ]]
		then num_jobs=${1}
		else echoerror "'${1}' is not a valid job count" && exit 1
		fi
		;;

	-h|--help) print_help && exit 0;;
	*) print_short_help ${1} && exit 1;;
	esac
	shift
done

if [ "${oglplus_use_cxxflags}" == "true" ]
then
	set - ${CXXFLAGS}
	while true
	do
		case "${1}" in
		"") break;;
		-I*)
			parse_path_spec_option "-I" "${1}" "${2}" || shift
			header_search_paths="${header_search_paths}${option_path};"
			unset option_path;;
		*);;
		esac
		shift
	done
fi

if [ "${oglplus_use_ldflags}" == "true" ]
then
	set - ${LDFLAGS}
	while true
	do
		case "${1}" in
		"") break;;
		-L*)
			parse_path_spec_option "-L" "${1}" "${2}" || shift
			library_search_paths="${library_search_paths}${option_path};"
			unset option_path;;
		*);;
		esac
		shift
	done
fi
#
# use the defaults for params that were not
# set explicitly
if [ "${oglplus_build_dir}" == "" ]
then oglplus_build_dir="${oglplus_default_build_dir}"
fi

#
# check for prerequisities
#
# the directories
for oglplus_dir in "config" "doc" "include/oglplus" "example" "source" "utils" "xslt"
do
	if [ ! -d "${oglplus_src_root}/${oglplus_dir}" ]
	then echoerror "Missing the header directory" && exit 4
	fi
done
#
# the cmake executable
if [ ! -x "$(which cmake)" ]
then echoerror "cmake is required." && exit 5
fi
#
# let cmake dump some system information into a temp file
# in a form usable by bash
# temporary file where the commands to be executed are stored
command_file=$(oglplus_make_temp_file)
cmake_info_file=$(oglplus_make_temp_file)
echo "cmake --system-information ${oglplus_cmake_options}" > ${command_file}
(. ${command_file}) |
grep -e "CMAKE_BUILD_TOOL" -e "CMAKE_INSTALL_PREFIX" |
tr ' ' '=' > ${cmake_info_file}
#
# use the system information
source ${cmake_info_file} || exit $?
# remove the system info file
rm -f ${cmake_info_file}
#
#
# check the main cmake script
if [ ! -f "${oglplus_src_root}/CMakeLists.txt" ]
then echoerror "Missing the main cmake script." && exit 5
fi
#
# check the build tool
if [ "${CMAKE_BUILD_TOOL}" == "" ]
then echoerror "No build tool usable by cmake found." && exit 5
fi
#
if [ ! -x "${CMAKE_BUILD_TOOL}" ]
then echoerror "${CMAKE_BUILD_TOOL} is required." && exit 5
fi
#
# shorten the build tool command if possible
if [ "$(which $(basename ${CMAKE_BUILD_TOOL}))" == "${CMAKE_BUILD_TOOL}" ]
then CMAKE_BUILD_TOOL="$(basename ${CMAKE_BUILD_TOOL})"
fi

# pass the no-examples option
if [ "${oglplus_no_examples}" == "true" ]
then oglplus_cmake_options="'-DOGLPLUS_NO_EXAMPLES=On' ${oglplus_cmake_options}"
fi

if [ "${oglplus_no_screenshots}" == "true" ]
then oglplus_cmake_options="'-DOGLPLUS_NO_SCREENSHOTS=On' ${oglplus_cmake_options}"
fi

# pass the no-boost-config option
if [ "${oglplus_no_boost_config}" == "true" ]
then oglplus_cmake_options="'-DOGLPLUS_NO_BOOST_CONFIG=On' ${oglplus_cmake_options}"
fi

# pass the no-docs option
if [ "${oglplus_no_docs}" == "true" ]
then oglplus_cmake_options="'-DOGLPLUS_NO_DOCS=On' ${oglplus_cmake_options}"
fi

# pass the forced GL header option to cmake
if [ "${oglplus_forced_gl_header}" != "" ]
then oglplus_cmake_options="'-DOGLPLUS_FORCE_GL_API_LIB=${oglplus_forced_gl_header}' ${oglplus_cmake_options}"
fi

# pass the forced GL initializer library option to cmake
if [ "${oglplus_forced_gl_init_lib}" != "" ]
then oglplus_cmake_options="'-DOGLPLUS_FORCE_GL_INIT_LIB=${oglplus_forced_gl_init_lib}' ${oglplus_cmake_options}"
fi

# pass the list of paths to search for libraries to cmake
if [ "${library_search_paths}" != "" ]
then oglplus_cmake_options="'-DLIBRARY_SEARCH_PATHS=${library_search_paths%;}' ${oglplus_cmake_options}"
fi

# pass the list of paths to search for headers to cmake
if [ "${header_search_paths}" != "" ]
then oglplus_cmake_options="'-DHEADER_SEARCH_PATHS=${header_search_paths%;}' ${oglplus_cmake_options}"
fi

# pass the install prefix to cmake
if [ "${oglplus_prefix}" != "" ]
then oglplus_cmake_options="'-DCMAKE_INSTALL_PREFIX=${oglplus_prefix}' ${oglplus_cmake_options}"
fi

# make the configuration commands
(
	exec > ${command_file}
	if [ "${from_scratch}" == "true" ]
	then echo "rm -rf '${oglplus_build_dir##${PWD%/}/}' &&"
	fi
	echo "mkdir -p '${oglplus_build_dir##${PWD%/}/}' &&"
	echo "cd '${oglplus_build_dir##${PWD%/}/}' &&"
	echo "cmake ${oglplus_cmake_options} $(shortest_path_from_to "${oglplus_build_dir}" "${oglplus_src_root}")"
	echo "configure_result=\$?"
	echo "cd '${PWD}'"
)

if ${dry_run}
then cat ${command_file}
else source ${command_file}
fi

rm -f ${command_file}

if ${dry_run}
then exit
fi

if [ ${configure_result:-0} -eq 0 ]
then
	if [ ${#num_jobs} -eq 0 ]
	then num_jobs=$(($(grep -c ^processor /proc/cpuinfo)+1))
	fi
	build_script=$(oglplus_make_temp_file)
	(
		echo
		echo "# Configuration completed successfully."
		echo -n "# To build "
		if ${install}
		then echo -n "and install "
		fi
		echo "OGLplus do the following:"
		echo
		echo "cd $(shortest_path_from_to "${PWD}" "${oglplus_build_dir}") &&"
		echo -n "${CMAKE_BUILD_TOOL} -j ${num_jobs}"
		if ${install}
		then echo -e " &&\n${CMAKE_BUILD_TOOL} install"
		else echo
		fi
		echo
		echo "# NOTE: installing may require administrative privilegues"
	) > ${build_script}

	if ${build}
	then (source ${build_script})
	elif ! ${quiet}
	then cat ${build_script}
	fi
	rm -f "${build_script}"
else
	echoerror "# Configuration failed with error code ${configure_result}."
	exit ${configure_result}
fi

exit
