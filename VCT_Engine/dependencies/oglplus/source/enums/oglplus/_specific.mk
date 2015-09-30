#  File source/enums/oglplus/_specific.mk
#
#  Automatically generated file, DO NOT modify manually.
#  Edit the enumeration source files in 'source/enums/oglplus'
#  or the 'source/enums/make_enum.py' script instead.
#
#  Copyright 2010-2015 Matus Chochlik.
#  Distributed under the Boost Software License, Version 1.0.
#  See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt
#

ROOT = ../../..

.PHONY: _impl_enum_bq_ipp
_impl_enum_bq_ipp:$(ROOT)/implement/oglplus/enums/buffer_indexed_target_bq.ipp $(ROOT)/implement/oglplus/enums/buffer_target_bq.ipp $(ROOT)/implement/oglplus/enums/framebuffer_target_bq.ipp $(ROOT)/implement/oglplus/enums/program_pipeline_stage_bq.ipp $(ROOT)/implement/oglplus/enums/renderbuffer_target_bq.ipp $(ROOT)/implement/oglplus/enums/texture_target_bq.ipp $(ROOT)/implement/oglplus/enums/transform_feedback_target_bq.ipp

$(ROOT)/implement/oglplus/enums/buffer_indexed_target_bq.ipp: buffer_indexed_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "buffer_indexed_target_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/buffer_target_bq.ipp: buffer_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "buffer_target_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/framebuffer_target_bq.ipp: framebuffer_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "framebuffer_target_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/program_pipeline_stage_bq.ipp: program_pipeline_stage.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "program_pipeline_stage_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/renderbuffer_target_bq.ipp: renderbuffer_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "renderbuffer_target_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/texture_target_bq.ipp: texture_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "texture_target_bq"
	git add "$@"

$(ROOT)/implement/oglplus/enums/transform_feedback_target_bq.ipp: transform_feedback_target.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_bq_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "transform_feedback_target_bq"
	git add "$@"

.PHONY: _impl_enum_type_ipp
_impl_enum_type_ipp: \
	$(ROOT)/implement/oglplus/enums/limit_query_type.ipp \
	$(ROOT)/implement/oglplus/enums/buffer_indexed_target_type.ipp \
	$(ROOT)/implement/oglplus/enums/pixel_parameter_type.ipp

$(ROOT)/implement/oglplus/enums/%_type.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_type_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$*_type"
	git add "$@"

