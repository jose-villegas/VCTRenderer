#!/bin/bash
# Copyright 2008-2014 Matus Chochlik. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
oglplus_archive_items="
	_prebuilt
	config
	doc
	example
	include
	implement
	source
	third_party
	test
	utils
	xslt
	LICENSE_1_0.txt
	README.rst
	CHANGELOG
	VERSION
	CMakeLists.txt
	configure.py
	configure.sh
	oglplus-config
	oglplus-config-comp
"

if [ ! -f ./configure.sh ]
then echo "Run from the OGLplus root dir!" &&  exit 1
fi

#
file_basename="oglplus-$(< VERSION)"

fixlnpath_sed='s|\./||;s|[^/]\+/|../|g'

mkdir -p ${file_basename}
for item in ${oglplus_archive_items}
do
	mkdir -p $(dirname ${file_basename}/${item})
	ln -s ../$(echo "`dirname ${item}`/" | sed "${fixlnpath_sed}")${item} ${file_basename}/${item}
done
tar -hcf ${file_basename}.tar ${file_basename}
gzip -9 ${file_basename}.tar
rm -rf ${file_basename}
