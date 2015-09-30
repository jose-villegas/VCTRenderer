#  Copyright 2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

.PHONY: all

all: html/index.html

.PHONY: clean

clean:
	rm -r bin/ html/

html/images:
	mkdir -p $@

html/images/callouts:
	mkdir -p $@

.PRECIOUS: images/callouts/%.png

images/callouts/%.png: images/callouts/%.svg
	inkscape --export-png=$@ -w 22 -h 12 $<

.PRECIOUS: images/%.png

images/%.png: images/%.svg
	inkscape --export-png=$@ -w 32 -h 32 $<

html/images/callouts/%.png: images/callouts/%.png | html/images/callouts
	cp $< $@

html/images/%.png: images/%.png | html/images
	cp $< $@

.PHONY: html_images

html_images: \
	html/images/callouts/1.png \
	html/images/callouts/2.png \
	html/images/callouts/3.png \
	html/images/callouts/4.png \
	html/images/callouts/5.png \
	html/images/callouts/6.png \
	html/images/callouts/7.png \
	html/images/callouts/8.png \
	html/images/callouts/9.png \
	html/images/callouts/10.png \
	html/images/callouts/11.png \
	html/images/callouts/12.png \
	html/images/callouts/13.png \
	html/images/callouts/14.png \
	html/images/callouts/15.png \
	html/images/note.png \
	html/images/home.png \
	html/images/next.png \
	html/images/prev.png \
	html/images/up.png

html/%.css: %.css
	cp $< $@

.PHONY: html_css

html_css: html/boostbook.css

html/index.html: html_images html_css
	bjam
