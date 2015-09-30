/*  Copyright 2014 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#version 330

const int prec = 1024;
const int prmo = prec-1;

float pack_pair(float hi, float lo)
{
	hi = floor(hi * prmo);
	lo = floor(lo * prmo);
	return hi*prec + lo;
}

float unpack_hi(float enc)
{
	return floor(enc / prec) / prmo;
}

float unpack_lo(float enc)
{
	return mod(enc, prec) / prmo;
}

