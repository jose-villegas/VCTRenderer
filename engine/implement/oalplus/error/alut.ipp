/**
 *  @file oalplus/error/alut.ipp
 *  @brief Implementation of ErrorALUT
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oalplus {

OALPLUS_LIB_FUNC
const char* ErrorALUT::Message(ALenum error_code)
{
	switch(error_code)
	{
		case ALUT_ERROR_OUT_OF_MEMORY:
			return "ALUT out of memory";
		case ALUT_ERROR_INVALID_OPERATION:
			return "Invalid ALUT operation";
		case ALUT_ERROR_NO_CURRENT_CONTEXT:
			return "There is no current AL context";
		case ALUT_ERROR_AL_ERROR_ON_ENTRY:
			return "An AL error on entry to ALUT call";
		case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
			return "An ALC error on entry to ALUT call";
		case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
			return "Unsupported file type";
		case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
			return "Unsupported mode within an otherwise usable file type";
		case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
			return "The sound data was corrupt or truncated";
	}
	return "Unknown ALUT error";
}

} // namespace oalplus

