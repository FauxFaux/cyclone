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

#ifndef AST_UTILS_H
#define AST_UTILS_H

data_declaration get_parameter(declaration d);
function_declarator get_fdeclarator(declarator d);
bool oldstyle_function(function_decl fn);
bool is_localvar(expression e);
compound_stmt parent_block(node n);
function_decl parent_function(node n);
expression expression_of_stmt(compound_expr ce); /* Return expression which is value of compound statement expression, or NULL of node ("void" compound expression) */
bool expression_used(expression e);

bool is_assignment(void *e);
bool is_increment(void *e);
data_declaration base_identifier(data_declaration d);
bool same_function(data_declaration d1, data_declaration d2);
bool call_to(data_declaration fnd, function_call fce);
expression ignore_fields(expression e);

/* True for expressions whose value is known to be 0 (but which
   may not be constant expressions) */
bool zero_expression(expression e);

expression build_int_constant(region r, location loc, type t, largest_int c);
expression build_uint_constant(region r, location loc, type t, largest_uint c);
#define build_zero(r, loc) build_int_constant(r, loc, int_type, 0)

expression build_identifier(region r, location loc, data_declaration id);

cval value_of_enumerator(enumerator e);

#endif
