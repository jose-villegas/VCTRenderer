#!/usr/bin/python
# coding=utf-8
#  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
#  Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
import os, sys

def print_newline(options):
	options.out.write(os.linesep)

def print_line(options, line_str):
	options.out.write(line_str)
	options.out.write(os.linesep)

def print_comment(options, cs, line_str = str()):
	print_line(options, "%s%s" % (cs, line_str))

def print_header(options, cs):

	print_comment(options, cs, "  File %s" % options.rel_output)
	print_comment(options, cs)
	print_comment(options, cs, "  Automatically generated file, DO NOT modify manually.")
	if options.rel_input:
		print_comment(options, cs, "  Edit the source '%s'" % options.rel_input)
		print_comment(options, cs, "  or the '%s' script instead." % options.rel_self)
	else:
		print_comment(options, cs, "  Edit the enumeration source files in 'source/enums/%s'" % options.library)
		print_comment(options, cs, "  or the '%s' script instead." % options.rel_self)
	print_comment(options, cs)
	print_comment(options, cs, "  Copyright 2010-%d Matus Chochlik." % options.year)
	print_comment(options, cs, "  Distributed under the Boost Software License, Version 1.0.")
	print_comment(options, cs, "  See accompanying file LICENSE_1_0.txt or copy at")
	print_comment(options, cs, "  http://www.boost.org/LICENSE_1_0.txt")
	print_comment(options, cs)

def print_cpp_header(options):
	print_header(options, "//")

def print_scr_header(options):
	print_header(options, "#")

def capitalize_name(name):

	old_chars = list(name)
	new_chars = list()

	next_to_lower = True

	for char in old_chars:
		if char == '_':
			next_to_lower = True
		else:
			if next_to_lower:
				new_chars.append(char)
			else:
				new_chars.append(char.lower())

			if char.isdigit():
				next_to_lower = True
			else:
				next_to_lower = False

	return str().join(new_chars)

def update_input_options(options, input_file):

	options.rel_input = os.path.relpath(input_file, options.root_dir)
	options.input_name = os.path.relpath(input_file, options.source_root_dir)
	options.input_name = os.path.splitext(options.input_name)[0]


	options.enum_name = None
	options.enum_type = "enum"

	with open(input_file, "r") as f:
		for line in f:
			line = line.strip()
			if line.startswith("#@"):
				options.enum_type = line[2:-1]
			elif line.startswith("#") and line.endswith("#"):
				options.enum_name = line[1:-1]

	if not options.enum_name:
		tmp = os.path.splitext(os.path.basename(input_file))[0]
		tmp = [x.capitalize() for x in tmp.split("_")]
		options.enum_name = str().join(tmp)

def parse_source(options, input_file = None):

	import string

	if not input_file:
		input_file = options.input[0]

	result = list()

	attribs = ["src_name", "dst_name", "doc", "bind_query", "assoc_type", "prefix"]

	with open(input_file, "r") as f:
		for line in f:
			line = line.strip()
			if line and not line.startswith("#"):
				values = line.split(":")
				av = dict(zip(attribs, values))

				if not av.get("dst_name"):
					av["dst_name"] = capitalize_name(av["src_name"])

				if not av.get("prefix"):
					av["prefix"] = options.base_lib_prefix

				if av.get("doc"):
					av["comma_doc"] = ": %s" % av.get("doc")
				else:
					av["comma_doc"] = str()

				result.append(type("Item", (object,), av))

	return result

def action_info(options):
	print("Input: %s" % options.input)
	print("Output: %s" % options.output)

def action_specific_mk(options):

	bind_query_inputs = set()
	assoc_type_inputs = set()

	for input_file in options.input:
		items = parse_source(options, input_file)
		for item in items:
			try:
				if item.bind_query:
					bind_query_inputs.add(input_file)
					break
			except AttributeError:
				pass
			try:
				if item.assoc_type:
					assoc_type_inputs.add(input_file)
					break
			except AttributeError:
				pass

	def make_bq_output_path(x):
		return "$(ROOT)/implement/%s/enums/%s_bq.ipp" % (
			options.library,
			os.path.splitext(os.path.relpath(x, options.source_root_dir))[0]
		)

	print_scr_header(options)
	print_newline(options)
	print_line(options, "ROOT = ../../..")
	print_newline(options)

	print_line(options, ".PHONY: _impl_enum_bq_ipp")
	print_line(options, "_impl_enum_bq_ipp:%s" % (
		str(" ").join([
			make_bq_output_path(x)
			for x in list(sorted(bind_query_inputs))
		])
	))
	print_newline(options)

	for input_file in sorted(bind_query_inputs):
		print_line(options, "%s: %s $(MAKE_ENUM)" % (
			make_bq_output_path(input_file),
			os.path.relpath(input_file, options.source_root_dir)
		))

		print_line(options, "	$(MAKE_ENUM) \\")
		print_line(options, "		--library $(LIBRARY) \\")
		print_line(options, "		--base-lib-prefix $(LIB_PREFIX)\\")
		print_line(options, "		--action impl_enum_bq_ipp \\")
		print_line(options, '		--input "$<" \\')
		print_line(options, '		--output "$@" \\')
		print_line(options, '		--output-id "%s_bq"' % (
			os.path.splitext(os.path.basename(input_file))[0]
		))
		print_line(options, '	git add "$@"')
		print_newline(options)

	def make_at_output_path(x):
		return "$(ROOT)/implement/%s/enums/%s_type.ipp" % (
			options.library,
			os.path.splitext(os.path.relpath(x, options.source_root_dir))[0]
		)

	print_line(options, ".PHONY: _impl_enum_type_ipp")
	print_line(options, "_impl_enum_type_ipp:%s" % (
		str(" ").join([
			make_at_output_path(x)
			for x in list(sorted(assoc_type_inputs))
		])
	))
	print_newline(options)

	for input_file in sorted(assoc_type_inputs):
		print_line(options, "%s: %s $(MAKE_ENUM)" % (
			make_at_output_path(input_file),
			os.path.relpath(input_file, options.source_root_dir)
		))

		print_line(options, "	$(MAKE_ENUM) \\")
		print_line(options, "		--library $(LIBRARY) \\")
		print_line(options, "		--base-lib-prefix $(LIB_PREFIX)\\")
		print_line(options, "		--action impl_enum_type_ipp \\")
		print_line(options, '		--input "$<" \\')
		print_line(options, '		--output "$@" \\')
		print_line(options, '		--output-id "%s_type"' % (
			os.path.splitext(os.path.basename(input_file))[0]
		))
		print_line(options, '	git add "$@"')
		print_newline(options)


def action_qbk_hpp(options):

	items = parse_source(options)

	print_cpp_header(options)
	print_line(options, "//[%s_enums_%s" % (options.library, options.output_id))
	print_newline(options)
	print_line(options, "enum class %s : %s%s" % (
		options.enum_name,
		options.base_lib_prefix,
		options.enum_type
	))
	print_line(options, "{")

	max_len = 0
	for item in items:
		max_len = max(max_len, len(item.dst_name))

	def print_item(options, item, term):
		fmt = "\t%s%"+str(max_len-len(item.dst_name)+3)+"s%s_%s%s"
		print_line(options, fmt % (
			item.dst_name,
			" = ",
			item.prefix,
			item.src_name,
			term
		))


	for item in items[:-1]:
		print_item(options, item, str(","))
	for item in items[-1:]:
		print_item(options, item, str(""))

	print_line(options, "};")
	print_newline(options)
	print_line(options, "template <>")
	print_line(options, "__Range<%s> __EnumValueRange<%s>(void);" % (
		options.enum_name,
		options.enum_name
	))
	print_newline(options)
	print_line(options, "__StrCRef __EnumValueName(%s);" % options.enum_name)
	print_newline(options)

	if options.enum_type == "bitfield":
		print_line(
			options,
			"__Bitfield<%s> operator | (%s b1, %s b2);" % (
				options.enum_name,
				options.enum_name,
				options.enum_name
			)
		)
		print_newline(options)
	print_line(options, "//]")

def action_qbk_class_hpp(options):

	items = parse_source(options)

	print_cpp_header(options)
	print_line(options, "//[%s_enums_%s_class" % (options.library, options.output_id))
	print_line(options, "#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES")
	print_line(options, "namespace enums {")
	print_newline(options)

	print_line(options, "template <typename Base, template<__%s> class Transform>" % options.enum_name)
	print_line(options, "class __EnumToClass<Base, __%s, Transform> /*<" %options.enum_name)
	print_line(options, "Specialization of __EnumToClass for the __%s enumeration." %options.enum_name)
	print_line(options, ">*/")
	print_line(options, " : public Base")
	print_line(options, "{")
	print_line(options, "public:")

	print_line(options, "	EnumToClass(void);")
	print_line(options, "	EnumToClass(Base&& base);")

	print_newline(options)

	for item in items:

		print_line(options, "	Transform<%s::%s>" % (
			options.enum_name,
			item.dst_name
		))
		print_line(options, "		%s;" % (
			item.dst_name
		))

	print_line(options, "};")
	print_newline(options)

	print_line(options, "} // namespace enums")
	print_line(options, "#endif")
	print_line(options, "//]")
	print_newline(options)

def action_incl_enum_hpp(options):

	items = parse_source(options)

	has_assoc_type = False

	for item in items:
		try:
			if item.assoc_type:
				has_assoc_type = True
				break
		except AttributeError:
			pass

	print_cpp_header(options)
	print_line(options, "#include <%s/enumerations.hpp>" % options.library)
	if options.enum_type == "bitfield":
		print_line(options, "#include <%s/bitfield.hpp>" % options.library)
	print_newline(options)

	print_line(options, "namespace %s {" % options.library)
	print_newline(options)
	print_line(options, "/// Enumeration %s" % (options.enum_name))
	print_line(options, "/**")
	print_line(options, " *  @ingroup %s_enumerations" % (options.library))
	print_line(options, " */")
	print_line(options, "%s_ENUM_CLASS_BEGIN(%s, %s%s)" % (
		options.library_uc,
		options.enum_name,
		options.base_lib_prefix,
		options.enum_type
	))
	print_line(options, "#include <%s/enums/%s.ipp>" % (
		options.library,
		options.input_name
	))

	print_line(options, "%s_ENUM_CLASS_END(%s)" % (
		options.library_uc,
		options.enum_name
	))
	print_newline(options)

	if options.enum_type == "bitfield":
		print_line(options, "%s_MAKE_BITFIELD(%s)" % (
			options.library_uc,
			options.enum_name
		))
		print_newline(options)

	if has_assoc_type:
		print_line(options, "#include <%s/enums/%s_type.ipp>" % (
			options.library,
			options.input_name
		))
		print_newline(options)

	print_line(options, "#if !%s_NO_ENUM_VALUE_NAMES" % (options.library_uc))
	print_line(options, "#include <%s/enums/%s_names.ipp>" % (
		options.library,
		options.input_name
	))
	print_line(options, "#endif")
	print_newline(options)

	print_line(options, "#if !%s_NO_ENUM_VALUE_RANGES" % (options.library_uc))
	print_line(options, "#include <%s/enums/%s_range.ipp>" % (
		options.library,
		options.input_name
	))
	print_line(options, "#endif")
	print_newline(options)

	print_line(options, "} // namespace %s" % options.library)


def action_incl_enum_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)

	print_line(options, "#if %s_DOCUMENTATION_ONLY" % options.library_uc)
	print_newline(options)

	for item in items[:-1]:
		print_line(options, "/// %s%s" % (item.src_name, item.comma_doc))
		print_line(options, "%s," % item.dst_name)
	for item in items[-1:]:
		print_line(options, "/// %s%s" % (item.src_name, item.comma_doc))
		print_line(options, "%s" % item.dst_name)

	print_newline(options)
	print_line(options, "#else // !%s_DOCUMENTATION_ONLY" % options.library_uc)
	print_newline(options)
	print_line(options, "#include <%s/enums/%s_def.ipp>" % (
		options.library,
		options.input_name
	))
	print_newline(options)
	print_line(options, "#endif")

def action_impl_enum_def_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)

	print_line(options, "#ifdef %s_LIST_NEEDS_COMMA" % options.library_uc)
	print_line(options, "# undef %s_LIST_NEEDS_COMMA" % options.library_uc)
	print_line(options, "#endif")
	print_newline(options)

	for item in items:

		print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))
		print_line(options, "# ifdef %s_LIST_NEEDS_COMMA" % options.library_uc)
		print_line(options, "   %s_ENUM_CLASS_COMMA" % options.library_uc)
		print_line(options, "# endif")

		print_line(options, "# if defined %s" % item.dst_name)
		print_line(options, "#  pragma push_macro(\"%s\")" % item.dst_name)
		print_line(options, "#  undef %s" % item.dst_name)
		print_line(options, "   %s_ENUM_CLASS_VALUE(%s, %s_%s)" % (
			options.library_uc,
			item.dst_name,
			item.prefix,
			item.src_name
		))
		print_line(options, "#  pragma pop_macro(\"%s\")" % item.dst_name)
		print_line(options, "# else")
		print_line(options, "   %s_ENUM_CLASS_VALUE(%s, %s_%s)" % (
			options.library_uc,
			item.dst_name,
			item.prefix,
			item.src_name
		))
		print_line(options, "# endif")

		print_line(options, "# ifndef %s_LIST_NEEDS_COMMA" % options.library_uc)
		print_line(options, "#  define %s_LIST_NEEDS_COMMA 1" % options.library_uc)
		print_line(options, "# endif")
		print_line(options, "#endif")

	print_line(options, "#ifdef %s_LIST_NEEDS_COMMA" % options.library_uc)
	print_line(options, "# undef %s_LIST_NEEDS_COMMA" % options.library_uc)
	print_line(options, "#endif")
	print_newline(options)


def action_impl_enum_names_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)

	print_line(options, "namespace enums {")
	print_line(options, "%s_LIB_FUNC StrCRef ValueName_(" % options.library_uc)
	print_line(options, "	%s*," % options.enum_name)
	print_line(options, "	%s%s value" % (options.base_lib_prefix, options.enum_type))
	print_line(options, ")")
	print_line(options, "#if (!%s_LINK_LIBRARY || defined(%s_IMPLEMENTING_LIBRARY)) && \\" % (
		options.library_uc,
		options.library_uc
	))
	print_line(options, "	!defined(%s_IMPL_EVN_%s)" % (
		options.library_uc,
		options.enum_name.upper()
	))
	print_line(options, "#define %s_IMPL_EVN_%s" % (
		options.library_uc,
		options.enum_name.upper()
	))
	print_line(options, "{")
	print_line(options, "switch(value)")
	print_line(options, "{")

	for item in items:

		print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))
		print_line(options, "	case %s_%s: return StrCRef(\"%s\");" % (
			item.prefix,
			item.src_name,
			item.src_name
		))
		print_line(options, "#endif")

	print_line(options, "	default:;")
	print_line(options, "}")
	print_line(options, "%s_FAKE_USE(value);" % options.library_uc)
	print_line(options, "return StrCRef();")
	print_line(options, "}")
	print_line(options, "#else")
	print_line(options, ";")
	print_line(options, "#endif")
	print_line(options, "} // namespace enums")
	print_newline(options)


def action_impl_enum_range_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)

	print_line(options, "namespace enums {")
	print_line(options, "%s_LIB_FUNC aux::CastIterRange<" % options.library_uc)
	print_line(options, "	const %s%s*," % (options.base_lib_prefix, options.enum_type))
	print_line(options, "	%s" % options.enum_name)
	print_line(options, "> ValueRange_(%s*)" % options.enum_name)
	print_line(options, "#if (!%s_LINK_LIBRARY || defined(%s_IMPLEMENTING_LIBRARY)) && \\" % (
		options.library_uc,
		options.library_uc
	))
	print_line(options, "	!defined(%s_IMPL_EVR_%s)" % (
		options.library_uc,
		options.enum_name.upper()
	))
	print_line(options, "#define %s_IMPL_EVR_%s" % (
		options.library_uc,
		options.enum_name.upper()
	))
	print_line(options, "{")
	print_line(options, "static const %s%s _values[] = {" % (
		options.base_lib_prefix,
		options.enum_type
	))

	for item in items:
		print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))
		print_line(options, "%s_%s," % (item.prefix, item.src_name))
		print_line(options, "#endif")

	print_line(options, "0")
	print_line(options, "};")
	print_line(options, "return aux::CastIterRange<")
	print_line(options, "	const %s%s*," % (options.base_lib_prefix, options.enum_type))
	print_line(options, "	%s" % options.enum_name)
	print_line(options, ">(_values, _values+sizeof(_values)/sizeof(_values[0])-1);")
	print_line(options, "}")
	print_line(options, "#else")
	print_line(options, ";")
	print_line(options, "#endif")
	print_line(options, "} // namespace enums")
	print_newline(options)


def action_impl_enum_class_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)
	print_line(options, "namespace enums {")

	print_line(options, "template <typename Base, template<%s> class Transform>" % options.enum_name)
	print_line(options, "class EnumToClass<Base, %s, Transform>" %options.enum_name)
	print_line(options, " : public Base")
	print_line(options, "{")
	print_line(options, "private:")
	print_line(options, "	Base& _base(void) { return *this; }")
	print_line(options, "public:")

	print_newline(options)

	for item in items:

		print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))

		print_line(options, "# if defined %s" % item.dst_name)
		print_line(options, "#  pragma push_macro(\"%s\")" % item.dst_name)
		print_line(options, "#  undef %s" % item.dst_name)
		print_line(options, "	Transform<%s::%s> %s;" % (
			options.enum_name,
			item.dst_name,
			item.dst_name
		))
		print_line(options, "#  pragma pop_macro(\"%s\")" % item.dst_name)
		print_line(options, "# else")
		print_line(options, "	Transform<%s::%s> %s;" % (
			options.enum_name,
			item.dst_name,
			item.dst_name
		))
		print_line(options, "# endif")
		print_line(options, "#endif")

	print_newline(options)
	print_line(options, "	EnumToClass(void) { }")
	print_line(options, "	EnumToClass(Base&& base)")
	print_line(options, "	 : Base(std::move(base))")

	for item in items:

		print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))

		print_line(options, "# if defined %s" % item.dst_name)
		print_line(options, "#  pragma push_macro(\"%s\")" % item.dst_name)
		print_line(options, "#  undef %s" % item.dst_name)
		print_line(options, "	 , %s(_base())" % item.dst_name)
		print_line(options, "#  pragma pop_macro(\"%s\")" % item.dst_name)
		print_line(options, "# else")
		print_line(options, "	 , %s(_base())" % item.dst_name)
		print_line(options, "# endif")
		print_line(options, "#endif")

	print_line(options, "	{ }")


	print_line(options, "};")
	print_newline(options)

	print_line(options, "} // namespace enums")
	print_newline(options)


def action_impl_enum_bq_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)
	print_newline(options)

	for item in items:
		try:
			print_line(options, "#if defined %s_%s && defined %s_%s" % (
				item.prefix, item.src_name,
				item.prefix, item.bind_query
			))
			print_line(options, "case %s_%s:" % (
				item.prefix, item.src_name
			))
			print_line(options, "	return %s_%s;" % (
				item.prefix, item.bind_query
			))
			print_line(options, "#endif")
		except AttributeError:
			pass

def action_impl_enum_type_ipp(options):

	items = parse_source(options)

	print_cpp_header(options)
	print_newline(options)

	print_line(options, "namespace enums {")

	for item in items:
		try:
			if item.assoc_type:
				print_line(options, "#if defined %s_%s" % (item.prefix, item.src_name))
				print_line(options, "template <>")
				print_line(options, "struct EnumAssocType<%s, %s::%s>" % (
					options.enum_name,
					options.enum_name,
					item.dst_name
				))
				print_line(options, "{ typedef %s Type; };" % (item.assoc_type))
				print_line(options, "#endif")
		except AttributeError:
			pass

	print_line(options, "} // namespace enums")

def action_smart_enums_ipp(options):

	enum_values = set()

	for input_file in options.input:
		items = parse_source(options, input_file)
		for item in items:
			enum_values.add(item.dst_name)

	print_cpp_header(options)
	for enum_value in sorted(enum_values):
		evp = (enum_value, enum_value)
		print_line(options, "struct %s {" % enum_value)
		print_line(options, "template <typename Enum, Enum = Enum::%s> operator Enum (void) const{ return Enum::%s; }" % evp)
		print_line(options, "template <typename Enum> friend bool operator==(Enum value, %s){ return value == Enum::%s; }" % evp)
		print_line(options, "template <typename Enum> friend bool operator!=(Enum value, %s){ return value != Enum::%s; }" % evp)
		print_line(options, "};")


def action_smart_values_ipp(options):

	enum_values = set()

	for input_file in options.input:
		items = parse_source(options, input_file)
		for item in items:
			enum_values.add(item.dst_name)

	print_cpp_header(options)
	for enum_value in sorted(enum_values):
		print_line(options, "OGLPLUS_CONSTEXPR %s::smart_enums::%s %s = {};" % (
			options.library,
			enum_value,
			enum_value
		))

def action_impl_lib_enum_value_name_ipp(options):


	print_cpp_header(options)

	print_line(options, "#if !%s_NO_ENUM_VALUE_NAMES" % options.library_uc)
	print_newline(options)

	for input_file in options.input:
		update_input_options(options, input_file)

		print_line(options, "%s_ENUM_CLASS_FWD(%s, %s%s)" % (
			options.library_uc,
			options.enum_name,
			options.base_lib_prefix,
			options.enum_type
		))
		print_line(options, "#include <%s/enums/%s_names.ipp>" % (
			options.library,
			options.input_name
		))
		print_newline(options)

	print_line(options, "#endif")
	print_newline(options)

def action_impl_lib_enum_value_range_ipp(options):


	print_cpp_header(options)

	print_line(options, "#if !%s_NO_ENUM_VALUE_RANGES" % options.library_uc)
	print_newline(options)

	for input_file in options.input:
		update_input_options(options, input_file)

		print_line(options, "%s_ENUM_CLASS_FWD(%s, %s%s)" % (
			options.library_uc,
			options.enum_name,
			options.base_lib_prefix,
			options.enum_type
		))
		print_line(options, "#include <%s/enums/%s_range.ipp>" % (
			options.library,
			options.input_name
		))
		print_newline(options)

	print_line(options, "#endif")
	print_newline(options)


actions = {
	"info":    action_info,
	"specific_mk":  action_specific_mk,
	"qbk_hpp": action_qbk_hpp,
	"qbk_class_hpp": action_qbk_class_hpp,
	"incl_enum_hpp": action_incl_enum_hpp,
	"incl_enum_ipp": action_incl_enum_ipp,
	"impl_enum_def_ipp": action_impl_enum_def_ipp,
	"impl_enum_names_ipp": action_impl_enum_names_ipp,
	"impl_enum_range_ipp": action_impl_enum_range_ipp,
	"impl_enum_class_ipp": action_impl_enum_class_ipp,
	"impl_enum_type_ipp": action_impl_enum_type_ipp,
	"impl_enum_bq_ipp": action_impl_enum_bq_ipp,
	"smart_enums_ipp": action_smart_enums_ipp,
	"smart_values_ipp": action_smart_values_ipp,
	"impl_lib_enum_value_name_ipp": action_impl_lib_enum_value_name_ipp,
	"impl_lib_enum_value_range_ipp": action_impl_lib_enum_value_range_ipp
}

multi_file_actions = [
	"specific_mk",
	"smart_enums_ipp",
	"smart_values_ipp",
	"impl_lib_enum_value_name_ipp",
	"impl_lib_enum_value_range_ipp"
]

def dispatch_action(options):
	if not options.action in multi_file_actions:
		if len(options.input) < 1:
			msg = "Missing input file for action '%s'!" % options.action
			raise RuntimeError(msg)
		if len(options.input) > 1:
			msg = "Too many input files for action '%s'!" % options.action
			raise RuntimeError(msg)
	if not options.output_id:
		raise RuntimeError("No output-id was specified!")

	actions[options.action](options)


def get_argument_parser():
	import argparse, datetime, sys

	root_dir = os.path.join(
		os.path.pardir,
		os.path.pardir,
		os.path.dirname(sys.argv[0])
	)

	argparser = argparse.ArgumentParser(
		prog="make_enum",
		description="""
			Helper script for generating various enum related source files.
		""",
		epilog="""
			Copyright (c) 2014 - %(year)d Matúš Chochlík.
			Permission is granted to copy, distribute and/or modify this document
			under the terms of the Boost Software License, Version 1.0.
			(See a copy at http://www.boost.org/LICENSE_1_0.txt)
		""" % { "year": datetime.datetime.now().year }
	)

	argparser.add_argument(
		"--root-dir",
		type=os.path.abspath,
		default=root_dir,
		action="store",
		help=""" Path to the OGLplus root directory. """
	)

	argparser.add_argument(
		"--library",
		action="store",
		help=""" The all-lowercase library name. """
	)

	argparser.add_argument(
		"--base-lib-prefix",
		action="store",
		help=""" The base library prefix. """
	)

	argparser.add_argument(
		"inputs",
		metavar="FILE",
		type=os.path.abspath,
		nargs="*",
		help="""Path to the input text file(s) to be processed. """
	)

	argparser.add_argument(
		"--input",
		type=os.path.abspath,
		default=list(),
		action="append",
		help=""" Path to the input text file to be processed. """
	)

	argparser.add_argument(
		"--output-id",
		action="store",
		help=""" The output id string. """
	)

	argparser.add_argument(
		"--output",
		type=os.path.abspath,
		action="store",
		help=""" Path to the output file. """
	)

	def ValidAction(arg):
		if actions.get(arg) is None:
			msg = "'%s' is not a valid action name" % str(arg)
			raise argparse.ArgumentTypeError(msg)
		return arg

	argparser.add_argument(
		"--action",
		type=ValidAction,
		default="Info",
		action="store",
		help=""" The action to be executed. """
	)

	return argparser

def get_options():

	argparser = get_argument_parser()
	options = argparser.parse_args()

	options.input += options.inputs
	options.inputs = None

	options.root_dir = os.path.abspath(options.root_dir)
	options.rel_self = os.path.relpath(sys.argv[0], options.root_dir)

	if options.output is None:
		options.out = sys.stdout
		options.rel_output = "N/A"
	else:
		options.out = open(options.output, "wt")
		options.rel_output = os.path.relpath(options.output, options.root_dir)

	options.source_root_dir = os.path.join(options.root_dir, "source", "enums", options.library)

	if len(options.input) == 1:
		update_input_options(options, options.input[0])
	else:
		options.rel_input = None

	if options.library:
		options.library_uc = options.library.upper()

	import datetime
	options.year = datetime.datetime.now().year

	return options


def main():
	dispatch_action(get_options())


if __name__ == "__main__": main()

