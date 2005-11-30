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

/* Constants come in three kinds:
   - "unknown": value is completely unknown (constant_unknown)
   - "address": value is the address of some global symbol and an offset 
                (constant_address)
   - "value": known value (integer, floating point)
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

known_cst make_unknown_cst(type t);
known_cst make_cst(cval c, type t);
known_cst make_address_cst(data_declaration ddecl, label_declaration ldecl,
			   largest_int offset, type t);
known_cst make_signed_cst(largest_int x, type t);
known_cst make_unsigned_cst(largest_uint x, type t);

lexical_cst fold_lexical_int(type itype, location loc, cstring tok,
			     bool iscomplex, largest_uint intvalue, bool overflow);
lexical_cst fold_lexical_real(type realtype, location loc, cstring tok);
/* XXX: What's the right type for charvalue ? (must hold wchar_t or int) */
lexical_cst fold_lexical_char(location loc, cstring tok,
			      bool wide_flag, int charvalue);
string_cst fold_lexical_string(location loc, cstring tok,
			       bool wide_flag, wchar_array stringvalue);

known_cst fold_label_address(expression e);
known_cst fold_sizeof(expression e, type stype);
known_cst fold_alignof(expression e, type atype);
known_cst fold_cast(expression e);
known_cst fold_address_of(expression e);
known_cst fold_unary(expression e);
known_cst fold_binary(type restype, expression e);
known_cst fold_conditional(expression e);
known_cst fold_identifier(expression e, data_declaration decl);

known_cst fold_add(type restype, known_cst c1, known_cst c2);

known_cst foldaddress_identifier(expression e, data_declaration decl);
known_cst foldaddress_string(string s);
known_cst foldaddress_field_ref(known_cst object, field_declaration fdecl);

bool definite_null(expression e);
bool definite_zero(expression e);
bool is_zero_constant(known_cst c);

/* Print a warning if a constant expression had overflow in folding.
   Invoke this function on every expression that the language
   requires to be a constant expression.
   Note the ANSI C standard says it is erroneous for a
   constant expression to overflow.  */
void constant_overflow_warning(known_cst c);

/*bool constant_address(known_cst c);*/
#define constant_address(c) cval_isaddress((c)->cval)
/* Returns: TRUE if c is an address constant
 */

/*bool constant_unknown(known_cst c);*/
#define constant_unknown(c) cval_isunknown((c)->cval)
/* Returns: TRUE if c is a constant whose value is not computable 
   at load time (these behave for most, but not all, purposes as 
   non-constant expressions)
 */

/*bool constant_integral(known_cst c);*/
#define constant_integral(c) cval_isinteger((c)->cval)
/* Returns: TRUE if c is an integer constant (signed or unsigned)
*/

/*bool constant_float(known_cst c);*/
#define constant_float(c) (cval_isfloating((c)->cval))
/* Returns: TRUE if c is a floating-point constant
*/

/*largest_uint constant_uint_value(known_cst c);*/
#define constant_uint_value(c) cval_uint_value((c)->cval)
/* Returns: Value of c as an unsigned integer.
   Requires: see cval_uint_value
*/

/*largest_int constant_sint_value(known_cst c);*/
#define constant_sint_value(c) cval_sint_value((c)->cval)
/* Returns: Value of c as an unsigned integer.
   Requires: see cval_sint_value
*/

/*long double constant_float_value(known_cst c);*/
#define constant_float_value(c) cval_float_value((c)->cval)
/* Returns: Value of c as an unsigned integer.
   Requires: see cval_float_value
*/

/*bool constant_knownbool(known_cst c);*/
/* Returns: TRUE if the truth-value of c can be determined (use 
   constant_boolvalue to get that value)
*/
#define constant_knownbool(c) cval_knownbool((c)->cval)

/*bool cval_boolvalue(cval c);*/
/* Returns: TRUE if c is a non-zero constant
   Requires: cval_knownbool(c)
 */
#define constant_boolvalue(c) cval_boolvalue((c)->cval)

#endif
