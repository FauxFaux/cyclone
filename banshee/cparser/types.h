/* This file is part of the RC compiler.

This file is derived from the GNU C Compiler. It is thus
   Copyright (C) 1987, 88, 89, 92-7, 1998 Free Software Foundation, Inc.
and
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
Boston, MA 02111-1307, USA. */

#ifndef TYPES_H
#define TYPES_H

#include "cval.h"

typedef enum 
{
  no_qualifiers = 0, 
  transparent_qualifier = 1,
#define Q(name, kind, qual, val) qual = val,
#include "qualifiers.h"
#undef Q
  last_qualifier,
  sameregion_qualifier = 0,
  parentptr_qualifier = 0,
  traditional_qualifier = 0,
  deletes_qualifier = 0
} type_quals;

extern type float_type, double_type, long_double_type, 
  int_type, unsigned_int_type, long_type, unsigned_long_type,
  long_long_type, unsigned_long_long_type, short_type, unsigned_short_type,
  char_type, char_array_type, wchar_type, wchar_array_type,
  unsigned_char_type, signed_char_type, void_type, ptr_void_type,
  size_t_type, ptrdiff_t_type, intptr_type;
extern type error_type;

void init_types(void);

/* Build types */

/* Return the 'complex t' version of basic type t (one of the integral or
   floating-point types) */
type make_complex_type(type t);

/* Return the base type of complex type t (one of the integral or
   floating-point types) */
type make_base_type(type t);

/* Return the type t with it's qualifiers set to qualifiers (old qualifiers
   are ignored). This is illegal for array types */
type make_qualified_type(type t, type_quals qualifiers);
type qualify_type1(type t, type t1);
type qualify_type2(type t, type t1, type t2);

/* Return 't' modified to have alignment 'new_alignment' */
type align_type(type t, int new_alignment);

/* Return type 'pointer to t' (unqualified) */
type make_pointer_type(type t);

/* Return type 'array [size] of t'. size is optional */
type make_array_type(type t, expression size);

/* Return type 'function with argument types argtypes returning t'.
   If oldstyle is true, this is an oldstyle function type and
   argtypes is NULL */
type make_function_type(type t, typelist argtypes, bool varargs, bool oldstyle);

/* Return the tagged type whose declaration is d */
type make_tagged_type(tag_declaration d);

typelist new_typelist(region r);
void typelist_append(typelist l, type t);

bool empty_typelist(typelist l);

/* Scanning */
typedef struct typelist_element *typelist_scanner;
void typelist_scan(typelist tl, typelist_scanner *scanner);
type typelist_next(typelist_scanner *scanner);

/* Size and alignment */
size_t type_size(type t); /* Requires: type_size_cc(t) */
size_t type_alignment(type t);

/* True if t has a size (void or not incomplete) */
bool type_has_size(type t);
/* True if the sizeof of t is a compile-time constant */
bool type_size_cc(type t);
/* Note: type_size_cc => type_has_size */

type common_type(type t1, type t2);

bool type_equal(type t1, type t2);
bool type_equal_unqualified(type t1, type t2);
bool type_compatible(type t1, type t2);
bool type_compatible_unqualified(type t1, type t2);

/* Return TRUE if T is not affected by default promotions.  */
bool type_self_promoting(type t);

bool type_incomplete(type t);

/* Return name of qualifier q (must not be a qualifier set) */
char *qualifier_name(type_quals q);

type_quals type_qualifiers(type t);
bool type_const(type t);
bool type_sameregion(type t);
bool type_parentptr(type t);
bool type_traditional(type t);
bool type_deletes(type t);
bool type_transparent(type t);
bool type_readonly(type t);
bool type_volatile(type t);

bool type_plain_char(type t);
bool type_signed_char(type t);
bool type_unsigned_char(type t);
bool type_short(type t);
bool type_unsigned_short(type t);
bool type_int(type t);
bool type_unsigned_int(type t);
bool type_long(type t);
bool type_unsigned_long(type t);
bool type_long_long(type t);
bool type_unsigned_long_long(type t);
bool type_long_double(type t);

bool type_tagged(type t);
bool type_integral(type t);	/* Does not include enum's */
bool type_floating(type t);
bool type_complex(type t);
bool type_float(type t);
bool type_double(type t);
bool type_void(type t);
bool type_char(type t);
bool type_function(type t);
bool type_array(type t);
bool type_pointer(type t);
bool type_enum(type t);
bool type_struct(type t);
bool type_union(type t);
bool type_integer(type t);	/* Does include enum's */
bool type_unsigned(type t);
bool type_smallerthanint(type t);
bool type_real(type t);
bool type_arithmetic(type t);
bool type_scalar(type t);
bool type_aggregate(type t);

type make_unsigned_type(type t);

type type_function_return_type(type t);
typelist type_function_arguments(type t);
bool type_function_varargs(type t);
bool type_function_oldstyle(type t);

/* Return TRUE if function type FNTYPE specifies a fixed number of parameters
   and none of their types is affected by default promotions.  */
bool self_promoting_args(type fntype);

type type_points_to(type t);
type type_array_of(type t);
type type_array_of_base(type t);
expression type_array_size(type t);
tag_declaration type_tag(type t);
type type_base(type t);

/* Build AST nodes such that "MODIFIERS D" represents the declaration of
   "T INSIDE", at location loc, allocating in region r */
void type2ast(region r, location loc, type t, declarator inside,
	      declarator *d, type_element *modifiers);

bool type_contains_pointers(type t);
bool type_contains_cross_pointers(type t);
bool type_contains_qualified_pointers(type t); /* True if any sameregion/traditional ptrs */
bool type_contains_union_with_pointers(type t);

type type_default_conversion(type from);
type function_call_type(function_call fcall);

void name_tag(tag_declaration tag);

/* Return the integral type of size 'size', unsigned if 'isunsigned' is true */
type type_for_size(int size, bool isunsigned);

type type_for_cval(cval c, bool isunsigned);

#endif
