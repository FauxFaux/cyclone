/* This file is part of the RC compiler.
   Copyright (C) 2000-2001 The Regents of the University of California.

RC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

RC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef CVAL_H
#define CVAL_H

/* Note: legal ops on complex are:
   add, sub, times, divide, realpart, imagpart, conjugate
*/
typedef struct {
  enum { cval_variable, cval_unk, cval_address,
	 cval_float, cval_float_complex,
	 cval_uint, cval_uint_complex,
	 cval_sint, cval_sint_complex } kind;
  /* Could be a union */
  long double d, d_i; /* for cval_float */
  struct data_declaration *ddecl; /* for cval_address */
  struct label_declaration *ldecl;  /* for cval_address */
  largest_int si, si_i;
  largest_uint ui, ui_i;
  size_t isize;
} cval;

extern cval cval_top; /* The non-constant value */
extern cval cval_unknown; /* The unknown value */
extern cval cval_zero; /* A zero value. Use cval_cast to make the desired 
			  kind of constant */

void cval_init(void);

cval make_cval_signed(largest_int i, type t);
cval make_cval_unsigned(largest_uint i, type t);
cval make_cval_float(long double d);
cval make_cval_complex(cval r, cval i);
cval make_cval_address(data_declaration ddecl, label_declaration ldecl,
		       largest_int offset);

/*bool cval_isunknown(cval c);*/
#define cval_isunknown(c) ((c).kind == cval_unk)
/* Return: TRUE if c is an unknown constant */

/*bool cval_istop(cval c);*/
#define cval_istop(c) ((c).kind == cval_variable)
/* Return: TRUE if c is not a constant */

/*bool cval_isaddress(cval c);*/
#define cval_isaddress(c) ((c).kind == cval_address)
/* Return: TRUE if c is not a constant */

bool cval_isinteger(cval c);
/* Return: TRUE if c is an integer constant */

bool cval_iscomplex(cval c);
/* Return: TRUE if c is a complex constant */

bool cval_knownbool(cval c);
/* Returns: TRUE if the truth-value of c can be determined (use cval_boolvalue
   to get that value)
*/

bool cval_boolvalue(cval c);
/* Returns: TRUE if c is a non-zero constant
   Requires: cval_knownbool(c)
 */

bool cval_knownvalue(cval c);
/* Returns: TRUE if the value of c can be determined (this is false for
   address constants, while cval_knownbool is true for address constants
   with offset 0)
*/

largest_uint cval_uint_value(cval c);
/* Returns: The value of c as an unsigned int
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/

largest_int cval_sint_value(cval c);
/* Returns: The value of c as a signed int
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/

long double cval_float_value(cval c);
/* Returns: The value of c as a long double
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/

bool cval_isone(cval c);
/* Returns: TRUE if c is 1 (FALSE for unknown constants)
   Requires: c not be cval_top
 */

/* All of these functions will return cval_top if the result is not a
   constant expression */

cval cval_cast(cval c, type to); /* Cast c to type to */
cval cval_not(cval c); /* !c */
cval cval_negate(cval c); /* -c */
cval cval_bitnot(cval c); /* ~c */
cval cval_conjugate(cval c); /* ~c */
cval cval_realpart(cval c); /* __real__ c */
cval cval_imagpart(cval c); /* __imag__ c */

/* The binary operators require that both arguments have been cast to a common
   type. */
cval cval_add(cval c1, cval c2);
cval cval_sub(cval c1, cval c2);
cval cval_times(cval c1, cval c2);
cval cval_divide(cval c1, cval c2);
cval cval_modulo(cval c1, cval c2);
cval cval_lshift(cval c1, cval c2);
cval cval_rshift(cval c1, cval c2);
cval cval_bitand(cval c1, cval c2);
cval cval_bitor(cval c1, cval c2);
cval cval_bitxor(cval c1, cval c2);
cval cval_eq(cval c1, cval c2);
/*cval cval_ne(cval c1, cval c2);*/
cval cval_leq(cval c1, cval c2);
/*cval cval_lt(cval c1, cval c2);*/
/*cval cval_geq(cval c1, cval c2);*/
/*cval cval_gt(cval c1, cval c2);*/
#define cval_gt(c1, c2) (cval_not(cval_leq((c1), (c2))))
#define cval_lt(c1, c2) (cval_gt((c2), (c1)))
#define cval_geq(c1, c2) (cval_leq((c2), (c1)))
#define cval_ne(c1, c2) (cval_not(cval_eq((c1), (c2))))

/* True if x fits in the range of type t */
bool uint_inrange(largest_uint x, type t);
bool sint_inrange(largest_int x, type t);

bool cval_inrange(cval c, type t);
/* Requires: constant_integral(c)
   Returns: TRUE if c is in range of type t. */

largest_int cval_intcompare(cval c1, cval c2);
/* Requires: cval_isinteger(c1) && cval_isinteger(c2)
   Returns: x, x<0 if c1 < c2, x = 0 if c1 = c2 and x > 0 if c1 > c2
*/

#endif
