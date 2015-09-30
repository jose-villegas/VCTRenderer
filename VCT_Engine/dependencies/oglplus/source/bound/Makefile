#  Copyright 2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
HERE := $(PWD)
ROOT = $(shell readlink -f $(HERE)/../..)
INCDIR = $(ROOT)/include/oglplus
DOCDIR = $(ROOT)/doc/quickbook
QRFDIR = $(DOCDIR)/oglplus/quickref

XSLTPROC ?= xsltproc
DOXYGEN ?= doxygen

OBJECTS = texture framebuffer renderbuffer buffer

all: bound_hpp clean

bound_hpp: bound_inc_hpp bound_doc_hpp

bound_inc_hpp: $(addprefix $(INCDIR)/bound/,$(addsuffix .hpp,$(OBJECTS)))

$(INCDIR)/bound/%.hpp: tmp/xml/oglplus.xml bound_inc_hpp.xsl | $(INCDIR)/bound
	$(XSLTPROC) --output $@ \
		--stringparam object $* \
		--stringparam year "$(shell date +%Y)" \
		bound_inc_hpp.xsl \
		tmp/xml/oglplus.xml

bound_doc_hpp: $(addprefix $(QRFDIR)/bound/,$(addsuffix .hpp,$(OBJECTS)))

$(QRFDIR)/bound/%.hpp: tmp/bound/%.hpp tmp/anchors.sed | $(QRFDIR)/bound
	sed -f tmp/anchors.sed < $< > $@

tmp/anchors.sed: tmp/anchors.txt
	sed 's/\([^|]\+\)|\(.*\)/s\/\\<\1\\>\/__\2\//' < $^ > $@

tmp/anchors.txt: Makefile
	@find $(DOCDIR) \
		-maxdepth 1 \
		-type f \
		-name '*.qbk' \
		-exec sed -n 's/^.*\[def __\(\w\+\)\s.*\s\[\?\([^]]\+\)\]\?\].*/\2|\1/p' {} \; |\
	grep -e '\(BoundObjOps\|Target\)' -v > $@

tmp/bound/%.hpp: tmp/xml/oglplus.xml bound_doc_hpp.xsl | tmp/bound
	$(XSLTPROC) --output $@ \
		--stringparam object $* \
		--stringparam year "$(shell date +%Y)" \
		bound_doc_hpp.xsl \
		tmp/xml/oglplus.xml

tmp/xml/oglplus.xml: tmp/xml/index.xml
	$(XSLTPROC) --output $@ \
		tmp/xml/combine.xslt \
		tmp/xml/index.xml

tmp/xml/index.xml: tmp/Doxyfile
	cd $(ROOT)/doc/doxygen &&\
	$(DOXYGEN) $(HERE)/tmp/Doxyfile

tmp/Doxyfile: Makefile | tmp
	@echo "@INCLUDE = $(ROOT)/doc/doxygen/Doxyfile.autohdr" > $@
	@echo "OUTPUT_DIRECTORY = $(HERE)/tmp" >> $@
	@echo "QUIET = YES" >> $@

tmp tmp/bound $(INCDIR)/bound $(QRFDIR)/bound:
	mkdir -p $@

clean:
	rm -rf tmp

.PHONY: clean
