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

#include <assert.h>
#include "jcollection.h"
#include "hashset.h"
#include "termhash.h"

/* 
   static term_hash jcoll_hash;
 */

struct jcoll_dict
{
  region r;
  term_hash hash; 
  get_stamp_fn_ptr get_stamp;
};

enum jcoll_type
{
  j_single,
  j_chain,
  j_join
};

/* generic jcoll type */ 
struct jcoll
{
  enum jcoll_type type;
  stamp st;
};

struct jcoll_single
{
  enum jcoll_type type;
  stamp st;
  gen_e entry;
};

struct jcoll_chain
{
  enum jcoll_type type;
  stamp st;
  gen_e_list sameregion entries;
};

struct jcoll_join
{
  enum jcoll_type type;
  stamp st;
  jcoll_list sameregion joins;
  gen_e_list sameregion cache;
};

typedef struct jcoll_single *jcoll_single;
typedef struct jcoll_chain *jcoll_chain;
typedef struct jcoll_join *jcoll_join;

DEFINE_NONPTR_LIST(jcoll_list,jcoll);

jcoll jcoll_new(jcoll_dict d, gen_e e)
{
  jcoll_single result = ralloc(d->r, struct jcoll_single);
  result->type = j_single;
  result->st = stamp_fresh();
  result->entry = e;
  return (jcoll)result;
}

jcoll jcoll_jjoin(jcoll_dict d,jcoll_list list)
{

  if (jcoll_list_empty(list))
    return NULL;
  else if (jcoll_list_length(list) == 1)
    return jcoll_list_head(list);

  else
    {
      int i = 0,
	length = jcoll_list_length(list) + 1;
      stamp sts[length];
      jcoll_join result;
      
      jcoll_list_scanner scan;
      jcoll temp;
      
      sts[i++] = j_join;

      jcoll_list_scan(list,&scan);
      while (jcoll_list_next(&scan,&temp))
	{
	  stamp st = temp ? temp->st : 0;
	  sts[i++] = st;
	}
      qsort(&sts[1],length-1,sizeof(int),(int (*)(const void *, const void *)) ptr_cmp);
      
      if ( NULL == (result = (jcoll_join)term_hash_find(d->hash,sts,length)) )
	{
	  result = ralloc(d->r,struct jcoll_join);
	  
	  result->type = j_join;
	  result->st = stamp_fresh();
	  result->joins = list;
	  result->cache = new_gen_e_list(d->r);
	  term_hash_insert(d->hash,(gen_e)result,sts,length);
	}
      return (jcoll)result;
    }

}

/*
  Hash chains 
 */
jcoll jcoll_create_chain(jcoll_dict d, gen_e_list elems)
{
  int i = 0,
    length = gen_e_list_length(elems) + 1;
  stamp sts[length];
  gen_e_list_scanner scan;
  gen_e temp;
  jcoll_chain result;

  sts[i++] = j_chain; 
  
  gen_e_list_scan(elems,&scan);
  while (gen_e_list_next(&scan,&temp))
    {
      sts[i++] = d->get_stamp(temp);
    }
  qsort(&sts[1],length-1,sizeof(int),(int (*)(const void *, const void *))ptr_cmp); // FIX, first pos should always be chain

  if ( NULL == (result = (jcoll_chain)term_hash_find(d->hash,sts,length)) )
    {
      result = ralloc(d->r,struct jcoll_chain);
      result->type = j_chain;
      result->st = stamp_fresh();
      result->entries = elems;
      term_hash_insert(d->hash,(gen_e)result,sts,
		       length);
    }
  return (jcoll)result;
}

typedef void (*japp_fn_ptr) (void *);

static void app_aux(hash_set h, get_stamp_fn_ptr get_stamp, japp_fn_ptr app,
		    jcoll j)
{
  if (! j)
    return;

  switch(j->type)
    {
    case j_single:
      {
	jcoll_single single = (jcoll_single) j;
	
	if (! hs_member(h,get_stamp(single->entry)) )
	    app(single->entry);
      }
      break;
    case j_chain:
      {
	jcoll_chain chain = (jcoll_chain) j;

	if (! hs_member(h,chain->st) )
	  {
	    gen_e_list_scanner scan;
	    gen_e entry;
	    
	    gen_e_list_scan(chain->entries, &scan);
	    while (gen_e_list_next(&scan, &entry))
	      {
		if (! hs_member(h, get_stamp(entry)) )
		  app(entry);
	      }
				   
	  }
	
      }
      break;
    case j_join:
      {
	jcoll_join join = (jcoll_join) j;

	if (! hs_member(h, join->st))
	  {
	    if (! gen_e_list_empty(join->cache))
	      {
		  gen_e_list_scanner scan;
		  gen_e entry;
	    
		  gen_e_list_scan(join->cache, &scan);
		  while (gen_e_list_next(&scan, &entry))
		    {
		      if (! hs_member(h, get_stamp(entry)) )
			app(entry);
		    }
	      }
	    else 
	      {
		jcoll_list_scanner scan;
		jcoll temp;

		jcoll_list_scan(join->joins, &scan);
		while (jcoll_list_next(&scan,&temp))
		  {
		    app_aux(h,get_stamp,app,temp);
		  }

	      }
	  }

      }
      break;
    }

}

static void jcoll_app(jcoll_dict d, japp_fn_ptr app, jcoll j) deletes
{
  region scratch_rgn = newregion();
  hash_set hash = hs_create(scratch_rgn);
  app_aux(hash,d->get_stamp, app, j);
  hs_delete(hash);
  deleteregion(scratch_rgn);
}
 
static gen_e_list jf_accum;
  
static void jcoll_accum(void *e) {
	gen_e_list_cons((gen_e) e, jf_accum);
}

gen_e_list jcoll_flatten(jcoll_dict d, jcoll j) deletes
{
  if (j == NULL)
    return new_gen_e_list(d->r);

  switch (j->type)
    {
    case j_single:
      {
	jcoll_single single = (jcoll_single)j;
	
	jf_accum = new_gen_e_list(d->r);
	gen_e_list_cons(single->entry,jf_accum);
      }
      break;
    case j_chain:
      {
	jcoll_chain chain = (jcoll_chain)j;
	/* accum = gen_e_list_copy(r,chain->entries); */
	jf_accum = chain->entries;
      }
      break;
    case j_join:
      {
	jcoll_join join = (jcoll_join)j;
	
	if (! gen_e_list_empty(join->cache))
	  return join->cache;
	else
	  {
	    jf_accum = new_gen_e_list(d->r);
	    jcoll_app(d, jcoll_accum,j);
	    
	    gen_e_list_append(join->cache,jf_accum /* gen_e_list_copy(r,accum)*/);
	  }
      }
      break;
    }

  return jf_accum;
}

jcoll_dict jcoll_create_dict(region r,get_stamp_fn_ptr get_stamp)
{
  jcoll_dict result = ralloc(r,struct jcoll_dict);

  result->r = r;
  result->hash = make_term_hash(r);
  result->get_stamp = get_stamp;
  return result;
}


void jcoll_delete_dict(jcoll_dict d)
{
  term_hash_delete(d->hash);
}
