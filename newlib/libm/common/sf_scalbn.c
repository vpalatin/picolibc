/* sf_scalbn.c -- float version of s_scalbn.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#include "fdlibm.h"
#include <limits.h>
#include <float.h>

#if INT_MAX > 50000
#define OVERFLOW_INT 50000
#else
#define OVERFLOW_INT 30000
#endif

#ifdef __STDC__
static const float
#else
static float
#endif
two25   =  3.355443200e+07,	/* 0x4c000000 */
twom25  =  2.9802322388e-08;	/* 0x33000000 */

#ifdef __STDC__
	float scalbnf (float x, int n)
#else
	float scalbnf (x,n)
	float x; int n;
#endif
{
	__int32_t  k,ix;
	__uint32_t hx;

	GET_FLOAT_WORD(ix,x);
	hx = ix&0x7fffffff;
        k = hx>>23;		/* extract exponent */
	if (FLT_UWORD_IS_ZERO(hx))
	    return x;
        if (!FLT_UWORD_IS_FINITE(hx))
	    return x;		/* NaN or Inf */
        if (FLT_UWORD_IS_SUBNORMAL(hx)) {
	    x *= two25;
	    GET_FLOAT_WORD(ix,x);
	    k = ((ix&0x7f800000)>>23) - 25; 
            if (n< -50000)
                return __math_uflowf(ix<0); 	/*underflow*/
        }
        if (n > OVERFLOW_INT) 	/* in case integer overflow in n+k */
            return __math_oflowf(ix<0);	        /*overflow*/
        k = k+n; 
        if (k > FLT_LARGEST_EXP)
            return __math_oflowf(ix<0);          /* overflow  */
        if (k > 0) 				/* normal result */
	    {SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23)); return x;}
        if (k < FLT_SMALLEST_EXP)
	    return __math_uflowf(ix<0);	        /*underflow*/
        k += 25;				/* subnormal result */
	SET_FLOAT_WORD(x,(ix&0x807fffff)|(k<<23));
        return x*twom25;
}

#if defined(HAVE_ALIAS_ATTRIBUTE)
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wmissing-attributes"
#endif
__strong_reference(scalbnf, ldexpf);
#else

float
ldexpf(float value, int exp)
{
    return scalbnf(value, exp);
}

#endif

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double scalbn(double x, int n)
#else
	double scalbn(x,n)
	double x;
	int n;
#endif
{
	return (double) scalbnf((float) x, n);
}

#if defined(HAVE_ALIAS_ATTRIBUTE)
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wmissing-attributes"
#endif
__strong_reference(scalbn, ldexp);
#else

double
ldexp(double value, int exp)
{
    return scalbn(value, exp);
}

#endif

#endif /* defined(_DOUBLE_IS_32BITS) */
