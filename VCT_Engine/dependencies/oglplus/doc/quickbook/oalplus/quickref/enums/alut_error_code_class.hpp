//  File doc/quickbook/oalplus/quickref/enums/alut_error_code_class.hpp
//
//  Automatically generated file, DO NOT modify manually.
//  Edit the source 'source/enums/oalplus/alut_error_code.txt'
//  or the 'source/enums/make_enum.py' script instead.
//
//  Copyright 2010-2015 Matus Chochlik.
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//[oalplus_enums_alut_error_code_class
#if !__OGLPLUS_NO_ENUM_VALUE_CLASSES
namespace enums {

template <typename Base, template<__ALUTErrorCode> class Transform>
class __EnumToClass<Base, __ALUTErrorCode, Transform> /*<
Specialization of __EnumToClass for the __ALUTErrorCode enumeration.
>*/
 : public Base
{
public:
	EnumToClass(void);
	EnumToClass(Base&& base);

	Transform<ALUTErrorCode::OutOfMemory>
		OutOfMemory;
	Transform<ALUTErrorCode::InvalidOperation>
		InvalidOperation;
	Transform<ALUTErrorCode::NoCurrentContext>
		NoCurrentContext;
	Transform<ALUTErrorCode::AlErrorOnEntry>
		AlErrorOnEntry;
	Transform<ALUTErrorCode::AlcErrorOnEntry>
		AlcErrorOnEntry;
	Transform<ALUTErrorCode::UnsupportedFileType>
		UnsupportedFileType;
	Transform<ALUTErrorCode::UnsupportedFileSubtype>
		UnsupportedFileSubtype;
	Transform<ALUTErrorCode::CorruptOrTruncatedData>
		CorruptOrTruncatedData;
};

} // namespace enums
#endif
//]

