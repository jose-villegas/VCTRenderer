#!/bin/bash
# Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
symbol=${1}
gl_hdr=${2:-$(dirname ${0})/../third_party/include/GL/glcorearb.h}
#
branch_stack=()
#
function push_branch()
{
	shift
	branch_stack=( "${branch_stack[@]}" "${*}" )
}
#
function pop_branch()
{
	if [ ${#branch_stack[*]} -gt 0 ]
	then unset branch_stack[${#branch_stack[@]}-1]
	fi
}
#
function get_branches()
{
	for b in ${branch_stack[@]}
	do
		case ${b} in
			GL_ARB*) echo ":$($0 ${b#GL_} ${gl_hdr})";;
			GL_*) echo -n ":${b}";;
		esac
	done
}
#
function process_define()
{
	echo ${2}$(get_branches)
}
#
function process_function()
{
	echo ${4}$(get_branches)
}
#
function process_extension()
{
	echo GL_${symbol}$(get_branches)
}
#
case ${symbol} in
	ARB_*)
		cat "${gl_hdr}" |
		while read line
		do
			case ${line} in
				"#if "*)    push_branch ${line};;
				"#ifdef"*)  push_branch ${line};;
				"#ifndef"*) push_branch ${line};;
				"#endif"*)  pop_branch;;
				"/* Reuse tokens from ${symbol}"*) process_extension ${line};;
			esac
		done
	;;
	GL_*|gl*|*)
		symbol=${symbol#gl}
		symbol=${symbol#GL_}
		cat "${gl_hdr}" |
		while read line
		do
			case ${line} in
				"#if "*)    push_branch ${line};;
				"#ifdef"*)  push_branch ${line};;
				"#ifndef"*) push_branch ${line};;
				"#endif"*)  pop_branch;;
				"#define GL_${symbol}"*) process_define ${line};;
				"GLAPI"*"APIENTRY gl${symbol}"*) process_function ${line};;
			esac
		done
	;;
esac
