/* Translate Cyclone abstract syntax to C abstract syntax.
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


#ifndef _TOC_H_
#define _TOC_H_

#include <hashtable.h>
#include "absyn.h"

namespace Toc {
using List;
using Absyn;

typedef Hashtable::table_t<`a,`b> table_t<`a,`b>;
 
 
// translate the declarations to C
list_t<decl_t> toc(table_t<fndecl_t,table_t<stmt_t,int>> pop_tables,
		   list_t<decl_t>);

// translate a Cyclone type to a C type (functionally)
type_t typ_to_c(type_t);
// this is just for other translation files
qvar_t temp_var();
extern stringptr_t ?globals;
// for passing New_e destinations to RemoveAggrs
@extensible datatype Absyn::AbsynAnnot { 
  extern NewInfo(exp_t,type_t);
};
void init();
void finish(); // call this when done with a batch of typ_to_c calls
}
#endif
