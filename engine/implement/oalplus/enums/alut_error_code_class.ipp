//  File implement/oalplus/enums/alut_error_code_class.ipp
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
namespace enums {
template <typename Base, template<ALUTErrorCode> class Transform>
class EnumToClass<Base, ALUTErrorCode, Transform>
 : public Base
{
private:
	Base& _base(void) { return *this; }
public:

#if defined AL_OUT_OF_MEMORY
# if defined OutOfMemory
#  pragma push_macro("OutOfMemory")
#  undef OutOfMemory
	Transform<ALUTErrorCode::OutOfMemory> OutOfMemory;
#  pragma pop_macro("OutOfMemory")
# else
	Transform<ALUTErrorCode::OutOfMemory> OutOfMemory;
# endif
#endif
#if defined AL_INVALID_OPERATION
# if defined InvalidOperation
#  pragma push_macro("InvalidOperation")
#  undef InvalidOperation
	Transform<ALUTErrorCode::InvalidOperation> InvalidOperation;
#  pragma pop_macro("InvalidOperation")
# else
	Transform<ALUTErrorCode::InvalidOperation> InvalidOperation;
# endif
#endif
#if defined AL_NO_CURRENT_CONTEXT
# if defined NoCurrentContext
#  pragma push_macro("NoCurrentContext")
#  undef NoCurrentContext
	Transform<ALUTErrorCode::NoCurrentContext> NoCurrentContext;
#  pragma pop_macro("NoCurrentContext")
# else
	Transform<ALUTErrorCode::NoCurrentContext> NoCurrentContext;
# endif
#endif
#if defined AL_AL_ERROR_ON_ENTRY
# if defined AlErrorOnEntry
#  pragma push_macro("AlErrorOnEntry")
#  undef AlErrorOnEntry
	Transform<ALUTErrorCode::AlErrorOnEntry> AlErrorOnEntry;
#  pragma pop_macro("AlErrorOnEntry")
# else
	Transform<ALUTErrorCode::AlErrorOnEntry> AlErrorOnEntry;
# endif
#endif
#if defined AL_ALC_ERROR_ON_ENTRY
# if defined AlcErrorOnEntry
#  pragma push_macro("AlcErrorOnEntry")
#  undef AlcErrorOnEntry
	Transform<ALUTErrorCode::AlcErrorOnEntry> AlcErrorOnEntry;
#  pragma pop_macro("AlcErrorOnEntry")
# else
	Transform<ALUTErrorCode::AlcErrorOnEntry> AlcErrorOnEntry;
# endif
#endif
#if defined AL_UNSUPPORTED_FILE_TYPE
# if defined UnsupportedFileType
#  pragma push_macro("UnsupportedFileType")
#  undef UnsupportedFileType
	Transform<ALUTErrorCode::UnsupportedFileType> UnsupportedFileType;
#  pragma pop_macro("UnsupportedFileType")
# else
	Transform<ALUTErrorCode::UnsupportedFileType> UnsupportedFileType;
# endif
#endif
#if defined AL_UNSUPPORTED_FILE_SUBTYPE
# if defined UnsupportedFileSubtype
#  pragma push_macro("UnsupportedFileSubtype")
#  undef UnsupportedFileSubtype
	Transform<ALUTErrorCode::UnsupportedFileSubtype> UnsupportedFileSubtype;
#  pragma pop_macro("UnsupportedFileSubtype")
# else
	Transform<ALUTErrorCode::UnsupportedFileSubtype> UnsupportedFileSubtype;
# endif
#endif
#if defined AL_CORRUPT_OR_TRUNCATED_DATA
# if defined CorruptOrTruncatedData
#  pragma push_macro("CorruptOrTruncatedData")
#  undef CorruptOrTruncatedData
	Transform<ALUTErrorCode::CorruptOrTruncatedData> CorruptOrTruncatedData;
#  pragma pop_macro("CorruptOrTruncatedData")
# else
	Transform<ALUTErrorCode::CorruptOrTruncatedData> CorruptOrTruncatedData;
# endif
#endif

	EnumToClass(void) { }
	EnumToClass(Base&& base)
	 : Base(std::move(base))
#if defined AL_OUT_OF_MEMORY
# if defined OutOfMemory
#  pragma push_macro("OutOfMemory")
#  undef OutOfMemory
	 , OutOfMemory(_base())
#  pragma pop_macro("OutOfMemory")
# else
	 , OutOfMemory(_base())
# endif
#endif
#if defined AL_INVALID_OPERATION
# if defined InvalidOperation
#  pragma push_macro("InvalidOperation")
#  undef InvalidOperation
	 , InvalidOperation(_base())
#  pragma pop_macro("InvalidOperation")
# else
	 , InvalidOperation(_base())
# endif
#endif
#if defined AL_NO_CURRENT_CONTEXT
# if defined NoCurrentContext
#  pragma push_macro("NoCurrentContext")
#  undef NoCurrentContext
	 , NoCurrentContext(_base())
#  pragma pop_macro("NoCurrentContext")
# else
	 , NoCurrentContext(_base())
# endif
#endif
#if defined AL_AL_ERROR_ON_ENTRY
# if defined AlErrorOnEntry
#  pragma push_macro("AlErrorOnEntry")
#  undef AlErrorOnEntry
	 , AlErrorOnEntry(_base())
#  pragma pop_macro("AlErrorOnEntry")
# else
	 , AlErrorOnEntry(_base())
# endif
#endif
#if defined AL_ALC_ERROR_ON_ENTRY
# if defined AlcErrorOnEntry
#  pragma push_macro("AlcErrorOnEntry")
#  undef AlcErrorOnEntry
	 , AlcErrorOnEntry(_base())
#  pragma pop_macro("AlcErrorOnEntry")
# else
	 , AlcErrorOnEntry(_base())
# endif
#endif
#if defined AL_UNSUPPORTED_FILE_TYPE
# if defined UnsupportedFileType
#  pragma push_macro("UnsupportedFileType")
#  undef UnsupportedFileType
	 , UnsupportedFileType(_base())
#  pragma pop_macro("UnsupportedFileType")
# else
	 , UnsupportedFileType(_base())
# endif
#endif
#if defined AL_UNSUPPORTED_FILE_SUBTYPE
# if defined UnsupportedFileSubtype
#  pragma push_macro("UnsupportedFileSubtype")
#  undef UnsupportedFileSubtype
	 , UnsupportedFileSubtype(_base())
#  pragma pop_macro("UnsupportedFileSubtype")
# else
	 , UnsupportedFileSubtype(_base())
# endif
#endif
#if defined AL_CORRUPT_OR_TRUNCATED_DATA
# if defined CorruptOrTruncatedData
#  pragma push_macro("CorruptOrTruncatedData")
#  undef CorruptOrTruncatedData
	 , CorruptOrTruncatedData(_base())
#  pragma pop_macro("CorruptOrTruncatedData")
# else
	 , CorruptOrTruncatedData(_base())
# endif
#endif
	{ }
};

} // namespace enums

