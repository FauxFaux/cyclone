/*
 * Copyright (c) 2000-2004
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef STAMP_H
#define STAMP_H

#include <stdio.h>
#include "regions.h"
#include "linkage.h"
#include "list.h"

EXTERN_C_BEGIN

typedef int stamp;

DECLARE_LIST(stamp_list,stamp);

stamp stamp_fresh(void);
stamp stamp_fresh_small(void);
stamp stamp_fresh_large(void);

stamp stamp_string(const char *) deletes;

const char *stamp_to_str(region r,stamp st);

void stamp_reset(void) deletes;
void stamp_init(void);

void stamp_serialize(FILE *f);
void stamp_deserialize(FILE *f);
void stamp_set_fields(void);

/* Taken from here: http://www.concentric.net/~Ttwang/tech/inthash.htm  */
unsigned long stamp_hash(void *key); 

bool stamp_eq(void *s1, void *s2);

/* Region persistence */
void write_module_stamp(FILE *f);
void update_module_stamp(translation t, FILE *f);

EXTERN_C_END

#endif /* STAMP_H */



