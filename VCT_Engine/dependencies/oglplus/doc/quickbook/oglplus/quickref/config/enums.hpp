/*
 *  Copyright 2014-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_enums

#ifndef OGLPLUS_NO_ENUM_VALUE_NAMES /*<
Compile-time switch disabling the functions returning enumerated value names.
Setting this preprocessor symbol to a non-zero value causes that
the __EnumValueName functions always return an empty string.
When set to zero these functions return a textual name of an enumerated
value passed as argument.

By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. enumeration value names are enabled, when not in low-profile mode
and disabled otherwise.
>*/
# define OGLPLUS_NO_ENUM_VALUE_NAMES __OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_NO_ENUM_VALUE_RANGES /*<
Compile-time switch disabling the functions returning enumerated value ranges.
Setting this preprocessor symbol to a nonzero value causes that
the __EnumValueRange functions always return an empty range.
When set to zero these functions return a range of all values in the
enumeration passed as the template argument.

By default this option is set to the same value as __OGLPLUS_LOW_PROFILE,
i.e. enumeration value ranges are enabled, when not in low-profile mode
and disabled otherwise.
>*/
# define OGLPLUS_NO_ENUM_VALUE_RANGES OGLPLUS_LOW_PROFILE
#endif

#ifndef OGLPLUS_NO_ENUM_VALUE_CLASSES /*<
Compile-time switch disabling the specializations of __enums_EnumToClass.
Setting this preprocessor symbol to a nonzero value causes that
the __enums_EnumToClass templates to be unspecialized for the individual enums.

By default this option is set to zero (except on compilers where template
specializations for enum-class is not implemented properly).
>*/
# define OGLPLUS_NO_ENUM_VALUE_CLASSES __OGLPLUS_NO_SCOPED_ENUM_TEMPLATE_PARAMS
#endif

//]

