#  Copyright 2014 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
ROOT = $(PARENT)/../..
MAKE_ENUM = $(PARENT)/make_enum.py

.PHONY: all
all: \
	_incl_enum_hpp \
	_incl_enum_ipp \
	_impl_enum_def_ipp \
	_impl_enum_names_ipp \
	_impl_enum_range_ipp \
	_impl_enum_class_ipp \
	_impl_enum_type_ipp \
	_impl_enum_bq_ipp \
	_smart_enums_ipp \
	_smart_values_ipp \
	_lib_enum_value_name_ipp \
	_lib_enum_value_range_ipp \
	_qbk_qref_hpp \
	_qbk_class_hpp

.PHONY: _incl_enum_hpp
_incl_enum_hpp: $(addprefix $(ROOT)/include/$(LIBRARY)/enums/,$(patsubst %.txt,%.hpp,$(SOURCES)))

$(ROOT)/include/$(LIBRARY)/enums/%.hpp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action incl_enum_hpp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _incl_enum_ipp
_incl_enum_ipp: $(addprefix $(ROOT)/include/$(LIBRARY)/enums/,$(patsubst %.txt,%.ipp,$(SOURCES)))

$(ROOT)/include/$(LIBRARY)/enums/%.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action incl_enum_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _impl_enum_def_ipp
_impl_enum_def_ipp: $(addprefix $(ROOT)/implement/$(LIBRARY)/enums/,$(patsubst %.txt,%_def.ipp,$(SOURCES)))

$(ROOT)/implement/$(LIBRARY)/enums/%_def.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_def_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _impl_enum_names_ipp
_impl_enum_names_ipp: $(addprefix $(ROOT)/implement/$(LIBRARY)/enums/,$(patsubst %.txt,%_names.ipp,$(SOURCES)))

$(ROOT)/implement/$(LIBRARY)/enums/%_names.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_names_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _impl_enum_range_ipp
_impl_enum_range_ipp: $(addprefix $(ROOT)/implement/$(LIBRARY)/enums/,$(patsubst %.txt,%_range.ipp,$(SOURCES)))

$(ROOT)/implement/$(LIBRARY)/enums/%_range.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_range_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _impl_enum_class_ipp
_impl_enum_class_ipp: $(addprefix $(ROOT)/implement/$(LIBRARY)/enums/,$(patsubst %.txt,%_class.ipp,$(SOURCES)))

$(ROOT)/implement/$(LIBRARY)/enums/%_class.ipp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_enum_class_ipp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"


_specific.mk: $(SOURCES) $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action specific_mk \
		--output "$@" \
		--output-id "none" \
		$(filter %.txt,$^)
	git add "$@"

sinclude _specific.mk

.PHONY: _smart_enums_ipp
_smart_enums_ipp: $(ROOT)/implement/$(LIBRARY)/detail/smart_enums.ipp

$(ROOT)/implement/$(LIBRARY)/detail/smart_enums.ipp: $(SOURCES) $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action smart_enums_ipp \
		--output "$@" \
		--output-id "none" \
		$(filter %.txt,$^)
	git add "$@"

.PHONY: _smart_values_ipp
_smart_enums_ipp: $(ROOT)/implement/$(LIBRARY)/detail/smart_values.ipp

$(ROOT)/implement/$(LIBRARY)/detail/smart_values.ipp: $(SOURCES) $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action smart_values_ipp \
		--output "$@" \
		--output-id "none" \
		$(filter %.txt,$^)
	git add "$@"

.PHONY: _lib_enum_value_name_ipp
_lib_enum_value_name_ipp: $(ROOT)/implement/$(LIBRARY)/lib/enum_value_name.ipp

$(ROOT)/implement/$(LIBRARY)/lib/enum_value_name.ipp: $(SOURCES) $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_lib_enum_value_name_ipp \
		--output "$@" \
		--output-id "none" \
		$(filter %.txt,$^)
	git add "$@"

.PHONY: _lib_enum_value_range_ipp
_lib_enum_value_range_ipp: $(ROOT)/implement/$(LIBRARY)/lib/enum_value_range.ipp

$(ROOT)/implement/$(LIBRARY)/lib/enum_value_range.ipp: $(SOURCES) $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action impl_lib_enum_value_range_ipp \
		--output "$@" \
		--output-id "none" \
		$(filter %.txt,$^)
	git add "$@"

.PHONY: _qbk_qref_hpp
_qbk_qref_hpp: $(addprefix $(ROOT)/doc/quickbook/$(LIBRARY)/quickref/enums/,$(patsubst %.txt,%.hpp,$(SOURCES)))

$(ROOT)/doc/quickbook/$(LIBRARY)/quickref/enums/%.hpp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action qbk_hpp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

.PHONY: _qbk_class_hpp
_qbk_class_hpp: $(addprefix $(ROOT)/doc/quickbook/$(LIBRARY)/quickref/enums/,$(patsubst %.txt,%_class.hpp,$(SOURCES)))

$(ROOT)/doc/quickbook/$(LIBRARY)/quickref/enums/%_class.hpp: %.txt $(MAKE_ENUM)
	$(MAKE_ENUM) \
		--library $(LIBRARY) \
		--base-lib-prefix $(LIB_PREFIX)\
		--action qbk_class_hpp \
		--input "$<" \
		--output "$@" \
		--output-id "$(subst /,_,$*)"
	git add "$@"

