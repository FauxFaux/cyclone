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

/* SAME: There's a lot of assumptions about floating point here, and behaviour
   of operations when performed on wider types. I believe them to be valid for
   IEEE with round-to-nearest. XXX: check.
   XXX: overflow.
*/
/* Note: this code is not particularly aiming for efficiency (in particular,
   complex constants will be inefficient) */
#include "parser.h"
#include "cval.h"

cval cval_top; /* The non-constant value */
cval cval_unknown; /* The unknown value */
cval cval_zero; /* A zero value. Use cval_cast to make the desired kind of
		   constant */

void cval_init(void)
{
  /* Code will be unhappy if this is not true. */
  assert(sizeof(largest_int) == sizeof(largest_uint));
  /* assert(This is a 2's complement machine); */

  cval_top.kind = cval_variable;
  cval_unknown.kind = cval_unk;
  cval_zero.kind = cval_sint;
  cval_zero.si = 0;
  cval_zero.isize = type_size(int_type);
}

cval make_cval_unsigned(largest_uint i, type t)
{
  cval c;

  assert(type_integral(t) && type_unsigned(t));
  c.kind = cval_uint;
  c.ui = i;
  c.isize = type_size(t);
  return c;
}

cval make_cval_signed(largest_int i, type t)
{
  cval c;

  assert(type_integral(t) && !type_unsigned(t));
  c.kind = cval_sint;
  c.si = i;
  c.isize = type_size(t);
  return c;
}

cval make_cval_float(long double d)
{
  cval c;
  c.kind = cval_float;
  c.d = d;
  return c;
}

cval make_cval_complex(cval r, cval i)
{
  assert(r.kind == i.kind);

  switch (r.kind)
    {
    case cval_float:
      r.d_i = i.d;
      r.kind = cval_float_complex;
      return r;
    case cval_uint:
      assert(r.isize == i.isize);
      r.kind = cval_uint_complex;
      r.ui_i = i.ui;
      return r;
    case cval_sint:
      assert(r.isize == i.isize);
      r.kind = cval_sint_complex;
      r.si_i = i.si;
      return r;

    default: abort(); return r;
    }
}

cval make_cval_address(data_declaration ddecl, label_declaration ldecl,
		       largest_int offset)
{
  cval c = make_cval_signed(offset, ptrdiff_t_type);

  assert(!(ldecl && ddecl));
  c.kind = cval_address;
  c.ddecl = ddecl;
  c.ldecl = ldecl;

  return c;
}

bool cval_isinteger(cval c)
{
  return c.kind == cval_sint || c.kind == cval_uint;
}

bool cval_iscomplex(cval c)
{
  return c.kind == cval_sint_complex || c.kind == cval_uint_complex ||
    c.kind == cval_float_complex;
}

bool cval_knownbool(cval c)
/* Returns: TRUE if the truth-value of c can be determined (use cval_boolvalue
   to get that value)
*/
{
  switch (c.kind) {
  default: case cval_variable: assert(0);
  case cval_unk: return FALSE;
  case cval_address: return c.si == 0;
  case cval_uint: case cval_sint: case cval_float:
  case cval_uint_complex: case cval_sint_complex: case cval_float_complex:
    return TRUE;
  }
}

bool cval_boolvalue(cval c)
/* Returns: TRUE if c is a non-zero constant
   Requires: cval_knownbool(c)
 */
{
  switch (c.kind) {
  default: assert(0);
  case cval_address: assert(c.si == 0); return TRUE;
  case cval_uint: return c.ui != 0;
  case cval_sint: return c.si != 0;
  case cval_float: return c.d != 0;
  case cval_uint_complex: return c.ui && c.ui_i;
  case cval_sint_complex: return c.si && c.si_i;
  case cval_float_complex: return c.d && c.d_i;
  }
}

bool cval_knownvalue(cval c)
/* Returns: TRUE if the value of c can be determined (this is false for
   address constants, while cval_knownbool is true for address constants
   with offset 0)
*/
{
  switch (c.kind) {
  default: case cval_variable: assert(0);
  case cval_unk: case cval_address: return FALSE;
  case cval_uint: case cval_sint: case cval_float:
  case cval_uint_complex: case cval_sint_complex: case cval_float_complex:
    return TRUE;
  }
}

largest_uint cval_uint_value(cval c)
/* Returns: The value of c as an unsigned int
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/
{
  switch (c.kind) {
  default: assert(0); return 0;
  case cval_uint: return c.ui;    
  case cval_sint: return c.si;
  case cval_float: return c.d;
  }
}

largest_int cval_sint_value(cval c)
/* Returns: The value of c as a signed int
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/
{
  switch (c.kind) {
  default: assert(0); return 0;
  case cval_uint: return c.ui;    
  case cval_sint: return c.si;
  case cval_float: return c.d;
  }
}

long double cval_float_value(cval c)
/* Returns: The value of c as a long double
   Requires: cval_knownvalue(c) && !cval_iscomplex(c)
*/
{
  switch (c.kind) {
  default: assert(0); return 0;
  case cval_uint: return c.ui;    
  case cval_sint: return c.si;
  case cval_float: return c.d;
  }
}

bool cval_isone(cval c)
/* Returns: TRUE if c is 1 (FALSE for unknown constants)
   Requires: c not be cval_top
 */
{
  switch (c.kind) {
  default: case cval_variable: assert(0);
  case cval_unk: case cval_address: return FALSE;
  case cval_uint: return c.ui == 1;
  case cval_sint: return c.si == 1;
  case cval_float: return c.d == 1;
  case cval_uint_complex: return c.ui == 1 && c.ui_i == 0;
  case cval_sint_complex: return c.si == 1 && c.si_i == 0;
  case cval_float_complex: return c.d == 1 && c.d_i == 0;
  }
}

static largest_uint truncate_unsigned(largest_uint x, size_t tsize)
{
  /* The shift is undefined for the size of largest_uint (and the masking is then
     a no-op) */
  assert(tsize <= sizeof(largest_uint));

  if (tsize == sizeof(largest_uint))
    return x;
  else
    return x & (((largest_uint)1 << BITSPERBYTE * tsize) - 1);
}

/* SAME: 2's complement */
static largest_int truncate_signed(largest_int x, size_t tsize)
{
  largest_uint umask, uval;

  assert(tsize <= sizeof(largest_int));

  if (tsize == sizeof(largest_int))
    return x;

  /* masking */
  umask = (((largest_uint)1 << BITSPERBYTE * tsize) - 1);
  uval = x & umask;
  /* sign extension */
  if (uval & ((largest_uint)1 << (BITSPERBYTE * tsize - 1)))
    return (largest_int)(((largest_uint)-1 & ~umask) | uval);
  else
    return uval;
}

/* All of these functions will return cval_top if the result is not a
   constant expression */

cval cval_cast(cval c, type to)
/* Returns: c cast to type to
   Requires: type_scalar(to)
*/
{
  if (cval_istop(c))
    return cval_top;

  if (cval_isunknown(c))
    return cval_unknown;

  if (type_complex(to))
    {
      type base = make_base_type(to);

      switch (c.kind)
	{
	case cval_address: return cval_top;
	case cval_sint: case cval_uint: case cval_float:
	  return make_cval_complex(cval_cast(c, base),
				   cval_cast(cval_zero, base));
	  return c;
	case cval_sint_complex: case cval_uint_complex: case cval_float_complex:
	  return make_cval_complex(cval_cast(cval_realpart(c), base),
				   cval_cast(cval_imagpart(c), base));
	default:assert(0); return c;
	}
    }

  if (cval_iscomplex(c))
    return cval_cast(cval_realpart(c), to);

  if (type_floating(to))
    {
      switch (c.kind)
	{
	case cval_address: return cval_top; /* And not cval_unknown for some reason */
	case cval_sint: case cval_uint:
	  c.kind = cval_float;
	  /* Note that the cast is necessary otherwise it would cast to the common
	     type of largest_int/largest_uint (largest_uint), so c.si would be
	     cast to unsigned. */
	  c.d = c.kind == cval_sint ? (long double)c.si : (long double)c.ui;
	  return c;
	case cval_float:
	  if (type_float(to))
	    c.d = (float)c.d;
	  else if (type_double(to))
	    c.d = (double)c.d;
	  return c;
	default:assert(0); return c;
	}
    }
  else
    {
      size_t tosize = type_size(to);

      c.isize = tosize;
      switch (c.kind)
	{
	case cval_float:
	  /* If it's floating, make it an integer */
	  /* Note: can't cast floating point number to a pointer */
	  assert(!type_pointer(to));
	  if (type_unsigned(to))
	    {
	      c.kind = cval_uint;
	      c.ui = c.d;
	    }
	  else
	    {
	      c.kind = cval_sint;
	      c.si = c.d;
	    }
	  return c;

	case cval_address:
	  /* Lose value if cast address of symbol to too-narrow a type */
	  if (tosize < type_size(intptr_type))
	    return cval_unknown;
	  /* Otherwise nothing happens (the offset is already restricted to
	     the range of intptr_type). */
	  return c;

	case cval_uint: case cval_sint:
	  if (type_unsigned(to) || type_pointer(to))
	    {
	      if (c.kind == cval_sint)
		c.ui = c.si;
	      c.ui = truncate_unsigned(c.ui, tosize);
	      c.kind = cval_uint;
	    }
	  else
	    {
	      if (c.kind == cval_uint)
		c.si = c.ui;
	      c.si = truncate_signed(c.si, tosize);
	      c.kind = cval_sint;
	    }
	  return c;

	default: assert(0); return c;
	}
    }
}

cval cval_not(cval c)
{
  if (!cval_knownbool(c))
    return cval_unknown;
  else
    return make_cval_signed(!cval_boolvalue(c), int_type);
}

cval cval_negate(cval c)
{
  switch (c.kind)
    {
    case cval_variable: return cval_top;
    case cval_unk: case cval_address: return cval_unknown;
    case cval_sint: c.si = -c.si; return c; /* XXX: overflow */
    case cval_uint: c.ui = truncate_unsigned(-c.ui, c.isize); return c;
    case cval_float: c.d = -c.d; return c;
    case cval_sint_complex:
      c.si = -c.si;
      c.si_i = -c.si_i; 
      return c; /* XXX: overflow */
    case cval_uint_complex:
      c.ui = truncate_unsigned(-c.ui, c.isize);
      c.ui_i = truncate_unsigned(-c.ui_i, c.isize);
      return c;
    case cval_float_complex: c.d = -c.d; c.d_i = -c.d_i; return c;
    default: abort(); return cval_top;
    }
}

cval cval_bitnot(cval c)
{
  switch (c.kind)
    {
    case cval_variable: return cval_top;
    case cval_unk: case cval_address: return cval_unknown;
    case cval_sint: c.si = truncate_signed(~c.si, c.isize); return c;
    case cval_uint: c.ui = truncate_unsigned(~c.ui, c.isize); return c;
    default: abort(); return cval_top;
    }
}

cval cval_conjugate(cval c)
{
  switch (c.kind)
    {
    case cval_variable: return cval_top;
    case cval_unk: return cval_unknown;
    case cval_sint_complex:
      c.si_i = -c.si_i; 
      return c; /* XXX: overflow */
    case cval_uint_complex:
      c.ui_i = truncate_unsigned(-c.ui_i, c.isize);
      return c;
    case cval_float_complex: c.d_i = -c.d_i; return c;
    default: abort(); return cval_top;
    }
}

cval cval_realpart(cval c)
{
  switch (c.kind)
    {
    case cval_variable: return cval_top;
    case cval_unk: return cval_unknown;
    case cval_sint_complex: c.kind = cval_sint; return c;
    case cval_uint_complex: c.kind = cval_uint; return c;
    case cval_float_complex: c.kind = cval_float; return c;
    case cval_sint: case cval_uint: case cval_float: return c;
    default: abort(); return cval_top;
    }
}

cval cval_imagpart(cval c)
{
  switch (c.kind)
    {
    case cval_variable: return cval_top;
    case cval_unk: return cval_unknown;
    case cval_sint_complex: c.kind = cval_sint; c.si = c.si_i; return c;
    case cval_uint_complex: c.kind = cval_uint; c.ui = c.ui_i; return c;
    case cval_float_complex: c.kind = cval_float; c.d = c.d_i; return c;
    case cval_sint: c.si = 0; return c;
    case cval_uint: c.ui = 0; return c;
    case cval_float: c.d = 0; return c;
    default: abort(); return cval_top;
    }
}

/* The binary operators require that both arguments have been cast to a common
   type. */
cval cval_add(cval c1, cval c2)
{
  if (cval_istop(c1) || cval_istop(c2))
    return cval_top;

  if (cval_isunknown(c1) || cval_isunknown(c2))
    return cval_unknown;

  if (cval_isaddress(c2))
    {
      cval tmp = c1; c1 = c2; c2 = tmp;
    }

  switch (c1.kind)
    {
    case cval_float:
      assert(c2.kind == cval_float);
      c1.d += c2.d;
      return c1;

    case cval_address:
      switch (c2.kind)
	{
	case cval_address: return cval_unknown;
	case cval_sint: c1.si = truncate_signed(c1.si + c2.si, c1.isize); return c1;
	case cval_uint: c1.si = truncate_signed(c1.si + c2.ui, c1.isize); return c1;
	default: assert(0); return c1;
	}

    case cval_sint:
      assert(c2.kind == cval_sint && c1.isize == c2.isize);
      c1.si = truncate_signed(c1.si + c2.si, c1.isize);
      return c1;
      
    case cval_uint:
      assert(c2.kind == cval_uint && c1.isize == c2.isize);
      c1.ui = truncate_unsigned(c1.ui + c2.ui, c1.isize);
      return c1;
      
    case cval_float_complex:
      assert(c2.kind == cval_float_complex);
      c1.d += c2.d;
      c1.d_i += c2.d_i;
      return c1;

    case cval_sint_complex:
      assert(c2.kind == cval_sint_complex && c1.isize == c2.isize);
      c1.si = truncate_signed(c1.si + c2.si, c1.isize);
      c1.si_i = truncate_signed(c1.si_i + c2.si_i, c1.isize);
      return c1;
      
    case cval_uint_complex:
      assert(c2.kind == cval_uint_complex && c1.isize == c2.isize);
      c1.ui = truncate_unsigned(c1.ui + c2.ui, c1.isize);
      c1.ui_i = truncate_unsigned(c1.ui_i + c2.ui_i, c1.isize);
      return c1;
      
    default:
      assert(0);
      return c1;
    }
}

cval cval_sub(cval c1, cval c2)
{
  if (cval_istop(c1) || cval_istop(c2))
    return cval_top;

  if (cval_isunknown(c1) || cval_isunknown(c2))
    return cval_unknown;

  switch (c1.kind)
    {
    case cval_float:
      assert(c2.kind == cval_float);
      c1.d -= c2.d;
      return c1;

    case cval_address:
      switch (c2.kind)
	{
	case cval_address: 
	  if (c1.ddecl != c2.ddecl || c1.ldecl != c2.ldecl)
	    return cval_unknown; /* Difference of different symbols */
	  c1.kind = cval_sint;
	  c1.si = truncate_signed(c1.si - c2.si, c1.isize);
	  return c1;

	case cval_sint: c1.si = truncate_signed(c1.si - c2.si, c1.isize); return c1;
	case cval_uint: c1.si = truncate_signed(c1.si - c2.ui, c1.isize); return c1;
	default: assert(0); return c1;
	}

    case cval_sint:
      if (c2.kind == cval_address)
	return cval_unknown;
      assert(c2.kind == cval_sint && c1.isize == c2.isize);
      c1.si = truncate_signed(c1.si - c2.si, c1.isize);
      return c1;
      
    case cval_uint:
      if (c2.kind == cval_address)
	return cval_unknown;
      assert(c2.kind == cval_uint && c1.isize == c2.isize);
      c1.ui = truncate_unsigned(c1.ui - c2.ui, c1.isize);
      return c1;
      
    case cval_float_complex:
      assert(c2.kind == cval_float_complex);
      c1.d -= c2.d;
      c1.d_i -= c2.d_i;
      return c1;

    case cval_sint_complex:
      assert(c2.kind == cval_sint_complex && c1.isize == c2.isize);
      c1.si = truncate_signed(c1.si - c2.si, c1.isize);
      c1.si_i = truncate_signed(c1.si_i - c2.si_i, c1.isize);
      return c1;
      
    case cval_uint_complex:
      assert(c2.kind == cval_uint_complex && c1.isize == c2.isize);
      c1.ui = truncate_unsigned(c1.ui - c2.ui, c1.isize);
      c1.ui_i = truncate_unsigned(c1.ui_i - c2.ui_i, c1.isize);
      return c1;
      
    default:
      assert(0);
      return c1;
    }
}

cval cval_times(cval c1, cval c2)
{
  if (cval_istop(c1) || cval_istop(c2))
    return cval_top;

  if (cval_isunknown(c1) || cval_isunknown(c2))
    return cval_unknown;

  if (cval_isaddress(c2))
    {
      cval tmp = c1; c1 = c2; c2 = tmp;
    }

  if (cval_iscomplex(c1))
    {
      cval c1r = cval_realpart(c1), c1i = cval_imagpart(c1),
	c2r = cval_realpart(c2), c2i = cval_imagpart(c2);

      assert(cval_iscomplex(c2));
      /* Note: this is what gcc does. The C99 standard appears to
	 require something rather more complicated (aka "do the right
	 thing") */
      return make_cval_complex(cval_sub(cval_times(c1r, c2r),
					cval_times(c1i, c2i)),
			       cval_add(cval_times(c1r, c2i),
					cval_times(c1i, c2r)));
    }

  switch (c1.kind)
    {
    case cval_float:
      assert(c2.kind == cval_float);
      c1.d *= c2.d;
      return c1;

    case cval_address:
      return cval_isone(c2) ? c1 : cval_unknown;

    case cval_sint:
      assert(c2.kind == cval_sint && c1.isize == c2.isize);
      c1.si = truncate_signed(c1.si * c2.si, c1.isize);
      return c1;
      
    case cval_uint:
      assert(c2.kind == cval_uint && c1.isize == c2.isize);
      c1.ui = truncate_unsigned(c1.ui * c2.ui, c1.isize);
      return c1;
      
    default:
      assert(0);
      return c1;
    }
}

cval cval_divide(cval c1, cval c2)
{
  if (cval_istop(c1) || cval_istop(c2))
    return cval_top;

  if (cval_isunknown(c1) || cval_isunknown(c2))
    return cval_unknown;

  if (cval_iscomplex(c1))
    {
      cval c1r = cval_realpart(c1), c1i = cval_imagpart(c1),
	c2r = cval_realpart(c2), c2i = cval_imagpart(c2);
      cval mag = cval_add(cval_times(c1r, c2r), cval_times(c1i, c2i));

      assert(cval_iscomplex(c2));
      /* Note: this is what gcc does. The C99 standard appears to
	 require something rather more complicated (aka "do the right
	 thing") */
      return make_cval_complex(cval_divide(cval_add(cval_times(c1r, c2r),
						    cval_times(c1i, c2i)),
					   mag),
			       cval_divide(cval_sub(cval_times(c1i, c2r),
						    cval_times(c1r, c2i)),
					   mag));
    }

  switch (c1.kind)
    {
    case cval_float:
      assert(c2.kind == cval_float);
      c1.d /= c2.d;
      return c1;

    case cval_address:
      return cval_isone(c2) ? c1 : cval_unknown;

    case cval_sint:
      if (c2.kind == cval_address)
	return cval_unknown;
      assert(c2.kind == cval_sint && c1.isize == c2.isize);
      if (c2.si == 0)
	return cval_top;
      /* Note that signed division can overflow (MININT / -1). */
      c1.si = truncate_signed(c1.si / c2.si, c1.isize);
      return c1;
      
    case cval_uint:
      if (c2.kind == cval_address)
	return cval_unknown;
      assert(c2.kind == cval_uint && c1.isize == c2.isize);
      if (c2.ui == 0)
	return cval_top;
      c1.ui /= c2.ui;
      return c1;
      
    default:
      assert(0);
      return c1;
    }
}

cval cval_modulo(cval c1, cval c2)
{
  if (cval_istop(c1) || cval_istop(c2))
    return cval_top;

  if (cval_isone(c2))
    return make_cval_signed(0, int_type);

  if (cval_isunknown(c1) || cval_isunknown(c2) ||
      cval_isaddress(c1) || cval_isaddress(c2))
    return cval_unknown;

  switch (c1.kind)
    {
    case cval_float:
      assert(c2.kind == cval_float);
      c1.d /= c2.d;
      return c1;

    case cval_sint:
      assert(c2.kind == cval_sint && c1.isize == c2.isize);
      if (c2.si == 0)
	return cval_top;
      c1.si = truncate_signed(c1.si % c2.si, c1.isize);
      return c1;
      
    case cval_uint:
      assert(c2.kind == cval_uint && c1.isize == c2.isize);
      if (c2.ui == 0)
	return cval_top;
      c1.ui %= c2.ui;
      return c1;
      
    default:
      assert(0);
      return c1;
    }
}

#define CVAL_BITOP(OP) \
{ \
  if (cval_istop(c1) || cval_istop(c2)) \
    return cval_top; \
 \
  if (cval_isunknown(c1) || cval_isunknown(c2) || \
      cval_isaddress(c1) || cval_isaddress(c2)) \
    return cval_unknown; \
 \
  assert(c1.kind == c2.kind && c1.isize == c2.isize); \
  switch (c1.kind) \
    { \
    case cval_sint: \
      c1.si = truncate_signed(c1.si OP c2.si, c1.isize); \
      return c1; \
       \
    case cval_uint: \
      c1.ui = truncate_signed(c1.ui OP c2.ui, c1.isize); \
      return c1; \
       \
    default: \
      assert(0); \
      return c1; \
    } \
}

cval cval_lshift(cval c1, cval c2) CVAL_BITOP(<<)
cval cval_rshift(cval c1, cval c2) CVAL_BITOP(>>)
cval cval_bitand(cval c1, cval c2) CVAL_BITOP(&)
cval cval_bitor(cval c1, cval c2)  CVAL_BITOP(|)
cval cval_bitxor(cval c1, cval c2) CVAL_BITOP(^)

#define CVAL_RELOP(OP) \
{ \
  bool res; \
 \
  if (cval_istop(c1) || cval_istop(c2)) \
    return cval_top; \
 \
  /* Surprisingly (?) &x == &x is not a constant expression */ \
  if (cval_isunknown(c1) || cval_isunknown(c2) || \
      cval_isaddress(c1) || cval_isaddress(c2)) \
    return cval_unknown; \
 \
  switch (c1.kind) \
    { \
    case cval_float: \
      assert(c2.kind == cval_float); \
      res = c1.d OP c2.d; \
      break; \
 \
    case cval_sint: \
      assert(c2.kind == cval_sint && c1.isize == c2.isize); \
      res = c1.si OP c2.si; \
      break; \
       \
    case cval_uint: \
      assert(c2.kind == cval_uint && c1.isize == c2.isize); \
      res = c1.ui OP c2.ui; \
      break; \
       \
    default: \
      assert(0); \
      res = FALSE; \
      break; \
    } \
  return make_cval_signed(res, int_type); \
}

static cval cval_simpleeq(cval c1, cval c2) CVAL_RELOP(==)
cval cval_leq(cval c1, cval c2) CVAL_RELOP(<=)

cval cval_eq(cval c1, cval c2)
{
  if (cval_iscomplex(c1) && cval_iscomplex(c2))
    {
      cval req = cval_simpleeq(cval_realpart(c1), cval_realpart(c2));
      cval ieq = cval_simpleeq(cval_imagpart(c1), cval_imagpart(c2));

      return make_cval_signed(cval_isone(req) && cval_isone(ieq), int_type);
    }
  else
    return cval_simpleeq(c1, c2);
}

/* True if x fits in the range of type t */
bool uint_inrange(largest_uint x, type t)
{
  size_t tsize = type_size(t);
  largest_uint max;

  assert(tsize <= sizeof(largest_uint));

  if (tsize == sizeof(largest_uint) && type_unsigned(t))
    return TRUE;

  max = (largest_uint)1 << (BITSPERBYTE * tsize - type_unsigned(t));

  return x < max;
}

bool sint_inrange(largest_int x, type t)
{
  size_t tsize = type_size(t);
  largest_int max;

  assert(tsize <= sizeof(largest_uint));

  if (x < 0 && type_unsigned(t))
    return FALSE;

  if (tsize == sizeof(largest_uint))
    return TRUE;

  max = (largest_int)1 << (BITSPERBYTE * tsize - type_unsigned(t));

  /* The x<0&&unsigned and largest_int cases have been handled above. */
  return x >= -max && x < max;
}

bool cval_inrange(cval c, type t)
/* Requires: constant_integral(c)
   Returns: TRUE if c is in range of type t. */
{
  switch (c.kind)
    {
    case cval_sint: return sint_inrange(c.si, t);
    case cval_uint: return sint_inrange(c.ui, t);
    default: abort(); return FALSE;
    }
}

largest_int cval_intcompare(cval c1, cval c2)
/* Requires: cval_isinteger(c1) && cval_isinteger(c2)
   Returns: x, x<0 if c1 < c2, x = 0 if c1 = c2 and x > 0 if c1 > c2
*/
{
  switch (c1.kind)
    {
    case cval_sint:
      switch (c2.kind)
	{
	case cval_sint:
	  return c1.si - c2.si;
	case cval_uint:
	  if (c1.si < 0)
	    return -1;
	  return c1.si - c2.ui; /* common type is largest_uint */
	default: abort(); return 0;
	}
    case cval_uint:
      switch (c2.kind)
	{
	case cval_sint:
	  if (c2.si < 0)
	    return 1;
	  return c1.ui - c2.si; /* common type is largest_uint */
	case cval_uint:
	  return c1.ui - c2.ui; /* ASSUME: 2's complement */
	default: abort(); return 0;
	}
    default: abort(); return 0;
    }
}

