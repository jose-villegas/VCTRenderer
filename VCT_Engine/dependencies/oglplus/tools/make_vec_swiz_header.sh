#!/bin/bash
# Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
RootDir=${1:-${PWD}}

HeaderDir="${RootDir}/include/oglplus/math"
mkdir -p ${HeaderDir}
Output="${HeaderDir}/vector_swizzle.ipp"


function PrintFileHeader()
{
	echo "/*"
	echo " *  .file ${2##${RootDir}/include/}"
	echo " *"
	echo " *  Automatically generated header file. DO NOT modify manually,"
	echo " *  edit '${1##${RootDir}/}' instead."
	echo " *"
	echo " *  Copyright 2010-$(date +%Y) Matus Chochlik. Distributed under the Boost"
	echo " *  Software License, Version 1.0. (See accompanying file"
	echo " *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)"
	echo " */"
	echo
}

function Combinations()
{
	[[ ${#1} -lt 2 ]] && return
	local Coords=${1}
	local SepCoords=$(echo ${Coords} | sed 's/./,&/g')
	local CombExpr=$(
		for I in $(seq 1 ${2})
		do echo -n "{${SepCoords:1}}"
		done
	)
	eval "echo ${CombExpr}"
}

function MakeSwizzle()
{
	local Coords=${1}
	local MaxN=${#Coords}

	echo "template <typename T, std::size_t N>"
	echo "class Swizzled_${Coords};"
	for N in $(seq 1 ${MaxN})
	do
		echo
		echo "template <typename T>"
		echo "class Swizzled_${Coords}<T, ${N}>"
		echo " : public Vector<T, ${N}>"
		echo "{"
		echo "public:"
		echo "	Swizzled_${Coords}(Vector<T, ${N}> v)"
		echo "	 : Vector<T, ${N}>(v)"
		echo "	{ }"
		echo
		for I in $(seq 1 ${N})
		do
			echo "	T ${Coords:$[I-1]:1}(void) const " \
				"{ return this->template At($[I-1]); }"
		done

		for I in $(seq 2 ${N})
		do
			echo
			for Comb in $(Combinations ${Coords:0:${N}} ${I})
			do
				echo -n "	Vector<T, ${#Comb}> ${Comb}(void) const" \
					"{ return Vector<T, ${#Comb}>("
				echo ${Comb} | sed 's/./&(),/g' | sed 's/,$/); }/'
			done
		done
		echo "};"
	done
	echo
	echo "template <typename T, std::size_t N>"
	echo "inline Swizzled_${Coords}<T, N> Swizzle_${Coords}(Vector<T, N> v)"
	echo "{"
	echo "	return Swizzled_${Coords}<T, N>(v);"
	echo "}"
}

(
	exec > ${Output}
	PrintFileHeader tools/$(basename $0) ${Output}
	MakeSwizzle xyzw
	echo
	echo "template <typename T, std::size_t N>"
	echo "inline Swizzled_xyzw<T, N> Swizzle(Vector<T, N> v)"
	echo "{"
	echo "	return Swizzled_xyzw<T, N>(v);"
	echo "}"
)
git add ${Output}
