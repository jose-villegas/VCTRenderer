#!/bin/bash
# Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
if [ ! -f ./configure.sh ]
then echo "Run from the OGLplus root dir!" &&  exit 1
fi

rm -rf ./_prebuilt ./_build

./configure.sh --from-scratch --no-examples --no-docs --quiet
(cd ./_build && make oglplus-textures -j 4)

#
# move built files to the pre-built directory
function move_files()
{
	build_dir="./_build"
	prebuilt="./_prebuilt"

	mkdir -p ${prebuilt}/${1}
	mv ${build_dir}/${1}/*.${2} ${prebuilt}/${1}/
}
#
move_files textures png
