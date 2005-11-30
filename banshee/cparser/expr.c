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

#include "parser.h"
#include "expr.h"
#include "types.h"
#include "c-parse.h"
#include "constants.h"
#include "unparse.h"
#include "semantics.h"
#include "stmt.h"
#include "rc.h"
#include "AST_utils.h"

/* On FreeBSD, alloca is defined in stdlib, on most other platforms,
   (cygwin, linux) it is in alloca.h */
#ifndef __FreeBSD__
#include <alloca.h>
#endif

/* Return TRUE if TTL and TTR are pointers to types that are equivalent,
   ignoring their qualifiers.  */
static bool compatible_pointer_targets(type ttl, type ttr, bool pedantic)
{
  int val;

  val = type_compatible_unqualified(ttl, ttr);

  if (val == 2 && pedantic)
    pedwarn("types are not quite compatible");
  return val != 0;
}

static bool compatible_pointer_types(type tl, type tr)
{
  return compatible_pointer_targets(type_points_to(tl), type_points_to(tr),
				    pedantic);
}

static void warn_for_assignment(const char *msg, const char *opname,
				const char *function, int argnum)
{
  static char argstring[] = "passing arg %d of `%s'";
  static char argnofun[] =  "passing arg %d";

  if (opname == 0)
    {
      char *tmpname;

      if (function)
	{
	  /* Function name is known; supply it.  */
	  tmpname = (char *)alloca(strlen(function) + sizeof(argstring) + 25 /*%d*/ + 1);
	  sprintf(tmpname, argstring, argnum, function);
	}
      else
	{
	  /* Function name unknown (call through ptr); just give arg number.  */
	  tmpname = (char *)alloca(sizeof(argnofun) + 25 /*%d*/ + 1);
	  sprintf(tmpname, argnofun, argnum);
	}
      opname = tmpname;
    }
  pedwarn(msg, opname);
}

static void incomplete_type_error(expression e, type t)
{
  /* Avoid duplicate error message.  */
  if (t == error_type)
    return;

  if (e && is_identifier(e))
    error("`%s' has an incomplete type", CAST(identifier, e)->cstring.data);
  else
    {
      while (type_array(t) && type_array_size(t))
	t = type_array_of(t);

      if (type_struct(t))
	error("invalid use of undefined type `struct %s'", type_tag(t)->name);
      else if (type_union(t))
	error("invalid use of undefined type `union %s'", type_tag(t)->name);
      else if (type_enum(t))
	error("invalid use of undefined type `enum %s'", type_tag(t)->name);
      else if (type_void(t))
	error("invalid use of void expression");
      else if (type_array(t))
	error("invalid use of array with unspecified bounds");
      else
	assert(0);
      /* XXX: Missing special message for typedef's */
    }
}

static type require_complete_type(expression e, type etype)
{
  if (!type_incomplete(etype))
    return e->type;

  incomplete_type_error(e, etype);

  return error_type;
}

type default_conversion(expression e)
{
  type from = e->type;

  if (type_enum(from))
    from = type_tag(from)->reptype;

  if (type_smallerthanint(from))
    {
      /* Traditionally, unsignedness is preserved in default promotions. */
      if (flag_traditional && type_unsigned(from))
	return unsigned_int_type;
      else
	return int_type;
    }

  if (flag_traditional && !flag_allow_single_precision && type_float(from))
    return double_type;

  if (type_void(from))
    {
      error("void value not ignored as it ought to be");
      return error_type;
    }

  if (type_function(from))
    {
      assert(!e->cst);
      e->cst = e->static_address;
      return make_pointer_type(from);
    }

  if (type_array(from))
    {
      if (!e->lvalue)
	{
	  error("invalid use of non-lvalue array");
	  return error_type;
	}
      assert(!e->cst);
      e->cst = e->static_address;
      /* It's being used as a pointer, so is not an lvalue */
      e->lvalue = FALSE;
      return make_pointer_type(type_array_of(from));
    }

  return from;
}

type default_conversion_for_assignment(expression e)
{
  if (type_array(e->type) || type_function(e->type))
    return default_conversion(e);
  else
    return e->type;
}

static void readonly_warning(expression e, char *context)
{
  char buf[80];

  strcpy(buf, context);

  if (is_field_ref(e))
    {
      field_ref field = CAST(field_ref, e);

      if (type_readonly(field->arg1->type))
	readonly_warning(field->arg1, context);
      else
	{
	  strcat(buf, " of read-only member `%s'");
	  pedwarn(buf, field->cstring.data);
	}
    }
  else if (is_identifier(e))
    {
      strcat(buf, " of read-only variable `%s'");
      pedwarn(buf, CAST(identifier, e)->cstring.data);
    }
  else
    pedwarn ("%s of read-only location", buf);
}

static bool check_writable_lvalue(expression e, char *context)
{
  if (!e->lvalue || type_array(e->type))
    {
      error("invalid lvalue in %s", context);
      return FALSE;
    }
  if (type_readonly(e->type))
    readonly_warning(e, context);
  return TRUE;
}

bool check_conversion(type to, type from)
{
  if (type_equal_unqualified(to, from))
    return TRUE;

  if (to == error_type || from == error_type)
    return FALSE;

  if (type_void(from))
    {
      error("void value not ignored as it ought to be");
      return FALSE;
    }

  if (type_void(to))
    return TRUE;

  if (type_integer(to))
    {
      if (!type_scalar(from))
	{
	  error("aggregate value used where an integer was expected");
	  return FALSE;
	}
    }
  else if (type_pointer(to))
    {
      if (!(type_integer(from) || type_pointer(from)))
	{
	  error("cannot convert to a pointer type");
	  return FALSE;
	}
    }
  else if (type_floating(to))
    {
      if (type_pointer(from))
	{
	  error("pointer value used where a floating point value was expected");
	  return FALSE;
	}
      else if (!type_arithmetic(from))
	{
	  error("aggregate value used where a float was expected");
	  return FALSE;
	}
    }
  else if (type_complex(to))
    {
      if (type_pointer(from))
	{
	  error("pointer value used where a complex was expected");
	  return FALSE;
	}
      else if (!type_arithmetic(from))
	{
	  error("aggregate value used where a complex was expected");
	  return FALSE;
	}
    }
  else
    {
      error("conversion to non-scalar type requested");
      return FALSE;
    }
  return TRUE;
}

static bool assignable_pointer_targets(type tt1, type tt2, bool pedantic)
{
  return type_void(tt1) || type_void(tt2)
    || compatible_pointer_targets(tt1, tt2, pedantic);
}

static void ptrconversion_warnings(type ttl, type ttr, expression rhs,
				   const char *context,
				   const char *funname, int parmnum,
				   bool pedantic)
{
  if (pedantic
      && ((type_void(ttl) && type_function(ttr)) ||
	  (type_function(ttl) && type_void(ttr) &&
	   !(rhs && definite_null(rhs)))))
    warn_for_assignment("ANSI forbids %s between function pointer and `void *'",
			context, funname, parmnum);

  /* Const and volatile mean something different for function
     types, so the usual warnings are not appropriate.  */
  else if (type_function(ttl) && type_function(ttr))
    {
      /* Because const and volatile on functions are
	 restrictions that say the function will not do
	 certain things, it is okay to use a const or volatile
	 function where an ordinary one is wanted, but not
	 vice-versa.  */
      if (type_const(ttl) && !type_const(ttr))
	warn_for_assignment("%s makes `const *' function pointer from non-const",
			    context, funname, parmnum);
      if (type_volatile(ttl) && !type_volatile(ttr))
	warn_for_assignment("%s makes `volatile *' function pointer from non-volatile",
			    context, funname, parmnum);
    }
  else if (!type_function(ttl) && !type_function(ttr))
    {
      if (!type_const(ttl) && type_const(ttr))
	warn_for_assignment("%s discards `const' from pointer target type",
			    context, funname, parmnum);
      if (!type_volatile(ttl) && type_volatile(ttr))
	warn_for_assignment("%s discards `volatile' from pointer target type",
			    context, funname, parmnum);

      /* If this is not a case of ignoring a mismatch in signedness,
	 no warning.  */
      if (!assignable_pointer_targets(ttl, ttr, FALSE) && pedantic)
	warn_for_assignment("pointer targets in %s differ in signedness",
			    context, funname, parmnum);
    }
}

/* Return TRUE if no error and lhstype and rhstype are not error_type */
bool check_assignment(type lhstype, type rhstype, expression rhs,
		      const char *context, data_declaration fundecl,
		      const char *funname, int parmnum)
{
  bool zerorhs = rhs && definite_zero(rhs);

  if (lhstype == error_type || rhstype == error_type)
    return FALSE;

  if (type_void(rhstype))
    {
      error("void value not ignored as it ought to be");
      return FALSE;
    }

  if (type_equal_unqualified(lhstype, rhstype))
    return TRUE;

  if (type_arithmetic(lhstype) && type_arithmetic(rhstype))
    {
      if (rhs)
	constant_overflow_warning(rhs->cst);
      return check_conversion(lhstype, rhstype);
    }
  if (parmnum && (type_qualifiers(lhstype) & transparent_qualifier))
    {
      /* See if we can match any field of lhstype */
      tag_declaration tag = type_tag(lhstype);
      field_declaration fields, marginal_field = NULL;

      /* I blame gcc for this horrible mess (and it's minor inconsistencies
	 with the regular rules) */
      /* pedantic warnings are skipped in here because we're already
	 issuing a warning for the use of this construct */
      for (fields = tag->fieldlist; fields; fields = fields->next)
	{
	  type ft = fields->type;

	  if (type_compatible(ft, rhstype))
	    break;

	  if (!type_pointer(ft))
	    continue;

	  if (type_pointer(rhstype))
	    {
	      type ttl = type_points_to(ft), ttr = type_points_to(rhstype);
	      bool goodmatch = assignable_pointer_targets(ttl, ttr, FALSE);

	      /* Any non-function converts to a [const][volatile] void *
		 and vice versa; otherwise, targets must be the same.
		 Meanwhile, the lhs target must have all the qualifiers of
		 the rhs.  */
	      if (goodmatch)
		{
		  /* If this type won't generate any warnings, use it.  */
		  if ((type_function(ttr) && type_function(ttl))
		      ? ((!type_const(ttl) | type_const(ttr))
			 & (!type_volatile(ttl) | type_volatile(ttr)))
		      : ((type_const(ttl) | !type_const(ttr))
			 & (type_volatile(ttl) | !type_volatile(ttr))))
		    break;

		  /* Keep looking for a better type, but remember this one.  */
		  if (!marginal_field)
		    marginal_field = fields;
		}
	    }

	  /* Can convert integer zero to any pointer type.  */
	  /* Note that this allows passing *any* null pointer (gcc bug?) */
	  if (zerorhs)
	    break;
	}

      if (fields || marginal_field)
	{
	  if (!fields)
	    {
	      /* We have only a marginally acceptable member type;
		 it needs a warning.  */
	      type ttl = type_points_to(marginal_field->type),
		ttr = type_points_to(rhstype);

	      ptrconversion_warnings(ttl, ttr, rhs, context, funname, parmnum,
				     FALSE);
	    }
	  
	  if (pedantic && !(fundecl && fundecl->in_system_header))
	    pedwarn("ANSI C prohibits argument conversion to union type");

	  return TRUE;
	}
    }

  if (type_pointer(lhstype) && type_pointer(rhstype))
    {
      type ttl = type_points_to(lhstype), ttr = type_points_to(rhstype);
      bool goodmatch = assignable_pointer_targets(ttl, ttr, pedantic);

      /* Any non-function converts to a [const][volatile] void *
	 and vice versa; otherwise, targets must be the same.
	 Meanwhile, the lhs target must have all the qualifiers of the rhs.  */
      if (goodmatch || (type_equal_unqualified(make_unsigned_type(ttl),
					       make_unsigned_type(ttr))))
	ptrconversion_warnings(ttl, ttr, rhs, context, funname, parmnum,
			       pedantic);
      else
	warn_for_assignment("%s from incompatible pointer type",
			    context, funname, parmnum);

      return check_conversion(lhstype, rhstype);
    }
  /* enum = ptr and ptr = enum counts as an error, so use type_integral */
  else if (type_pointer(lhstype) && type_integral(rhstype))
    {
      if (!zerorhs)
	warn_for_assignment("%s makes pointer from integer without a cast",
			    context, funname, parmnum);
      return check_conversion(lhstype, rhstype);
    }
  else if (type_integral(lhstype) && type_pointer(rhstype))
    {
      warn_for_assignment("%s makes integer from pointer without a cast",
			  context, funname, parmnum);
      return check_conversion(lhstype, rhstype);
    }

  if (!context)
    if (funname)
      error("incompatible type for argument %d of `%s'", parmnum, funname);
    else
      error("incompatible type for argument %d of indirect function call",
	    parmnum);
  else
    error("incompatible types in %s", context);

  return FALSE;
}

expression make_error_expr(location loc)
{
  expression result = CAST(expression, new_error_expr(parse_region, loc));

  result->type = error_type;

  return result;
}

expression make_comma(location loc, expression elist)
{
  expression result = CAST(expression, new_comma(parse_region, loc, elist));
  expression e;

  scan_expression (e, elist)
    if (e->next) /* Not last */
      {
#if 0
	if (!e->side_effects)
	  {
	    /* The left-hand operand of a comma expression is like an expression
	       statement: with -W or -Wunused, we should warn if it doesn't have
	       any side-effects, unless it was explicitly cast to (void).  */
	    if ((extra_warnings || warn_unused)
		&& !(TREE_CODE (TREE_VALUE (list)) == CONVERT_EXPR
		      && TREE_TYPE (TREE_VALUE (list)) == void_type_node))
	      warning ("left-hand operand of comma expression has no effect");
	  }
	else if (warn_unused)
	  warn_if_unused_value(e);
#endif
      }
    else
      {
	if (type_array(e->type))
	  result->type = default_conversion(e);
	else
	  result->type = e->type;

	if (!pedantic)
	  {
	    /* XXX: I seemed to believe that , could be a constant expr in GCC,
	       but cst3.c seems to disagree. Check gcc code again ?
	       (It's a bad idea anyway) */
	    result->lvalue = e->lvalue;
	    result->isregister = e->isregister;
	    result->bitfield = e->bitfield;
	  }
      }

  return result;
}

static void check_dereference(expression result, type dereferenced,
			      const char *errorstring)
{
  if (type_pointer(dereferenced))
    {
      type t = type_points_to(dereferenced);

      result->type = t;
#if 0
      if (TYPE_SIZE (t) == 0 && TREE_CODE (t) != ARRAY_TYPE)
	{
	  error ("dereferencing pointer to incomplete type");
	  return error_mark_node;
	}
#endif
      if (type_void(t) && unevaluated_expression == 0)
	warning("dereferencing `void *' pointer");
      result->side_effects |= type_volatile(t) || flag_volatile;
    }
  else
    {
      result->type = error_type;
      if (dereferenced != error_type)
	error("invalid type argument of `%s'", errorstring);
    }
  result->lvalue = TRUE;
}

expression make_dereference(location loc, expression e)
{
  expression result = CAST(expression, new_dereference(parse_region, loc, e));

  result->side_effects = e->side_effects;
  check_dereference(result, default_conversion(e), "unary *");
  result->static_address = e->cst;

  return result;
}

expression make_extension_expr(location loc, expression e)
{
  expression result = CAST(expression, new_extension_expr(parse_region, loc, e));

  result->type = e->type;
  result->lvalue = e->lvalue;
  result->side_effects = e->side_effects;
  result->cst = e->cst;
  result->bitfield = e->bitfield;
  result->isregister = e->isregister;
  result->static_address = e->static_address;
  
  return result;
}

expression make_address_of(location loc, expression e)
{
  expression result = CAST(expression, new_address_of(parse_region, loc, e));

  result->type = error_type;

  if (e->type == error_type)
    ;
  else if (e->bitfield)
    error("attempt to take address of a bit-field structure member");
  else
    {
      if (e->isregister)
	pedwarn("address of a register variable requested");

      if (!(type_function(e->type) || e->lvalue))
	error("invalid lvalue in unary `&'");

      result->type = make_pointer_type(e->type);
      result->cst = e->static_address;
    }
  return result;
}

expression make_unary(location loc, int unop, expression e)
{
  switch (unop)
    {
    case kind_address_of:
      return make_address_of(loc, e);
    case kind_preincrement:
      return make_preincrement(loc, e);
    case kind_predecrement:
      return make_predecrement(loc, e);
    default:
      {
	expression result = CAST(expression, newkind_unary(parse_region, unop, loc, e));
	type etype = default_conversion(e);
	const char *errstring = NULL;

	if (etype == error_type)
	  result->type = error_type;
	else
	  {
	    switch (unop)
	      {
	      case kind_unary_plus:
		if (!type_arithmetic(etype))
		  errstring = "wrong type argument to unary plus";
		break;
	      case kind_unary_minus:
		if (!type_arithmetic(etype))
		  errstring = "wrong type argument to unary minus";
		break;
	      case kind_bitnot:
		if (type_complex(etype))
		  result->kind = kind_conjugate;
		else if (!type_integer(etype))
		  errstring = "wrong type argument to bit-complement";
		break;
	      case kind_not:
		if (!type_scalar(etype))
		  errstring = "wrong type argument to unary exclamation mark";
		else
		  etype = int_type;
		break;
	      case kind_realpart: case kind_imagpart:
		if (!type_arithmetic(etype))
		  if (unop == kind_realpart)
		    errstring = "wrong type argument to __real__";
		  else
		    errstring = "wrong type argument to __imag__";
		else
		  etype = type_complex(etype) ? make_base_type(etype) : etype;
	      default:
		assert(0);
	      }
	    if (errstring)
	      {
		error(errstring);
		result->type = error_type;
	      }
	    else
	      {
		result->type = etype;
		result->cst = fold_unary(result);
	      }
	  }
	return result;
      }
    }
}

expression make_label_address(location loc, id_label label)
{
  expression result = CAST(expression, new_label_address(parse_region, loc, label));

  use_label(label);

  result->type = ptr_void_type;
  result->cst = fold_label_address(result);

  if (pedantic)
    pedwarn("ANSI C forbids `&&'");

  return result;
}

void check_sizeof(expression result, type stype)
{
  if (type_function(stype))
    {
      if (pedantic || warn_pointer_arith)
	pedwarn("sizeof applied to a function type");
    }
  else if (type_void(stype))
    {
      if (pedantic || warn_pointer_arith)
	pedwarn("sizeof applied to a void type");
    }
  else if (type_incomplete(stype))
    error("sizeof applied to an incomplete type");

  result->type = size_t_type;
  result->cst = fold_sizeof(result, stype);
}

expression make_sizeof_expr(location loc, expression e)
{
  expression result = CAST(expression, new_sizeof_expr(parse_region, loc, e));
  check_sizeof(result, e->type);
  return result;
}

expression make_sizeof_type(location loc, asttype t)
{
  expression result = CAST(expression, new_sizeof_type(parse_region, loc, t));
  check_sizeof(result, t->type);
  return result;
}

expression make_alignof_expr(location loc, expression e)
{
  expression result = CAST(expression, new_alignof_expr(parse_region, loc, e));

  result->type = size_t_type;
  result->cst = fold_alignof(result, e->type);

  return result;
}

expression make_alignof_type(location loc, asttype t)
{
  expression result = CAST(expression, new_alignof_type(parse_region, loc, t));

  result->type = size_t_type;
  result->cst = fold_alignof(result, t->type);

  return result;
}

expression make_cast(location loc, asttype t, expression e)
{
  expression result = CAST(expression, new_cast(parse_region, loc, e, t));
  type castto = t->type;
  
  if (castto == error_type || type_void(castto))
    ; /* Do nothing */
  else if (type_array(castto))
    {
      error("cast specifies array type");
      castto = error_type;
    }
  else if (type_function(castto))
    {
      error("cast specifies function type");
      castto = error_type;
    }
  else if (type_equal_unqualified(castto, e->type))
    {
      if (pedantic && type_aggregate(castto))
	pedwarn("ANSI C forbids casting nonscalar to the same type");
    }
  else
    {
      type etype = e->type;

      /* Convert functions and arrays to pointers,
	 but don't convert any other types.  */
      if (type_function(etype) || type_array(etype))
	etype = default_conversion(e);

      if (type_union(castto))
	{
	  tag_declaration utag = type_tag(castto);
	  field_declaration ufield;

	  /* Look for etype as a field of the union */
	  for (ufield = utag->fieldlist; ufield; ufield = ufield->next)
	    if (ufield->name && type_equal_unqualified(ufield->type, etype))
	      {
		if (pedantic)
		  pedwarn("ANSI C forbids casts to union type");
		break;
	      }
	  if (!ufield)
	    error("cast to union type from type not present in union");
	}
      else 
	{
	  /* Optionally warn about potentially worrisome casts.  */

	  if (warn_cast_qual && type_pointer(etype) && type_pointer(castto))
	    {
	      type ep = type_points_to(etype), cp = type_points_to(castto);

	      if (type_volatile(ep) && !type_volatile(cp))
		pedwarn("cast discards `volatile' from pointer target type");
	      if (type_const(ep) && !type_const(cp))
		pedwarn("cast discards `const' from pointer target type");
	    }

	  /* This warning is weird */
	  if (warn_bad_function_cast && is_function_call(e) &&
	      !type_equal_unqualified(castto, etype))
	    warning ("cast does not match function type");

#if 0
	  /* Warn about possible alignment problems.  */
	  if (STRICT_ALIGNMENT && warn_cast_align
	      && TREE_CODE (type) == POINTER_TYPE
	      && TREE_CODE (otype) == POINTER_TYPE
	      && TREE_CODE (TREE_TYPE (otype)) != VOID_TYPE
	      && TREE_CODE (TREE_TYPE (otype)) != FUNCTION_TYPE
	      /* Don't warn about opaque types, where the actual alignment
		 restriction is unknown.  */
	      && !((TREE_CODE (TREE_TYPE (otype)) == UNION_TYPE
		    || TREE_CODE (TREE_TYPE (otype)) == RECORD_TYPE)
		   && TYPE_MODE (TREE_TYPE (otype)) == VOIDmode)
	      && TYPE_ALIGN (TREE_TYPE (type)) > TYPE_ALIGN (TREE_TYPE (otype)))
	    warning ("cast increases required alignment of target type");

	  if (TREE_CODE (type) == INTEGER_TYPE
	      && TREE_CODE (otype) == POINTER_TYPE
	      && TYPE_PRECISION (type) != TYPE_PRECISION (otype)
	      && !TREE_CONSTANT (value))
	    warning ("cast from pointer to integer of different size");

	  if (TREE_CODE (type) == POINTER_TYPE
	      && TREE_CODE (otype) == INTEGER_TYPE
	      && TYPE_PRECISION (type) != TYPE_PRECISION (otype)
#if 0
	      /* Don't warn about converting 0 to pointer,
		 provided the 0 was explicit--not cast or made by folding.  */
	      && !(TREE_CODE (value) == INTEGER_CST && integer_zerop (value))
#endif
	      /* Don't warn about converting any constant.  */
	      && !TREE_CONSTANT (value))
	    warning ("cast to pointer from integer of different size");
#endif

	  check_conversion(castto, etype);
	}
    }

  result->lvalue = !pedantic && e->lvalue;
  result->isregister = e->isregister;
  result->bitfield = e->bitfield;
  result->static_address = e->static_address;
  result->type = castto;
  result->cst = fold_cast(result);

  return result;
}

type pointer_int_sum(type ptype, type itype)
{
  type pointed = type_points_to(ptype);

  if (type_void(pointed))
    {
      if (pedantic || warn_pointer_arith)
	pedwarn("pointer of type `void *' used in arithmetic");
    }
  else if (type_function(pointed))
    {
      if (pedantic || warn_pointer_arith)
	pedwarn("pointer to a function used in arithmetic");
    }
  else if (type_incomplete(pointed))
    error("arithmetic on pointer to an incomplete type");

  return ptype;
}

bool valid_compare(type t1, type t2, expression e1)
{
  if (type_void(type_points_to(t1)))
    {
      if (pedantic && type_function(type_points_to(t2)) && !definite_null(e1))
	pedwarn("ANSI C forbids comparison of `void *' with function pointer");
      return TRUE;
    }
  return FALSE;
}

type check_binary(int binop, expression e1, expression e2)
{
  type t1 = default_conversion(e1), t2 = default_conversion(e2);
  type rtype = NULL;
  bool common = FALSE;

  /* XXX: Misc warnings (see build_binary_op) */
  if (t1 == error_type || t2 == error_type)
    rtype = error_type;
  else switch(binop)
    {
    case kind_plus:
      if (type_pointer(t1) && type_integer(t2))
	rtype = pointer_int_sum(t1, t2);
      else if (type_pointer(t2) && type_integer(t1))
	rtype = pointer_int_sum(t2, t1);
      else
	common = TRUE;
      break;

    case kind_minus: 
      if (type_pointer(t1) && type_integer(t2))
	rtype = pointer_int_sum(t1, t2);
      else if (type_pointer(t1) && type_pointer(t2) &&
	       compatible_pointer_types(t1, t2))
	rtype = ptrdiff_t_type;
      else
	common = TRUE;
      break;

    case kind_plus_assign: case kind_minus_assign:
      if (type_pointer(t1) && type_integer(t2))
	rtype = pointer_int_sum(t1, t2);
      else
	common = TRUE;
      break;

    case kind_times: case kind_divide:
    case kind_times_assign: case kind_divide_assign:
      common = TRUE;
      break;

    case kind_modulo: case kind_bitand: case kind_bitor: case kind_bitxor:
    case kind_lshift: case kind_rshift:
    case kind_modulo_assign: case kind_bitand_assign: case kind_bitor_assign:
    case kind_bitxor_assign: case kind_lshift_assign: case kind_rshift_assign:
      if (type_integer(t1) && type_integer(t2))
	rtype = common_type(t1, t2);
      break;

    case kind_leq: case kind_geq: case kind_lt: case kind_gt:
      rtype = int_type; /* Default to assuming success */
      if (type_real(t1) && type_real(t2))
	;
      else if (type_pointer(t1) && type_pointer(t2))
	{
	  if (compatible_pointer_types(t1, t2))
	    {
	      /* XXX: how can this happen ? */
	      if (type_incomplete(t1) != type_incomplete(t2))
		pedwarn("comparison of complete and incomplete pointers");
	      else if (pedantic && type_function(type_points_to(t1)))
		pedwarn("ANSI C forbids ordered comparisons of pointers to functions");
	    }
	  else
	    pedwarn("comparison of distinct pointer types lacks a cast");
	}
      /* XXX: Use of definite_zero may lead to extra warnings when !extra_warnings */
      else if ((type_pointer(t1) && definite_zero(e2)) ||
	       (type_pointer(t2) && definite_zero(e1)))
	{
	  if (pedantic || extra_warnings)
	    pedwarn("ordered comparison of pointer with integer zero");
	}
      else if ((type_pointer(t1) && type_integer(t2)) ||
	       (type_pointer(t2) && type_integer(t1)))
	{
	  if (!flag_traditional)
	    pedwarn("comparison between pointer and integer");
	}
      else
	rtype = NULL; /* Force error */
      break;

    case kind_eq: case kind_ne:
      rtype = int_type; /* Default to assuming success */
      if (type_arithmetic(t1) && type_arithmetic(t2))
	;
      else if (type_pointer(t1) && type_pointer(t2))
	{
	  if (!compatible_pointer_types(t1, t2) &&
	      !valid_compare(t1, t2, e1) &&
	      !valid_compare(t2, t1, e2))
	    pedwarn("comparison of distinct pointer types lacks a cast");
	}
      else if ((type_pointer(t1) && definite_null(e2)) ||
	       (type_pointer(t2) && definite_null(e1)))
	;
      else if ((type_pointer(t1) && type_integer(t2)) ||
	       (type_pointer(t2) && type_integer(t1)))
	{
	  if (!flag_traditional)
	    pedwarn("comparison between pointer and integer");
	}
      else
	rtype = NULL; /* Force error */
      break;

    case kind_andand: case kind_oror:
      if (type_scalar(t1) && type_scalar(t2))
	rtype = int_type;
      break;

    default: assert(0); break;
    }

  if (common && type_arithmetic(t1) && type_arithmetic(t2))
    rtype = common_type(t1, t2);

  if (!rtype)
    {
      error("invalid operands to binary %s", binary_op_name(binop));
      rtype = error_type;
    }

  return rtype;
}

expression make_binary(location loc, int binop, expression e1, expression e2)
{
  expression result = CAST(expression, newkind_binary(parse_region, binop, loc, e1, e2));

  result->type = check_binary(binop, e1, e2);
  if (result->type != error_type)
    {
      result->cst = fold_binary(result->type, result);
    }

  /* XXX: The warn_parentheses stuff (and a <= b <= c) */
#if 0
  unsigned_conversion_warning (result, arg1);
  unsigned_conversion_warning (result, arg2);
  overflow_warning (result);
#endif

  return result;
}

static bool voidstar_conditional(type t1, type t2)
{
  if (type_void(t1))
    {
      if (pedantic && type_function(t2))
	pedwarn("ANSI C forbids conditional expr between `void *' and function pointer");
      return TRUE;
    }
  return FALSE;
}

static bool pointerint_conditional(type t1, type t2, expression e2)
{
  if (type_pointer(t1) && type_integer(t2))
    {
      if (!definite_zero(e2))
	pedwarn("pointer/integer type mismatch in conditional expression");
      return TRUE;
    }
  return FALSE;
}

expression make_conditional(location loc, expression cond,
			    expression true, expression false)
{
  expression result =
    CAST(expression, new_conditional(parse_region, loc, cond, true, false));
  type ctype, ttype, ftype, rtype = NULL;
  bool truelvalue = true ? true->lvalue : FALSE;

  ctype = default_conversion(cond);

  if (!true)
    {
      true = cond;
      truelvalue = FALSE; /* Not an lvalue in gcc ! */
    }

  if (type_void(true->type))
    ttype = true->type;
  else
    ttype = default_conversion(true);
  
  if (type_void(false->type))
    ftype = false->type;
  else
    ftype = default_conversion(false);

  if (ctype == error_type || ttype == error_type || ftype == error_type)
    rtype = error_type;
  else if (type_equal(ttype, ftype))
    rtype = ttype;
  else if (type_equal_unqualified(ttype, ftype))
    rtype = make_qualified_type(ttype, no_qualifiers);
  else if (type_real(ttype) && type_real(ftype))
    /* This should probably be type_arithmetic. See complex3.c/C9X */
    rtype = common_type(ttype, ftype);
  else if (type_void(ttype) || type_void(ftype))
    {
      if (pedantic && (!type_void(ttype) || !type_void(ftype)))
	pedwarn("ANSI C forbids conditional expr with only one void side");
      rtype = void_type;
    }
  else if (type_pointer(ttype) && type_pointer(ftype))
    {
      type tpointsto = type_points_to(ttype), fpointsto = type_points_to(ftype);

      if (compatible_pointer_types(ttype, ftype))
	rtype = common_type(tpointsto, fpointsto);
      else if (definite_null(true) && type_void(tpointsto))
	rtype = fpointsto;
      else if (definite_null(false) && type_void(fpointsto))
	rtype = tpointsto;
      else if (voidstar_conditional(tpointsto, fpointsto))
	rtype = tpointsto; /* void * result */
      else if (voidstar_conditional(fpointsto, tpointsto))
	rtype = fpointsto; /* void * result */
      else
	{
	  pedwarn("pointer type mismatch in conditional expression");
	  /* Slight difference from GCC: I qualify the result type with
	     the appropriate qualifiers */
	  rtype = void_type;
	}

      /* Qualifiers depend on both types */
      rtype = make_pointer_type(qualify_type2(rtype, tpointsto, fpointsto));
    }
  else if (pointerint_conditional(ttype, ftype, false))
    rtype = ttype;
  else if (pointerint_conditional(ftype, ttype, true))
    rtype = ftype;
  else if (flag_cond_mismatch)
    rtype = void_type;
  else
    {
      error("type mismatch in conditional expression");
      rtype = error_type;
    }
  
  /* Qualifiers depend on both types */
  if (rtype != error_type)
    rtype = qualify_type2(rtype, ttype, ftype);

  result->type = rtype;
  result->lvalue = !pedantic && truelvalue && false->lvalue;
  result->isregister = true->isregister || false->isregister;
  result->bitfield = true->bitfield || false->bitfield;
  result->cst = fold_conditional(result);

  return result;
}

expression make_assign(location loc, int binop, expression e1, expression e2)
{
  expression result = CAST(expression, newkind_binary(parse_region, binop,
						      loc, e1, e2));
  type t1 = require_complete_type(e1, e1->type), t2;

  result->type = error_type;
  if (t1 != error_type && e2->type != error_type)
    {
      expression rhs;

      if (binop == kind_assign)
	{
	  t2 = default_conversion_for_assignment(e2);
	  rhs = e2;
	}
      else
	{
	  t2 = check_binary(binop, e1, e2);
	  rhs = NULL;
	}

      if (check_writable_lvalue(e1, "assignment") &&
	  check_assignment(e1->type, t2, rhs, "assignment", NULL, NULL, 0))
	result->type = make_qualified_type(e1->type, no_qualifiers);
    }

  return result;
}

expression make_identifier(location loc, cstring id, bool maybe_implicit)
{
  /* XXX: Should pass decl as argument (lexer looked it up already) */
  data_declaration decl = lookup_id(id.data, FALSE);
  identifier result = new_identifier(parse_region, loc, id, NULL);

  if (decl && decl->islimbo) /* Limbo declarations don't really exist */
    decl = NULL;

  if (!decl && maybe_implicit) /* An implicit function declaration */
    decl = implicitly_declare(result);

  if (!decl)
    {
      if (!current_function_decl)
	error("`%s' undeclared here (not in a function)", id.data);
      else if (!env_lookup(current_function_decl->undeclared_variables, id.data, FALSE))
	{
	  static bool undeclared_variable_notice;

	  error("`%s' undeclared (first use in this function)", id.data);
	  env_add(current_function_decl->undeclared_variables, id.data, (void *)1);
	  if (!undeclared_variable_notice)
	    {
	      error("(Each undeclared identifier is reported only once");
	      error("for each function it appears in.)");
	      undeclared_variable_notice = TRUE;
	    }
	}
      decl = bad_decl;
    }

  /* XXX: check for register variables of containing function */
  result->type = decl->type;
  result->lvalue = decl->kind == decl_variable ||
    decl->kind == decl_magic_string;
  result->cst = fold_identifier(CAST(expression, result), decl);
  result->isregister = decl->kind == decl_variable &&
    decl->vtype == variable_register;
  result->static_address = foldaddress_identifier(CAST(expression, result), decl);
  result->ddecl = decl;

  decl->isused = TRUE;

  return CAST(expression, result);
}

expression make_compound_expr(location loc, statement block)
{
  if (is_error_stmt(block))
    return make_error_expr(loc);
  else
    {
      expression result = CAST(expression, new_compound_expr(parse_region, loc, block));
      compound_stmt bs = CAST(compound_stmt, block);
      statement last_stmt = CAST(statement, last_node(CAST(node, bs->stmts)));

      if (last_stmt && is_expression_stmt(last_stmt))
	result->type = CAST(expression_stmt, last_stmt)->arg1->type;
      else
	result->type = void_type;

      return result;
    }
}

static void check_arguments(type fntype, expression arglist,
			    data_declaration fundecl, const char *name)
{
  typelist_scanner parmtypes;
  int parmnum = 1;
  type parmtype;

  /* XXX: pseudo-protos for old style arglists */
  if (!type_function_oldstyle(fntype))
    {
      typelist_scan(type_function_arguments(fntype), &parmtypes);

      while ((parmtype = typelist_next(&parmtypes)) && arglist)
	{
	  type argtype = arglist->type;

	  if (type_incomplete(parmtype))
	    error("type of formal parameter %d is incomplete", parmnum);
	  else
	    {
	      if (warn_conversion)
		{
		  if (type_integer(parmtype) && type_floating(argtype))
		    warn_for_assignment("%s as integer rather than floating due to prototype",
					NULL, name, parmnum);
		  else if (type_floating(parmtype) && type_integer(argtype))
		    warn_for_assignment ("%s as floating rather than integer due to prototype",
					 NULL, name, parmnum);
		  else if (type_complex(parmtype) && type_floating(argtype))
		    warn_for_assignment ("%s as complex rather than floating due to prototype",
					NULL, name, parmnum);
		  else if (type_floating(parmtype) && type_complex(argtype))
		    warn_for_assignment ("%s as floating rather than complex due to prototype",
					NULL, name, parmnum);
		  /* Warn if any argument is passed as `float',
		     since without a prototype it would be `double'.  */
		  else if (type_float(parmtype) && type_floating(argtype))
		    warn_for_assignment ("%s as `float' rather than `double' due to prototype",
					NULL, name, parmnum);
#if 0
		  else
		    {
		      /* Type that would have been passed w/o proto */
		      type type1 = default_conversion(arglist);

		      /* No warning if function asks for enum
			 and the actual arg is that enum type.  */
		      if (type_enum(parmtype) && type_equal_unqualified(parmtype, argtype))
			;
		      /* XXX: else messy stuff that cannot easily be done w/o constant
			 folding and type size info */
		    }
#endif
		}
	      check_assignment(parmtype, default_conversion_for_assignment(arglist),
			       arglist, NULL, fundecl, name, parmnum);
	    }
	  parmnum++;
	  arglist = CAST(expression, arglist->next);
	}
      if (parmtype)
	{
	  if (name)
	    error("too few arguments to function `%s'", name);
	  else
	    error("too few arguments to function");
	}
      else if (arglist && !type_function_varargs(fntype))
	{
	  if (name)
	    error("too many arguments to function `%s'", name);
	  else
	    error("too many arguments to function");
	}
    }

  /* Checks for arguments with no corresponding argument type */
  while (arglist)
    {
      require_complete_type(arglist, default_conversion(arglist));
      arglist = CAST(expression, arglist->next);
    }
}

expression make_function_call(location loc, expression fn, expression arglist)
{
  expression result = CAST(expression, new_function_call(parse_region, loc, fn, arglist, NULL));
  type fntype = default_conversion(fn), rettype;

  result->type = error_type;
  if (fntype == error_type)
    ;
  else if (!(type_pointer(fntype) && type_function(type_points_to(fntype))))
    error("called object is not a function");
  else
    {
      char *funname = NULL;
      data_declaration fundecl = NULL;

      if (is_identifier(fn))
	{
	  identifier fnid = CAST(identifier, fn);

	  if (fnid->ddecl->kind == decl_function)
	    {
	      funname = fnid->cstring.data;
	      fundecl = fnid->ddecl;
	    }
	}

      fntype = type_points_to(fntype);

      check_arguments(fntype, arglist, fundecl, funname);

      rettype = type_function_return_type(fntype);
      result->type = rettype;
      if (!type_void(rettype))
	result->type = require_complete_type(result, rettype);

      /*
       if (type_deletes(fntype) &&
         (!current_function_decl ||
          !type_deletes(current_function_decl->ddecl->type)))
       error("unsafe call to a function that may delete a region");
      */
       /*       result = rc_check_function_call(CAST(function_call, result));*/
   }

  return result;
}

expression make_va_arg(location loc, expression arg, asttype type)
{
  expression va_arg_id = build_identifier(parse_region, loc, builtin_va_arg_decl);
  expression result = CAST(expression, new_function_call(parse_region, loc, va_arg_id, arg, type));

  if (!type_equal_unqualified(arg->type, builtin_va_list_type))
    error("first argument to `va_arg' not of type `va_list'");

  if (!type_self_promoting(type->type))
    {
      static bool gave_help;

      error("char, short and float are automatically promoted when passed through `...'");
      if (!gave_help)
	{
	  gave_help = TRUE;
	  error("(so you should pass `int', `unsigned' or `double' to `va_arg')");
	}
    }
  result->type = type->type;

  return result;
}

expression make_array_ref(location loc, expression array, expression index)
{
  expression result = CAST(expression, new_array_ref(parse_region, loc, array, index));
  type atype, itype = default_conversion(index);

  if (warn_char_subscripts && type_char(index->type))
    warning("subscript has type `char'");

  if (type_array(array->type) && !array->lvalue)
    {
      /* Some special GCC extensions */
      /* XXX: Ignoring the weird register stuff, going for a simple version
	 which seems essentially identical for our purposes */
      if (pedantic)
	pedwarn("ANSI C forbids subscripting non-lvalue array");
      atype = make_pointer_type(type_array_of(array->type));

       /* this should not be possible (non-lvalue arrays come from
	  array fields of non-lvalue struct expressions) */
      assert(!array->static_address);
    }
  else
    atype = default_conversion(array);

  /* Put the integer in ITYPE to simplify error checking.  */
  if (type_integer(atype))
    {
      type temp = atype;
      atype = itype; 
      itype = temp;
    }

  if (!type_pointer(atype) || type_function(type_points_to(atype)))
    {
      error("subscripted value is neither array nor pointer");
      result->type = error_type;
      result->lvalue = TRUE;
    }
  else
    {
      check_dereference(result, atype, "array indexing");
      result->static_address = fold_binary(atype, result);
    }

  if (!type_integer(itype))
    error("array subscript is not an integer");

  return result;
}

expression make_field_ref(location loc, expression object, cstring field)
{
  type otype = object->type;
  expression result = CAST(expression, new_field_ref(parse_region, loc, object, field));

  result->type = error_type;

  if (type_aggregate(otype))
    {
      tag_declaration tag = type_tag(otype);

      if (!tag->defined)
	incomplete_type_error(NULL, otype);
      else
	{
	  field_declaration fdecl = env_lookup(tag->fields, field.data, FALSE);

	  if (!fdecl)
	    error(type_struct(otype) ? "structure has no member named `%s'"
		  : "union has no member named `%s'", field.data);
	  else
	    {
	      result->type = qualify_type2(fdecl->type, fdecl->type, object->type);
	      result->bitfield = fdecl->bitwidth >= 0;
	      result->static_address = foldaddress_field_ref(object->static_address, fdecl);

	    }
	}
    }
  else if (otype != error_type)
    error("request for member `%s' in something not a structure or union",
	  field.data);

  result->lvalue = object->lvalue;

  return result;
}

static expression increment(unary result, char *name)
{
  expression e = result->arg1;
  type etype = e->type;

  result->type = error_type;

  if (!type_scalar(etype))
    error("wrong type argument to %s", name);
  else 
    {
      if (type_incomplete(etype))
	error("%s of pointer to unknown structure or union", name);
      else if (type_pointer(etype) && (pedantic || warn_pointer_arith) &&
	       (type_void(type_points_to(etype)) ||
		type_function(type_points_to(etype))))
	pedwarn("wrong type argument to %s", name);

      if (check_writable_lvalue(e, name))
	result->type = etype;
    }
  return CAST(expression, result);
}

expression make_postincrement(location loc, expression e)
{
  return increment(CAST(unary, new_postincrement(parse_region, loc, e)),
		   "increment");
}

expression make_preincrement(location loc, expression e)
{
  return increment(CAST(unary, new_preincrement(parse_region, loc, e)),
			"increment");
}

expression make_postdecrement(location loc, expression e)
{
  return increment(CAST(unary, new_postdecrement(parse_region, loc, e)),
		   "decrement");
}

expression make_predecrement(location loc, expression e)
{
  return increment(CAST(unary, new_predecrement(parse_region, loc, e)),
		   "decrement");
}

static size_t extract_strings(expression string_components,
			      wchar_t *into, bool *wide)
{
  size_t total_length = 0;
  expression astring;
  
  *wide = FALSE;
  scan_expression (astring, string_components)
    {
      const wchar_t *chars;
      size_t length;

      if (!type_equal(type_array_of(astring->type), char_type))
	*wide = TRUE;

      if (is_identifier(astring))
	{
	  data_declaration dd = CAST(identifier, astring)->ddecl;

	  chars = dd->chars;
	  length = dd->chars_length;
	}
      else
	{
	  string_cst s = CAST(string_cst, astring);

	  chars = s->chars;
	  length = s->length;
	}
      if (into)
	{
	  memcpy(into, chars, length * sizeof(wchar_t));
	  into += length;
	}

      total_length += length;
    }

  return total_length;
}

string make_string(location loc, expression string_components)
{
  string s = new_string(parse_region, loc, string_components, NULL);
  size_t total_length = 0;
  bool wide;

  total_length = extract_strings(string_components, NULL, &wide);
  s->ddecl = declare_string(NULL, wide, total_length);
  s->type = s->ddecl->type;
  extract_strings(string_components, (wchar_t *)s->ddecl->chars, &wide);
  s->static_address = foldaddress_string(s);
  s->lvalue = TRUE;

  return s;
}
