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

#ifndef UTIL_H
#define UTIL_H

#include "cstring.h"

double gettime(void);

/* A generic closure is defined as:
typedef struct closure_type_name {
  result (*fn)(struct closure_type_name *closure, other arguments);
  -- typically include fields for all alternatives to simplify life
  type1 data1;
  ...
  typen datan;
} *closure_type_name;
*/

#define CALL0(closure) ((closure)->fn((closure)))
#define CALL1(closure, a1) ((closure)->fn((closure), a1))
#define CALL2(closure, a1, a2) ((closure)->fn((closure), a1, a2))
#define CALL3(closure, a1, a2, a3) ((closure)->fn((closure), a1, a2, a3))

unsigned long align_to(unsigned long n, unsigned long alignment);

/* least common multiple */
unsigned long lcm(unsigned long x, unsigned long y);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

DECLARE_ARRAY(wchar_array, wchar_t)

#endif
