// This is a C header file to be used by the output of the Cyclone
// to C translator.  The corresponding definitions are in file lib/runtime_cyc.c
#ifndef _CYC_INCLUDE_H_
#define _CYC_INCLUDE_H_

#include <setjmp.h>

#ifdef NO_CYC_PREFIX
#define ADD_PREFIX(x) x
#else
#define ADD_PREFIX(x) Cyc_##x
#endif

#ifndef offsetof
// should be size_t, but int is fine.
#define offsetof(t,n) ((int)(&(((t *)0)->n)))
#endif

//// Tagged arrays
struct _tagged_arr { 
  unsigned char *curr; 
  unsigned char *base; 
  unsigned char *last_plus_one; 
};

//// Discriminated Unions
struct _xtunion_struct { char *tag; };

// Need one of these per thread (we don't have threads)
// The runtime maintains a stack that contains either _handler_cons
// structs or _RegionHandle structs.  The tag is 0 for a handler_cons
// and 1 for a region handle.  
struct _RuntimeStack {
  int tag; // 0 for an exception handler, 1 for a region handle
  struct _RuntimeStack *next;
};

//// Regions
struct _RegionPage {
#ifdef CYC_REGION_PROFILE
  unsigned total_bytes;
  unsigned free_bytes;
#endif
  struct _RegionPage *next;
  char data[0];
};

struct _RegionHandle {
  struct _RuntimeStack s;
  struct _RegionPage *curr;
  char               *offset;
  char               *last_plus_one;
#ifdef CYC_REGION_PROFILE
  const char         *name;
#endif
};

extern struct _RegionHandle _new_region(const char *);
extern void * _region_malloc(struct _RegionHandle *, unsigned);
extern void * _region_calloc(struct _RegionHandle *, unsigned t, unsigned n);
extern void   _free_region(struct _RegionHandle *);
extern void   _reset_region(struct _RegionHandle *);

//// Exceptions 
struct _handler_cons {
  struct _RuntimeStack s;
  jmp_buf handler;
};
extern void _push_handler(struct _handler_cons *);
extern void _push_region(struct _RegionHandle *);
extern void _npop_handler(int);
extern void _pop_handler();
extern void _pop_region();

#ifndef _throw
extern int _throw_null();
extern int _throw_arraybounds();
extern int _throw_badalloc();
extern int _throw(void* e);
#endif

extern struct _xtunion_struct *_exn_thrown;

//// Built-in Exceptions
extern struct _xtunion_struct ADD_PREFIX(Null_Exception_struct);
extern struct _xtunion_struct * ADD_PREFIX(Null_Exception);
extern struct _xtunion_struct ADD_PREFIX(Array_bounds_struct);
extern struct _xtunion_struct * ADD_PREFIX(Array_bounds);
extern struct _xtunion_struct ADD_PREFIX(Match_Exception_struct);
extern struct _xtunion_struct * ADD_PREFIX(Match_Exception);
extern struct _xtunion_struct ADD_PREFIX(Bad_alloc_struct);
extern struct _xtunion_struct * ADD_PREFIX(Bad_alloc);

//// Built-in Run-time Checks and company
#ifdef NO_CYC_NULL_CHECKS
#define _check_null(ptr) (ptr)
#else
#define _check_null(ptr) \
  ({ void*_check_null_temp = (void*)(ptr); \
     if (!_check_null_temp) _throw_null(); \
     _check_null_temp; })
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  ((char *)ptr) + (elt_sz)*(index); })
#else
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  void*_cks_ptr = (void*)(ptr); \
  unsigned _cks_bound = (bound); \
  unsigned _cks_elt_sz = (elt_sz); \
  unsigned _cks_index = (index); \
  if (!_cks_ptr) _throw_null(); \
  if (!_cks_index >= _cks_bound) _throw_arraybounds(); \
  ((char *)_cks_ptr) + _cks_elt_sz*_cks_index; })
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_known_subscript_notnull(bound,index) (index)
#else
#define _check_known_subscript_notnull(bound,index) ({ \
  unsigned _cksnn_bound = (bound); \
  unsigned _cksnn_index = (index); \
  if (_cksnn_index >= _cksnn_bound) _throw_arraybounds(); \
  _cksnn_index; })
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_unknown_subscript(arr,elt_sz,index) ({ \
  struct _tagged_arr _cus_arr = (arr); \
  unsigned _cus_elt_sz = (elt_sz); \
  unsigned _cus_index = (index); \
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index; \
  _cus_ans; })
#else
#define _check_unknown_subscript(arr,elt_sz,index) ({ \
  struct _tagged_arr _cus_arr = (arr); \
  unsigned _cus_elt_sz = (elt_sz); \
  unsigned _cus_index = (index); \
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index; \
  if (!_cus_arr.base) _throw_null(); \
  if (_cus_ans < _cus_arr.base || _cus_ans >= _cus_arr.last_plus_one) \
    _throw_arraybounds(); \
  _cus_ans; })
#endif

#define _tag_arr(tcurr,elt_sz,num_elts) ({ \
  struct _tagged_arr _tag_arr_ans; \
  _tag_arr_ans.base = _tag_arr_ans.curr = (void*)(tcurr); \
  _tag_arr_ans.last_plus_one = _tag_arr_ans.base + (elt_sz) * (num_elts); \
  _tag_arr_ans; })

#define _init_tag_arr(arr_ptr,arr,elt_sz,num_elts) ({ \
  struct _tagged_arr *_itarr_ptr = (arr_ptr); \
  void* _itarr = (arr); \
  _itarr_ptr->base = _itarr_ptr->curr = _itarr; \
  _itarr_ptr->last_plus_one = ((char *)_itarr) + (elt_sz) * (num_elts); \
  _itarr_ptr; })

#ifdef NO_CYC_BOUNDS_CHECKS
#define _untag_arr(arr,elt_sz,num_elts) ((arr).curr)
#else
#define _untag_arr(arr,elt_sz,num_elts) ({ \
  struct _tagged_arr _arr = (arr); \
  unsigned char *_curr = _arr.curr; \
  if (_curr < _arr.base || _curr + (elt_sz) * (num_elts) > _arr.last_plus_one)\
    _throw_arraybounds(); \
  _curr; })
#endif

#define _get_arr_size(arr,elt_sz) \
  ({struct _tagged_arr _get_arr_size_temp = (arr); \
    unsigned char *_get_arr_size_curr=_get_arr_size_temp.curr; \
    unsigned char *_get_arr_size_last=_get_arr_size_temp.last_plus_one; \
    (_get_arr_size_curr < _get_arr_size_temp.base || \
     _get_arr_size_curr >= _get_arr_size_last) ? 0 : \
    ((_get_arr_size_last - _get_arr_size_curr) / (elt_sz));})

#define _tagged_arr_plus(arr,elt_sz,change) ({ \
  struct _tagged_arr _ans = (arr); \
  _ans.curr += ((int)(elt_sz))*(change); \
  _ans; })

#define _tagged_arr_inplace_plus(arr_ptr,elt_sz,change) ({ \
  struct _tagged_arr * _arr_ptr = (arr_ptr); \
  _arr_ptr->curr += ((int)(elt_sz))*(change); \
  *_arr_ptr; })

#define _tagged_arr_inplace_plus_post(arr_ptr,elt_sz,change) ({ \
  struct _tagged_arr * _arr_ptr = (arr_ptr); \
  struct _tagged_arr _ans = *_arr_ptr; \
  _arr_ptr->curr += ((int)(elt_sz))*(change); \
  _ans; })

//// Allocation
extern void* GC_malloc(int);
extern void* GC_malloc_atomic(int);
extern void* GC_calloc(unsigned,unsigned);
extern void* GC_calloc_atomic(unsigned,unsigned);

static inline void* _cycalloc(int n) {
  void * ans = (void *)GC_malloc(n);
  if(!ans)
    _throw_badalloc();
  return ans;
}
static inline void* _cycalloc_atomic(int n) {
  void * ans = (void *)GC_malloc_atomic(n);
  if(!ans)
    _throw_badalloc();
  return ans;
}
static inline void* _cyccalloc(unsigned n, unsigned s) {
  void* ans = (void*)GC_calloc(n,s);
  if (!ans)
    _throw_badalloc();
  return ans;
}
static inline void* _cyccalloc_atomic(unsigned n, unsigned s) {
  void* ans = (void*)GC_calloc_atomic(n,s);
  if (!ans)
    _throw_badalloc();
  return ans;
}
#define MAX_MALLOC_SIZE (1 << 28)
static inline unsigned int _check_times(unsigned x, unsigned y) {
  unsigned long long whole_ans = 
    ((unsigned long long)x)*((unsigned long long)y);
  unsigned word_ans = (unsigned)whole_ans;
  if(word_ans < whole_ans || word_ans > MAX_MALLOC_SIZE)
    _throw_badalloc();
  return word_ans;
}

#if defined(CYC_REGION_PROFILE) 
extern void* _profile_GC_malloc(int,char *file,int lineno);
extern void* _profile_GC_malloc_atomic(int,char *file,int lineno);
extern void* _profile_region_malloc(struct _RegionHandle *, unsigned,
                                     char *file,int lineno);
extern struct _RegionHandle _profile_new_region(const char *rgn_name,
						char *file,int lineno);
extern void _profile_free_region(struct _RegionHandle *,
				 char *file,int lineno);
#  if !defined(RUNTIME_CYC)
#define _new_region(n) _profile_new_region(n,__FILE__ ":" __FUNCTION__,__LINE__)
#define _free_region(r) _profile_free_region(r,__FILE__ ":" __FUNCTION__,__LINE__)
#define _region_malloc(rh,n) _profile_region_malloc(rh,n,__FILE__ ":" __FUNCTION__,__LINE__)
#  endif
#define _cycalloc(n) _profile_GC_malloc(n,__FILE__ ":" __FUNCTION__,__LINE__)
#define _cycalloc_atomic(n) _profile_GC_malloc_atomic(n,__FILE__ ":" __FUNCTION__,__LINE__)
#endif
#endif
 struct Cyc_Std__types_fd_set{int fds_bits[2];};struct Cyc_Core_Opt{void*v;};struct
_tagged_arr Cyc_Core_new_string(unsigned int);extern char Cyc_Core_Invalid_argument[
21];struct Cyc_Core_Invalid_argument_struct{char*tag;struct _tagged_arr f1;};extern
char Cyc_Core_Failure[12];struct Cyc_Core_Failure_struct{char*tag;struct
_tagged_arr f1;};extern char Cyc_Core_Impossible[15];struct Cyc_Core_Impossible_struct{
char*tag;struct _tagged_arr f1;};extern char Cyc_Core_Not_found[14];extern char Cyc_Core_Unreachable[
16];struct Cyc_Core_Unreachable_struct{char*tag;struct _tagged_arr f1;};struct Cyc_List_List{
void*hd;struct Cyc_List_List*tl;};int Cyc_List_length(struct Cyc_List_List*x);
extern char Cyc_List_List_mismatch[18];extern char Cyc_List_Nth[8];struct Cyc_Rope_Rope_node;
struct Cyc_Rope_Rope_node*Cyc_Rope_from_string(struct _tagged_arr);struct
_tagged_arr Cyc_Rope_to_string(struct Cyc_Rope_Rope_node*);struct Cyc_Rope_Rope_node*
Cyc_Rope_concat(struct Cyc_Rope_Rope_node*,struct Cyc_Rope_Rope_node*);struct Cyc_Rope_Rope_node*
Cyc_Rope_concata(struct _tagged_arr);struct Cyc_Rope_Rope_node*Cyc_Rope_concatl(
struct Cyc_List_List*);unsigned int Cyc_Rope_length(struct Cyc_Rope_Rope_node*);int
Cyc_Rope_cmp(struct Cyc_Rope_Rope_node*,struct Cyc_Rope_Rope_node*);unsigned int
Cyc_Std_strlen(struct _tagged_arr s);int Cyc_Std_strcmp(struct _tagged_arr s1,struct
_tagged_arr s2);struct _tagged_arr Cyc_Std_strncpy(struct _tagged_arr,struct
_tagged_arr,unsigned int);struct Cyc_Rope_String_rope_struct{int tag;struct
_tagged_arr f1;};struct Cyc_Rope_Array_rope_struct{int tag;struct _tagged_arr f1;};
struct Cyc_Rope_Rope_node{void*v;};struct Cyc_Rope_Rope_node*Cyc_Rope_from_string(
struct _tagged_arr s){return({struct Cyc_Rope_Rope_node*_tmp0=_cycalloc(sizeof(*
_tmp0));_tmp0->v=(void*)((void*)({struct Cyc_Rope_String_rope_struct*_tmp1=
_cycalloc(sizeof(*_tmp1));_tmp1[0]=({struct Cyc_Rope_String_rope_struct _tmp2;
_tmp2.tag=0;_tmp2.f1=s;_tmp2;});_tmp1;}));_tmp0;});}struct Cyc_Rope_Rope_node*Cyc_Rope_concat(
struct Cyc_Rope_Rope_node*r1,struct Cyc_Rope_Rope_node*r2){return({struct Cyc_Rope_Rope_node*
_tmp3=_cycalloc(sizeof(*_tmp3));_tmp3->v=(void*)((void*)({struct Cyc_Rope_Array_rope_struct*
_tmp4=_cycalloc(sizeof(*_tmp4));_tmp4[0]=({struct Cyc_Rope_Array_rope_struct _tmp5;
_tmp5.tag=1;_tmp5.f1=_tag_arr(({struct Cyc_Rope_Rope_node**_tmp6=_cycalloc(
sizeof(struct Cyc_Rope_Rope_node*)* 2);_tmp6[0]=r1;_tmp6[1]=r2;_tmp6;}),sizeof(
struct Cyc_Rope_Rope_node*),2);_tmp5;});_tmp4;}));_tmp3;});}struct Cyc_Rope_Rope_node*
Cyc_Rope_concata(struct _tagged_arr rs){return({struct Cyc_Rope_Rope_node*_tmp7=
_cycalloc(sizeof(*_tmp7));_tmp7->v=(void*)((void*)({struct Cyc_Rope_Array_rope_struct*
_tmp8=_cycalloc(sizeof(*_tmp8));_tmp8[0]=({struct Cyc_Rope_Array_rope_struct _tmp9;
_tmp9.tag=1;_tmp9.f1=rs;_tmp9;});_tmp8;}));_tmp7;});}struct Cyc_Rope_Rope_node*
Cyc_Rope_concatl(struct Cyc_List_List*l){return({struct Cyc_Rope_Rope_node*_tmpA=
_cycalloc(sizeof(*_tmpA));_tmpA->v=(void*)((void*)({struct Cyc_Rope_Array_rope_struct*
_tmpB=_cycalloc(sizeof(*_tmpB));_tmpB[0]=({struct Cyc_Rope_Array_rope_struct _tmpC;
_tmpC.tag=1;_tmpC.f1=({unsigned int _tmpD=(unsigned int)((int(*)(struct Cyc_List_List*
x))Cyc_List_length)(l);struct Cyc_Rope_Rope_node**_tmpE=(struct Cyc_Rope_Rope_node**)
_cycalloc(_check_times(sizeof(struct Cyc_Rope_Rope_node*),_tmpD));struct
_tagged_arr _tmp10=_tag_arr(_tmpE,sizeof(struct Cyc_Rope_Rope_node*),(unsigned int)((
int(*)(struct Cyc_List_List*x))Cyc_List_length)(l));{unsigned int _tmpF=_tmpD;
unsigned int i;for(i=0;i < _tmpF;i ++){_tmpE[i]=({struct Cyc_Rope_Rope_node*r=(
struct Cyc_Rope_Rope_node*)((struct Cyc_List_List*)_check_null(l))->hd;l=l->tl;r;});}}
_tmp10;});_tmpC;});_tmpB;}));_tmpA;});}unsigned int Cyc_Rope_length(struct Cyc_Rope_Rope_node*
r){void*_tmp11=(void*)r->v;struct _tagged_arr _tmp12;struct _tagged_arr _tmp13;_LL1:
if(*((int*)_tmp11)!= 0)goto _LL3;_tmp12=((struct Cyc_Rope_String_rope_struct*)
_tmp11)->f1;_LL2: return(unsigned int)Cyc_Std_strlen(_tmp12);_LL3: if(*((int*)
_tmp11)!= 1)goto _LL0;_tmp13=((struct Cyc_Rope_Array_rope_struct*)_tmp11)->f1;_LL4: {
unsigned int total=0;unsigned int sz=_get_arr_size(_tmp13,sizeof(struct Cyc_Rope_Rope_node*));{
unsigned int i=0;for(0;i < sz;i ++){total +=Cyc_Rope_length(((struct Cyc_Rope_Rope_node**)
_tmp13.curr)[(int)i]);}}return total;}_LL0:;}static unsigned int Cyc_Rope_flatten_it(
struct _tagged_arr s,unsigned int i,struct Cyc_Rope_Rope_node*r){void*_tmp14=(void*)
r->v;struct _tagged_arr _tmp15;struct _tagged_arr _tmp16;_LL6: if(*((int*)_tmp14)!= 0)
goto _LL8;_tmp15=((struct Cyc_Rope_String_rope_struct*)_tmp14)->f1;_LL7: {
unsigned int _tmp17=Cyc_Std_strlen(_tmp15);Cyc_Std_strncpy(_tagged_arr_plus(s,
sizeof(char),(int)i),_tmp15,_tmp17);return i + _tmp17;}_LL8: if(*((int*)_tmp14)!= 1)
goto _LL5;_tmp16=((struct Cyc_Rope_Array_rope_struct*)_tmp14)->f1;_LL9: {
unsigned int _tmp18=_get_arr_size(_tmp16,sizeof(struct Cyc_Rope_Rope_node*));{int j=
0;for(0;j < _tmp18;j ++){i=Cyc_Rope_flatten_it(s,i,((struct Cyc_Rope_Rope_node**)
_tmp16.curr)[j]);}}return i;}_LL5:;}struct _tagged_arr Cyc_Rope_to_string(struct Cyc_Rope_Rope_node*
r){struct _tagged_arr s=Cyc_Core_new_string(Cyc_Rope_length(r));Cyc_Rope_flatten_it(
s,0,r);(void*)(r->v=(void*)((void*)({struct Cyc_Rope_String_rope_struct*_tmp19=
_cycalloc(sizeof(*_tmp19));_tmp19[0]=({struct Cyc_Rope_String_rope_struct _tmp1A;
_tmp1A.tag=0;_tmp1A.f1=(struct _tagged_arr)s;_tmp1A;});_tmp19;})));return s;}int
Cyc_Rope_cmp(struct Cyc_Rope_Rope_node*r1,struct Cyc_Rope_Rope_node*r2){return Cyc_Std_strcmp((
struct _tagged_arr)Cyc_Rope_to_string(r1),(struct _tagged_arr)Cyc_Rope_to_string(
r2));}
