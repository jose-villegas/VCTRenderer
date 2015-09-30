/*
 *  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

//[oglplus_config_compiler

#ifndef OGLPLUS_NO_SITE_CONFIG /*<
Disables the usage of the automatically-generated
[link oglplus.quickref.config.site site configuration header].
>*/
# include <oglplus/config/site.hpp>
#endif

#ifndef OGLPLUS_USE_BOOST_CONFIG /*<
Enables the use of the
[@http://www.boost.org/doc/libs/latest/libs/config/config.htm Boost.Config]
library for the detection of compiler capabilities.
If not set in the site-config nor defined otherwise, the use
of Boost.Config is disabled by default.
>*/
# define OGLPLUS_USE_BOOST_CONFIG 0
#endif

#if OGLPLUS_USE_BOOST_CONFIG
# include <boost/config.hpp>
#endif

// The default values for the preprocessor symbols indicating
// compiler capabilities are also defined here.

//]

