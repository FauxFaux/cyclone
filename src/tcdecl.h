/* Type checking declarations.
   Copyright (C) 2001 Greg Morrisett
   This file is part of the Cyclone compiler.

   The Cyclone compiler is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The Cyclone compiler is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the Cyclone compiler; see the file COPYING. If not,
   write to the Free Software Foundation, Inc., 59 Temple Place -
   Suite 330, Boston, MA 02111-1307, USA. */


#ifndef _TCDECL_H_
#define _TCDECL_H_

#include <stdio.h>
#include <position.h>
#include <list.h>
#include "absyn.h"

namespace Tcdecl {
  using Absyn;
  //  using String;
  using Position;

  extern datatype exn {extern Incompatible};

  // used by Interface
  struct Xdatatypefielddecl {
    datatypedecl_t base; //no fields here
    datatypefield_t field;
  };
  typedef struct Xdatatypefielddecl @ xdatatypefielddecl_t;

  // if msg0 == null, don't print any message; just throw exception Incompatible
  // otherwise call terr(loc, msg) where msg is *msg0 + " " + msg1
extern void merr(seg_t loc, string_t<`r2> * msg1, string_t fmt,
		 ... inject parg_t<`r> ap : {`r2} > `r) 
  __attribute__((format(printf,3,4)));

  // all these functions work the same way :
  // _ they check compatibility between d0 (previous top-level declaration)
  //   and d1 (new top-level declaration)
  // _ if something's wrong, call merge_err and return null (or Unresolved_b for merge_binding)
  // _ don't modify d0 or d1
  // _ if d1 is included in d0 (i.e. d1 doesn't bring any new information w.r.t d0), return d0 itself
  //   hence the test for inclusion d1 <= d0 can be written : d0 == merge(d0, d1)
  // _ from a 'semantic' point of view, merge(d0, d1) = merge(d1, d0)
  // _ we assume the declarations have already been type-checked

  // t and v describe the type and the name of the object involved
  // we allow externC and extern on aggregate or enum definitions
extern $(scope_t, bool) merge_scope(scope_t s0, scope_t s1,  
				    string_t t, string_t v,
				    seg_t loc, string_t * msg,
                                    bool allow_externC_extern_merge);

extern struct Aggrdecl  * merge_aggrdecl(aggrdecl_t d0, aggrdecl_t d1,
					 seg_t loc, string_t * msg);

extern struct Datatypedecl  * merge_datatypedecl(datatypedecl_t d0, datatypedecl_t d1,
					     seg_t loc, string_t * msg);
extern struct Enumdecl    * merge_enumdecl(enumdecl_t d0, enumdecl_t d1, 
					   seg_t loc, string_t * msg);
extern struct Vardecl     * merge_vardecl(vardecl_t d0, vardecl_t d1, 
					  seg_t loc, string_t * msg);
extern struct Typedefdecl * merge_typedefdecl(typedefdecl_t d0,
					      typedefdecl_t d1,
					      seg_t loc, string_t * msg);

extern binding_t            merge_binding    (binding_t d0, binding_t d1,
					      seg_t loc, string_t * msg);
  
extern struct Xdatatypefielddecl * 
  merge_xdatatypefielddecl (xdatatypefielddecl_t d0, xdatatypefielddecl_t d1, 
                          seg_t loc, string_t * msg);

  // sort the list of fields, trying to merge the duplicate
  // *res is set to false if something's wrong
extern List::list_t<datatypefield_t> 
sort_xdatatype_fields(List::list_t<datatypefield_t,`H> f, bool @ res,
		    var_t v, seg_t loc, string_t * msg);
}

#endif /* _TCDECL_H_ */
