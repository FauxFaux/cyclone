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
#include "semantics.h"
#include "input.h"
#include "flags.h"
#include "c-parse.h"
#include "env.h"
#include "expr.h"
#include "stmt.h"
#include "AST_utils.h"
#include "constants.h"

/* Predefined __builtin_va_list type */
type builtin_va_list_type;
data_declaration builtin_va_arg_decl;

/* The function currently being defined */
function_decl current_function_decl;

data_declaration bad_decl;

static type implicit_function_type;

static environment current_env;
environment global_env;

static tag_ref pending_invalid_xref;

static known_cst onecst, zerocst;
static expression oneexpr;

environment new_environment(region r, environment parent, bool parm_level)
{
  environment env = ralloc(r, struct environment);

#if 0
  env->fdecl = NULL;
#endif
  env->parent = parent;
  env->parm_level = parm_level;
  if (parent)
    {
      env->fdecl = parent->fdecl;
      env->id_env = new_env(r, parent->id_env);
      /* ANSI C is weird */
      if (parent->parm_level)
	env->tag_env = parent->tag_env;
      else
	env->tag_env = new_env(r, parent->tag_env);
    }
  else
    {
      env->id_env = new_env(r, NULL);
      env->tag_env = new_env(r, NULL);
    }

  return env;
}

void init_data_declaration(data_declaration dd, declaration ast,
			   const char *name, type t)
{
  dd->kind = 0;
  dd->name = name;
  dd->type = t;

  dd->shadowed = NULL;
  dd->ast = ast;
  dd->definition = NULL;
  dd->isexternalscope = FALSE;
  dd->isfilescoperef = FALSE;
  dd->needsmemory = FALSE;
  dd->isused = FALSE;
  dd->in_system_header = ast->location.in_system_header;
  dd->ftype = 0;
  dd->isinline = FALSE;
  dd->isexterninline = FALSE;
  dd->oldstyle_args = NULL;
  dd->vtype = 0;
  dd->islocal = FALSE;
  dd->isparameter = FALSE;
  dd->islimbo = FALSE;
  dd->value = NULL;
  dd->id = -1;
  dd->addressed = FALSE;
  dd->istemp = FALSE;
  dd->interesting = FALSE;
  dd->sr_uses = 0;
  dd->chars = NULL;
  dd->chars_length = 0;
}

data_declaration lookup_id(const char *s, bool this_level_only)
{
  return env_lookup(current_env->id_env, s, this_level_only);
}

data_declaration lookup_global_id(const char *s)
{
  return env_lookup(global_env->id_env, s, TRUE);
}

data_declaration declare(environment b, data_declaration from,
			 bool ignore_shadow)
{
  data_declaration dd = ralloc(parse_region, struct data_declaration);

  *dd = *from;

  if (dd->name)
    {
      /* In PCC-compatibility mode, extern decls of vars with no current decl
	 take effect at top level no matter where they are.  */
      /* We don't do the GCC "type exists at global scope" check because
	 it's mostly meaningless.
	 XXX: review if I start freeing mem earlier */
      if (flag_traditional && dd->isfilescoperef && !lookup_id(dd->name, FALSE))
	b = global_env;

      if (b != global_env)
	{
	  /* Here to install a non-global value.  */
	  data_declaration shadowed = lookup_id(dd->name, FALSE);

	  /* Warn if shadowing an argument at the top level of the body.  */
	  if (shadowed && shadowed->islocal && !dd->isfilescoperef
	      /* This warning doesn't apply to the parms of a nested fcn.  */
	      && !b->parm_level
	      /* Check that this is one level down from the parms.  */
	      && b->parent->parm_level
	      /* Check that the decl exists in the parm level */
	      && env_lookup(b->parent->id_env, dd->name, TRUE))
	    {
	      if (shadowed->isparameter)
		pedwarn("declaration of `%s' shadows a parameter", dd->name);
	      else
		pedwarn("declaration of `%s' shadows a symbol from the parameter list",
			 dd->name);
	    }
	  /* Maybe warn if shadowing something else.  */
	  else if (warn_shadow && !ignore_shadow && !dd->isfilescoperef)
	    {
	      char *warnstring = 0;

	      if (dd->isparameter
		  && b->parent->parm_level)
		/* Don't warn about the parm names in function declarator
		   within a function declarator.
		   It would be nice to avoid warning in any function
		   declarator in a declaration, as opposed to a definition,
		   but there is no way to tell it's not a definition.  */
		;
	      else if (shadowed && shadowed->isparameter)
		warnstring = "declaration of `%s' shadows a parameter";
	      else if (shadowed && shadowed->islocal)
		warnstring = "declaration of `%s' shadows previous local";
	      else if (shadowed)
		warnstring = "declaration of `%s' shadows global declaration";

	      if (warnstring)
		(error_shadow ? error : warning)(warnstring, dd->name);
	    }
	}

      /* Add a limbo version of external identifiers to the global scope
	 if this is the first time this external symbol is seen */
      if (dd->isfilescoperef && b != global_env && !lookup_global_id(dd->name))
	{
	  data_declaration limbodd = ralloc(parse_region,
					    struct data_declaration);

	  *limbodd = *dd;
	  limbodd->islimbo = TRUE;
	  env_add(global_env->id_env, limbodd->name, limbodd);
	  assert(!dd->shadowed);
	  dd->shadowed = limbodd;

	  if (dd->kind == decl_function && dd->ftype == function_implicit &&
	      mesg_implicit_function_declaration)
	    warning_or_error(mesg_implicit_function_declaration != 2,
			     "implicit declaration of function `%s'", dd->name);
	}
      /* Warn of implicit decl following explicit local extern decl.
	 This is probably a program designed for traditional C.  */
      else if (dd->kind == decl_function && dd->ftype == function_implicit &&
	       warn_traditional)
	warning("function `%s' was previously declared within a block",
		dd->name);

      if (warn_main && (b == global_env || dd->isfilescoperef) &&
	  dd->kind != decl_function && !strcmp(dd->name, "main"))
	warning_with_decl(dd->ast, "`main' is usually a function");

      /* Parameters are declared before the function_decl is created
	 (and may be declared in several places). The id's of parameters
	 are set in start_function */
      if (dd->kind == decl_variable && dd->islocal && !dd->isparameter)
	dd->id = b->fdecl->nlocals++;

   }
  assert(!dd->islimbo);
  env_add(b->id_env, dd->name, dd);

  return dd;
}

tag_declaration declare_tag_env(environment env, tag_ref t)
{
  tag_declaration tdecl = ralloc(parse_region, struct tag_declaration);
  const char *name = t->word1 ? t->word1->cstring.data : NULL;

  tdecl->kind = t->kind;
  tdecl->name = name;
  tdecl->ast = t;
#if 0
  tdecl->fields = NULL;
  tdecl->fieldlist = NULL;
  tdecl->defined = tdecl->being_defined = FALSE;
  tdecl->fields_const = tdecl->fields_volatile = FALSE;
  tdecl->transparent_union = FALSE;
  tdecl->size = tdecl->alignment = 0;
  tdecl->size_cc = FALSE;
#endif

  if (name)
    {
      tdecl->shadowed = env_lookup(env->tag_env, name, FALSE);
      if (tdecl->shadowed && warn_shadow)
	(error_shadow ? error : warning)
	  ("tag %s shadows enclosing struct/union/enum", name);
    }
  else
    tdecl->shadowed = NULL;

  /* We register all tags in the environment, even unnamed ones. */
  env_add(env->tag_env, name, tdecl);

  return tdecl;
}

tag_declaration lookup_tag_env(environment env, tag_ref t, bool this_level_only)
{
  tag_declaration found =
    env_lookup(env->tag_env, t->word1->cstring.data, this_level_only);

  /* Check if wrong kind */
  if (found && found->kind != t->kind)
    pending_invalid_xref = t;

  return found;
}

tag_declaration declare_tag(tag_ref t)
{
  return declare_tag_env(current_env, t);
}

tag_declaration lookup_tag(tag_ref t, bool this_level_only)
{
  return lookup_tag_env(current_env, t, this_level_only);
}

tag_declaration declare_global_tag(tag_ref t)
{
  return declare_tag_env(global_env, t);
}

tag_declaration lookup_global_tag(tag_ref t)
{
  return lookup_tag_env(global_env, t, TRUE);
}

/* If elements is 'struct foo' shadow tag foo in the current environment */
void shadow_tag(type_element elements)
{
  shadow_tag_warned(elements, 0);
}

/* Like shadow_tag, but warned is: 1 => we have done a pedwarn;
   2 => we have done a warning, but no pedwarn. */
void shadow_tag_warned(type_element elements, int warned)
{
  type_element elem;
  int found_tag = 0;

  pending_invalid_xref = 0;

  scan_type_element (elem, elements)
    {
      ast_kind code = elem->kind;

      if (code == kind_struct_ref || code == kind_union_ref ||
	  code == kind_enum_ref)
	{
	  tag_ref tag = CAST(tag_ref, elem);
	  word name = tag->word1;

	  found_tag++;

	  if (name == 0)
	    {
	      if (warned != 1 && code != kind_enum_ref)
		/* Empty unnamed enum OK */
		{
		  pedwarn ("unnamed struct/union that defines no instances");
		  warned = 1;
		}
	    }
	  else
	    {
	      void *tagdecl = lookup_tag(tag, TRUE);

	      if (tagdecl == 0)
		declare_tag(tag);
	      else
		pending_xref_error();
	    }
	}
      else
	{
	  if (!warned && ! in_system_header)
	    {
	      warning("useless keyword or type name in empty declaration");
	      warned = 2;
	    }
	}
    }

  if (found_tag > 1)
    error("two types specified in one empty declaration");

  if (warned != 1)
    {
      if (found_tag == 0)
	pedwarn("empty declaration");
    }
}

/* Print an error message now
   for a recent invalid struct, union or enum cross reference.
   We don't print them immediately because they are not invalid
   when used in the `struct foo;' construct for shadowing.  */

void pending_xref_error(void)
{
  if (pending_invalid_xref != 0)
    {
      error_with_location(pending_invalid_xref->location,
			  "`%s' defined as wrong kind of tag",
			  pending_invalid_xref->word1->cstring.data);
      pending_invalid_xref = 0;
    }
}

/* True if the parameters is just the parameter list '(void)' */
bool is_void_parms(declaration parms)
{
  data_decl dd;
  variable_decl vd;

  if (!parms || parms->next || !is_data_decl(parms))
    return FALSE;

  dd = CAST(data_decl, parms);
  vd = CAST(variable_decl, dd->decls);
  assert(!vd->next);

  return !vd->declarator && dd->modifiers && !dd->modifiers->next &&
    is_rid(dd->modifiers) && CAST(rid, dd->modifiers)->id == RID_VOID;
}

const char *tagkind_name(int tagkind)
{
  switch (tagkind)
    {
    case kind_struct_ref: return "struct";
    case kind_union_ref: return "union";
    case kind_enum_ref: return "enum";
    default: assert(0); return NULL;
    }
}

/* At end of parameter list, warn about any struct, union or enum tags
   defined within.  Do so because these types cannot ever become complete.  */
static void parmlist_tags_warning(environment parm_env)
{
  env_scanner scan_tags;
  const char *tagname;
  tag_declaration tagdecl;
  static bool already = FALSE;

  env_scan(parm_env->tag_env, &scan_tags);

  while (env_next(&scan_tags, &tagname, (void **)&tagdecl))
    {
      int kind = tagdecl->kind;
      const char *kindname = tagkind_name(kind);

      /* An anonymous union parm type is meaningful as a GNU extension.
	 So don't warn for that.  */
      if (kind == kind_union_ref && !tagname && !pedantic)
	continue;

      if (tagname)
	warning("`%s %s' declared inside parameter list", kindname,
		tagname);
      else
	warning("anonymous %s declared inside parameter list", kindname);

      if (!already)
	{
	  warning("its scope is only this definition or declaration,");
	  warning("which is probably not what you want.");
	  already = TRUE;
	}
    }
}

typelist make_arg_types(bool definition, declaration parameters, bool *varargs)
{
  declaration parameter;
  typelist arg_types = new_typelist(parse_region);

  *varargs = FALSE;
  if (!is_void_parms(parameters))
    scan_declaration (parameter, parameters)
      if (is_ellipsis_decl(parameter))
	*varargs = TRUE;
      else if (is_error_decl(parameter))
	{
	  /* Make an "accept everything" signature for arg lists with
	     error_decls */
	  *varargs = TRUE;
	  arg_types = new_typelist(parse_region);
	  typelist_append(arg_types, error_type);

	  return arg_types;
	}
      else
	{
	  data_decl dp = CAST(data_decl, parameter);
	  variable_decl vp = CAST(variable_decl, dp->decls);

	  assert(!vp->next);
	  if (!vp->ddecl->name && definition)
	    error_with_decl(dp->decls, "parameter name omitted");
	  typelist_append(arg_types, vp->ddecl->type);
	}

  return arg_types;
}

bool new_style(declaration parameters)
{
  return parameters && !is_oldidentifier_decl(parameters);
}

static void check_duplicate_rid(int specbits, rid rspec)
{
  if (specbits & 1 << rspec->id)
    pedwarn_with_location(rspec->location, "duplicate `%s'", rid_name(rspec));
}

static void check_duplicate_qualifiers1(location l, type_quals new1, type_quals old)
{
  if (old & new1)
    pedwarn_with_location(l, "duplicate `%s'", qualifier_name(new1));
}

static void check_duplicate_qualifiers(location l, type_quals new, type_quals old)
{
  type_quals qual;

  for (qual = 1; qual < last_qualifier; qual <<= 1)
    if (new & qual)
      check_duplicate_qualifiers1(l, qual, old);
}

static void check_legal_qualifiers(location l, type_quals quals)
{
  int count = 0;

  count += (quals & sameregion_qualifier) != 0;
  count += (quals & parentptr_qualifier) != 0;
  count += (quals & traditional_qualifier) != 0;

  if (count > 1)
    {
      error_with_location(l, "must specify at most one of sameregion/parentptr/traditional qualifiers");
    }
}

static type_quals parse_qualifiers(location l, type_element qlist)
{
  qualifier q;
  type_quals tqs = no_qualifiers;

  /* Actually qlist is a list of qualifier */
  scan_qualifier (q, CAST(qualifier, qlist))
    {
      check_duplicate_qualifiers1(q->location, q->id, tqs);
      tqs |= q->id;
    }
  check_legal_qualifiers(l, tqs);
  return tqs;
}

void parse_declarator(type_element modifiers, declarator d, bool bitfield, 
		      int *oclass, bool *oinlinep, const char **oname,
		      type *ot, bool *owarn_defaulted_int,
		      function_declarator *ofunction_declarator)
{
  location loc = d ? d->location : modifiers->location;
  int specbits = 0, nclasses = 0;
  type_quals specquals = no_qualifiers;
  bool longlong = FALSE;
  const char *printname;
  type_element spec;
  type t = NULL;
  bool modified;

  *owarn_defaulted_int = FALSE;

  /* We get the name now so that we have a name for error messages */
  *oname = declarator_name(d);
  printname = *oname ? *oname : "type name";

  *oclass = 0;

  scan_type_element (spec, modifiers)
    {
      type newtype = NULL;

      switch (spec->kind)
	{
	case kind_rid:
	  {
	    rid rspec = CAST(rid, spec);
	    int id = rspec->id;

	    switch (id)
	      {
	      case RID_INT: newtype = int_type; break;
	      case RID_CHAR: newtype = char_type; break;
	      case RID_FLOAT: newtype = float_type; break;
	      case RID_DOUBLE: newtype = double_type; break;
	      case RID_VOID: newtype = void_type; break;
		
	      case RID_AUTO: case RID_STATIC: case RID_EXTERN:
	      case RID_REGISTER: case RID_TYPEDEF:
		*oclass = id;
		nclasses++;
		break;

	      case RID_LONG: /* long long detection */
		if (specbits & 1 << RID_LONG) 
		  {
		    if (longlong)
		      error_with_location(spec->location,
					  "`long long long' is too long for GCC");
		    else
		      {
			if (pedantic && !in_system_header)
			  pedwarn_with_location(spec->location,
						"ANSI C does not support `long long'");
			longlong = TRUE;
		      }
		    break;
		  }
		/* Fall through */
	      default:
		check_duplicate_rid(specbits, rspec);
		break;
	      }
	    specbits |= 1 << id;
	    break;
	  }
	case kind_qualifier:
	  {
	    qualifier q = CAST(qualifier, spec);
	    int id = q->id;

	    check_duplicate_qualifiers1(loc, id, specquals);
	    specquals |= id;
	    break;
	  }
	case kind_typename:
	  newtype = CAST(typename, spec)->ddecl->type;
	  break;
	case kind_typeof_type:
	  newtype = CAST(typeof_type, spec)->asttype->type;
	  break;
	case kind_typeof_expr:
	  newtype = CAST(typeof_expr, spec)->arg1->type;
	  break;
	case kind_struct_ref: case kind_union_ref: case kind_enum_ref:
	  newtype = make_tagged_type(CAST(tag_ref, spec)->tdecl);
	  break;
	default: assert(0); break;
	}

      if (newtype)
	{
	  if (t)
	    error_with_location(spec->location,
	       "two or more data types in declaration of `%s'", printname);
	  else
	    t = newtype;
	}
    }

  /* Long double is a special combination.  */
  if ((specbits & 1 << RID_LONG) && !longlong && (specbits & 1 << RID_DOUBLE))
    {
      specbits &= ~(1 << RID_LONG);
      t = long_double_type;
    }

  modified = !!(specbits & (1 << RID_LONG | 1 << RID_SHORT |
			    1 << RID_SIGNED | 1 << RID_UNSIGNED));

  /* No type at all: default to `int', or `double' (if complex specified
     and no long/short/signed/unsigned) */
  if (!t)
    {
      if (specbits & 1 << RID_COMPLEX)
	{
	  if (!modified)
	    {
	      specbits |= 1 << RID_DOUBLE;
	      t = double_type;
	    }
	  else
	    {
	      specbits |= 1 << RID_INT;
	      t = int_type;
	    }
	}
      else
	{
	  /* Defer defaulted int warning to caller (msg depends on context) */
	  if (!modified)
	    *owarn_defaulted_int = TRUE;

	  specbits |= 1 << RID_INT;
	  t = int_type;
	}
    }

  if (nclasses > 1)
    error_with_location(loc, "multiple storage classes in declaration of `%s'", printname);

  /* Now process the modifiers that were specified
     and check for invalid combinations.  */

  /* Check all other uses of type modifiers.  */
  if (modified)
    {
      int ok = 0;

      if ((specbits & 1 << RID_LONG) && (specbits & 1 << RID_SHORT))
	error_with_location(loc, "both long and short specified for `%s'", printname);
      else if ((specbits & 1 << RID_SHORT) && !(specbits & 1 << RID_INT))
	{
	  static int already = 0;

	  error_with_location(loc, "short invalid for `%s'", printname);
	  if (!already && !pedantic)
	    {
	      error_with_location(loc, "short is only valid with int");
	      already = 1;
	    }
	}
      else if ((specbits & 1 << RID_LONG) && !(specbits & 1 << RID_INT))
	{
	  static int already = 0;

	  error_with_location(loc, "long invalid for `%s'", printname);
	  if (!already && !pedantic)
	    {
	      error_with_location(loc, "long is only valid with int or double");
	      already = 1;
	    }
	}
      else if ((specbits & 1 << RID_SIGNED) && (specbits & 1 << RID_UNSIGNED))
	error_with_location(loc, "both signed and unsigned specified for `%s'", printname);
      else if (((specbits & 1 << RID_SIGNED) || (specbits & 1 << RID_UNSIGNED))
	       && !(specbits & (1 << RID_INT | 1 << RID_CHAR)))
	error_with_location(loc, "signed or unsigned invalid for `%s'", printname);
      else
	ok = 1;

      /* Discard the type modifiers if they are invalid.  */
      if (! ok)
	{
	  specbits &= ~(1 << RID_LONG | 1 << RID_SHORT
			| 1 << RID_UNSIGNED | 1 << RID_SIGNED);
	  longlong = 0;
	}
    }

  if ((specbits & 1 << RID_COMPLEX) && !(type_integral(t) || type_floating(t)))
    {
      error_with_location(loc, "complex invalid for `%s'", printname);
      specbits &= ~(1 << RID_COMPLEX);
    }

  /* Decide whether an integer type is signed or not.
     Optionally treat bitfields as signed by default.  */
  if ((specbits & 1 << RID_UNSIGNED)
      /* Traditionally, all bitfields are unsigned.  */
      || (bitfield && flag_traditional
	  && (!explicit_flag_signed_bitfields || !flag_signed_bitfields))
      || (bitfield && !flag_signed_bitfields
	  && ((specbits & 1 << RID_INT) || (specbits & 1 << RID_CHAR))
	  && !(specbits & 1 << RID_SIGNED)))
    {
      if (longlong)
	t = unsigned_long_long_type;
      else if (specbits & 1 << RID_LONG)
	t = unsigned_long_type;
      else if (specbits & 1 << RID_SHORT)
	t = unsigned_short_type;
      else if (t == char_type)
	t = unsigned_char_type;
      else
	t = unsigned_int_type;
    }
  else if ((specbits & 1 << RID_SIGNED) && (specbits & 1 << RID_CHAR))
    t = signed_char_type;
  else if (longlong)
    t = long_long_type;
  else if (specbits & 1 << RID_LONG)
    t = long_type;
  else if (specbits & 1 << RID_SHORT)
    t = short_type;

  if (specbits & 1 << RID_COMPLEX)
    t = make_complex_type(t);

  /* Check for qualifiers redundant with base type */
  check_duplicate_qualifiers(loc, specquals, type_qualifiers(t));

  specquals |= type_qualifiers(t);
  check_legal_qualifiers(loc, specquals);

  t = make_qualified_type(t, specquals);

  *oinlinep = !!(specbits & 1 << RID_INLINE);
  if (pedantic && type_function(t) && (type_const(t) || type_volatile(t)) &&
      !in_system_header)
    pedwarn_with_location(loc, "ANSI C forbids const or volatile function types");

  /* Now figure out the structure of the declarator proper.
     Descend through it, creating more complex types, until we reach
     the declared identifier (or NULL, in an abstract declarator).  */

  while (d && d->kind != kind_identifier_declarator)
    {
      switch (d->kind)
	{
	case kind_array_declarator:
	  {
	    array_declarator ad = CAST(array_declarator, d);
	    expression size = ad->arg1;

	    d = ad->declarator;

	    /* Check for some types that there cannot be arrays of.  */
	    if (type_void(t))
	      {
		error_with_location(d->location,
				    "declaration of `%s' as array of voids", printname);
		t = error_type;
	      }

	    if (type_function(t))
	      {
		error_with_location(d->location,
				    "declaration of `%s' as array of functions", printname);
		t = error_type;
	      }

	    if (size && is_error_expr(size))
	      t = error_type;

	    if (t == error_type)
	      continue;

	    if (size)
	      {
		if (!type_integer(size->type))
		  {
		    error_with_location(d->location,
					"size of array `%s' has non-integer type", printname);
		    size = oneexpr;
		  }

		if (pedantic && definite_zero(size))
		  pedwarn_with_location(d->location,
					"ANSI C forbids zero-size array `%s'", printname);

		if (size->cst && constant_integral(size->cst))
		  {
		    if (pedantic)
		      constant_overflow_warning(size->cst);
		    if (cval_intcompare(size->cst->cval, cval_zero) < 0)
		      {
			error_with_location(d->location,
					    "size of array `%s' is negative", printname);
			size = oneexpr;
		      }
		  }
		else
		  {
		    if (!current_function_decl)
		      {
			if (size->cst)
			  error_with_location(d->location, "type size can't be explicitly evaluated");
			else
			  error_with_location(d->location, "variable-size type declared outside of any function");
		      }
		    else if (pedantic)
		      {
			if (size->cst)
			  pedwarn_with_location(d->location, "ANSI C forbids array `%s' whose size can't be evaluated", printname);
			else
			  pedwarn_with_location(d->location, "ANSI C forbids variable-size array `%s'", printname);
		      }


		    size->cst = NULL; /* Not usefully constant, don't confuse
					 the array type */
		  }
	      }

	    /* Build the array type itself, then merge any constancy or
	       volatility  */
	    t = make_array_type(t, size);
	    break;
	  }

	case kind_function_declarator:
	  {
	    function_declarator fd = CAST(function_declarator, d);
	    type_quals fnquals = parse_qualifiers(fd->location, fd->qualifiers);

	    d = fd->declarator;
	    if (ofunction_declarator)
	      *ofunction_declarator = fd;

	    /* Declaring a function type.
	       Make sure we have a valid type for the function to return.  */
	    if (t == error_type)
	      t = int_type;

	    /* Warn about some types functions can't return.  */
	    if (type_function(t))
	      {
		error_with_location(d->location,
				    "`%s' declared as function returning a function",
		      printname);
		t = int_type;
	      }
	    if (type_array(t))
	      {
		error_with_location(d->location,
				    "`%s' declared as function returning an array",
		      printname);
		t = int_type;
	      }

#ifndef TRADITIONAL_RETURN_FLOAT
	    /* Traditionally, declaring return type float means double.  */
	    if (flag_traditional && type_float(t))
	      t = qualify_type1(double_type, t);
#endif /* TRADITIONAL_RETURN_FLOAT */

	    /* Construct the function type */
	    if (new_style(fd->parms))
	      {
		bool varargs;
		bool definition = ofunction_declarator && 
		  d && d->kind == kind_identifier_declarator;
		typelist argtypes = make_arg_types(definition, fd->parms,
						   &varargs);
		t = make_function_type(t, argtypes, varargs, FALSE);
	      }
	    else  /* Old-style function */
	      t = make_function_type(t, NULL, FALSE, TRUE);

	    t = make_qualified_type(t, fnquals);
	    break;
	  }

	case kind_pointer_declarator:
	  {
	    pointer_declarator pd = CAST(pointer_declarator, d);

	    d = pd->declarator;
	    t = make_qualified_type(make_pointer_type(t),
				    parse_qualifiers(pd->location, pd->qualifiers));
	    break;
	  }

	default: assert(0);
	}
    }

  *ot = t;
}

declarator make_function_declarator(location l, declarator d, declaration parms, type_element quals)
{
  declaration parm;

  environment penv = poplevel();
  function_declarator fd =
    new_function_declarator(parse_region, l, d, parms, quals, penv);

  if (new_style(parms) && !is_void_parms(parms))
    scan_declaration (parm, parms)
      if (!is_ellipsis_decl(parm) && !is_error_decl(parm))
	{
	  variable_decl vp = CAST(variable_decl, CAST(data_decl, parm)->decls);

	  if (!vp->ddecl)
	    {
	      error_with_location(l, "parameter declared void");
	      vp->ddecl = bad_decl;
	    }
	}

  parmlist_tags_warning(penv);

  return CAST(declarator, fd);
}

/* Return zero if the declaration NEWDECL is valid
   when the declaration OLDDECL (assumed to be for the same name and kind
   of declaration) has already been seen.
   Otherwise return an error message format string with a %s
   where the identifier should go.  */

static char *redeclaration_error_message(data_declaration newdecl,
					 data_declaration olddecl,
					 bool newinitialised)
{
  if (olddecl->islimbo)
    return 0;

  if (newdecl->kind == decl_typedef)
    {
      if (flag_traditional && type_compatible(newdecl->type, olddecl->type))
	return 0;
      /* This gets a warning later */
      if (olddecl->in_system_header || newdecl->in_system_header)
	return 0;
      return "redefinition of `%s'";
    }
  else if (newdecl->kind == decl_function)
    {
      /* Declarations of functions can insist on internal linkage
	 but they can't be inconsistent with internal linkage,
	 so there can be no error on that account.
	 However defining the same name twice is no good.  */
      if (olddecl->definition && newdecl->definition
	  /* However, defining once as extern inline and a second
	     time in another way is ok.  */
	  && !(olddecl->isexterninline && !newdecl->isexterninline))
	return "redefinition of `%s'";
      return 0;
    }
  else if (newdecl->kind == decl_constant)
    return "redefinition of `%s'";
  else if (current_env == global_env)
    {
      /* Objects declared at top level:  */
      /* If at least one is a reference, it's ok.  */
      if (newdecl->isfilescoperef || olddecl->isfilescoperef)
	return 0;
      /* Reject two definitions with initialisation.  */
      if (newinitialised && olddecl->initialiser)
	return "redefinition of `%s'";
      /* Now we have two tentative defs, or one tentative and one real def.  */
      /* Insist that the linkage match.  */
      if (olddecl->isexternalscope != newdecl->isexternalscope)
	return "conflicting declarations of `%s'";
      return 0;
    }
  else
    {
      /* Newdecl has block scope.  If olddecl has block scope also, then
	 reject two definitions, and reject a definition together with an
	 external reference.  Otherwise, it is OK, because newdecl must
	 be an extern reference to olddecl.  */
      if (!(newdecl->isexternalscope && olddecl->isexternalscope))
#if 0
	Why check the context ?
	  && DECL_CONTEXT (newdecl) == DECL_CONTEXT (olddecl)
#endif
	return "redeclaration of `%s'";
      return 0;
    }
}

/* Return TRUE if t1 looks like a modern declaration for malloc (&friends) and
   t2 looks like an oldstyle declaration thereof */
static bool looks_like_malloc_redeclaration(type t1, type t2)
{
  type t1return = type_function_return_type(t1);
  type t2return = type_function_return_type(t2);

  return
    type_function_oldstyle(t1) &&
    type_pointer(t1return) && type_pointer(t2return) &&
    type_void(type_points_to(t2return)) &&
    type_char(type_points_to(t1return)) &&
    self_promoting_args(t2);
}

void show_previous_decl(void (*message)(declaration d, const char *format, ...),
			data_declaration olddecl)
{
  if (olddecl->kind == decl_function && olddecl->ftype == function_implicit)
    message(olddecl->ast, "previous implicit declaration of `%s'", olddecl->name);
  else
    message(olddecl->ast, "previous declaration of `%s'", olddecl->name);
}

/* Handle when a new declaration NEWDECL
   has the same name as an old one OLDDECL
   in the same binding contour.
   Prints an error message if appropriate.

   If safely possible, alter OLDDECL to look like NEWDECL, and return 1.
   Otherwise, return 0.

   When DIFFERENT_BINDING_LEVEL is true, NEWDECL is an external declaration,
   and OLDDECL is in an outer binding level and should thus not be changed.  */

static int duplicate_decls(data_declaration newdecl, data_declaration olddecl,
			   bool different_binding_level, bool newinitialised)
{
  type oldtype = olddecl->type;
  type newtype = newdecl->type;
  char *errmsg = 0;
  const char *name = olddecl->name;
  void (*previous_message)(declaration d, const char *format, ...) = NULL;
  bool types_match;

  assert(!(newdecl->kind == decl_function &&
	   newdecl->ftype == function_implicit));

  /* New decl is completely inconsistent with the old one =>
     tell caller to replace the old one. This is an error,
     except if traditional and in different binding levels */
  if (newdecl->kind != olddecl->kind)
    {
      bool iswarning = 
	(flag_traditional && different_binding_level) || olddecl->islimbo;

      warning_or_error(iswarning,
		       "`%s' redeclared as different kind of symbol", name);
      show_previous_decl(iswarning ? warning_with_decl : error_with_decl, olddecl);
      newdecl->shadowed = olddecl;
      return 0;
    }

  /* Allow an earlier function declaration with 'deletes' to be
     contagious to later declarations. This is needed to add 'deletes'
     to functions like yyparse where we cannot change some declarations. */
  if (olddecl->kind == decl_function && type_deletes(oldtype) &&
      !type_deletes(newtype))
    {
      newtype = make_qualified_type
	(newtype, type_qualifiers(newtype) | deletes_qualifier);
      newdecl->type = newtype;
    }

  if (newtype == error_type || oldtype == error_type)
    types_match = FALSE;
  else
    types_match = type_compatible_unqualified(newtype, oldtype);

  /* For real parm decl following a forward decl, or a declaration of an old
     style parameter (oldtype == void) return 1 so old decl will be reused. */
  if ((oldtype == void_type || types_match) && newdecl->isparameter &&
      !olddecl->isused)
    return 1;

  /* The new declaration is the same kind of object as the old one.
     The declarations may partially match.  Print warnings if they don't
     match enough.  Ultimately, copy most of the information from the new
     decl to the old one, and keep using the old one.  */

  if (flag_traditional && olddecl->kind == decl_function
      && olddecl->ftype == function_implicit)
    /* If -traditional, avoid error for redeclaring fcn
       after implicit decl.  */
    ;
  else if (olddecl->kind == decl_function &&
      type_deletes(oldtype) != type_deletes(newtype))
    {
      error("conflicting declarations for `%s'", name);
      previous_message = error_with_decl;
    }
  /* Permit char *foo () to match void *foo (...) if not pedantic,
     if one of them came from a system header file.  */
  else if (!types_match && olddecl->kind == decl_function
	   && (olddecl->in_system_header || newdecl->in_system_header)
	   && (looks_like_malloc_redeclaration(oldtype, newtype) ||
	       looks_like_malloc_redeclaration(newtype, oldtype)))
    {
      if (pedantic)
	pedwarn_with_decl(newdecl->ast, "conflicting types for `%s'", name);
      /* Make sure we keep void * as ret type, not char *.  */
      if (type_void(type_points_to(type_function_return_type(oldtype))))
	newdecl->type = newtype = oldtype;

      /* Set IN_SYSTEM_HEADER, so that if we see another declaration
	 we will come back here again.  */
      newdecl->in_system_header = TRUE;
    }
  else if (!types_match
	   /* Permit char *foo (int, ...); followed by char *foo ();
	      if not pedantic.  */
	   && !(olddecl->kind == decl_function && !pedantic
		&& type_function_oldstyle(newtype)
		/* Return types must still match.  */
		&& type_compatible(type_function_return_type(oldtype),
				   type_function_return_type(newtype))))
    {
      void (*message)(const char *format, ...) =
	olddecl->islimbo ? warning : error;

      previous_message =
	olddecl->islimbo ? warning_with_decl : error_with_decl;

      message("conflicting types for `%s'", name);
      /* Check for function type mismatch
	 involving an empty arglist vs a nonempty one.  */
      if (newdecl->kind == decl_function
	  && type_compatible(type_function_return_type(oldtype),
			     type_function_return_type(newtype))
	  && ((type_function_oldstyle(oldtype) && !olddecl->definition)
	      || (type_function_oldstyle(newtype) && !newdecl->definition)))
	{
	  /* Classify the problem further.  */
	  if (type_function_varargs(newtype) || type_function_varargs(oldtype))
	    {
	      message("A parameter list with an ellipsis can't match");
	      message("an empty parameter name list declaration.");
	    }
	  else
	    {
	      typelist_scanner scanargs;
	      type t;
	      typelist args = type_function_arguments(oldtype);

	      if (!args)
		args = type_function_arguments(newtype);

	      typelist_scan(args, &scanargs);
	      while ((t = typelist_next(&scanargs)))
		if (!type_self_promoting(t))
		  {
		    message("An argument type that has a default promotion");
		    message("can't match an empty parameter name list declaration.");
		    break;
		  }
	    }
	}
    }
  else
    {
      errmsg = redeclaration_error_message(newdecl, olddecl, newinitialised);
      if (errmsg)
	{
	  error_with_decl(newdecl->ast, errmsg, name);
	  previous_message = error_with_decl;
	}
      else if (newdecl->kind == decl_typedef &&
	       (olddecl->in_system_header || newdecl->in_system_header))
	{
	  warning_with_decl(newdecl->ast, "redefinition of `%s'");
	  previous_message = warning_with_decl;
	}
      else if (olddecl->kind == decl_function
	       && olddecl->oldstyle_args
	       && !type_function_oldstyle(newtype))
	{
	  int nargs;
	  typelist_scanner oldparms, newparms;
	  /* Prototype decl follows defn w/o prototype.  */

	  typelist_scan(olddecl->oldstyle_args, &oldparms);
	  typelist_scan(type_function_arguments(newtype), &newparms);
	  nargs = 1;
	  errmsg = NULL;
	  for (;;)
	    {
	      type oldparm = typelist_next(&oldparms);
	      type newparm = typelist_next(&newparms);

	      if (!oldparm && !newparm)
		break;

	      if (!oldparm || !newparm)
		{
		  errmsg = "prototype for `%s' follows and number of arguments";
		  break;
		}
	      /* Type for passing arg must be consistent
		 with that declared for the arg.  */
	      if (!type_compatible(oldparm, newparm)
		  /* If -traditional, allow `unsigned int' instead of `int'
		     in the prototype.  */
		  && !(flag_traditional
		       && type_equal_unqualified(oldparm, int_type)
		       && type_equal_unqualified(newparm, unsigned_int_type)))
		{
		  errmsg = "prototype for `%s' follows and argument %d";
		  break;
		}
	      nargs++;
	    }
	  if (errmsg)
	    {
	      warning_or_error_with_decl(olddecl->islimbo, newdecl->ast,
					 errmsg, name, nargs);
	      warning_or_error_with_decl(olddecl->islimbo, olddecl->ast,
			      "doesn't match non-prototype definition here");
	    }
	  else
	    {
	      warning_with_decl(newdecl->ast, "prototype for `%s' follows",
				name);
	      warning_with_decl(olddecl->ast, "non-prototype definition here");
	    }
	}
      /* Warn about mismatches in various flags. */
      else if (newdecl->kind == decl_function)
	{
	  /* Warn if function is now inline
	     but was previously declared not inline and has been called. */
	  if (!olddecl->isinline && newdecl->isinline)
	    {
	      if (olddecl->isused)
		{
		  warning("`%s' declared inline after being called", name);
		  previous_message = warning_with_decl;
		}
	      if (olddecl->definition)
		{
		  warning("`%s' declared inline after its definition", name);
		  previous_message = warning_with_decl;
		}
	    }

	  /* Warn for static following external */
	  if (newdecl->ftype == function_static &&
	      olddecl->ftype == function_implicit)
	    {
	      pedwarn("`%s' was declared implicitly `extern' and later `static'", name);
	      previous_message = pedwarn_with_decl;
	    }
	  else if (newdecl->ftype == function_static &&
		   olddecl->ftype == function_normal)
	    {
	      pedwarn("static declaration for `%s' follows non-static", name);
	      previous_message = pedwarn_with_decl;
	    }
	}
      else if (newdecl->kind == decl_variable)
	{
	  /* If pedantic, warn when static declaration follows a non-static
	     declaration. */
	  if (pedantic &&
	      olddecl->isexternalscope && !newdecl->isexternalscope)
	    {
	      pedwarn("static declaration for `%s' follows non-static", name);
	      previous_message = pedwarn_with_decl;
	    }
	  /* Warn when const declaration follows a non-const declaration */
	  if (!type_const(oldtype) && type_const(newtype))
	    warning("const declaration for `%s' follows non-const", name);
	  /* These bits are logically part of the type, for variables. */
	  else if (pedantic && 
		   (type_const(oldtype) != type_const(newtype)
		    || type_volatile(oldtype) != type_volatile(newtype)))
	    {
	      pedwarn("type qualifiers for `%s' conflict with previous decl",
		      name);
	      previous_message = pedwarn_with_decl;
	    }
	}
    }

  /* Optionally warn about more than one declaration for the same name.  */
  /* Let's try a different test than GCC */
  if (errmsg == 0 && warn_redundant_decls && !olddecl->islimbo &&
      !(olddecl->isfilescoperef && !newdecl->isfilescoperef))
    {
      warning_with_decl(newdecl->ast, "redundant redeclaration of `%s' in same scope", name);
      previous_message = warning_with_decl;
    }

  if (previous_message)
    show_previous_decl(previous_message, olddecl);


  /* Copy all the DECL_... slots specified in the new decl
     except for any that we copy here from the old type. */

  /* If either decl says `inline', this fn is inline,
     unless its definition was passed already.  */
  if (newdecl->isinline && !olddecl->definition)
    olddecl->isinline = TRUE;
  newdecl->isinline = olddecl->isinline;

  if (different_binding_level)
    {
      /* newdecl must be a reference to something at file scope */
      assert(newdecl->isfilescoperef && !newdecl->needsmemory);
      assert(!(newdecl->kind == decl_variable &&
	       newdecl->vtype == variable_static));
      assert(!(newdecl->kind == decl_function &&
	       (newdecl->ftype == function_implicit ||
		newdecl->ftype == function_nested)));

      /* We copy some info over to the newdecl which will shadow olddecl */
      newdecl->shadowed = olddecl;
      newdecl->definition = olddecl->definition;
      newdecl->isinline = olddecl->isinline;
      newdecl->isexterninline = olddecl->isexterninline;
      newdecl->oldstyle_args = olddecl->oldstyle_args;
      if (olddecl->in_system_header)
	newdecl->in_system_header = TRUE;

      newdecl->isexternalscope = olddecl->isexternalscope;

      /* We don't copy the type */

      return 0;
    }

  /* newdecl should be too new to have any oldstyle args yet */
  assert(!newdecl->oldstyle_args);

  /* Merge the data types specified in the two decls.  */
  if (types_match)
    olddecl->type = common_type(newtype, oldtype);
  else if (newtype != error_type)
    /* GCC keeps the old type. I think it makes more sense to keep the new
       one. And it means I can examine the decl in current_function_decl
       and find the type of the current function, not something random */
    olddecl->type = newtype;
  olddecl->islimbo = FALSE;

  if (newdecl->definition)
    olddecl->definition = newdecl->definition;

  olddecl->isexternalscope &= newdecl->isexternalscope;
  olddecl->isfilescoperef &= newdecl->isfilescoperef;
  olddecl->needsmemory |= newdecl->needsmemory;

  /* For functions, static overrides non-static.  */
  if (newdecl->kind == decl_function)
    {
      if (olddecl->ftype != function_static)
	olddecl->ftype = newdecl->ftype;
    }

  olddecl->in_system_header = newdecl->in_system_header =
    olddecl->in_system_header || newdecl->in_system_header;

  /* Point to the latest declaration */
  olddecl->ast = newdecl->ast;

  return 1;
}

void ignored_attribute(attribute attr)
{
  warning_with_location(attr->location, "`%s' attribute directive ignored",
			attr->word1->cstring.data);
}

static void transparent_union_argument(data_declaration ddecl)
{
  ddecl->type = make_qualified_type
    (ddecl->type, type_qualifiers(ddecl->type) | transparent_qualifier);
}

/* Note: fdecl->bitwidth is not yet set when this is called */
void handle_attribute(attribute attr, data_declaration ddecl, 
		      field_declaration fdecl, tag_declaration tdecl)
{
  if (!strcmp(attr->word1->cstring.data, "transparent_union") ||
      !strcmp(attr->word1->cstring.data, "__transparent_union__"))
    {
      if (attr->word2 || attr->args)
	error_with_location(attr->location, "wrong number of arguments specified for `transparent_union' attribute");

      if (ddecl && ddecl->kind == decl_variable && ddecl->isparameter &&
	  type_union(ddecl->type))
	transparent_union_argument(ddecl);
      else if (ddecl && ddecl->kind == decl_typedef && type_union(ddecl->type))
	transparent_union_argument(ddecl);
      else if (tdecl && tdecl->kind == kind_union_ref)
	{
	  tdecl->transparent_union = TRUE;
	  /* XXX: Missing validity checks (need cst folding I think) */
	}
      else
	ignored_attribute(attr);
    }
  else if (!strcmp(attr->word1->cstring.data, "packed") ||
	   !strcmp(attr->word1->cstring.data, "__packed__"))
    {
      if (tdecl)
	tdecl->packed = TRUE;
      else if (fdecl)
	fdecl->packed = TRUE;
      else
	ignored_attribute(attr);
    }
}

void handle_attributes(attribute alist, data_declaration ddecl, 
		       field_declaration fdecl, tag_declaration tdecl)
{
  scan_attribute (alist, alist)
    handle_attribute(alist, ddecl, fdecl, tdecl);
}

void ignored_attributes(attribute alist)
{
  scan_attribute (alist, alist)
    ignored_attribute(alist);
}

bool is_doublecharstar(type t)
{
  if (!type_pointer(t))
    return FALSE;
  t = type_points_to(t);
  if (!type_pointer(t))
    return FALSE;
  return type_equal_unqualified(type_points_to(t), char_type);
}

void check_function(data_declaration dd, declaration fd, int class,
		    bool inlinep, const char *name, type function_type,
		    bool nested, bool isdeclaration, bool defaulted_int)
{
  type return_type;

  if (defaulted_int && (warn_implicit_int || warn_return_type))
    warning("return-type defaults to `int'");

  return_type = type_function_return_type(function_type);

  /* XXX: Does this volatile/const stuff actually work with my imp ? */
  if (pedantic && type_void(return_type) &&
      (type_const(return_type) || type_volatile(return_type)) &&
      !in_system_header)
    pedwarn("ANSI C forbids const or volatile void function return type");

  if (type_volatile(function_type) && !type_void(return_type))
    warning("`noreturn' function returns non-void value");

  /* Record presence of `inline', if it is reasonable.  */
  if (inlinep && !strcmp(name, "main") && !nested)
    {
      warning("cannot inline function `main'");
      inlinep = FALSE;
    }

  /* Warn for unlikely, improbable, or stupid declarations of `main'. */
  if (warn_main && !strcmp("main", name) && !nested)
    {
      if (!type_equal_unqualified(return_type, int_type))
	pedwarn("return type of `%s' is not `int'", name);

      /* Just being "bug"-compatible w/ GCC here */
      if (!type_function_oldstyle(function_type))
	{
	  typelist_scanner scanargs;
	  type argtype;
	  int argct = 0;

	  typelist_scan(type_function_arguments(function_type), &scanargs);
	  while ((argtype = typelist_next(&scanargs)))
	    {
	      ++argct;
	      switch (argct)
		{
		case 1:
		  if (!type_equal_unqualified(argtype, int_type))
		    pedwarn("first argument of `%s' should be `int'", name);
		  break;

		case 2:
		  if (!is_doublecharstar(argtype))
		    pedwarn("second argument of `%s' should be `char **'",
			    name);
		  break;

		case 3:
		  if (!is_doublecharstar(argtype))
		    pedwarn("third argument of `%s' should probably be `char **'",
			    name);
		  break;
		}
	    }

	  /* It is intentional that this message does not mention the third
	     argument, which is warned for only pedantically, because it's
	     blessed by mention in an appendix of the standard. */
	  if (argct > 0 && (argct < 2 || argct > 3))
	    pedwarn("`%s' takes only zero or two arguments", name);

	  if (argct == 3 && pedantic)
	    pedwarn("third argument of `%s' is deprecated", name);

	  if (class == RID_STATIC)
	    pedwarn("`%s' is normally a non-static function", name);
	}
    }

  init_data_declaration(dd, fd, name, function_type);
  dd->kind = decl_function;
  dd->isexternalscope = FALSE;
  if (nested)
    dd->ftype = function_nested;
  else if (class == RID_STATIC)
    dd->ftype = function_static;
  else
    {
      dd->ftype = function_normal;
      dd->isexternalscope = TRUE;
    }
  /* XXX: Should probably be FALSE for extern inline */
  dd->needsmemory = !isdeclaration;
  dd->isinline = inlinep;
  dd->isexterninline = inlinep && class == RID_EXTERN;
  dd->isfilescoperef = dd->isexterninline || isdeclaration;
}

data_declaration declare_string(const char *name, bool wide, size_t length)
{
  struct data_declaration tempdecl;
  expression expr_l = build_uint_constant(parse_region, dummy_location, size_t_type, length);
  type value_type = make_array_type(wide ? wchar_type : char_type, expr_l);

  init_data_declaration(&tempdecl, new_error_decl(parse_region, dummy_location),
			name, value_type);
  tempdecl.kind = decl_magic_string;
  tempdecl.needsmemory = TRUE;
  tempdecl.in_system_header = TRUE;
  tempdecl.vtype = variable_static;

  /* Save value. */
  tempdecl.chars_length = length;
  tempdecl.chars = rarrayalloc(parse_region, length, wchar_t);

  return declare(current_env, &tempdecl, TRUE);
}

static void declare_magic_string(const char *name, const char *value)
{
  int i, l = strlen(value);
  data_declaration dd = declare_string(name, FALSE, l);
  wchar_t *chars = (wchar_t *)dd->chars;

  /* Save value. */
  for (i = 0; i < l; i++)
    chars[i] = value[i];
}

static void declare_builtin_type(const char *name, type t)
{
  struct data_declaration tempdecl;

  init_data_declaration(&tempdecl, new_error_decl(parse_region, dummy_location),
			name, t);
  tempdecl.kind = decl_typedef;
  tempdecl.in_system_header = TRUE;

  declare(current_env, &tempdecl, TRUE);
}

static tag_declaration make_anonymous_struct(void)
{
  tag_ref tref = newkind_tag_ref(parse_region, kind_struct_ref, dummy_location,
				 NULL, NULL, NULL, TRUE);

  return declare_global_tag(tref);
}

static void declare_builtin_types(void)
{
  builtin_va_list_type = 
    make_pointer_type(make_tagged_type(make_anonymous_struct()));
  declare_builtin_type("__builtin_va_list", builtin_va_list_type);
}

static data_declaration declare_builtin(const char *name, data_kind kind, type t)
{
  struct data_declaration tempdecl;

  init_data_declaration(&tempdecl, new_error_decl(parse_region, dummy_location),
			name, t);
  tempdecl.kind = kind;
  tempdecl.needsmemory = TRUE;
  tempdecl.in_system_header = TRUE;
  tempdecl.vtype = variable_static;

  return declare(global_env, &tempdecl, TRUE);
}

data_declaration declare_builtin_identifier(const char *name, type t)
{
  return declare_builtin(name, decl_variable, t);
}

data_declaration declare_builtin_function(const char *name, type t)
{
  return declare_builtin(name, decl_function, t);
}

static void declare_builtin_identifiers(void)
{
  typelist emptylist = new_typelist(parse_region);
  type default_function_type = make_function_type(int_type, emptylist, FALSE, TRUE);

  /* Use = as a suffix for this "dummy" identifier (used in function_call
     nodes that represent calls to __builtin_va_arg) */
  builtin_va_arg_decl = declare_builtin_identifier("=va_arg", int_type);

  declare_builtin_function("__builtin_constant_p", default_function_type);
}

static void declare_function_name(void)
{
  const char *name, *printable_name;

  if (current_function_decl == NULL)
    {
      name = "";
      printable_name = "top level";
    }
  else
    {
      name = current_function_decl->ddecl->name;
      printable_name = name;
    }

  declare_magic_string("__FUNCTION__", name);
  declare_magic_string("__PRETTY_FUNCTION__", printable_name);
}

/* Start definition of function 'elements d' with attributes attribs.
   nested is true for nested function definitions.
   Returns false in case of error.
   Sets current_function_decl to the declaration for this function */
bool start_function(type_element elements, declarator d, attribute attribs,
		    bool nested)
{
  int class;
  bool inlinep;
  const char *name;
  type function_type;
  bool defaulted_int, old_decl_has_prototype, normal_function;
  data_declaration old_decl, ddecl;
  function_decl fdecl;
  function_declarator fdeclarator;
  struct data_declaration tempdecl;
  env_scanner scan;
  const char *id;
  void *idval;

  if (!nested)
    assert(current_env == global_env && current_function_decl == NULL);

  parse_declarator(elements, d, FALSE, &class, &inlinep, &name, &function_type,
		   &defaulted_int, &fdeclarator);

  if (!type_function(function_type))
    return FALSE;


  /* We don't set current_function_decl yet so that error messages do not
     say "In function <thisfunctioname>" as we're not "in" the function yet */
  fdecl = new_function_decl(parse_region, d->location, d, elements, attribs,
			    NULL, NULL, current_function_decl, NULL);
  fdecl->declared_type = function_type;
  fdecl->undeclared_variables = new_env(parse_region, NULL);
  fdecl->current_loop = NULL;

  if (class == RID_AUTO)
    {
      if (pedantic || !nested)
	pedwarn("function definition declared `auto'");
      class = 0;
    }
  else if (class == RID_REGISTER)
    {
      error("function definition declared `register'");
      class = 0;
    }
  else if (class == RID_TYPEDEF)
    {
      error("function definition declared `typedef'");
      class = 0;
    }
  else if (class == RID_EXTERN && nested)
    {
      error("nested function `%s' declared `extern'", name);
      class = 0;
    }
  else if ((class == RID_STATIC || inlinep) && nested)
    {
      if (pedantic)
	pedwarn("invalid storage class for function `%s'", name);
      class = 0;
    }

  if (!type_void(type_function_return_type(function_type)) &&
      type_incomplete(type_function_return_type(function_type)))
    {
      error("return-type is an incomplete type");
      function_type = qualify_type1
	(make_function_type(void_type,
			    type_function_arguments(function_type),
			    type_function_varargs(function_type),
			    type_function_oldstyle(function_type)),
	 function_type);
    }

  old_decl = lookup_id(name, TRUE);
  old_decl_has_prototype = old_decl && old_decl->kind == decl_function &&
    !type_function_oldstyle(old_decl->type);

  normal_function = !nested && class != RID_STATIC;
  /* Optionally warn of old-fashioned def with no previous prototype.  */
  if (warn_strict_prototypes
      && type_function_oldstyle(function_type)
      && !old_decl_has_prototype)
    warning("function declaration isn't a prototype");
  /* Optionally warn of any global def with no previous prototype.  */
  else if (warn_missing_prototypes
	   && normal_function && !old_decl_has_prototype
	   && strcmp("main", name))
    warning("no previous prototype for `%s'", name);
  /* Optionally warn of any def with no previous prototype
     if the function has already been used.  */
  else if (warn_missing_prototypes
	   && old_decl && old_decl->ftype == function_implicit)
    warning("`%s' was used with no prototype before its definition", name);
  /* Optionally warn of any global def with no previous declaration.  */
  else if (warn_missing_declarations
	   && normal_function && !old_decl && strcmp("main", name))
    warning("no previous declaration for `%s'", name);
  /* Optionally warn of any def with no previous declaration
     if the function has already been used.  */
  else if (warn_missing_declarations
	   && old_decl && old_decl->ftype == function_implicit)
    warning("`%s' was used with no declaration before its definition", name);

  check_function(&tempdecl, CAST(declaration, fdecl), class,
		 inlinep, name, function_type, nested, FALSE, defaulted_int);
  tempdecl.definition = tempdecl.ast;

  /* If return types match and old declaraton has a prototype and new
     declaration hasn't, borrow the prototype. We will check for errors
     in store_parm_decls. */
  if (old_decl_has_prototype && type_function_oldstyle(function_type) &&
      type_compatible(type_function_return_type(old_decl->type),
		      type_function_return_type(function_type)))
    {
      function_type = qualify_type1
	(make_function_type(type_function_return_type(function_type),
			    type_function_arguments(old_decl->type),
			    type_function_varargs(old_decl->type),
			    FALSE), old_decl->type);

      tempdecl.type = function_type;
    }

  if (old_decl && duplicate_decls(&tempdecl, old_decl, FALSE, FALSE))
    ddecl = old_decl;
  else
    ddecl = declare(current_env, &tempdecl, FALSE);

  fdecl->base_labels = fdecl->scoped_labels =
    new_env(parse_region,
	    current_function_decl ? current_function_decl->scoped_labels : NULL);
  fdecl->ddecl = ddecl;
  fdecl->fdeclarator = fdeclarator;

  current_env = fdeclarator->env;
  current_env->fdecl = current_function_decl = fdecl;

  /* Set id of parameters (done here rather than in declare because parameters
     of a given function may be declared several times) */
  env_scan(current_env->id_env, &scan);
  while (env_next(&scan, &id, &idval))
    {
      data_declaration iddecl = idval;

      if (iddecl->kind == decl_variable)
	{
	  assert(iddecl->isparameter);
	  iddecl->id = current_function_decl->nlocals++;
	}
    }

  handle_attributes(attribs, ddecl, NULL, NULL);

  return TRUE;
}

data_declaration implicitly_declare(identifier fnid)
{
  struct data_declaration tempdecl;
  declaration pseudo_ast =
    CAST(declaration, new_implicit_decl(parse_region, fnid->location, fnid));

  init_data_declaration(&tempdecl, pseudo_ast,
			fnid->cstring.data, implicit_function_type);
  tempdecl.kind = decl_function;
  tempdecl.isexternalscope = TRUE;
  tempdecl.isfilescoperef = TRUE;
  tempdecl.ftype = function_implicit;
  /* Point to the limbo version of any previous implicit declaration */
  tempdecl.shadowed = lookup_global_id(tempdecl.name);

  return declare(current_env, &tempdecl, FALSE);
}

/* Declare parameters, either from new style declarations in the 
   declarator, or from old_parms */
void store_parm_decls(declaration old_parms)
{
  if (!oldstyle_function(current_function_decl))
    {
      /* This case is when the function was defined with an ANSI prototype.
	 The parms already have decls, so we need not do anything here
	 except record them as in effect
	 and complain if any redundant old-style parm decls were written.  */
      if (old_parms)
	error_with_decl(CAST(declaration, current_function_decl),
			"parm types given both in parmlist and separately");
    }
  else
    {
      oldidentifier_decl parm, parms;

      current_function_decl->old_parms = old_parms;
      /* Need to either:
	 - compare arg types to previous prototype
	 - or build pseudo-prototype for this function
      */
      parms = CAST(oldidentifier_decl,
		   current_function_decl->fdeclarator->parms);
      scan_oldidentifier_decl (parm, parms)
	/* If no declaration given, default to int.  */
	if (parm->ddecl->type == void_type)
	  {
	    parm->ddecl->type = int_type;
	    if (extra_warnings)
	      warning_with_decl(CAST(declaration, parm),
				"type of `%s' defaults to `int'",
				parm->cstring.data);
	  }
    }

  /* Declare __FUNCTION__ and __PRETTY_FUNCTION__ for this function.  */
  declare_function_name();
}

/* End definition of current function, furnishing it it's body. */
declaration finish_function(statement body)
{
  declaration fn = CAST(declaration, current_function_decl);

  current_function_decl->stmt = body;
  assert(current_env->parm_level);
  poplevel(); /* Pop parameter level */
  check_labels();
  current_function_decl = current_function_decl->parent_function;

  return fn;
}

/* Start a new scope */
void pushlevel(bool parm_level)
{
  current_env = new_environment(parse_region, current_env, parm_level);
}

/* Pop back to enclosing scope */
environment poplevel(void)
{
  environment old = current_env;

  current_env = current_env->parent;

  return old;
}

void push_label_level(void)
{
  current_function_decl->scoped_labels =
    new_env(parse_region, current_function_decl->scoped_labels);
}

void pop_label_level(void)
{
  check_labels();
  current_function_decl->scoped_labels =
    env_parent(current_function_decl->scoped_labels);
  assert(current_function_decl->scoped_labels);
}

const char *declarator_name(declarator d)
{
  while (d)
    {
      switch (d->kind)
	{
	case kind_identifier_declarator:
	  return CAST(identifier_declarator, d)->cstring.data;
	case kind_function_declarator:
	  d = CAST(function_declarator, d)->declarator; break;
	case kind_array_declarator:
	  d = CAST(array_declarator, d)->declarator; break;
	case kind_pointer_declarator:
	  d = CAST(pointer_declarator, d)->declarator; break;
	default: assert(0);
	}
    }
  return NULL;
}

void check_parameter(data_declaration dd,
		     type_element elements, variable_decl vd)
{
  int class;
  bool inlinep;
  const char *name, *printname;
  bool defaulted_int;
  type parm_type;

  parse_declarator(elements, vd->declarator, FALSE,
		   &class, &inlinep, &name, &parm_type, &defaulted_int, NULL);
  vd->declared_type = parm_type;
  printname = name ? name : "type name";

  /* Storage class checks */
  if (class && class != RID_REGISTER)
    {
      error("storage class specified for parameter `%s'", printname);
      class = 0;
    }

  if (inlinep)
    pedwarn_with_decl(CAST(declaration, vd),
		      "parameter `%s' declared `inline'", printname);

  /* A parameter declared as an array of T is really a pointer to T.
     One declared as a function is really a pointer to a function.  */
  if (type_array(parm_type))
    /* Transfer const-ness of array into that of type pointed to.  */
    parm_type =
      make_pointer_type(qualify_type1(type_array_of(parm_type), parm_type));
  else if (type_function(parm_type))
    parm_type = make_pointer_type(parm_type);

  init_data_declaration(dd, CAST(declaration, vd), name, parm_type);
  dd->kind = decl_variable;
  dd->definition = dd->ast;
  dd->isexternalscope = FALSE;
  dd->isfilescoperef = FALSE;
  dd->needsmemory = FALSE;
  dd->isused = TRUE;
  dd->vtype = class == RID_REGISTER ? variable_register : variable_normal;
  dd->islocal = dd->isparameter = TRUE;
}

/* Start definition of variable 'elements d' with attributes
   extra_attributes and attributes, asm specification astmt.
   If initialised is true, the variable has an initialiser.
   Returns the declaration for the variable.
*/
declaration start_decl(declarator d, asm_stmt astmt, type_element elements,
		       bool initialised, attribute extra_attributes,
		       attribute attributes)
{
  variable_decl vd = 
    new_variable_decl(parse_region, d->location, d, extra_attributes, NULL,
		      astmt, NULL);
  struct data_declaration tempdecl;
  data_declaration ddecl = NULL, old_decl;

  if (current_env->parm_level)
    {
      check_parameter(&tempdecl, elements, vd);

      if (type_void(tempdecl.type))
	{
	  error("parameter `%s' declared void", tempdecl.name);
	  tempdecl.type = int_type;
	}

      /* Update environment for old-style declarations only if function
	 doesn't have a prototype. We will report an error message later
	 for arguments specified for functions with prototypes. */
      if (oldstyle_function(current_function_decl))
	{
	  /* Traditionally, a parm declared float is actually a double.  */
	  if (flag_traditional &&
	      type_equal_unqualified(tempdecl.type, float_type))
	    tempdecl.type = qualify_type1(double_type, tempdecl.type);

	  old_decl = lookup_id(tempdecl.name, TRUE);

	  if (old_decl && duplicate_decls(&tempdecl, old_decl, FALSE, FALSE))
	    {
	      /* Don't allow more than one "real" duplicate
		 of a forward parm decl.  */
	      ddecl = old_decl;
	      ddecl->type = tempdecl.type;
	      ddecl->ast = CAST(declaration, vd);
	      ddecl->isused = TRUE;
	    }
	  else
	    {
	      error("declaration for parameter `%s' but no such parameter",
		    tempdecl.name);
	      ddecl = declare(current_env, &tempdecl, FALSE);
	    }
	}
      else
	/* Make a dummy decl to keep everyone happy */
	ddecl = declare(current_env, &tempdecl, FALSE);

      if (initialised)
	error("parameter `%s' is initialized",
	      vd->ddecl->name ? vd->ddecl->name : "type name");
    }
  else
    {
      int class;
      bool inlinep;
      const char *name, *printname;
      bool defaulted_int;
      type var_type;
      bool different_binding_level = FALSE;

      parse_declarator(elements, d, FALSE, &class, &inlinep, &name, &var_type,
		       &defaulted_int, NULL);
      vd->declared_type = var_type;
      printname = name ? name : "type name";

      if (warn_implicit_int && defaulted_int && !type_function(var_type))
	warning("type defaults to `int' in declaration of `%s'", printname);

      init_data_declaration(&tempdecl, CAST(declaration, vd), name, var_type);

      /* The fun begins */

      /* `extern' with initialization is invalid if not at top level.  */
      if (class == RID_EXTERN && initialised)
	{
	  if (current_env == global_env)
	    warning("`%s' initialized and declared `extern'", printname);
	  else
	    error("`%s' has both `extern' and initializer", printname);
	}

      if (class == RID_AUTO && current_env == global_env)
	{
	  error("top-level declaration of `%s' specifies `auto'", printname);
	  class = 0;
	}

      if (class == RID_TYPEDEF)
	{
	  /* typedef foo = bar  means give foo the same type as bar.
	     We haven't parsed bar yet, so `finish_decl' will fix that up.
	     Any other case of an initialization in a TYPE_DECL is an error. */
	  if (initialised && (pedantic || elements->next))
	    error("typedef `%s' is initialized", printname);

	  tempdecl.kind = decl_typedef;
	  tempdecl.definition = tempdecl.ast;
	  tempdecl.isexternalscope = FALSE;
	  tempdecl.isfilescoperef = FALSE;
	  tempdecl.needsmemory = FALSE;
	}
      else if (type_function(var_type))
	{
	  bool nested = current_env != global_env && class == RID_AUTO;

	  if (initialised)
	    error("function `%s' is initialized like a variable",
		  printname);

	  if (class == RID_AUTO && pedantic)
	    pedwarn("invalid storage class for function `%s'", name);
	  if (class == RID_REGISTER)
	    {
	      error("invalid storage class for function `%s'", name);
	      class = 0;
	    }
	  /* Function declaration not at top level.
	     Storage classes other than `extern' are not allowed
	     and `extern' makes no difference.  */
	  if (current_env != global_env && pedantic
	      && (class == RID_STATIC || class == RID_INLINE))
	    pedwarn("invalid storage class for function `%s'", name);

	  check_function(&tempdecl, CAST(declaration, vd), class, inlinep,
			 name, var_type, nested, TRUE, defaulted_int);
	}
      else
	{
	  int extern_ref = !initialised && class == RID_EXTERN;

	  if (type_void(var_type) &&
	      !(class == RID_EXTERN ||
		(current_env == global_env &&
		 !(class == RID_STATIC || class == RID_REGISTER))))
	    {
	      error("variable `%s' declared void", printname);
	      var_type = int_type;
	    }

	  /* It's a variable.  */
	  if (inlinep)
	    pedwarn("variable `%s' declared `inline'", printname);
#if 0
	  /* Don't allow initializations for incomplete types
	     except for arrays which might be completed by the initialization.  */
	  if (TYPE_SIZE (TREE_TYPE (decl)) != 0)
	    {
	      /* A complete type is ok if size is fixed.  */

	      if (TREE_CODE (TYPE_SIZE (TREE_TYPE (decl))) != INTEGER_CST
		  || C_DECL_VARIABLE_SIZE (decl))
		{
		  error ("variable-sized object may not be initialized");
		  initialised = 0;
		}
	    }
	  else if (TREE_CODE (TREE_TYPE (decl)) != ARRAY_TYPE)
	    {
	      error ("variable `%s' has initializer but incomplete type",
		     IDENTIFIER_POINTER (DECL_NAME (decl)));
	      initialised = 0;
	    }
	  else if (TYPE_SIZE (TREE_TYPE (TREE_TYPE (decl))) == 0)
	    {
	      error ("elements of array `%s' have incomplete type",
		     IDENTIFIER_POINTER (DECL_NAME (decl)));
	      initialised = 0;
	    }
#endif
	  tempdecl.kind = decl_variable;
	  tempdecl.vtype =
	    class == RID_REGISTER ? variable_register :
	    class == RID_STATIC ? variable_static :
	    variable_normal;
	  tempdecl.isfilescoperef = extern_ref;
	  if (!extern_ref)
	    tempdecl.definition = tempdecl.ast;
	  if (current_env == global_env)
	    {
	      tempdecl.isexternalscope =
		class != RID_STATIC && class != RID_REGISTER;
	      tempdecl.needsmemory = !extern_ref;
	      tempdecl.islocal = FALSE;
	    }
	  else
	    {
	      tempdecl.isexternalscope = extern_ref;
	      tempdecl.needsmemory = class == RID_STATIC;
	      tempdecl.islocal = !(extern_ref || class == RID_STATIC);
	    }
	}

      if (warn_nested_externs && tempdecl.isfilescoperef &&
	  current_env != global_env && !tempdecl.in_system_header)
	warning("nested extern declaration of `%s'", printname);

      old_decl = lookup_id(name, TRUE);
      /* Check the global environment if declaring something with file
	 scope */
      if (!old_decl && tempdecl.isfilescoperef)
	{
	  old_decl = lookup_global_id(name);
	  /* global typedefs don't count */
	  if (old_decl && old_decl->kind == decl_typedef)
	    old_decl = NULL;
	  if (old_decl)
	    different_binding_level = TRUE;
	}

      if (old_decl &&
	  duplicate_decls(&tempdecl, old_decl, different_binding_level, initialised))
	ddecl = old_decl;
      else
	ddecl = declare(current_env, &tempdecl, FALSE);
    }
  assert(ddecl);
  vd->ddecl = ddecl;

  handle_attributes(extra_attributes, ddecl, NULL, NULL);
  handle_attributes(attributes, ddecl, NULL, NULL);

  return CAST(declaration, vd);
}

/* Finish definition of decl, furnishing the optional initialiser init.
   Returns decl */
declaration finish_decl(declaration decl, expression init)
{
  variable_decl vd = CAST(variable_decl, decl);
  data_declaration dd = vd->ddecl;

  vd->arg1 = init;
  dd->initialiser = init;

  if (init)
    {
      /* XXX: This is missing a lot of stuff */
      if (dd->kind == decl_typedef)
	dd->type = init->type;
      else if (type_array(dd->type))
	{
	  /* XXX: lots missing */
	  if (!type_array_size(dd->type))
	    {
	      expression size = build_uint_constant(parse_region, dummy_location, size_t_type, 0);

	      dd->type = make_array_type(type_array_of(dd->type), size);
	    }
	}
      else
	check_assignment(dd->type, default_conversion_for_assignment(init),
			 init, "initialization", NULL, NULL, 0);
    }

  return decl;
}

/* Create definition of function parameter 'elements d' with attributes
   extra_attributes and attributes.
   Returns the declaration for the parameter.
*/
declaration declare_parameter(declarator d, type_element elements,
			      attribute extra_attributes, attribute attributes,
			      bool abstract)
{
  /* There must be at least a declarator or some form of type specification */
  location l =
    d ? d->location : elements->location;
  variable_decl vd =
    new_variable_decl(parse_region, l, d, attributes, NULL, NULL, NULL);
  data_decl dd =
    new_data_decl(parse_region, l, elements, extra_attributes,
		  CAST(declaration, vd));
  data_declaration ddecl = NULL, old_decl = NULL;
  struct data_declaration tempdecl;

  check_parameter(&tempdecl, elements, vd);

  if (tempdecl.name)
    old_decl = lookup_id(tempdecl.name, TRUE);

  if (old_decl && duplicate_decls(&tempdecl, old_decl, FALSE, FALSE))
    {
      /* Don't allow more than one "real" duplicate
	 of a forward parm decl.  */
      ddecl = old_decl;
      ddecl->isused = TRUE;
    }
  else if (!type_void(tempdecl.type))
    ddecl = declare(current_env, &tempdecl, FALSE);

  if (ddecl)
    {
      /* Forward transparent union property from union to parameter */
      if (type_union(ddecl->type) && type_tag(ddecl->type)->transparent_union)
	transparent_union_argument(ddecl);

      handle_attributes(extra_attributes, ddecl, NULL, NULL);
      handle_attributes(attributes, ddecl, NULL, NULL);
    }
  else
    {
      ignored_attributes(extra_attributes);
      ignored_attributes(attributes);
    }

  vd->ddecl = ddecl;
  return CAST(declaration, dd);
}

declaration declare_old_parameter(location l, cstring id)
{
  oldidentifier_decl d = new_oldidentifier_decl(parse_region, l, id, NULL);
  data_declaration ddecl;

  if ((ddecl = lookup_id(id.data, TRUE)))
    error("duplicate parameter name `%s' in parameter list", id.data);
  else
    {
      struct data_declaration tempdecl;

      /* The void type indicates that this is an old-style declaration */
      /* Note that isused is left FALSE to allow one declaration */
      init_data_declaration(&tempdecl, CAST(declaration, d), id.data,
			    void_type);
      tempdecl.kind = decl_variable;
      tempdecl.definition = tempdecl.ast;
      tempdecl.isexternalscope = FALSE;
      tempdecl.isfilescoperef = FALSE;
      tempdecl.needsmemory = FALSE;
      tempdecl.vtype = variable_normal;
      tempdecl.islocal = tempdecl.isparameter = TRUE;
      ddecl = declare(current_env, &tempdecl, FALSE);
    }
  d->ddecl = ddecl;

  return CAST(declaration, d);
}

/* Start definition of struct/union (indicated by skind) type tag. */
type_element start_struct(location l, ast_kind skind, word tag)
{
  tag_ref tref = newkind_tag_ref(parse_region, skind, l, tag, NULL, NULL, TRUE);
  tag_declaration tdecl = tag ? lookup_tag(tref, TRUE) : NULL;

  pending_xref_error();

  if (tdecl && tdecl->kind == skind)
    {
      if (tdecl->defined || tdecl->being_defined)
	{
	  error((skind == kind_union_ref ? "redefinition of `union %s'"
		 : "redefinition of `struct %s'"),
		tag->cstring.data);
	  tdecl = declare_tag(tref);
	}
    }
  else
    tdecl = declare_tag(tref);

  tref->tdecl = tdecl;
  tdecl->being_defined = TRUE;

  return CAST(type_element, tref);
}

/* Finish definition of struct/union furnishing the fields and attribs.
   Computes size and alignment of struct/union (see ASSUME: comments).
   Returns t */
type_element finish_struct(type_element t, declaration fields,
			   attribute attribs)
{
  tag_ref s = CAST(tag_ref, t);
  tag_declaration tdecl = s->tdecl;
  bool hasmembers = FALSE;
  field_declaration *nextfield = &tdecl->fieldlist;
  declaration fdecl;
  size_t offset = 0, alignment = BITSPERBYTE, size = 0;
  bool size_cc = TRUE;
  bool isunion = tdecl->kind == kind_union_ref;

  s->fields = fields;
  s->attributes = attribs;
  handle_attributes(attribs, NULL, NULL, tdecl);
  tdecl->fields = new_env(parse_region, NULL);
  tdecl->defined = TRUE;
  tdecl->being_defined = FALSE;

  scan_declaration (fdecl, fields)
    {
      declaration fieldlist;
      data_decl flist;
      field_decl field;

      /* Skip down to real list of fields */
      fieldlist = fdecl;
      while (is_extension_decl(fieldlist))
	fieldlist = CAST(extension_decl, fieldlist)->decl;
      flist = CAST(data_decl, fieldlist);

      scan_field_decl (field, CAST(field_decl, flist->decls))
	{
	  int class;
	  bool inlinep;
	  const char *name, *printname;
	  bool defaulted_int;
	  type field_type, base_field_type, tmpft;
	  location floc = field->location;
	  field_declaration fdecl = ralloc(parse_region, struct field_declaration);
	  long long bitwidth;
	  size_t falign, fsize;

	  parse_declarator(flist->modifiers, field->declarator,
			   field->arg1 != NULL, &class, &inlinep, &name,
			   &tmpft, &defaulted_int, NULL);
	  field_type = tmpft;

	  printname = name ? name : "(anonymous)";

	  if (name)
	    hasmembers = TRUE;

	  if (!tdecl->fields_const)
	    {
	      base_field_type = type_base(field_type);
	      if (type_const(base_field_type) ||
		  ((type_struct(base_field_type) || type_union(base_field_type)) &&
		   type_tag(base_field_type)->fields_const))
		tdecl->fields_const = TRUE;
	    }

	  /* XXX: Surely we should do the same as for const here ? */
	  if (type_volatile(field_type))
	    tdecl->fields_volatile = TRUE;

	  if (type_function(field_type))
	    {
	      error_with_location(floc, "field `%s' declared as a function", printname);
	      field_type = make_pointer_type(field_type);
	    }
	  else if (type_void(field_type))
	    {
	      error_with_location(floc, "field `%s' declared void", printname);
	      field_type = error_type;
	    }
	  else if (type_incomplete(field_type)) 
	    {
	      error_with_location(floc, "field `%s' has incomplete type", printname);
	      field_type = error_type;
	    }

	  *nextfield = fdecl;
	  nextfield = &fdecl->next;
#if 0
	  fdecl->next = NULL;
#endif
	  fdecl->name = name;
	  fdecl->type = field_type;
	  fdecl->ast = field;
	  if (name)
	    {
	      if (env_lookup(tdecl->fields, name, TRUE))
		error_with_location(floc, "duplicate member `%s'", name);
	      env_add(tdecl->fields, name, fdecl);
	    }

	  handle_attributes(field->attributes, NULL, fdecl, NULL);
	  handle_attributes(flist->attributes, NULL, fdecl, NULL);
	  field_type = fdecl->type; /* attributes might change type */

	  bitwidth = -1;
	  if (field->arg1)
	    {
	      known_cst cwidth = field->arg1->cst;

	      if (!type_integer(field_type))
		error_with_location(floc, "bit-field `%s' has invalid type", printname);
	      else if (!(type_integer(field->arg1->type) && constant_integral(cwidth)))
		error_with_location(floc, "bit-field `%s' width not an integer constant",
				    printname);
	      else
		{
		  largest_uint width = constant_uint_value(cwidth);

		  if (pedantic)
		    constant_overflow_warning(cwidth);

		  /* Detect and ignore out of range field width.  */
		  if (!type_unsigned(cwidth->type) &&
		      constant_sint_value(cwidth) < 0)
		    error_with_location(floc, "negative width in bit-field `%s'", printname);
		  else if (width > type_size(field_type) * BITSPERBYTE)
		    /* Note that in this case the field gets handled as non-bitfield and
		       compilation may be successful */
		    pedwarn_with_location(floc, "width of `%s' exceeds its type", printname);
		  else if (width == 0 && name)
		    error_with_location(floc, "zero width for bit-field `%s'", name);
		  else
		    bitwidth = width;
		}

#if 0
	      if (DECL_INITIAL (x) && pedantic
		  && TYPE_MAIN_VARIANT (TREE_TYPE (x)) != integer_type_node
		  && TYPE_MAIN_VARIANT (TREE_TYPE (x)) != unsigned_type_node
		  /* Accept an enum that's equivalent to int or unsigned int.  */
		  && !(TREE_CODE (TREE_TYPE (x)) == ENUMERAL_TYPE
		       && (TYPE_PRECISION (TREE_TYPE (x))
			   == TYPE_PRECISION (integer_type_node))))
		pedwarn_with_decl (x, "bit-field `%s' type invalid in ANSI C");


#if 0
	  if (TREE_CODE (TREE_TYPE (x)) == ENUMERAL_TYPE
	      && (width < min_precision (TYPE_MIN_VALUE (TREE_TYPE (x)),
					 TREE_UNSIGNED (TREE_TYPE (x)))
		  || width < min_precision (TYPE_MAX_VALUE (TREE_TYPE (x)),
					    TREE_UNSIGNED (TREE_TYPE (x)))))
	    warning_with_decl (x, "`%s' is narrower than values of its type");
#endif

#endif
	    }

	  /* ASSUME: for bitfields we're assuming that:
	     - alignment of record is forced to be at least that of the base
	       (except for width 0 which just forces alignment of the current offset
	       with the base type's alignment)
	     - bit fields cannot span more units of alignment of their type
	       than the type itself
	     This is gcc's behaviour when PCC_BITFIELD_TYPE_MATTERS is defined */

	  fdecl->bitwidth = bitwidth;
	  fdecl->offset_cc = size_cc || isunion;

	  if (type_size_cc(field_type))
	    fsize = type_size(field_type) * BITSPERBYTE;
	  else
	    {
	      size_cc = FALSE;
	      fsize = BITSPERBYTE; /* actual value is unimportant */
	    }
	  /* don't care about alignment if no size (type_incomplete(field_type)
	     is true, so we got an error above */
	  falign = (type_has_size(field_type) ? type_alignment(field_type) : 1)
	    * BITSPERBYTE;

	  if (bitwidth == 0)
	    {
	      offset = align_to(offset, falign);
	      falign = 1; /* No structure alignment implications */
	    }
	  else if (bitwidth > 0)
	    {
	      if (((offset + bitwidth + falign - 1) / falign - offset / falign)
		  > fsize / falign)
		offset = align_to(offset, falign);
	    }
	  else /* regular field */
	    {
	      offset = align_to(offset, falign); 
	      bitwidth = fsize;
	    }
	  fdecl->offset = offset;

	  if (!isunion)
	    {
	      offset += bitwidth;
	      size = offset;
	    }
	  else
	    size = bitwidth > size ? bitwidth : size;

	  alignment = lcm(alignment, falign); /* Note: GCC uses max. Must be hoping for powers of 2 */
	}
    }

  if (pedantic && !hasmembers)
    pedwarn("%s has no %smembers",
	    (s->kind == kind_union_ref ? "union" : "structure"),
	    (fields ? "named " : ""));

  /* ASSUME: see previous assume */
  tdecl->size = align_to(offset, alignment) / BITSPERBYTE;
  tdecl->alignment = alignment / BITSPERBYTE;
  tdecl->size_cc = size_cc;

  return t;
}

/* Return a reference to struct/union/enum (indicated by skind) type tag */
type_element xref_tag(location l, ast_kind skind, word tag)
{
  tag_ref tref = newkind_tag_ref(parse_region, skind, l, tag, NULL, NULL, FALSE);
  tag_declaration tdecl = lookup_tag(tref, FALSE);

  if (!tdecl)
    tdecl = declare_tag(tref);

  tref->tdecl = tdecl;

  return CAST(type_element, tref);
}

static known_cst last_enum_value;

/* Start definition of struct/union (indicated by skind) type tag. */
type_element start_enum(location l, word tag)
{
  enum_ref tref = new_enum_ref(parse_region, l, tag, NULL, NULL, TRUE);
  tag_declaration tdecl = tag ? lookup_tag(tref, TRUE) : NULL;

  pending_xref_error();

  if (tdecl && tdecl->kind == kind_enum_ref)
    {
      if (tdecl->defined)
	error("redefinition of `enum %s'", tag->cstring.data);
    }
  else
    tdecl = declare_tag(tref);

  tref->tdecl = tdecl;
  tdecl->being_defined = TRUE;
  tdecl->packed = flag_short_enums;
  last_enum_value = NULL;

  return CAST(type_element, tref);
}

/* Finish definition of enum furnishing the names and attribs.
   Returns t */
type_element finish_enum(type_element t, declaration names,
			 attribute attribs)
{
  tag_ref s = CAST(tag_ref, t);
  tag_declaration tdecl = s->tdecl;
  cval smallest, largest;
  bool enum_isunsigned;
  type type_smallest, type_largest, enum_reptype;
  enumerator v, values = CAST(enumerator, names);

  s->fields = names;
  s->attributes = attribs;
  handle_attributes(attribs, NULL, NULL, tdecl);
  tdecl->fields = 0;
  tdecl->defined = TRUE;
  tdecl->being_defined = FALSE;

  /* Pick a representation type for this enum. */

  /* First, find largest and smallest values defined in this enum. */
  if (!names)
    smallest = largest = cval_zero;
  else
    {
      smallest = largest = value_of_enumerator(values);

      scan_enumerator (v, CAST(enumerator, values->next))
	{
	  cval vv = value_of_enumerator(v);

	  if (cval_intcompare(vv, largest) > 0)
	    largest = vv;
	  if (cval_intcompare(vv, smallest) < 0)
	    smallest = vv;
	}
    }

  /* Pick a type that will hold the smallest and largest values. */
  enum_isunsigned = cval_intcompare(smallest, cval_zero) >= 0;
  type_smallest = type_for_cval(smallest, enum_isunsigned);
  type_largest = type_for_cval(largest, enum_isunsigned);
  assert(type_smallest);
  if (!type_largest)
    {
      assert(!enum_isunsigned);
      warning("enumeration values exceed range of largest integer");
      type_largest = long_long_type;
    }
  if (type_size(type_smallest) > type_size(type_largest))
    enum_reptype = type_smallest;
  else
    enum_reptype = type_largest;

  /* int is the smallest possible type for an enum (except if the 
     enum has the packed attribute) */
  if (!tdecl->packed && type_size(enum_reptype) < type_size(int_type))
    enum_reptype = int_type;

  tdecl->reptype = enum_reptype;
  tdecl->size_cc = TRUE;
  tdecl->size = type_size(enum_reptype);
  tdecl->alignment = type_alignment(enum_reptype);

  /* Change type of all enum constants to enum_reptype */
  scan_enumerator (v, values)
    {
      known_cst val = v->ddecl->value;

      val->type = enum_reptype;
      val->cval = cval_cast(val->cval, enum_reptype);
    }

  return t;
}

declaration make_enumerator(location loc, cstring id, expression value)
{
  declaration ast = CAST(declaration, new_enumerator(parse_region, loc, id, value, NULL));
  struct data_declaration tempdecl;
  data_declaration ddecl, old_decl;
  known_cst cval = NULL;

  init_data_declaration(&tempdecl, ast, id.data, int_type);
  tempdecl.kind = decl_constant;
  tempdecl.definition = ast;

  if (value)
    {
      if (!type_integer(value->type) || !value->cst ||
	  !constant_integral(value->cst))
	error("enumerator value for `%s' not integer constant", id.data);
      else
	cval = value->cst;
    }

  if (!cval)
    {
      /* Last value + 1 */
      if (last_enum_value)
	{
	  /* No clear logic anywhere to specify which type we should use
	     (ANSI C must specify int, cf warning below) */
	  type addtype = type_unsigned(last_enum_value->type) ?
	    unsigned_long_long_type : long_long_type;

	  cval = fold_add(addtype, last_enum_value, onecst);
	}
      else
	cval = zerocst;
    }

  if (pedantic && !cval_inrange(cval->cval, int_type))
    {
      pedwarn("ANSI C restricts enumerator values to range of `int'");
      cval = zerocst;
    }

  tempdecl.value = last_enum_value = cval;
  if (type_size(cval->type) >= type_size(int_type))
    tempdecl.value->type = cval->type;
  else
    tempdecl.value->type =
      type_for_size(type_size(int_type), flag_traditional && type_unsigned(cval->type));

  old_decl = lookup_id(id.data, TRUE);

  if (old_decl && duplicate_decls(&tempdecl, old_decl, FALSE, FALSE))
    ddecl = old_decl;
  else
    ddecl = declare(current_env, &tempdecl, FALSE);

  CAST(enumerator, ast)->ddecl = ddecl;

  return ast;
}

/* Create declaration of field 'elements d : bitfield' with attributes
   extra_attributes and attributes.
   d can be NULL, bitfield can be NULL, but not both at the same time.
   Returns the declaration for the field.
*/
declaration make_field(declarator d, expression bitfield,
		       type_element elements, attribute extra_attributes,
		       attribute attributes)
{
  /* We get at least one of a declarator or a bitfield */
  location l = d ? d->location : bitfield->location;

  return
    CAST(declaration,
	 new_field_decl(parse_region, l, d, extra_attributes, bitfield));
}


/* Create and return type 'elements d' where d is an absolute declarator */
asttype make_type(type_element elements, declarator d)
{
  location l = elements ? elements->location : d->location;
  int class;
  bool inlinep;
  const char *name;
  bool defaulted_int;
  asttype t = new_asttype(parse_region, l, d, elements);

  parse_declarator(t->qualifiers, t->declarator, FALSE, &class, &inlinep, &name, 
		   &t->type, &defaulted_int, NULL);
  assert(t->type && !(defaulted_int || inlinep || class || name));

  return t;
}


/* Save the rest of the line in the directives list. Return '\n' */
int save_directive(char *directive)
{
  static char *directive_buffer = NULL;
  static unsigned buffer_length = 0;
  char *p;
  char *buffer_limit;
  int looking_for = 0;
  int char_escaped = 0;

  if (buffer_length == 0)
    {
      directive_buffer = (char *)xmalloc (128);
      buffer_length = 128;
    }

  buffer_limit = &directive_buffer[buffer_length];

  for (p = directive_buffer; ; )
    {
      int c;

      /* Make buffer bigger if it is full.  */
      if (p >= buffer_limit)
        {
	  unsigned bytes_used = (p - directive_buffer);

	  buffer_length *= 2;
	  directive_buffer
	    = (char *)xrealloc (directive_buffer, buffer_length);
	  p = &directive_buffer[bytes_used];
	  buffer_limit = &directive_buffer[buffer_length];
        }

      c = getc (finput);

      /* Discard initial whitespace.  */
      if ((c == ' ' || c == '\t') && p == directive_buffer)
	continue;

      /* Detect the end of the directive.  */
      if (looking_for == 0
	  && (c == '\n' || c == EOF))
	{
          ungetc (c, finput);
	  c = '\0';
	}

      *p++ = c;

      if (c == 0)
	break;

      /* Handle string and character constant syntax.  */
      if (looking_for)
	{
	  if (looking_for == c && !char_escaped)
	    looking_for = 0;	/* Found terminator... stop looking.  */
	}
      else
        if (c == '\'' || c == '"')
	  looking_for = c;	/* Don't stop buffering until we see another
				   another one of these (or an EOF).  */

      /* Handle backslash.  */
      char_escaped = (c == '\\' && ! char_escaped);
    }
  return '\n';
}

/* Returns name of r */
static char *rid_name_int(int id)
{
  switch (id)
    {
    case RID_INT: return "int";
    case RID_CHAR: return "char";
    case RID_FLOAT: return "float";
    case RID_DOUBLE: return "double";
    case RID_VOID: return "void";
    case RID_UNSIGNED: return "unsigned";
    case RID_SHORT: return "short";
    case RID_LONG: return "long";
    case RID_AUTO: return "auto";
    case RID_STATIC: return "static";
    case RID_EXTERN: return "extern";
    case RID_REGISTER: return "register";
    case RID_TYPEDEF: return "typedef";
    case RID_SIGNED: return "signed";
    case RID_INLINE: return "__inline";
    case RID_COMPLEX: return "__complex";
    default: assert(0);
    }
  assert(0);
  return NULL;
}

/* Returns name of r */
char *rid_name(rid r)
{
  return rid_name_int(r->id);
}

/* If statement list l1 ends with an unfinished label, attach l2 to that
   label. Otherwise attach l2 to the end of l1 */
statement chain_with_labels(statement l1, statement l2)
{
  node last, last_label;

  if (!l1) return l2;
  if (!l2) return l1;

  last_label = last = last_node(CAST(node, l1));
  /* There may be an unfinished sub-label due to 'a: b:' */
  while (last_label->kind == kind_labeled_stmt)
    {
      labeled_stmt ls = CAST(labeled_stmt, last_label);

      if (!ls->stmt) /* An unfinished labeled statement */
	{
	  ls->stmt = l2;
	  return l1;
	}
      last_label = CAST(node, ls->stmt);
    }

  last->next = CAST(node, l2);

  return l1;
}

void split_type_elements(type_element tlist, type_element *odeclspecs,
			 attribute *oattributes)
{
  type_element declspecs = NULL;
  attribute attributes = NULL;

  while (tlist)
    {
      type_element te = tlist;
      tlist = CAST(type_element, tlist->next);
      te->next = NULL;

      if (te->kind == kind_attribute)
	attributes = attribute_chain(attributes, CAST(attribute, te));
      else
	declspecs = type_element_chain(declspecs, te);
    }
  *odeclspecs = declspecs;
  *oattributes = attributes;
}

void init_semantics(void)
{
  global_env = current_env = new_environment(parse_region, NULL, FALSE);

  bad_decl = ralloc(parse_region, struct data_declaration);
  bad_decl->kind = decl_error;
  bad_decl->name = "undeclared";
  bad_decl->type = error_type;
  bad_decl->ast = new_error_decl(parse_region, last_location);

  implicit_function_type = make_function_type(int_type, NULL, FALSE, TRUE);

  /* Create the global bindings for __FUNCTION__ and __PRETTY_FUNCTION__.  */
  declare_function_name ();

  /* Declare builtin type __builtin_va_list */
  declare_builtin_types();
  declare_builtin_identifiers();

  onecst = make_signed_cst(1, int_type);
  zerocst = make_signed_cst(0, int_type);
  oneexpr = build_uint_constant(parse_region, dummy_location, size_t_type, 1);
}
