#include <setjmp.h>
/* This is a C header file to be used by the output of the Cyclone to
   C translator.  The corresponding definitions are in file lib/runtime_*.c */
#ifndef _CYC_INCLUDE_H_
#define _CYC_INCLUDE_H_

/* Need one of these per thread (see runtime_stack.c). The runtime maintains 
   a stack that contains either _handler_cons structs or _RegionHandle structs.
   The tag is 0 for a handler_cons and 1 for a region handle.  */
struct _RuntimeStack {
  int tag; 
  struct _RuntimeStack *next;
  void (*cleanup)(struct _RuntimeStack *frame);
};

#ifndef offsetof
/* should be size_t, but int is fine. */
#define offsetof(t,n) ((int)(&(((t *)0)->n)))
#endif

/* Fat pointers */
struct _fat_ptr {
  unsigned char *curr; 
  unsigned char *base; 
  unsigned char *last_plus_one; 
};  

/* Discriminated Unions */
struct _xtunion_struct { char *tag; };

/* Regions */
struct _RegionPage
#ifdef CYC_REGION_PROFILE
{ unsigned total_bytes;
  unsigned free_bytes;
  /* MWH: wish we didn't have to include the stuff below ... */
  struct _RegionPage *next;
  char data[1];
}
#endif
; // abstract -- defined in runtime_memory.c
struct _RegionHandle {
  struct _RuntimeStack s;
  struct _RegionPage *curr;
  char               *offset;
  char               *last_plus_one;
  struct _DynRegionHandle *sub_regions;
#ifdef CYC_REGION_PROFILE
  const char         *name;
#else
  unsigned used_bytes;
  unsigned wasted_bytes;
#endif
};
struct _DynRegionFrame {
  struct _RuntimeStack s;
  struct _DynRegionHandle *x;
};
// A dynamic region is just a region handle.  The wrapper struct is for type
// abstraction.
struct Cyc_Core_DynamicRegion {
  struct _RegionHandle h;
};

struct _RegionHandle _new_region(const char*);
void* _region_malloc(struct _RegionHandle*, unsigned);
void* _region_calloc(struct _RegionHandle*, unsigned t, unsigned n);
void   _free_region(struct _RegionHandle*);
struct _RegionHandle*_open_dynregion(struct _DynRegionFrame*,struct _DynRegionHandle*);
void   _pop_dynregion();

/* Pools */
struct _pool; // defined in runtime_memory.c
struct _PoolHandle {
  struct _RuntimeStack s;
  struct _pool *p;
};
struct _PoolHandle _new_pool(void);
void _free_pool(struct _PoolHandle *h);
void _push_pool(struct _PoolHandle * r);
void _pop_pool(void);

/* Exceptions */
struct _handler_cons {
  struct _RuntimeStack s;
  jmp_buf handler;
};
void _push_handler(struct _handler_cons *);
void _push_region(struct _RegionHandle *);
void _npop_handler(int);
void _pop_handler();
void _pop_region();

#ifndef _throw
void* _throw_null_fn(const char*,unsigned);
void* _throw_arraybounds_fn(const char*,unsigned);
void* _throw_badalloc_fn(const char*,unsigned);
void* _throw_match_fn(const char*,unsigned);
void* _throw_fn(void*,const char*,unsigned);
void* _rethrow(void*);
#define _throw_null() (_throw_null_fn(__FILE__,__LINE__))
#define _throw_arraybounds() (_throw_arraybounds_fn(__FILE__,__LINE__))
#define _throw_badalloc() (_throw_badalloc_fn(__FILE__,__LINE__))
#define _throw_match() (_throw_match_fn(__FILE__,__LINE__))
#define _throw(e) (_throw_fn((e),__FILE__,__LINE__))
#endif

struct _xtunion_struct* Cyc_Core_get_exn_thrown();
/* Built-in Exceptions */
struct Cyc_Null_Exception_exn_struct { char *tag; };
struct Cyc_Array_bounds_exn_struct { char *tag; };
struct Cyc_Match_Exception_exn_struct { char *tag; };
struct Cyc_Bad_alloc_exn_struct { char *tag; };
extern char Cyc_Null_Exception[];
extern char Cyc_Array_bounds[];
extern char Cyc_Match_Exception[];
extern char Cyc_Bad_alloc[];

/* Built-in Run-time Checks and company */
#ifdef CYC_ANSI_OUTPUT
#define _INLINE  
#else
#define _INLINE inline
#endif

#ifdef NO_CYC_NULL_CHECKS
#define _check_null(ptr) (ptr)
#else
#define _check_null(ptr) \
  ({ void*_cks_null = (void*)(ptr); \
     if (!_cks_null) _throw_null(); \
     _cks_null; })
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_known_subscript_notnull(ptr,bound,elt_sz,index)\
   (((char*)ptr) + (elt_sz)*(index))
#ifdef NO_CYC_NULL_CHECKS
#define _check_known_subscript_null _check_known_subscript_notnull
#else
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  char*_cks_ptr = (char*)(ptr);\
  int _index = (index);\
  if (!_cks_ptr) _throw_null(); \
  _cks_ptr + (elt_sz)*_index; })
#endif
#define _zero_arr_plus_fn(orig_x,orig_sz,orig_i,f,l) ((orig_x)+(orig_i))
#define _zero_arr_plus_char_fn _zero_arr_plus_fn
#define _zero_arr_plus_short_fn _zero_arr_plus_fn
#define _zero_arr_plus_int_fn _zero_arr_plus_fn
#define _zero_arr_plus_float_fn _zero_arr_plus_fn
#define _zero_arr_plus_double_fn _zero_arr_plus_fn
#define _zero_arr_plus_longdouble_fn _zero_arr_plus_fn
#define _zero_arr_plus_voidstar_fn _zero_arr_plus_fn
#else
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  char*_cks_ptr = (char*)(ptr); \
  unsigned _cks_index = (index); \
  if (!_cks_ptr) _throw_null(); \
  if (_cks_index >= (bound)) _throw_arraybounds(); \
  _cks_ptr + (elt_sz)*_cks_index; })
#define _check_known_subscript_notnull(ptr,bound,elt_sz,index) ({ \
  char*_cks_ptr = (char*)(ptr); \
  unsigned _cks_index = (index); \
  if (_cks_index >= (bound)) _throw_arraybounds(); \
  _cks_ptr + (elt_sz)*_cks_index; })

/* _zero_arr_plus_*_fn(x,sz,i,filename,lineno) adds i to zero-terminated ptr
   x that has at least sz elements */
char* _zero_arr_plus_char_fn(char*,unsigned,int,const char*,unsigned);
short* _zero_arr_plus_short_fn(short*,unsigned,int,const char*,unsigned);
int* _zero_arr_plus_int_fn(int*,unsigned,int,const char*,unsigned);
float* _zero_arr_plus_float_fn(float*,unsigned,int,const char*,unsigned);
double* _zero_arr_plus_double_fn(double*,unsigned,int,const char*,unsigned);
long double* _zero_arr_plus_longdouble_fn(long double*,unsigned,int,const char*, unsigned);
void** _zero_arr_plus_voidstar_fn(void**,unsigned,int,const char*,unsigned);
#endif

/* _get_zero_arr_size_*(x,sz) returns the number of elements in a
   zero-terminated array that is NULL or has at least sz elements */
int _get_zero_arr_size_char(const char*,unsigned);
int _get_zero_arr_size_short(const short*,unsigned);
int _get_zero_arr_size_int(const int*,unsigned);
int _get_zero_arr_size_float(const float*,unsigned);
int _get_zero_arr_size_double(const double*,unsigned);
int _get_zero_arr_size_longdouble(const long double*,unsigned);
int _get_zero_arr_size_voidstar(const void**,unsigned);

/* _zero_arr_inplace_plus_*_fn(x,i,filename,lineno) sets
   zero-terminated pointer *x to *x + i */
char* _zero_arr_inplace_plus_char_fn(char**,int,const char*,unsigned);
short* _zero_arr_inplace_plus_short_fn(short**,int,const char*,unsigned);
int* _zero_arr_inplace_plus_int(int**,int,const char*,unsigned);
float* _zero_arr_inplace_plus_float_fn(float**,int,const char*,unsigned);
double* _zero_arr_inplace_plus_double_fn(double**,int,const char*,unsigned);
long double* _zero_arr_inplace_plus_longdouble_fn(long double**,int,const char*,unsigned);
void** _zero_arr_inplace_plus_voidstar_fn(void***,int,const char*,unsigned);
/* like the previous functions, but does post-addition (as in e++) */
char* _zero_arr_inplace_plus_post_char_fn(char**,int,const char*,unsigned);
short* _zero_arr_inplace_plus_post_short_fn(short**x,int,const char*,unsigned);
int* _zero_arr_inplace_plus_post_int_fn(int**,int,const char*,unsigned);
float* _zero_arr_inplace_plus_post_float_fn(float**,int,const char*,unsigned);
double* _zero_arr_inplace_plus_post_double_fn(double**,int,const char*,unsigned);
long double* _zero_arr_inplace_plus_post_longdouble_fn(long double**,int,const char *,unsigned);
void** _zero_arr_inplace_plus_post_voidstar_fn(void***,int,const char*,unsigned);
#define _zero_arr_plus_char(x,s,i) \
  (_zero_arr_plus_char_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_short(x,s,i) \
  (_zero_arr_plus_short_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_int(x,s,i) \
  (_zero_arr_plus_int_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_float(x,s,i) \
  (_zero_arr_plus_float_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_double(x,s,i) \
  (_zero_arr_plus_double_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_longdouble(x,s,i) \
  (_zero_arr_plus_longdouble_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_plus_voidstar(x,s,i) \
  (_zero_arr_plus_voidstar_fn(x,s,i,__FILE__,__LINE__))
#define _zero_arr_inplace_plus_char(x,i) \
  _zero_arr_inplace_plus_char_fn((char **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_short(x,i) \
  _zero_arr_inplace_plus_short_fn((short **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_int(x,i) \
  _zero_arr_inplace_plus_int_fn((int **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_float(x,i) \
  _zero_arr_inplace_plus_float_fn((float **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_double(x,i) \
  _zero_arr_inplace_plus_double_fn((double **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_longdouble(x,i) \
  _zero_arr_inplace_plus_longdouble_fn((long double **)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_voidstar(x,i) \
  _zero_arr_inplace_plus_voidstar_fn((void ***)(x),i,__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_char(x,i) \
  _zero_arr_inplace_plus_post_char_fn((char **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_short(x,i) \
  _zero_arr_inplace_plus_post_short_fn((short **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_int(x,i) \
  _zero_arr_inplace_plus_post_int_fn((int **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_float(x,i) \
  _zero_arr_inplace_plus_post_float_fn((float **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_double(x,i) \
  _zero_arr_inplace_plus_post_double_fn((double **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_longdouble(x,i) \
  _zero_arr_inplace_plus_post_longdouble_fn((long double **)(x),(i),__FILE__,__LINE__)
#define _zero_arr_inplace_plus_post_voidstar(x,i) \
  _zero_arr_inplace_plus_post_voidstar_fn((void***)(x),(i),__FILE__,__LINE__)

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_fat_subscript(arr,elt_sz,index) ((arr).curr + (elt_sz) * (index))
#define _untag_fat_ptr(arr,elt_sz,num_elts) ((arr).curr)
#else
#define _check_fat_subscript(arr,elt_sz,index) ({ \
  struct _fat_ptr _cus_arr = (arr); \
  unsigned char *_cus_ans = _cus_arr.curr + (elt_sz) * (index); \
  /* JGM: not needed! if (!_cus_arr.base) _throw_null();*/ \
  if (_cus_ans < _cus_arr.base || _cus_ans >= _cus_arr.last_plus_one) \
    _throw_arraybounds(); \
  _cus_ans; })
#define _untag_fat_ptr(arr,elt_sz,num_elts) ({ \
  struct _fat_ptr _arr = (arr); \
  unsigned char *_curr = _arr.curr; \
  if ((_curr < _arr.base || _curr + (elt_sz) * (num_elts) > _arr.last_plus_one) &&\
      _curr != (unsigned char *)0) \
    _throw_arraybounds(); \
  _curr; })
#endif

#define _tag_fat(tcurr,elt_sz,num_elts) ({ \
  struct _fat_ptr _ans; \
  unsigned _num_elts = (num_elts);\
  _ans.base = _ans.curr = (void*)(tcurr); \
  /* JGM: if we're tagging NULL, ignore num_elts */ \
  _ans.last_plus_one = _ans.base ? (_ans.base + (elt_sz) * _num_elts) : 0; \
  _ans; })

#define _get_fat_size(arr,elt_sz) \
  ({struct _fat_ptr _arr = (arr); \
    unsigned char *_arr_curr=_arr.curr; \
    unsigned char *_arr_last=_arr.last_plus_one; \
    (_arr_curr < _arr.base || _arr_curr >= _arr_last) ? 0 : \
    ((_arr_last - _arr_curr) / (elt_sz));})

#define _fat_ptr_plus(arr,elt_sz,change) ({ \
  struct _fat_ptr _ans = (arr); \
  int _change = (change);\
  _ans.curr += (elt_sz) * _change;\
  _ans; })
#define _fat_ptr_inplace_plus(arr_ptr,elt_sz,change) ({ \
  struct _fat_ptr * _arr_ptr = (arr_ptr); \
  _arr_ptr->curr += (elt_sz) * (change);\
  *_arr_ptr; })
#define _fat_ptr_inplace_plus_post(arr_ptr,elt_sz,change) ({ \
  struct _fat_ptr * _arr_ptr = (arr_ptr); \
  struct _fat_ptr _ans = *_arr_ptr; \
  _arr_ptr->curr += (elt_sz) * (change);\
  _ans; })

//Not a macro since initialization order matters. Defined in runtime_zeroterm.c.
struct _fat_ptr _fat_ptr_decrease_size(struct _fat_ptr,unsigned sz,unsigned numelts);

/* Allocation */
void* GC_malloc(int);
void* GC_malloc_atomic(int);
void* GC_calloc(unsigned,unsigned);
void* GC_calloc_atomic(unsigned,unsigned);
// bound the allocation size to be < MAX_ALLOC_SIZE. See macros below for usage.
#define MAX_MALLOC_SIZE (1 << 28)
void* _bounded_GC_malloc(int,const char*,int);
void* _bounded_GC_malloc_atomic(int,const char*,int);
void* _bounded_GC_calloc(unsigned,unsigned,const char*,int);
void* _bounded_GC_calloc_atomic(unsigned,unsigned,const char*,int);
/* these macros are overridden below ifdef CYC_REGION_PROFILE */
#ifndef CYC_REGION_PROFILE
#define _cycalloc(n) _bounded_GC_malloc(n,__FILE__,__LINE__)
#define _cycalloc_atomic(n) _bounded_GC_malloc_atomic(n,__FILE__,__LINE__)
#define _cyccalloc(n,s) _bounded_GC_calloc(n,s,__FILE__,__LINE__)
#define _cyccalloc_atomic(n,s) _bounded_GC_calloc_atomic(n,s,__FILE__,__LINE__)
#endif

static _INLINE unsigned int _check_times(unsigned x, unsigned y) {
  unsigned long long whole_ans = 
    ((unsigned long long) x)*((unsigned long long)y);
  unsigned word_ans = (unsigned)whole_ans;
  if(word_ans < whole_ans || word_ans > MAX_MALLOC_SIZE)
    _throw_badalloc();
  return word_ans;
}

#define _CYC_MAX_REGION_CONST 2
#define _CYC_MIN_ALIGNMENT (sizeof(double))

#ifdef CYC_REGION_PROFILE
extern int rgn_total_bytes;
#endif

static _INLINE void *_fast_region_malloc(struct _RegionHandle *r, unsigned orig_s) {  
  if (r > (struct _RegionHandle *)_CYC_MAX_REGION_CONST && r->curr != 0) { 
#ifdef CYC_NOALIGN
    unsigned s =  orig_s;
#else
    unsigned s =  (orig_s + _CYC_MIN_ALIGNMENT - 1) & (~(_CYC_MIN_ALIGNMENT -1)); 
#endif
    char *result; 
    result = r->offset; 
    if (s <= (r->last_plus_one - result)) {
      r->offset = result + s; 
#ifdef CYC_REGION_PROFILE
    r->curr->free_bytes = r->curr->free_bytes - s;
    rgn_total_bytes += s;
#endif
      return result;
    }
  } 
  return _region_malloc(r,orig_s); 
}

#ifdef CYC_REGION_PROFILE
/* see macros below for usage. defined in runtime_memory.c */
void* _profile_GC_malloc(int,const char*,const char*,int);
void* _profile_GC_malloc_atomic(int,const char*,const char*,int);
void* _profile_GC_calloc(unsigned,unsigned,const char*,const char*,int);
void* _profile_GC_calloc_atomic(unsigned,unsigned,const char*,const char*,int);
void* _profile_region_malloc(struct _RegionHandle*,unsigned,const char*,const char*,int);
void* _profile_region_calloc(struct _RegionHandle*,unsigned,unsigned,const char *,const char*,int);
struct _RegionHandle _profile_new_region(const char*,const char*,const char*,int);
void _profile_free_region(struct _RegionHandle*,const char*,const char*,int);
#ifndef RUNTIME_CYC
#define _new_region(n) _profile_new_region(n,__FILE__,__FUNCTION__,__LINE__)
#define _free_region(r) _profile_free_region(r,__FILE__,__FUNCTION__,__LINE__)
#define _region_malloc(rh,n) _profile_region_malloc(rh,n,__FILE__,__FUNCTION__,__LINE__)
#define _region_calloc(rh,n,t) _profile_region_calloc(rh,n,t,__FILE__,__FUNCTION__,__LINE__)
#  endif
#define _cycalloc(n) _profile_GC_malloc(n,__FILE__,__FUNCTION__,__LINE__)
#define _cycalloc_atomic(n) _profile_GC_malloc_atomic(n,__FILE__,__FUNCTION__,__LINE__)
#define _cyccalloc(n,s) _profile_GC_calloc(n,s,__FILE__,__FUNCTION__,__LINE__)
#define _cyccalloc_atomic(n,s) _profile_GC_calloc_atomic(n,s,__FILE__,__FUNCTION__,__LINE__)
#endif
#endif
 struct Cyc_Core_Opt{void*v;};struct _tuple0{void*f1;void*f2;};
# 108 "core.h"
void*Cyc_Core_fst(struct _tuple0*);
# 114
void*Cyc_Core_identity(void*);extern char Cyc_Core_Invalid_argument[17U];struct Cyc_Core_Invalid_argument_exn_struct{char*tag;struct _fat_ptr f1;};extern char Cyc_Core_Failure[8U];struct Cyc_Core_Failure_exn_struct{char*tag;struct _fat_ptr f1;};extern char Cyc_Core_Impossible[11U];struct Cyc_Core_Impossible_exn_struct{char*tag;struct _fat_ptr f1;};extern char Cyc_Core_Not_found[10U];struct Cyc_Core_Not_found_exn_struct{char*tag;};extern char Cyc_Core_Unreachable[12U];struct Cyc_Core_Unreachable_exn_struct{char*tag;struct _fat_ptr f1;};
# 168
extern struct _RegionHandle*Cyc_Core_heap_region;
# 171
extern struct _RegionHandle*Cyc_Core_unique_region;struct Cyc_Core_DynamicRegion;struct Cyc_Core_NewDynamicRegion{struct Cyc_Core_DynamicRegion*key;};struct Cyc_Core_ThinRes{void*arr;unsigned nelts;};struct Cyc_List_List{void*hd;struct Cyc_List_List*tl;};
# 61 "list.h"
extern int Cyc_List_length(struct Cyc_List_List*x);
# 76
extern struct Cyc_List_List*Cyc_List_map(void*(*f)(void*),struct Cyc_List_List*x);extern char Cyc_List_List_mismatch[14U];struct Cyc_List_List_mismatch_exn_struct{char*tag;};
# 135
extern void Cyc_List_iter_c(void(*f)(void*,void*),void*env,struct Cyc_List_List*x);
# 178
extern struct Cyc_List_List*Cyc_List_imp_rev(struct Cyc_List_List*x);
# 195
extern struct Cyc_List_List*Cyc_List_imp_append(struct Cyc_List_List*x,struct Cyc_List_List*y);extern char Cyc_List_Nth[4U];struct Cyc_List_Nth_exn_struct{char*tag;};
# 242
extern void*Cyc_List_nth(struct Cyc_List_List*x,int n);
# 258
extern int Cyc_List_exists(int(*pred)(void*),struct Cyc_List_List*x);
# 322
extern int Cyc_List_mem(int(*compare)(void*,void*),struct Cyc_List_List*l,void*x);
# 391
extern struct Cyc_List_List*Cyc_List_filter(int(*f)(void*),struct Cyc_List_List*x);struct Cyc___cycFILE;struct Cyc_String_pa_PrintArg_struct{int tag;struct _fat_ptr f1;};struct Cyc_Int_pa_PrintArg_struct{int tag;unsigned long f1;};struct Cyc_Double_pa_PrintArg_struct{int tag;double f1;};struct Cyc_LongDouble_pa_PrintArg_struct{int tag;long double f1;};struct Cyc_ShortPtr_pa_PrintArg_struct{int tag;short*f1;};struct Cyc_IntPtr_pa_PrintArg_struct{int tag;unsigned long*f1;};struct Cyc_ShortPtr_sa_ScanfArg_struct{int tag;short*f1;};struct Cyc_UShortPtr_sa_ScanfArg_struct{int tag;unsigned short*f1;};struct Cyc_IntPtr_sa_ScanfArg_struct{int tag;int*f1;};struct Cyc_UIntPtr_sa_ScanfArg_struct{int tag;unsigned*f1;};struct Cyc_StringPtr_sa_ScanfArg_struct{int tag;struct _fat_ptr f1;};struct Cyc_DoublePtr_sa_ScanfArg_struct{int tag;double*f1;};struct Cyc_FloatPtr_sa_ScanfArg_struct{int tag;float*f1;};struct Cyc_CharPtr_sa_ScanfArg_struct{int tag;struct _fat_ptr f1;};extern char Cyc_FileCloseError[15U];struct Cyc_FileCloseError_exn_struct{char*tag;};extern char Cyc_FileOpenError[14U];struct Cyc_FileOpenError_exn_struct{char*tag;struct _fat_ptr f1;};struct Cyc_timeval{long tv_sec;long tv_usec;};
# 49 "string.h"
extern int Cyc_strcmp(struct _fat_ptr s1,struct _fat_ptr s2);
extern int Cyc_strptrcmp(struct _fat_ptr*s1,struct _fat_ptr*s2);struct Cyc_Position_Error;struct Cyc_Relations_Reln;struct _union_Nmspace_Rel_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_Abs_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_C_n{int tag;struct Cyc_List_List*val;};struct _union_Nmspace_Loc_n{int tag;int val;};union Cyc_Absyn_Nmspace{struct _union_Nmspace_Rel_n Rel_n;struct _union_Nmspace_Abs_n Abs_n;struct _union_Nmspace_C_n C_n;struct _union_Nmspace_Loc_n Loc_n;};struct _tuple1{union Cyc_Absyn_Nmspace f1;struct _fat_ptr*f2;};
# 158 "absyn.h"
enum Cyc_Absyn_Scope{Cyc_Absyn_Static =0U,Cyc_Absyn_Abstract =1U,Cyc_Absyn_Public =2U,Cyc_Absyn_Extern =3U,Cyc_Absyn_ExternC =4U,Cyc_Absyn_Register =5U};struct Cyc_Absyn_Tqual{int print_const: 1;int q_volatile: 1;int q_restrict: 1;int real_const: 1;unsigned loc;};
# 179
enum Cyc_Absyn_Size_of{Cyc_Absyn_Char_sz =0U,Cyc_Absyn_Short_sz =1U,Cyc_Absyn_Int_sz =2U,Cyc_Absyn_Long_sz =3U,Cyc_Absyn_LongLong_sz =4U};
enum Cyc_Absyn_Sign{Cyc_Absyn_Signed =0U,Cyc_Absyn_Unsigned =1U,Cyc_Absyn_None =2U};
enum Cyc_Absyn_AggrKind{Cyc_Absyn_StructA =0U,Cyc_Absyn_UnionA =1U};
# 184
enum Cyc_Absyn_AliasQual{Cyc_Absyn_Aliasable =0U,Cyc_Absyn_Unique =1U,Cyc_Absyn_Top =2U};
# 189
enum Cyc_Absyn_KindQual{Cyc_Absyn_AnyKind =0U,Cyc_Absyn_MemKind =1U,Cyc_Absyn_BoxKind =2U,Cyc_Absyn_RgnKind =3U,Cyc_Absyn_EffKind =4U,Cyc_Absyn_IntKind =5U,Cyc_Absyn_BoolKind =6U,Cyc_Absyn_PtrBndKind =7U};struct Cyc_Absyn_Kind{enum Cyc_Absyn_KindQual kind;enum Cyc_Absyn_AliasQual aliasqual;};struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct{int tag;struct Cyc_Absyn_Kind*f1;};struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct{int tag;struct Cyc_Core_Opt*f1;};struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct{int tag;struct Cyc_Core_Opt*f1;struct Cyc_Absyn_Kind*f2;};struct Cyc_Absyn_Tvar{struct _fat_ptr*name;int identity;void*kind;};struct Cyc_Absyn_PtrLoc{unsigned ptr_loc;unsigned rgn_loc;unsigned zt_loc;};struct Cyc_Absyn_PtrAtts{void*rgn;void*nullable;void*bounds;void*zero_term;struct Cyc_Absyn_PtrLoc*ptrloc;};struct Cyc_Absyn_PtrInfo{void*elt_type;struct Cyc_Absyn_Tqual elt_tq;struct Cyc_Absyn_PtrAtts ptr_atts;};struct Cyc_Absyn_VarargInfo{struct _fat_ptr*name;struct Cyc_Absyn_Tqual tq;void*type;int inject;};struct Cyc_Absyn_FnInfo{struct Cyc_List_List*tvars;void*effect;struct Cyc_Absyn_Tqual ret_tqual;void*ret_type;struct Cyc_List_List*args;int c_varargs;struct Cyc_Absyn_VarargInfo*cyc_varargs;struct Cyc_List_List*rgn_po;struct Cyc_List_List*attributes;struct Cyc_Absyn_Exp*requires_clause;struct Cyc_List_List*requires_relns;struct Cyc_Absyn_Exp*ensures_clause;struct Cyc_List_List*ensures_relns;struct Cyc_Absyn_Vardecl*return_value;};struct Cyc_Absyn_UnknownDatatypeInfo{struct _tuple1*name;int is_extensible;};struct _union_DatatypeInfo_UnknownDatatype{int tag;struct Cyc_Absyn_UnknownDatatypeInfo val;};struct _union_DatatypeInfo_KnownDatatype{int tag;struct Cyc_Absyn_Datatypedecl**val;};union Cyc_Absyn_DatatypeInfo{struct _union_DatatypeInfo_UnknownDatatype UnknownDatatype;struct _union_DatatypeInfo_KnownDatatype KnownDatatype;};
# 292
union Cyc_Absyn_DatatypeInfo Cyc_Absyn_KnownDatatype(struct Cyc_Absyn_Datatypedecl**);struct Cyc_Absyn_UnknownDatatypeFieldInfo{struct _tuple1*datatype_name;struct _tuple1*field_name;int is_extensible;};struct _union_DatatypeFieldInfo_UnknownDatatypefield{int tag;struct Cyc_Absyn_UnknownDatatypeFieldInfo val;};struct _tuple2{struct Cyc_Absyn_Datatypedecl*f1;struct Cyc_Absyn_Datatypefield*f2;};struct _union_DatatypeFieldInfo_KnownDatatypefield{int tag;struct _tuple2 val;};union Cyc_Absyn_DatatypeFieldInfo{struct _union_DatatypeFieldInfo_UnknownDatatypefield UnknownDatatypefield;struct _union_DatatypeFieldInfo_KnownDatatypefield KnownDatatypefield;};
# 305
union Cyc_Absyn_DatatypeFieldInfo Cyc_Absyn_KnownDatatypefield(struct Cyc_Absyn_Datatypedecl*,struct Cyc_Absyn_Datatypefield*);struct _tuple3{enum Cyc_Absyn_AggrKind f1;struct _tuple1*f2;struct Cyc_Core_Opt*f3;};struct _union_AggrInfo_UnknownAggr{int tag;struct _tuple3 val;};struct _union_AggrInfo_KnownAggr{int tag;struct Cyc_Absyn_Aggrdecl**val;};union Cyc_Absyn_AggrInfo{struct _union_AggrInfo_UnknownAggr UnknownAggr;struct _union_AggrInfo_KnownAggr KnownAggr;};
# 312
union Cyc_Absyn_AggrInfo Cyc_Absyn_KnownAggr(struct Cyc_Absyn_Aggrdecl**);struct Cyc_Absyn_ArrayInfo{void*elt_type;struct Cyc_Absyn_Tqual tq;struct Cyc_Absyn_Exp*num_elts;void*zero_term;unsigned zt_loc;};struct Cyc_Absyn_Aggr_td_Absyn_Raw_typedecl_struct{int tag;struct Cyc_Absyn_Aggrdecl*f1;};struct Cyc_Absyn_Enum_td_Absyn_Raw_typedecl_struct{int tag;struct Cyc_Absyn_Enumdecl*f1;};struct Cyc_Absyn_Datatype_td_Absyn_Raw_typedecl_struct{int tag;struct Cyc_Absyn_Datatypedecl*f1;};struct Cyc_Absyn_TypeDecl{void*r;unsigned loc;};struct Cyc_Absyn_VoidCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_IntCon_Absyn_TyCon_struct{int tag;enum Cyc_Absyn_Sign f1;enum Cyc_Absyn_Size_of f2;};struct Cyc_Absyn_FloatCon_Absyn_TyCon_struct{int tag;int f1;};struct Cyc_Absyn_RgnHandleCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_TagCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_HeapCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_UniqueCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_RefCntCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_AccessCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_JoinCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_RgnsCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_TrueCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_FalseCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_ThinCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_FatCon_Absyn_TyCon_struct{int tag;};struct Cyc_Absyn_EnumCon_Absyn_TyCon_struct{int tag;struct _tuple1*f1;struct Cyc_Absyn_Enumdecl*f2;};struct Cyc_Absyn_AnonEnumCon_Absyn_TyCon_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_BuiltinCon_Absyn_TyCon_struct{int tag;struct _fat_ptr f1;struct Cyc_Absyn_Kind*f2;};struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct{int tag;union Cyc_Absyn_DatatypeInfo f1;};struct Cyc_Absyn_DatatypeFieldCon_Absyn_TyCon_struct{int tag;union Cyc_Absyn_DatatypeFieldInfo f1;};struct Cyc_Absyn_AggrCon_Absyn_TyCon_struct{int tag;union Cyc_Absyn_AggrInfo f1;};struct Cyc_Absyn_AppType_Absyn_Type_struct{int tag;void*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Evar_Absyn_Type_struct{int tag;struct Cyc_Core_Opt*f1;void*f2;int f3;struct Cyc_Core_Opt*f4;};struct Cyc_Absyn_VarType_Absyn_Type_struct{int tag;struct Cyc_Absyn_Tvar*f1;};struct Cyc_Absyn_PointerType_Absyn_Type_struct{int tag;struct Cyc_Absyn_PtrInfo f1;};struct Cyc_Absyn_ArrayType_Absyn_Type_struct{int tag;struct Cyc_Absyn_ArrayInfo f1;};struct Cyc_Absyn_FnType_Absyn_Type_struct{int tag;struct Cyc_Absyn_FnInfo f1;};struct Cyc_Absyn_TupleType_Absyn_Type_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_AnonAggrType_Absyn_Type_struct{int tag;enum Cyc_Absyn_AggrKind f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_TypedefType_Absyn_Type_struct{int tag;struct _tuple1*f1;struct Cyc_List_List*f2;struct Cyc_Absyn_Typedefdecl*f3;void*f4;};struct Cyc_Absyn_ValueofType_Absyn_Type_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_TypeDeclType_Absyn_Type_struct{int tag;struct Cyc_Absyn_TypeDecl*f1;void**f2;};struct Cyc_Absyn_TypeofType_Absyn_Type_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_NoTypes_Absyn_Funcparams_struct{int tag;struct Cyc_List_List*f1;unsigned f2;};struct Cyc_Absyn_WithTypes_Absyn_Funcparams_struct{int tag;struct Cyc_List_List*f1;int f2;struct Cyc_Absyn_VarargInfo*f3;void*f4;struct Cyc_List_List*f5;struct Cyc_Absyn_Exp*f6;struct Cyc_Absyn_Exp*f7;};
# 414 "absyn.h"
enum Cyc_Absyn_Format_Type{Cyc_Absyn_Printf_ft =0U,Cyc_Absyn_Scanf_ft =1U};struct Cyc_Absyn_Regparm_att_Absyn_Attribute_struct{int tag;int f1;};struct Cyc_Absyn_Stdcall_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Cdecl_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Fastcall_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Noreturn_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Const_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Aligned_att_Absyn_Attribute_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Packed_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Section_att_Absyn_Attribute_struct{int tag;struct _fat_ptr f1;};struct Cyc_Absyn_Nocommon_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Shared_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Unused_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Weak_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Dllimport_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Dllexport_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_No_instrument_function_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Constructor_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Destructor_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_No_check_memory_usage_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Format_att_Absyn_Attribute_struct{int tag;enum Cyc_Absyn_Format_Type f1;int f2;int f3;};struct Cyc_Absyn_Initializes_att_Absyn_Attribute_struct{int tag;int f1;};struct Cyc_Absyn_Noliveunique_att_Absyn_Attribute_struct{int tag;int f1;};struct Cyc_Absyn_Consume_att_Absyn_Attribute_struct{int tag;int f1;};struct Cyc_Absyn_Pure_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Mode_att_Absyn_Attribute_struct{int tag;struct _fat_ptr f1;};struct Cyc_Absyn_Alias_att_Absyn_Attribute_struct{int tag;struct _fat_ptr f1;};struct Cyc_Absyn_Always_inline_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_No_throw_att_Absyn_Attribute_struct{int tag;};struct Cyc_Absyn_Carray_mod_Absyn_Type_modifier_struct{int tag;void*f1;unsigned f2;};struct Cyc_Absyn_ConstArray_mod_Absyn_Type_modifier_struct{int tag;struct Cyc_Absyn_Exp*f1;void*f2;unsigned f3;};struct Cyc_Absyn_Pointer_mod_Absyn_Type_modifier_struct{int tag;struct Cyc_Absyn_PtrAtts f1;struct Cyc_Absyn_Tqual f2;};struct Cyc_Absyn_Function_mod_Absyn_Type_modifier_struct{int tag;void*f1;};struct Cyc_Absyn_TypeParams_mod_Absyn_Type_modifier_struct{int tag;struct Cyc_List_List*f1;unsigned f2;int f3;};struct Cyc_Absyn_Attributes_mod_Absyn_Type_modifier_struct{int tag;unsigned f1;struct Cyc_List_List*f2;};struct _union_Cnst_Null_c{int tag;int val;};struct _tuple4{enum Cyc_Absyn_Sign f1;char f2;};struct _union_Cnst_Char_c{int tag;struct _tuple4 val;};struct _union_Cnst_Wchar_c{int tag;struct _fat_ptr val;};struct _tuple5{enum Cyc_Absyn_Sign f1;short f2;};struct _union_Cnst_Short_c{int tag;struct _tuple5 val;};struct _tuple6{enum Cyc_Absyn_Sign f1;int f2;};struct _union_Cnst_Int_c{int tag;struct _tuple6 val;};struct _tuple7{enum Cyc_Absyn_Sign f1;long long f2;};struct _union_Cnst_LongLong_c{int tag;struct _tuple7 val;};struct _tuple8{struct _fat_ptr f1;int f2;};struct _union_Cnst_Float_c{int tag;struct _tuple8 val;};struct _union_Cnst_String_c{int tag;struct _fat_ptr val;};struct _union_Cnst_Wstring_c{int tag;struct _fat_ptr val;};union Cyc_Absyn_Cnst{struct _union_Cnst_Null_c Null_c;struct _union_Cnst_Char_c Char_c;struct _union_Cnst_Wchar_c Wchar_c;struct _union_Cnst_Short_c Short_c;struct _union_Cnst_Int_c Int_c;struct _union_Cnst_LongLong_c LongLong_c;struct _union_Cnst_Float_c Float_c;struct _union_Cnst_String_c String_c;struct _union_Cnst_Wstring_c Wstring_c;};
# 506
enum Cyc_Absyn_Primop{Cyc_Absyn_Plus =0U,Cyc_Absyn_Times =1U,Cyc_Absyn_Minus =2U,Cyc_Absyn_Div =3U,Cyc_Absyn_Mod =4U,Cyc_Absyn_Eq =5U,Cyc_Absyn_Neq =6U,Cyc_Absyn_Gt =7U,Cyc_Absyn_Lt =8U,Cyc_Absyn_Gte =9U,Cyc_Absyn_Lte =10U,Cyc_Absyn_Not =11U,Cyc_Absyn_Bitnot =12U,Cyc_Absyn_Bitand =13U,Cyc_Absyn_Bitor =14U,Cyc_Absyn_Bitxor =15U,Cyc_Absyn_Bitlshift =16U,Cyc_Absyn_Bitlrshift =17U,Cyc_Absyn_Numelts =18U};
# 513
enum Cyc_Absyn_Incrementor{Cyc_Absyn_PreInc =0U,Cyc_Absyn_PostInc =1U,Cyc_Absyn_PreDec =2U,Cyc_Absyn_PostDec =3U};struct Cyc_Absyn_VarargCallInfo{int num_varargs;struct Cyc_List_List*injectors;struct Cyc_Absyn_VarargInfo*vai;};struct Cyc_Absyn_StructField_Absyn_OffsetofField_struct{int tag;struct _fat_ptr*f1;};struct Cyc_Absyn_TupleIndex_Absyn_OffsetofField_struct{int tag;unsigned f1;};
# 531
enum Cyc_Absyn_Coercion{Cyc_Absyn_Unknown_coercion =0U,Cyc_Absyn_No_coercion =1U,Cyc_Absyn_Null_to_NonNull =2U,Cyc_Absyn_Other_coercion =3U};struct Cyc_Absyn_MallocInfo{int is_calloc;struct Cyc_Absyn_Exp*rgn;void**elt_type;struct Cyc_Absyn_Exp*num_elts;int fat_result;int inline_call;};struct Cyc_Absyn_Const_e_Absyn_Raw_exp_struct{int tag;union Cyc_Absyn_Cnst f1;};struct Cyc_Absyn_Var_e_Absyn_Raw_exp_struct{int tag;void*f1;};struct Cyc_Absyn_Pragma_e_Absyn_Raw_exp_struct{int tag;struct _fat_ptr f1;};struct Cyc_Absyn_Primop_e_Absyn_Raw_exp_struct{int tag;enum Cyc_Absyn_Primop f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_AssignOp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Core_Opt*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_Increment_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;enum Cyc_Absyn_Incrementor f2;};struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_FnCall_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;struct Cyc_Absyn_VarargCallInfo*f3;int f4;};struct Cyc_Absyn_Throw_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;int f2;};struct Cyc_Absyn_NoInstantiate_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Instantiate_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct{int tag;void*f1;struct Cyc_Absyn_Exp*f2;int f3;enum Cyc_Absyn_Coercion f4;};struct Cyc_Absyn_Address_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_New_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_Sizeoftype_e_Absyn_Raw_exp_struct{int tag;void*f1;};struct Cyc_Absyn_Sizeofexp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Offsetof_e_Absyn_Raw_exp_struct{int tag;void*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Deref_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_AggrMember_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;int f3;int f4;};struct Cyc_Absyn_AggrArrow_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;int f3;int f4;};struct Cyc_Absyn_Subscript_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_Tuple_e_Absyn_Raw_exp_struct{int tag;struct Cyc_List_List*f1;};struct _tuple9{struct _fat_ptr*f1;struct Cyc_Absyn_Tqual f2;void*f3;};struct Cyc_Absyn_CompoundLit_e_Absyn_Raw_exp_struct{int tag;struct _tuple9*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Array_e_Absyn_Raw_exp_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_Comprehension_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Vardecl*f1;struct Cyc_Absyn_Exp*f2;struct Cyc_Absyn_Exp*f3;int f4;};struct Cyc_Absyn_ComprehensionNoinit_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;void*f2;int f3;};struct Cyc_Absyn_Aggregate_e_Absyn_Raw_exp_struct{int tag;struct _tuple1*f1;struct Cyc_List_List*f2;struct Cyc_List_List*f3;struct Cyc_Absyn_Aggrdecl*f4;};struct Cyc_Absyn_AnonStruct_e_Absyn_Raw_exp_struct{int tag;void*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Datatype_e_Absyn_Raw_exp_struct{int tag;struct Cyc_List_List*f1;struct Cyc_Absyn_Datatypedecl*f2;struct Cyc_Absyn_Datatypefield*f3;};struct Cyc_Absyn_Enum_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Enumdecl*f1;struct Cyc_Absyn_Enumfield*f2;};struct Cyc_Absyn_AnonEnum_e_Absyn_Raw_exp_struct{int tag;void*f1;struct Cyc_Absyn_Enumfield*f2;};struct Cyc_Absyn_Malloc_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_MallocInfo f1;};struct Cyc_Absyn_Swap_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Exp*f2;};struct Cyc_Absyn_UnresolvedMem_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Core_Opt*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_StmtExp_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Stmt*f1;};struct Cyc_Absyn_Tagcheck_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _fat_ptr*f2;};struct Cyc_Absyn_Valueof_e_Absyn_Raw_exp_struct{int tag;void*f1;};struct Cyc_Absyn_Asm_e_Absyn_Raw_exp_struct{int tag;int f1;struct _fat_ptr f2;struct Cyc_List_List*f3;struct Cyc_List_List*f4;struct Cyc_List_List*f5;};struct Cyc_Absyn_Extension_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Assert_e_Absyn_Raw_exp_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Exp{void*topt;void*r;unsigned loc;void*annot;};struct Cyc_Absyn_Skip_s_Absyn_Raw_stmt_struct{int tag;};struct Cyc_Absyn_Exp_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Seq_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Return_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_IfThenElse_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Stmt*f2;struct Cyc_Absyn_Stmt*f3;};struct _tuple10{struct Cyc_Absyn_Exp*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_While_s_Absyn_Raw_stmt_struct{int tag;struct _tuple10 f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Break_s_Absyn_Raw_stmt_struct{int tag;};struct Cyc_Absyn_Continue_s_Absyn_Raw_stmt_struct{int tag;};struct Cyc_Absyn_Goto_s_Absyn_Raw_stmt_struct{int tag;struct _fat_ptr*f1;};struct Cyc_Absyn_For_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct _tuple10 f2;struct _tuple10 f3;struct Cyc_Absyn_Stmt*f4;};struct Cyc_Absyn_Switch_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Exp*f1;struct Cyc_List_List*f2;void*f3;};struct Cyc_Absyn_Fallthru_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_List_List*f1;struct Cyc_Absyn_Switch_clause**f2;};struct Cyc_Absyn_Decl_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Decl*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Label_s_Absyn_Raw_stmt_struct{int tag;struct _fat_ptr*f1;struct Cyc_Absyn_Stmt*f2;};struct Cyc_Absyn_Do_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct _tuple10 f2;};struct Cyc_Absyn_TryCatch_s_Absyn_Raw_stmt_struct{int tag;struct Cyc_Absyn_Stmt*f1;struct Cyc_List_List*f2;void*f3;};struct Cyc_Absyn_Stmt{void*r;unsigned loc;void*annot;};struct Cyc_Absyn_Wild_p_Absyn_Raw_pat_struct{int tag;};struct Cyc_Absyn_Var_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Vardecl*f1;struct Cyc_Absyn_Pat*f2;};struct Cyc_Absyn_AliasVar_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Tvar*f1;struct Cyc_Absyn_Vardecl*f2;};struct Cyc_Absyn_Reference_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Vardecl*f1;struct Cyc_Absyn_Pat*f2;};struct Cyc_Absyn_TagInt_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Tvar*f1;struct Cyc_Absyn_Vardecl*f2;};struct Cyc_Absyn_Tuple_p_Absyn_Raw_pat_struct{int tag;struct Cyc_List_List*f1;int f2;};struct Cyc_Absyn_Pointer_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Pat*f1;};struct Cyc_Absyn_Aggr_p_Absyn_Raw_pat_struct{int tag;union Cyc_Absyn_AggrInfo*f1;struct Cyc_List_List*f2;struct Cyc_List_List*f3;int f4;};struct Cyc_Absyn_Datatype_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Datatypedecl*f1;struct Cyc_Absyn_Datatypefield*f2;struct Cyc_List_List*f3;int f4;};struct Cyc_Absyn_Null_p_Absyn_Raw_pat_struct{int tag;};struct Cyc_Absyn_Int_p_Absyn_Raw_pat_struct{int tag;enum Cyc_Absyn_Sign f1;int f2;};struct Cyc_Absyn_Char_p_Absyn_Raw_pat_struct{int tag;char f1;};struct Cyc_Absyn_Float_p_Absyn_Raw_pat_struct{int tag;struct _fat_ptr f1;int f2;};struct Cyc_Absyn_Enum_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Enumdecl*f1;struct Cyc_Absyn_Enumfield*f2;};struct Cyc_Absyn_AnonEnum_p_Absyn_Raw_pat_struct{int tag;void*f1;struct Cyc_Absyn_Enumfield*f2;};struct Cyc_Absyn_UnknownId_p_Absyn_Raw_pat_struct{int tag;struct _tuple1*f1;};struct Cyc_Absyn_UnknownCall_p_Absyn_Raw_pat_struct{int tag;struct _tuple1*f1;struct Cyc_List_List*f2;int f3;};struct Cyc_Absyn_Exp_p_Absyn_Raw_pat_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_Pat{void*r;void*topt;unsigned loc;};struct Cyc_Absyn_Switch_clause{struct Cyc_Absyn_Pat*pattern;struct Cyc_Core_Opt*pat_vars;struct Cyc_Absyn_Exp*where_clause;struct Cyc_Absyn_Stmt*body;unsigned loc;};struct Cyc_Absyn_Unresolved_b_Absyn_Binding_struct{int tag;struct _tuple1*f1;};struct Cyc_Absyn_Global_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Funname_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Fndecl*f1;};struct Cyc_Absyn_Param_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Local_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Pat_b_Absyn_Binding_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Vardecl{enum Cyc_Absyn_Scope sc;struct _tuple1*name;unsigned varloc;struct Cyc_Absyn_Tqual tq;void*type;struct Cyc_Absyn_Exp*initializer;void*rgn;struct Cyc_List_List*attributes;int escapes;int is_proto;};struct Cyc_Absyn_Fndecl{enum Cyc_Absyn_Scope sc;int is_inline;struct _tuple1*name;struct Cyc_Absyn_Stmt*body;struct Cyc_Absyn_FnInfo i;void*cached_type;struct Cyc_Core_Opt*param_vardecls;struct Cyc_Absyn_Vardecl*fn_vardecl;enum Cyc_Absyn_Scope orig_scope;};struct Cyc_Absyn_Aggrfield{struct _fat_ptr*name;struct Cyc_Absyn_Tqual tq;void*type;struct Cyc_Absyn_Exp*width;struct Cyc_List_List*attributes;struct Cyc_Absyn_Exp*requires_clause;};struct Cyc_Absyn_AggrdeclImpl{struct Cyc_List_List*exist_vars;struct Cyc_List_List*rgn_po;struct Cyc_List_List*fields;int tagged;};struct Cyc_Absyn_Aggrdecl{enum Cyc_Absyn_AggrKind kind;enum Cyc_Absyn_Scope sc;struct _tuple1*name;struct Cyc_List_List*tvs;struct Cyc_Absyn_AggrdeclImpl*impl;struct Cyc_List_List*attributes;int expected_mem_kind;};struct Cyc_Absyn_Datatypefield{struct _tuple1*name;struct Cyc_List_List*typs;unsigned loc;enum Cyc_Absyn_Scope sc;};struct Cyc_Absyn_Datatypedecl{enum Cyc_Absyn_Scope sc;struct _tuple1*name;struct Cyc_List_List*tvs;struct Cyc_Core_Opt*fields;int is_extensible;};struct Cyc_Absyn_Enumfield{struct _tuple1*name;struct Cyc_Absyn_Exp*tag;unsigned loc;};struct Cyc_Absyn_Enumdecl{enum Cyc_Absyn_Scope sc;struct _tuple1*name;struct Cyc_Core_Opt*fields;};struct Cyc_Absyn_Typedefdecl{struct _tuple1*name;struct Cyc_Absyn_Tqual tq;struct Cyc_List_List*tvs;struct Cyc_Core_Opt*kind;void*defn;struct Cyc_List_List*atts;int extern_c;};struct Cyc_Absyn_Var_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Absyn_Fn_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Fndecl*f1;};struct Cyc_Absyn_Let_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Pat*f1;struct Cyc_Core_Opt*f2;struct Cyc_Absyn_Exp*f3;void*f4;};struct Cyc_Absyn_Letv_d_Absyn_Raw_decl_struct{int tag;struct Cyc_List_List*f1;};struct Cyc_Absyn_Region_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Tvar*f1;struct Cyc_Absyn_Vardecl*f2;struct Cyc_Absyn_Exp*f3;};struct Cyc_Absyn_Pool_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Tvar*f1;};struct Cyc_Absyn_Aggr_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Aggrdecl*f1;};struct Cyc_Absyn_Datatype_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Datatypedecl*f1;};struct Cyc_Absyn_Enum_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Enumdecl*f1;};struct Cyc_Absyn_Typedef_d_Absyn_Raw_decl_struct{int tag;struct Cyc_Absyn_Typedefdecl*f1;};struct Cyc_Absyn_Namespace_d_Absyn_Raw_decl_struct{int tag;struct _fat_ptr*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_Using_d_Absyn_Raw_decl_struct{int tag;struct _tuple1*f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_ExternC_d_Absyn_Raw_decl_struct{int tag;struct Cyc_List_List*f1;};struct _tuple11{unsigned f1;struct Cyc_List_List*f2;};struct Cyc_Absyn_ExternCinclude_d_Absyn_Raw_decl_struct{int tag;struct Cyc_List_List*f1;struct Cyc_List_List*f2;struct Cyc_List_List*f3;struct _tuple11*f4;};struct Cyc_Absyn_Porton_d_Absyn_Raw_decl_struct{int tag;};struct Cyc_Absyn_Portoff_d_Absyn_Raw_decl_struct{int tag;};struct Cyc_Absyn_Tempeston_d_Absyn_Raw_decl_struct{int tag;};struct Cyc_Absyn_Tempestoff_d_Absyn_Raw_decl_struct{int tag;};struct Cyc_Absyn_Decl{void*r;unsigned loc;};struct Cyc_Absyn_ArrayElement_Absyn_Designator_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Absyn_FieldName_Absyn_Designator_struct{int tag;struct _fat_ptr*f1;};extern char Cyc_Absyn_EmptyAnnot[11U];struct Cyc_Absyn_EmptyAnnot_Absyn_AbsynAnnot_struct{char*tag;};
# 886 "absyn.h"
int Cyc_Absyn_qvar_cmp(struct _tuple1*,struct _tuple1*);
# 889
int Cyc_Absyn_tvar_cmp(struct Cyc_Absyn_Tvar*,struct Cyc_Absyn_Tvar*);
# 902
void*Cyc_Absyn_compress(void*);
# 907
int Cyc_Absyn_type2bool(int def,void*);
# 916
void*Cyc_Absyn_new_evar(struct Cyc_Core_Opt*k,struct Cyc_Core_Opt*tenv);
# 928
extern void*Cyc_Absyn_heap_rgn_type;extern void*Cyc_Absyn_unique_rgn_type;
# 930
extern void*Cyc_Absyn_empty_effect;
# 932
extern void*Cyc_Absyn_true_type;extern void*Cyc_Absyn_false_type;
# 934
extern void*Cyc_Absyn_var_type(struct Cyc_Absyn_Tvar*);extern void*Cyc_Absyn_access_eff(void*);extern void*Cyc_Absyn_join_eff(struct Cyc_List_List*);extern void*Cyc_Absyn_regionsof_eff(void*);
# 953
struct _tuple1*Cyc_Absyn_datatype_print_arg_qvar (void);
struct _tuple1*Cyc_Absyn_datatype_scanf_arg_qvar (void);
# 963
void*Cyc_Absyn_bounds_one (void);
# 1003
struct Cyc_Absyn_Exp*Cyc_Absyn_uint_exp(unsigned,unsigned);
# 29 "unify.h"
int Cyc_Unify_unify(void*,void*);struct _union_RelnOp_RConst{int tag;unsigned val;};struct _union_RelnOp_RVar{int tag;struct Cyc_Absyn_Vardecl*val;};struct _union_RelnOp_RNumelts{int tag;struct Cyc_Absyn_Vardecl*val;};struct _union_RelnOp_RType{int tag;void*val;};struct _union_RelnOp_RParam{int tag;unsigned val;};struct _union_RelnOp_RParamNumelts{int tag;unsigned val;};struct _union_RelnOp_RReturn{int tag;unsigned val;};union Cyc_Relations_RelnOp{struct _union_RelnOp_RConst RConst;struct _union_RelnOp_RVar RVar;struct _union_RelnOp_RNumelts RNumelts;struct _union_RelnOp_RType RType;struct _union_RelnOp_RParam RParam;struct _union_RelnOp_RParamNumelts RParamNumelts;struct _union_RelnOp_RReturn RReturn;};
# 50 "relations-ap.h"
enum Cyc_Relations_Relation{Cyc_Relations_Req =0U,Cyc_Relations_Rneq =1U,Cyc_Relations_Rlte =2U,Cyc_Relations_Rlt =3U};struct Cyc_Relations_Reln{union Cyc_Relations_RelnOp rop1;enum Cyc_Relations_Relation relation;union Cyc_Relations_RelnOp rop2;};
# 84
struct Cyc_List_List*Cyc_Relations_exp2relns(struct _RegionHandle*r,struct Cyc_Absyn_Exp*e);
# 129
int Cyc_Relations_consistent_relations(struct Cyc_List_List*rlns);struct Cyc_RgnOrder_RgnPO;
# 37 "tcutil.h"
int Cyc_Tcutil_is_char_type(void*);
# 43
int Cyc_Tcutil_is_function_type(void*);
# 46
int Cyc_Tcutil_is_array_type(void*);
# 63
void*Cyc_Tcutil_pointer_elt_type(void*);
# 71
struct Cyc_Absyn_Exp*Cyc_Tcutil_get_bounds_exp(void*def,void*b);
# 83
struct Cyc_Absyn_Exp*Cyc_Tcutil_get_bounds_exp(void*def,void*b);
# 86
int Cyc_Tcutil_is_integral(struct Cyc_Absyn_Exp*);
# 93
void*Cyc_Tcutil_copy_type(void*);
# 100
struct Cyc_Absyn_Kind*Cyc_Tcutil_type_kind(void*);
# 102
int Cyc_Tcutil_coerce_uint_type(struct Cyc_Absyn_Exp*);
# 128
void*Cyc_Tcutil_substitute(struct Cyc_List_List*,void*);
# 146
void*Cyc_Tcutil_fndecl2type(struct Cyc_Absyn_Fndecl*);
# 156
void Cyc_Tcutil_check_bitfield(unsigned,void*field_typ,struct Cyc_Absyn_Exp*width,struct _fat_ptr*fn);
# 158
void Cyc_Tcutil_check_unique_vars(struct Cyc_List_List*,unsigned,struct _fat_ptr err_msg);
void Cyc_Tcutil_check_unique_tvars(unsigned,struct Cyc_List_List*);
# 200
struct Cyc_Absyn_Tvar*Cyc_Tcutil_new_tvar(void*);
# 202
int Cyc_Tcutil_new_tvar_id (void);
# 210
int Cyc_Tcutil_is_const_exp(struct Cyc_Absyn_Exp*);
# 214
int Cyc_Tcutil_extract_const_from_typedef(unsigned,int declared_const,void*);
# 217
void Cyc_Tcutil_check_no_qual(unsigned,void*);
# 228
void*Cyc_Tcutil_promote_array(void*t,void*rgn,int convert_tag);
# 235
int Cyc_Tcutil_force_type2bool(int desired,void*);
# 243
int Cyc_Tcutil_admits_zero(void*);
void Cyc_Tcutil_replace_rops(struct Cyc_List_List*,struct Cyc_Relations_Reln*);
# 246
int Cyc_Tcutil_fast_tvar_cmp(struct Cyc_Absyn_Tvar*,struct Cyc_Absyn_Tvar*);
# 28 "kinds.h"
extern struct Cyc_Absyn_Kind Cyc_Kinds_rk;
extern struct Cyc_Absyn_Kind Cyc_Kinds_ak;
extern struct Cyc_Absyn_Kind Cyc_Kinds_bk;
# 32
extern struct Cyc_Absyn_Kind Cyc_Kinds_ek;
extern struct Cyc_Absyn_Kind Cyc_Kinds_ik;
extern struct Cyc_Absyn_Kind Cyc_Kinds_boolk;
extern struct Cyc_Absyn_Kind Cyc_Kinds_ptrbk;
# 37
extern struct Cyc_Absyn_Kind Cyc_Kinds_trk;
extern struct Cyc_Absyn_Kind Cyc_Kinds_tak;
# 40
extern struct Cyc_Absyn_Kind Cyc_Kinds_tmk;
# 42
extern struct Cyc_Absyn_Kind Cyc_Kinds_urk;
# 44
extern struct Cyc_Absyn_Kind Cyc_Kinds_ubk;
# 66
struct Cyc_Core_Opt*Cyc_Kinds_kind_to_opt(struct Cyc_Absyn_Kind*);
# 73
struct Cyc_Absyn_Kind*Cyc_Kinds_tvar_kind(struct Cyc_Absyn_Tvar*,struct Cyc_Absyn_Kind*def);
# 78
void*Cyc_Kinds_compress_kb(void*);
# 82
int Cyc_Kinds_kind_leq(struct Cyc_Absyn_Kind*,struct Cyc_Absyn_Kind*);
# 86
struct Cyc_Core_Opt*Cyc_Kinds_kind_to_bound_opt(struct Cyc_Absyn_Kind*);
int Cyc_Kinds_constrain_kinds(void*,void*);
# 41 "attributes.h"
int Cyc_Atts_fntype_att(void*);
# 45
void Cyc_Atts_check_field_atts(unsigned,struct _fat_ptr*fn,struct Cyc_List_List*);struct _tuple12{unsigned f1;int f2;};
# 28 "evexp.h"
extern struct _tuple12 Cyc_Evexp_eval_const_uint_exp(struct Cyc_Absyn_Exp*e);struct Cyc_Iter_Iter{void*env;int(*next)(void*env,void*dest);};struct Cyc_Dict_T;struct Cyc_Dict_Dict{int(*rel)(void*,void*);struct _RegionHandle*r;const struct Cyc_Dict_T*t;};extern char Cyc_Dict_Present[8U];struct Cyc_Dict_Present_exn_struct{char*tag;};extern char Cyc_Dict_Absent[7U];struct Cyc_Dict_Absent_exn_struct{char*tag;};extern char Cyc_Tcenv_Env_error[10U];struct Cyc_Tcenv_Env_error_exn_struct{char*tag;};struct Cyc_Tcenv_Genv{struct Cyc_Dict_Dict aggrdecls;struct Cyc_Dict_Dict datatypedecls;struct Cyc_Dict_Dict enumdecls;struct Cyc_Dict_Dict typedefs;struct Cyc_Dict_Dict ordinaries;};struct Cyc_Tcenv_Fenv;struct Cyc_Tcenv_Tenv{struct Cyc_List_List*ns;struct Cyc_Tcenv_Genv*ae;struct Cyc_Tcenv_Fenv*le;int allow_valueof: 1;int in_extern_c_include: 1;int in_tempest: 1;int tempest_generalize: 1;int in_extern_c_inc_repeat: 1;};
# 73 "tcenv.h"
struct Cyc_Tcenv_Fenv*Cyc_Tcenv_bogus_fenv(void*ret_type,struct Cyc_List_List*args);
# 77
struct Cyc_Absyn_Aggrdecl**Cyc_Tcenv_lookup_aggrdecl(struct Cyc_Tcenv_Tenv*,unsigned,struct _tuple1*);
struct Cyc_Absyn_Datatypedecl**Cyc_Tcenv_lookup_datatypedecl(struct Cyc_Tcenv_Tenv*,unsigned,struct _tuple1*);
# 80
struct Cyc_Absyn_Enumdecl**Cyc_Tcenv_lookup_enumdecl(struct Cyc_Tcenv_Tenv*,unsigned,struct _tuple1*);
struct Cyc_Absyn_Typedefdecl*Cyc_Tcenv_lookup_typedefdecl(struct Cyc_Tcenv_Tenv*,unsigned,struct _tuple1*);
# 83
struct Cyc_Tcenv_Tenv*Cyc_Tcenv_allow_valueof(struct Cyc_Tcenv_Tenv*);
# 89
enum Cyc_Tcenv_NewStatus{Cyc_Tcenv_NoneNew =0U,Cyc_Tcenv_InNew =1U,Cyc_Tcenv_InNewAggr =2U};
# 99
struct Cyc_List_List*Cyc_Tcenv_lookup_type_vars(struct Cyc_Tcenv_Tenv*);
# 26 "tcexp.h"
void*Cyc_Tcexp_tcExp(struct Cyc_Tcenv_Tenv*,void**,struct Cyc_Absyn_Exp*);
# 36 "tc.h"
void Cyc_Tc_tcAggrdecl(struct Cyc_Tcenv_Tenv*,unsigned,struct Cyc_Absyn_Aggrdecl*);
void Cyc_Tc_tcDatatypedecl(struct Cyc_Tcenv_Tenv*,unsigned,struct Cyc_Absyn_Datatypedecl*);
void Cyc_Tc_tcEnumdecl(struct Cyc_Tcenv_Tenv*,unsigned,struct Cyc_Absyn_Enumdecl*);struct Cyc_Warn_String_Warn_Warg_struct{int tag;struct _fat_ptr f1;};struct Cyc_Warn_Qvar_Warn_Warg_struct{int tag;struct _tuple1*f1;};struct Cyc_Warn_Typ_Warn_Warg_struct{int tag;void*f1;};struct Cyc_Warn_TypOpt_Warn_Warg_struct{int tag;void*f1;};struct Cyc_Warn_Exp_Warn_Warg_struct{int tag;struct Cyc_Absyn_Exp*f1;};struct Cyc_Warn_Stmt_Warn_Warg_struct{int tag;struct Cyc_Absyn_Stmt*f1;};struct Cyc_Warn_Aggrdecl_Warn_Warg_struct{int tag;struct Cyc_Absyn_Aggrdecl*f1;};struct Cyc_Warn_Tvar_Warn_Warg_struct{int tag;struct Cyc_Absyn_Tvar*f1;};struct Cyc_Warn_KindBound_Warn_Warg_struct{int tag;void*f1;};struct Cyc_Warn_Kind_Warn_Warg_struct{int tag;struct Cyc_Absyn_Kind*f1;};struct Cyc_Warn_Attribute_Warn_Warg_struct{int tag;void*f1;};struct Cyc_Warn_Vardecl_Warn_Warg_struct{int tag;struct Cyc_Absyn_Vardecl*f1;};struct Cyc_Warn_Int_Warn_Warg_struct{int tag;int f1;};
# 67 "warn.h"
void Cyc_Warn_err2(unsigned,struct _fat_ptr);
# 69
void Cyc_Warn_warn2(unsigned,struct _fat_ptr);
# 71
void*Cyc_Warn_impos2(struct _fat_ptr);
# 72 "flags.h"
extern int Cyc_Flags_tovc_r;
enum Cyc_Flags_C_Compilers{Cyc_Flags_Gcc_c =0U,Cyc_Flags_Vc_c =1U};
# 87 "flags.h"
enum Cyc_Flags_Cyclone_Passes{Cyc_Flags_Cpp =0U,Cyc_Flags_Parsing =1U,Cyc_Flags_Binding =2U,Cyc_Flags_CurrentRegion =3U,Cyc_Flags_TypeChecking =4U,Cyc_Flags_Jumps =5U,Cyc_Flags_FlowAnalysis =6U,Cyc_Flags_VCGen =7U,Cyc_Flags_CheckInsertion =8U,Cyc_Flags_Toc =9U,Cyc_Flags_AggregateRemoval =10U,Cyc_Flags_LabelRemoval =11U,Cyc_Flags_EvalOrder =12U,Cyc_Flags_CCompiler =13U,Cyc_Flags_AllPasses =14U};
# 34 "tctyp.h"
void Cyc_Tctyp_check_valid_toplevel_type(unsigned,struct Cyc_Tcenv_Tenv*,void*);
# 29 "currgn.h"
struct _fat_ptr Cyc_CurRgn_curr_rgn_name;
# 33
struct _fat_ptr Cyc_CurRgn_curr_poolrgn_name;
# 39
int Cyc_CurRgn_is_curr_rgn(struct Cyc_Absyn_Tvar*v);
int Cyc_CurRgn_is_not_curr_rgn(struct Cyc_Absyn_Tvar*v);struct Cyc_Tctyp_CVTEnv{unsigned loc;struct Cyc_Tcenv_Tenv*te;struct Cyc_List_List*kind_env;int fn_result;int generalize_evars;struct Cyc_List_List*free_vars;struct Cyc_List_List*free_evars;};
# 84 "tctyp.cyc"
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_type(struct Cyc_Tctyp_CVTEnv,struct Cyc_Absyn_Kind*,void*,int,int);struct _tuple13{void*f1;int f2;};
# 90
static struct Cyc_List_List*Cyc_Tctyp_add_free_evar(struct Cyc_List_List*es,void*e,int b){
void*_tmp0=Cyc_Absyn_compress(e);void*_stmttmp0=_tmp0;void*_tmp1=_stmttmp0;int _tmp2;if(((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp1)->tag == 1U){_LL1: _tmp2=((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp1)->f3;_LL2: {int i=_tmp2;
# 93
{struct Cyc_List_List*es2=es;for(0;es2 != 0;es2=es2->tl){
struct _tuple13*_tmp3=(struct _tuple13*)es2->hd;struct _tuple13*_stmttmp1=_tmp3;struct _tuple13*_tmp4=_stmttmp1;int*_tmp6;void*_tmp5;_LL6: _tmp5=_tmp4->f1;_tmp6=(int*)& _tmp4->f2;_LL7: {void*t=_tmp5;int*b2=_tmp6;
void*_tmp7=Cyc_Absyn_compress(t);void*_stmttmp2=_tmp7;void*_tmp8=_stmttmp2;int _tmp9;if(((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp8)->tag == 1U){_LL9: _tmp9=((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp8)->f3;_LLA: {int j=_tmp9;
# 97
if(i == j){
if(b != *b2)*b2=1;
return es;}
# 101
goto _LL8;}}else{_LLB: _LLC:
 goto _LL8;}_LL8:;}}}
# 105
return({struct Cyc_List_List*_tmpB=_cycalloc(sizeof(*_tmpB));({struct _tuple13*_tmp303=({struct _tuple13*_tmpA=_cycalloc(sizeof(*_tmpA));_tmpA->f1=e,_tmpA->f2=b;_tmpA;});_tmpB->hd=_tmp303;}),_tmpB->tl=es;_tmpB;});}}else{_LL3: _LL4:
 return es;}_LL0:;}
# 111
static struct Cyc_List_List*Cyc_Tctyp_add_bound_tvar(struct Cyc_List_List*tvs,struct Cyc_Absyn_Tvar*tv){
if(tv->identity == - 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmpD=({struct Cyc_Warn_String_Warn_Warg_struct _tmp27D;_tmp27D.tag=0U,({struct _fat_ptr _tmp304=({const char*_tmp11="bound tvar id for ";_tag_fat(_tmp11,sizeof(char),19U);});_tmp27D.f1=_tmp304;});_tmp27D;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmpE=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp27C;_tmp27C.tag=7U,_tmp27C.f1=tv;_tmp27C;});struct Cyc_Warn_String_Warn_Warg_struct _tmpF=({struct Cyc_Warn_String_Warn_Warg_struct _tmp27B;_tmp27B.tag=0U,({struct _fat_ptr _tmp305=({const char*_tmp10=" is NULL";_tag_fat(_tmp10,sizeof(char),9U);});_tmp27B.f1=_tmp305;});_tmp27B;});void*_tmpC[3U];_tmpC[0]=& _tmpD,_tmpC[1]=& _tmpE,_tmpC[2]=& _tmpF;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpC,sizeof(void*),3U));});
return({struct Cyc_List_List*_tmp12=_cycalloc(sizeof(*_tmp12));_tmp12->hd=tv,_tmp12->tl=tvs;_tmp12;});}
# 116
static struct Cyc_List_List*Cyc_Tctyp_remove_bound_tvars(struct Cyc_List_List*tvs,struct Cyc_List_List*btvs,struct Cyc_Absyn_Tvar*(*get)(void*)){
# 118
struct Cyc_List_List*r=0;
for(0;tvs != 0;tvs=tvs->tl){
struct Cyc_List_List*b=btvs;
for(0;b != 0;b=b->tl){
if(({int _tmp306=(get(tvs->hd))->identity;_tmp306 == ((struct Cyc_Absyn_Tvar*)b->hd)->identity;}))
break;}
if(b == 0)
r=({struct Cyc_List_List*_tmp13=_cycalloc(sizeof(*_tmp13));_tmp13->hd=tvs->hd,_tmp13->tl=r;_tmp13;});}
# 127
r=Cyc_List_imp_rev(r);
return r;}
# 135
static void Cyc_Tctyp_check_free_evars(struct Cyc_List_List*free_evars,void*in_typ,unsigned loc){
for(0;free_evars != 0;free_evars=free_evars->tl){
void*e=(*((struct _tuple13*)free_evars->hd)).f1;
{void*_tmp14=Cyc_Absyn_compress(e);void*_stmttmp3=_tmp14;void*_tmp15=_stmttmp3;if(((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp15)->tag == 1U){_LL1: _LL2:
 goto _LL0;}else{_LL3: _LL4:
 continue;}_LL0:;}{
# 142
void*t;
struct _fat_ptr s;
{struct Cyc_Absyn_Kind*_tmp16=Cyc_Tcutil_type_kind(e);struct Cyc_Absyn_Kind*_stmttmp4=_tmp16;struct Cyc_Absyn_Kind*_tmp17=_stmttmp4;switch(((struct Cyc_Absyn_Kind*)_tmp17)->kind){case Cyc_Absyn_RgnKind: switch(((struct Cyc_Absyn_Kind*)_tmp17)->aliasqual){case Cyc_Absyn_Unique: _LL6: _LL7:
 t=Cyc_Absyn_unique_rgn_type;s=({const char*_tmp18="unique region";_tag_fat(_tmp18,sizeof(char),14U);});goto _LL5;case Cyc_Absyn_Aliasable: _LL8: _LL9:
 goto _LLB;case Cyc_Absyn_Top: _LLA: _LLB:
 t=Cyc_Absyn_heap_rgn_type;s=({const char*_tmp19="heap";_tag_fat(_tmp19,sizeof(char),5U);});goto _LL5;default: goto _LL12;}case Cyc_Absyn_EffKind: _LLC: _LLD:
 t=Cyc_Absyn_empty_effect;s=({const char*_tmp1A="{}";_tag_fat(_tmp1A,sizeof(char),3U);});goto _LL5;case Cyc_Absyn_BoolKind: _LLE: _LLF:
 t=Cyc_Absyn_false_type;s=({const char*_tmp1B="@false";_tag_fat(_tmp1B,sizeof(char),7U);});goto _LL5;case Cyc_Absyn_PtrBndKind: _LL10: _LL11:
 t=Cyc_Absyn_bounds_one();s=({const char*_tmp1C="bounds_one";_tag_fat(_tmp1C,sizeof(char),11U);});goto _LL5;default: _LL12: _LL13:
# 152
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1E=({struct Cyc_Warn_String_Warn_Warg_struct _tmp281;_tmp281.tag=0U,({struct _fat_ptr _tmp307=({const char*_tmp23="hidden type variable ";_tag_fat(_tmp23,sizeof(char),22U);});_tmp281.f1=_tmp307;});_tmp281;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp1F=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp280;_tmp280.tag=2U,_tmp280.f1=(void*)e;_tmp280;});struct Cyc_Warn_String_Warn_Warg_struct _tmp20=({struct Cyc_Warn_String_Warn_Warg_struct _tmp27F;_tmp27F.tag=0U,({struct _fat_ptr _tmp308=({const char*_tmp22=" isn't abstracted in type ";_tag_fat(_tmp22,sizeof(char),27U);});_tmp27F.f1=_tmp308;});_tmp27F;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp21=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp27E;_tmp27E.tag=2U,_tmp27E.f1=(void*)in_typ;_tmp27E;});void*_tmp1D[4U];_tmp1D[0]=& _tmp1E,_tmp1D[1]=& _tmp1F,_tmp1D[2]=& _tmp20,_tmp1D[3]=& _tmp21;({unsigned _tmp309=loc;Cyc_Warn_err2(_tmp309,_tag_fat(_tmp1D,sizeof(void*),4U));});});
return;}_LL5:;}
# 155
if(!Cyc_Unify_unify(e,t))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp25=({struct Cyc_Warn_String_Warn_Warg_struct _tmp286;_tmp286.tag=0U,({struct _fat_ptr _tmp30A=({const char*_tmp2C="can't unify evar ";_tag_fat(_tmp2C,sizeof(char),18U);});_tmp286.f1=_tmp30A;});_tmp286;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp26=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp285;_tmp285.tag=2U,_tmp285.f1=(void*)e;_tmp285;});struct Cyc_Warn_String_Warn_Warg_struct _tmp27=({struct Cyc_Warn_String_Warn_Warg_struct _tmp284;_tmp284.tag=0U,({struct _fat_ptr _tmp30B=({const char*_tmp2B=" with ";_tag_fat(_tmp2B,sizeof(char),7U);});_tmp284.f1=_tmp30B;});_tmp284;});struct Cyc_Warn_String_Warn_Warg_struct _tmp28=({struct Cyc_Warn_String_Warn_Warg_struct _tmp283;_tmp283.tag=0U,_tmp283.f1=s;_tmp283;});struct Cyc_Warn_String_Warn_Warg_struct _tmp29=({struct Cyc_Warn_String_Warn_Warg_struct _tmp282;_tmp282.tag=0U,({struct _fat_ptr _tmp30C=({const char*_tmp2A="!";_tag_fat(_tmp2A,sizeof(char),2U);});_tmp282.f1=_tmp30C;});_tmp282;});void*_tmp24[5U];_tmp24[0]=& _tmp25,_tmp24[1]=& _tmp26,_tmp24[2]=& _tmp27,_tmp24[3]=& _tmp28,_tmp24[4]=& _tmp29;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp24,sizeof(void*),5U));});}}}
# 166
static struct Cyc_List_List*Cyc_Tctyp_add_free_tvar(unsigned loc,struct Cyc_List_List*tvs,struct Cyc_Absyn_Tvar*tv){
{struct Cyc_List_List*tvs2=tvs;for(0;tvs2 != 0;tvs2=tvs2->tl){
if(Cyc_strptrcmp(((struct Cyc_Absyn_Tvar*)tvs2->hd)->name,tv->name)== 0){
void*k1=((struct Cyc_Absyn_Tvar*)tvs2->hd)->kind;
void*k2=tv->kind;
if(!Cyc_Kinds_constrain_kinds(k1,k2))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E=({struct Cyc_Warn_String_Warn_Warg_struct _tmp28C;_tmp28C.tag=0U,({struct _fat_ptr _tmp30D=({const char*_tmp36="type variable ";_tag_fat(_tmp36,sizeof(char),15U);});_tmp28C.f1=_tmp30D;});_tmp28C;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp2F=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp28B;_tmp28B.tag=7U,_tmp28B.f1=tv;_tmp28B;});struct Cyc_Warn_String_Warn_Warg_struct _tmp30=({struct Cyc_Warn_String_Warn_Warg_struct _tmp28A;_tmp28A.tag=0U,({struct _fat_ptr _tmp30E=({const char*_tmp35=" used with inconsistent kinds ";_tag_fat(_tmp35,sizeof(char),31U);});_tmp28A.f1=_tmp30E;});_tmp28A;});struct Cyc_Warn_KindBound_Warn_Warg_struct _tmp31=({struct Cyc_Warn_KindBound_Warn_Warg_struct _tmp289;_tmp289.tag=8U,_tmp289.f1=(void*)k1;_tmp289;});struct Cyc_Warn_String_Warn_Warg_struct _tmp32=({struct Cyc_Warn_String_Warn_Warg_struct _tmp288;_tmp288.tag=0U,({
struct _fat_ptr _tmp30F=({const char*_tmp34=" and ";_tag_fat(_tmp34,sizeof(char),6U);});_tmp288.f1=_tmp30F;});_tmp288;});struct Cyc_Warn_KindBound_Warn_Warg_struct _tmp33=({struct Cyc_Warn_KindBound_Warn_Warg_struct _tmp287;_tmp287.tag=8U,_tmp287.f1=(void*)k2;_tmp287;});void*_tmp2D[6U];_tmp2D[0]=& _tmp2E,_tmp2D[1]=& _tmp2F,_tmp2D[2]=& _tmp30,_tmp2D[3]=& _tmp31,_tmp2D[4]=& _tmp32,_tmp2D[5]=& _tmp33;({unsigned _tmp310=loc;Cyc_Warn_err2(_tmp310,_tag_fat(_tmp2D,sizeof(void*),6U));});});
if(tv->identity == - 1)
tv->identity=((struct Cyc_Absyn_Tvar*)tvs2->hd)->identity;else{
if(tv->identity != ((struct Cyc_Absyn_Tvar*)tvs2->hd)->identity)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp38=({struct Cyc_Warn_String_Warn_Warg_struct _tmp28D;_tmp28D.tag=0U,({struct _fat_ptr _tmp311=({const char*_tmp39="same type variable has different identity!";_tag_fat(_tmp39,sizeof(char),43U);});_tmp28D.f1=_tmp311;});_tmp28D;});void*_tmp37[1U];_tmp37[0]=& _tmp38;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp37,sizeof(void*),1U));});}
return tvs;}}}
# 180
({int _tmp312=Cyc_Tcutil_new_tvar_id();tv->identity=_tmp312;});
return({struct Cyc_List_List*_tmp3A=_cycalloc(sizeof(*_tmp3A));_tmp3A->hd=tv,_tmp3A->tl=tvs;_tmp3A;});}
# 186
static struct Cyc_List_List*Cyc_Tctyp_fast_add_free_tvar(struct Cyc_List_List*tvs,struct Cyc_Absyn_Tvar*tv){
if(tv->identity == - 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp3C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp28E;_tmp28E.tag=0U,({struct _fat_ptr _tmp313=({const char*_tmp3D="fast_add_free_tvar: bad identity in tv";_tag_fat(_tmp3D,sizeof(char),39U);});_tmp28E.f1=_tmp313;});_tmp28E;});void*_tmp3B[1U];_tmp3B[0]=& _tmp3C;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp3B,sizeof(void*),1U));});
{struct Cyc_List_List*tvs2=tvs;for(0;tvs2 != 0;tvs2=tvs2->tl){
struct Cyc_Absyn_Tvar*_tmp3E=(struct Cyc_Absyn_Tvar*)tvs2->hd;struct Cyc_Absyn_Tvar*tv2=_tmp3E;
if(tv2->identity == - 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp40=({struct Cyc_Warn_String_Warn_Warg_struct _tmp28F;_tmp28F.tag=0U,({struct _fat_ptr _tmp314=({const char*_tmp41="fast_add_free_tvar: bad identity in tvs2";_tag_fat(_tmp41,sizeof(char),41U);});_tmp28F.f1=_tmp314;});_tmp28F;});void*_tmp3F[1U];_tmp3F[0]=& _tmp40;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp3F,sizeof(void*),1U));});
if(tv2->identity == tv->identity)
return tvs;}}
# 196
return({struct Cyc_List_List*_tmp42=_cycalloc(sizeof(*_tmp42));_tmp42->hd=tv,_tmp42->tl=tvs;_tmp42;});}struct _tuple14{struct Cyc_Absyn_Tvar*f1;int f2;};
# 202
static struct Cyc_List_List*Cyc_Tctyp_fast_add_free_tvar_bool(struct Cyc_List_List*tvs,struct Cyc_Absyn_Tvar*tv,int b){
if(tv->identity == - 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp44=({struct Cyc_Warn_String_Warn_Warg_struct _tmp290;_tmp290.tag=0U,({struct _fat_ptr _tmp315=({const char*_tmp45="fast_add_free_tvar_bool: bad identity in tv";_tag_fat(_tmp45,sizeof(char),44U);});_tmp290.f1=_tmp315;});_tmp290;});void*_tmp43[1U];_tmp43[0]=& _tmp44;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp43,sizeof(void*),1U));});
{struct Cyc_List_List*tvs2=tvs;for(0;tvs2 != 0;tvs2=tvs2->tl){
struct _tuple14*_tmp46=(struct _tuple14*)tvs2->hd;struct _tuple14*_stmttmp5=_tmp46;struct _tuple14*_tmp47=_stmttmp5;int*_tmp49;struct Cyc_Absyn_Tvar*_tmp48;_LL1: _tmp48=_tmp47->f1;_tmp49=(int*)& _tmp47->f2;_LL2: {struct Cyc_Absyn_Tvar*tv2=_tmp48;int*b2=_tmp49;
if(tv2->identity == - 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp4B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp291;_tmp291.tag=0U,({struct _fat_ptr _tmp316=({const char*_tmp4C="fast_add_free_tvar_bool: bad identity in tvs2";_tag_fat(_tmp4C,sizeof(char),46U);});_tmp291.f1=_tmp316;});_tmp291;});void*_tmp4A[1U];_tmp4A[0]=& _tmp4B;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp4A,sizeof(void*),1U));});
if(tv2->identity == tv->identity){
*b2=*b2 || b;
return tvs;}}}}
# 214
return({struct Cyc_List_List*_tmp4E=_cycalloc(sizeof(*_tmp4E));({struct _tuple14*_tmp317=({struct _tuple14*_tmp4D=_cycalloc(sizeof(*_tmp4D));_tmp4D->f1=tv,_tmp4D->f2=b;_tmp4D;});_tmp4E->hd=_tmp317;}),_tmp4E->tl=tvs;_tmp4E;});}
# 221
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_type_level_exp(struct Cyc_Absyn_Exp*e,struct Cyc_Tctyp_CVTEnv env){
env.loc=e->loc;
{void*_tmp4F=e->r;void*_stmttmp6=_tmp4F;void*_tmp50=_stmttmp6;struct Cyc_Absyn_Exp*_tmp51;struct Cyc_Absyn_Exp*_tmp52;void*_tmp53;void*_tmp54;void*_tmp55;struct Cyc_Absyn_Exp*_tmp57;void*_tmp56;struct Cyc_Absyn_Exp*_tmp59;struct Cyc_Absyn_Exp*_tmp58;struct Cyc_Absyn_Exp*_tmp5B;struct Cyc_Absyn_Exp*_tmp5A;struct Cyc_Absyn_Exp*_tmp5D;struct Cyc_Absyn_Exp*_tmp5C;struct Cyc_Absyn_Exp*_tmp60;struct Cyc_Absyn_Exp*_tmp5F;struct Cyc_Absyn_Exp*_tmp5E;struct Cyc_List_List*_tmp61;switch(*((int*)_tmp50)){case 0U: _LL1: _LL2:
 goto _LL4;case 32U: _LL3: _LL4:
 goto _LL6;case 33U: _LL5: _LL6:
 goto _LL8;case 2U: _LL7: _LL8:
 goto _LLA;case 1U: _LL9: _LLA:
 goto _LL0;case 3U: _LLB: _tmp61=((struct Cyc_Absyn_Primop_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_LLC: {struct Cyc_List_List*es=_tmp61;
# 230
for(0;es != 0;es=es->tl){
env=Cyc_Tctyp_i_check_type_level_exp((struct Cyc_Absyn_Exp*)es->hd,env);}
goto _LL0;}case 6U: _LLD: _tmp5E=((struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_tmp5F=((struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_tmp60=((struct Cyc_Absyn_Conditional_e_Absyn_Raw_exp_struct*)_tmp50)->f3;_LLE: {struct Cyc_Absyn_Exp*e1=_tmp5E;struct Cyc_Absyn_Exp*e2=_tmp5F;struct Cyc_Absyn_Exp*e3=_tmp60;
# 234
env=Cyc_Tctyp_i_check_type_level_exp(e1,env);
_tmp5C=e2;_tmp5D=e3;goto _LL10;}case 7U: _LLF: _tmp5C=((struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_tmp5D=((struct Cyc_Absyn_And_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_LL10: {struct Cyc_Absyn_Exp*e1=_tmp5C;struct Cyc_Absyn_Exp*e2=_tmp5D;
_tmp5A=e1;_tmp5B=e2;goto _LL12;}case 8U: _LL11: _tmp5A=((struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_tmp5B=((struct Cyc_Absyn_Or_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_LL12: {struct Cyc_Absyn_Exp*e1=_tmp5A;struct Cyc_Absyn_Exp*e2=_tmp5B;
_tmp58=e1;_tmp59=e2;goto _LL14;}case 9U: _LL13: _tmp58=((struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_tmp59=((struct Cyc_Absyn_SeqExp_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_LL14: {struct Cyc_Absyn_Exp*e1=_tmp58;struct Cyc_Absyn_Exp*e2=_tmp59;
# 239
env=Cyc_Tctyp_i_check_type_level_exp(e1,env);
env=Cyc_Tctyp_i_check_type_level_exp(e2,env);
goto _LL0;}case 14U: _LL15: _tmp56=(void*)((struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_tmp57=((struct Cyc_Absyn_Cast_e_Absyn_Raw_exp_struct*)_tmp50)->f2;_LL16: {void*t=_tmp56;struct Cyc_Absyn_Exp*e1=_tmp57;
# 243
env=Cyc_Tctyp_i_check_type_level_exp(e1,env);
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tak,t,1,0);
goto _LL0;}case 19U: _LL17: _tmp55=(void*)((struct Cyc_Absyn_Offsetof_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_LL18: {void*t=_tmp55;
_tmp54=t;goto _LL1A;}case 17U: _LL19: _tmp54=(void*)((struct Cyc_Absyn_Sizeoftype_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_LL1A: {void*t=_tmp54;
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tak,t,1,0);goto _LL0;}case 39U: _LL1B: _tmp53=(void*)((struct Cyc_Absyn_Valueof_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_LL1C: {void*t=_tmp53;
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_ik,t,1,0);goto _LL0;}case 18U: _LL1D: _tmp52=((struct Cyc_Absyn_Sizeofexp_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_LL1E: {struct Cyc_Absyn_Exp*e=_tmp52;
_tmp51=e;goto _LL20;}case 41U: _LL1F: _tmp51=((struct Cyc_Absyn_Extension_e_Absyn_Raw_exp_struct*)_tmp50)->f1;_LL20: {struct Cyc_Absyn_Exp*e=_tmp51;
env=Cyc_Tctyp_i_check_type_level_exp(e,env);goto _LL0;}default: _LL21: _LL22:
({struct Cyc_Warn_String_Warn_Warg_struct _tmp63=({struct Cyc_Warn_String_Warn_Warg_struct _tmp292;_tmp292.tag=0U,({struct _fat_ptr _tmp318=({const char*_tmp64="bad expression in Tctyp::i_check_type_level_exp";_tag_fat(_tmp64,sizeof(char),48U);});_tmp292.f1=_tmp318;});_tmp292;});void*_tmp62[1U];_tmp62[0]=& _tmp63;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp62,sizeof(void*),1U));});}_LL0:;}
# 253
return env;}
# 256
static int Cyc_Tctyp_typedef_tvar_is_ptr_rgn(struct Cyc_Absyn_Tvar*tvar,struct Cyc_Absyn_Typedefdecl*td){
if(td == 0)
return 1;
if(td->defn == 0)
return 0;{
void*_tmp65=Cyc_Absyn_compress((void*)_check_null(td->defn));void*_stmttmp7=_tmp65;void*_tmp66=_stmttmp7;void*_tmp67;if(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp66)->tag == 3U){_LL1: _tmp67=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp66)->f1).ptr_atts).rgn;_LL2: {void*r=_tmp67;
# 263
void*_tmp68=Cyc_Absyn_compress(r);void*_stmttmp8=_tmp68;void*_tmp69=_stmttmp8;struct Cyc_Absyn_Tvar*_tmp6A;if(((struct Cyc_Absyn_VarType_Absyn_Type_struct*)_tmp69)->tag == 2U){_LL6: _tmp6A=((struct Cyc_Absyn_VarType_Absyn_Type_struct*)_tmp69)->f1;_LL7: {struct Cyc_Absyn_Tvar*tv=_tmp6A;
return Cyc_Absyn_tvar_cmp(tvar,tv)== 0;}}else{_LL8: _LL9:
 return 0;}_LL5:;}}else{_LL3: _LL4:
# 267
 return 0;}_LL0:;}}
# 271
static struct Cyc_Absyn_Kind*Cyc_Tctyp_tvar_inst_kind(struct Cyc_Absyn_Tvar*tvar,struct Cyc_Absyn_Kind*def_kind,struct Cyc_Absyn_Kind*expected_kind,struct Cyc_Absyn_Typedefdecl*td){
# 273
void*_tmp6B=Cyc_Kinds_compress_kb(tvar->kind);void*_stmttmp9=_tmp6B;void*_tmp6C=_stmttmp9;switch(*((int*)_tmp6C)){case 2U: if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp6C)->f2)->kind == Cyc_Absyn_RgnKind){if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp6C)->f2)->aliasqual == Cyc_Absyn_Top){_LL1: _LL2:
 goto _LL4;}else{goto _LL5;}}else{goto _LL5;}case 0U: if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp6C)->f1)->kind == Cyc_Absyn_RgnKind){if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp6C)->f1)->aliasqual == Cyc_Absyn_Top){_LL3: _LL4: {
# 282
enum Cyc_Absyn_KindQual _tmp6D=expected_kind->kind;enum Cyc_Absyn_KindQual k=_tmp6D;
if((((int)k == (int)2U ||(int)k == (int)1U)||(int)k == (int)0U)&&
 Cyc_Tctyp_typedef_tvar_is_ptr_rgn(tvar,td)){
if((int)expected_kind->aliasqual == (int)Cyc_Absyn_Aliasable)
return& Cyc_Kinds_rk;
if((int)expected_kind->aliasqual == (int)Cyc_Absyn_Unique)
return& Cyc_Kinds_urk;}
# 290
return& Cyc_Kinds_trk;}}else{goto _LL5;}}else{goto _LL5;}default: _LL5: _LL6:
 return Cyc_Kinds_tvar_kind(tvar,def_kind);}_LL0:;}
# 294
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_check_type_inst(struct Cyc_Tctyp_CVTEnv env,struct Cyc_List_List**targsp,struct Cyc_List_List*tvs,struct Cyc_Absyn_Kind*expected_kind,int allow_abs_aggr,struct _tuple1*typname,struct Cyc_Absyn_Typedefdecl*td){
# 299
struct Cyc_List_List*_tmp6E=*targsp;struct Cyc_List_List*targs=_tmp6E;
for(0;targs != 0 && tvs != 0;(targs=targs->tl,tvs=tvs->tl)){
void*t=(void*)targs->hd;
struct Cyc_Absyn_Tvar*tv=(struct Cyc_Absyn_Tvar*)tvs->hd;
# 305
{struct _tuple0 _tmp6F=({struct _tuple0 _tmp293;({void*_tmp319=Cyc_Kinds_compress_kb(tv->kind);_tmp293.f1=_tmp319;}),_tmp293.f2=t;_tmp293;});struct _tuple0 _stmttmpA=_tmp6F;struct _tuple0 _tmp70=_stmttmpA;struct Cyc_Absyn_Tvar*_tmp71;if(((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp70.f1)->tag == 1U){if(((struct Cyc_Absyn_VarType_Absyn_Type_struct*)_tmp70.f2)->tag == 2U){_LL1: _tmp71=((struct Cyc_Absyn_VarType_Absyn_Type_struct*)_tmp70.f2)->f1;_LL2: {struct Cyc_Absyn_Tvar*tv2=_tmp71;
# 307
({struct Cyc_List_List*_tmp31A=Cyc_Tctyp_add_free_tvar(env.loc,env.kind_env,tv2);env.kind_env=_tmp31A;});
({struct Cyc_List_List*_tmp31B=Cyc_Tctyp_fast_add_free_tvar_bool(env.free_vars,tv2,1);env.free_vars=_tmp31B;});
continue;}}else{goto _LL3;}}else{_LL3: _LL4:
 goto _LL0;}_LL0:;}{
# 312
struct Cyc_Absyn_Kind*k=Cyc_Kinds_tvar_kind(tv,& Cyc_Kinds_bk);
env=Cyc_Tctyp_i_check_type(env,k,t,1,allow_abs_aggr);
Cyc_Tcutil_check_no_qual(env.loc,t);}}
# 316
if(targs != 0)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp73=({struct Cyc_Warn_String_Warn_Warg_struct _tmp295;_tmp295.tag=0U,({struct _fat_ptr _tmp31C=({const char*_tmp75="too many type arguments for ";_tag_fat(_tmp75,sizeof(char),29U);});_tmp295.f1=_tmp31C;});_tmp295;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp74=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp294;_tmp294.tag=1U,_tmp294.f1=typname;_tmp294;});void*_tmp72[2U];_tmp72[0]=& _tmp73,_tmp72[1]=& _tmp74;({unsigned _tmp31D=env.loc;Cyc_Warn_err2(_tmp31D,_tag_fat(_tmp72,sizeof(void*),2U));});});{
# 319
struct Cyc_List_List*hidden_ts=0;
for(0;tvs != 0;tvs=tvs->tl){
void*e=Cyc_Absyn_new_evar(0,0);
hidden_ts=({struct Cyc_List_List*_tmp76=_cycalloc(sizeof(*_tmp76));_tmp76->hd=e,_tmp76->tl=hidden_ts;_tmp76;});{
struct Cyc_Absyn_Kind*k=Cyc_Tctyp_tvar_inst_kind((struct Cyc_Absyn_Tvar*)tvs->hd,& Cyc_Kinds_bk,expected_kind,td);
env=Cyc_Tctyp_i_check_type(env,k,e,1,allow_abs_aggr);}}
# 326
({struct Cyc_List_List*_tmp31F=({struct Cyc_List_List*_tmp31E=*targsp;((struct Cyc_List_List*(*)(struct Cyc_List_List*x,struct Cyc_List_List*y))Cyc_List_imp_append)(_tmp31E,((struct Cyc_List_List*(*)(struct Cyc_List_List*x))Cyc_List_imp_rev)(hidden_ts));});*targsp=_tmp31F;});
return env;}}struct _tuple15{struct Cyc_Tctyp_CVTEnv f1;struct Cyc_List_List*f2;};
# 331
static struct _tuple15 Cyc_Tctyp_check_clause(struct Cyc_Tctyp_CVTEnv env,struct _fat_ptr clause_name,struct Cyc_Absyn_Exp*clause){
# 334
if(clause == 0)
return({struct _tuple15 _tmp296;_tmp296.f1=env,_tmp296.f2=0;_tmp296;});
Cyc_Tcexp_tcExp(env.te,0,clause);
if(!Cyc_Tcutil_is_integral(clause))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp78=({struct Cyc_Warn_String_Warn_Warg_struct _tmp29A;_tmp29A.tag=0U,_tmp29A.f1=clause_name;_tmp29A;});struct Cyc_Warn_String_Warn_Warg_struct _tmp79=({struct Cyc_Warn_String_Warn_Warg_struct _tmp299;_tmp299.tag=0U,({struct _fat_ptr _tmp320=({const char*_tmp7D=" clause has type ";_tag_fat(_tmp7D,sizeof(char),18U);});_tmp299.f1=_tmp320;});_tmp299;});struct Cyc_Warn_TypOpt_Warn_Warg_struct _tmp7A=({struct Cyc_Warn_TypOpt_Warn_Warg_struct _tmp298;_tmp298.tag=3U,_tmp298.f1=(void*)clause->topt;_tmp298;});struct Cyc_Warn_String_Warn_Warg_struct _tmp7B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp297;_tmp297.tag=0U,({
struct _fat_ptr _tmp321=({const char*_tmp7C=" instead of integral type";_tag_fat(_tmp7C,sizeof(char),26U);});_tmp297.f1=_tmp321;});_tmp297;});void*_tmp77[4U];_tmp77[0]=& _tmp78,_tmp77[1]=& _tmp79,_tmp77[2]=& _tmp7A,_tmp77[3]=& _tmp7B;({unsigned _tmp322=env.loc;Cyc_Warn_err2(_tmp322,_tag_fat(_tmp77,sizeof(void*),4U));});});
env=Cyc_Tctyp_i_check_type_level_exp(clause,env);{
struct Cyc_List_List*_tmp7E=Cyc_Relations_exp2relns(Cyc_Core_heap_region,clause);struct Cyc_List_List*relns=_tmp7E;
if(!Cyc_Relations_consistent_relations(relns))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp80=({struct Cyc_Warn_String_Warn_Warg_struct _tmp29E;_tmp29E.tag=0U,_tmp29E.f1=clause_name;_tmp29E;});struct Cyc_Warn_String_Warn_Warg_struct _tmp81=({struct Cyc_Warn_String_Warn_Warg_struct _tmp29D;_tmp29D.tag=0U,({struct _fat_ptr _tmp323=({const char*_tmp85=" clause '";_tag_fat(_tmp85,sizeof(char),10U);});_tmp29D.f1=_tmp323;});_tmp29D;});struct Cyc_Warn_Exp_Warn_Warg_struct _tmp82=({struct Cyc_Warn_Exp_Warn_Warg_struct _tmp29C;_tmp29C.tag=4U,_tmp29C.f1=clause;_tmp29C;});struct Cyc_Warn_String_Warn_Warg_struct _tmp83=({struct Cyc_Warn_String_Warn_Warg_struct _tmp29B;_tmp29B.tag=0U,({struct _fat_ptr _tmp324=({const char*_tmp84="' may be unsatisfiable";_tag_fat(_tmp84,sizeof(char),23U);});_tmp29B.f1=_tmp324;});_tmp29B;});void*_tmp7F[4U];_tmp7F[0]=& _tmp80,_tmp7F[1]=& _tmp81,_tmp7F[2]=& _tmp82,_tmp7F[3]=& _tmp83;({unsigned _tmp325=clause->loc;Cyc_Warn_err2(_tmp325,_tag_fat(_tmp7F,sizeof(void*),4U));});});
return({struct _tuple15 _tmp29F;_tmp29F.f1=env,_tmp29F.f2=relns;_tmp29F;});}}
# 348
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_aggr(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*expected_kind,union Cyc_Absyn_AggrInfo*info,struct Cyc_List_List**targs,int allow_abs_aggr){
# 351
struct Cyc_Tctyp_CVTEnv _tmp86=env;struct Cyc_Tcenv_Tenv*_tmp88;unsigned _tmp87;_LL1: _tmp87=_tmp86.loc;_tmp88=_tmp86.te;_LL2: {unsigned loc=_tmp87;struct Cyc_Tcenv_Tenv*te=_tmp88;
union Cyc_Absyn_AggrInfo _tmp89=*info;union Cyc_Absyn_AggrInfo _stmttmpB=_tmp89;union Cyc_Absyn_AggrInfo _tmp8A=_stmttmpB;struct Cyc_Absyn_Aggrdecl*_tmp8B;struct Cyc_Core_Opt*_tmp8E;struct _tuple1*_tmp8D;enum Cyc_Absyn_AggrKind _tmp8C;if((_tmp8A.UnknownAggr).tag == 1){_LL4: _tmp8C=((_tmp8A.UnknownAggr).val).f1;_tmp8D=((_tmp8A.UnknownAggr).val).f2;_tmp8E=((_tmp8A.UnknownAggr).val).f3;_LL5: {enum Cyc_Absyn_AggrKind k=_tmp8C;struct _tuple1*n=_tmp8D;struct Cyc_Core_Opt*tgd=_tmp8E;
# 354
struct Cyc_Absyn_Aggrdecl**adp;
{struct _handler_cons _tmp8F;_push_handler(& _tmp8F);{int _tmp91=0;if(setjmp(_tmp8F.handler))_tmp91=1;if(!_tmp91){
adp=Cyc_Tcenv_lookup_aggrdecl(te,loc,n);{
struct Cyc_Absyn_Aggrdecl*_tmp92=*adp;struct Cyc_Absyn_Aggrdecl*ad=_tmp92;
if((int)ad->kind != (int)k){
if((int)ad->kind == (int)Cyc_Absyn_StructA)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp94=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2A3;_tmp2A3.tag=0U,({struct _fat_ptr _tmp326=({const char*_tmp99="expecting struct ";_tag_fat(_tmp99,sizeof(char),18U);});_tmp2A3.f1=_tmp326;});_tmp2A3;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp95=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2A2;_tmp2A2.tag=1U,_tmp2A2.f1=n;_tmp2A2;});struct Cyc_Warn_String_Warn_Warg_struct _tmp96=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2A1;_tmp2A1.tag=0U,({struct _fat_ptr _tmp327=({const char*_tmp98="instead of union ";_tag_fat(_tmp98,sizeof(char),18U);});_tmp2A1.f1=_tmp327;});_tmp2A1;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp97=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2A0;_tmp2A0.tag=1U,_tmp2A0.f1=n;_tmp2A0;});void*_tmp93[4U];_tmp93[0]=& _tmp94,_tmp93[1]=& _tmp95,_tmp93[2]=& _tmp96,_tmp93[3]=& _tmp97;({unsigned _tmp328=loc;Cyc_Warn_err2(_tmp328,_tag_fat(_tmp93,sizeof(void*),4U));});});else{
# 362
({struct Cyc_Warn_String_Warn_Warg_struct _tmp9B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2A7;_tmp2A7.tag=0U,({struct _fat_ptr _tmp329=({const char*_tmpA0="expecting union ";_tag_fat(_tmpA0,sizeof(char),17U);});_tmp2A7.f1=_tmp329;});_tmp2A7;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp9C=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2A6;_tmp2A6.tag=1U,_tmp2A6.f1=n;_tmp2A6;});struct Cyc_Warn_String_Warn_Warg_struct _tmp9D=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2A5;_tmp2A5.tag=0U,({struct _fat_ptr _tmp32A=({const char*_tmp9F="instead of struct ";_tag_fat(_tmp9F,sizeof(char),19U);});_tmp2A5.f1=_tmp32A;});_tmp2A5;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp9E=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2A4;_tmp2A4.tag=1U,_tmp2A4.f1=n;_tmp2A4;});void*_tmp9A[4U];_tmp9A[0]=& _tmp9B,_tmp9A[1]=& _tmp9C,_tmp9A[2]=& _tmp9D,_tmp9A[3]=& _tmp9E;({unsigned _tmp32B=loc;Cyc_Warn_err2(_tmp32B,_tag_fat(_tmp9A,sizeof(void*),4U));});});}}
# 364
if(((unsigned)tgd &&(int)tgd->v)&&(!((unsigned)ad->impl)|| !((struct Cyc_Absyn_AggrdeclImpl*)_check_null(ad->impl))->tagged))
({struct Cyc_Warn_String_Warn_Warg_struct _tmpA2=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2A9;_tmp2A9.tag=0U,({struct _fat_ptr _tmp32C=({const char*_tmpA4="@tagged qualfiers don't agree on union ";_tag_fat(_tmpA4,sizeof(char),40U);});_tmp2A9.f1=_tmp32C;});_tmp2A9;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpA3=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2A8;_tmp2A8.tag=1U,_tmp2A8.f1=n;_tmp2A8;});void*_tmpA1[2U];_tmpA1[0]=& _tmpA2,_tmpA1[1]=& _tmpA3;({unsigned _tmp32D=loc;Cyc_Warn_err2(_tmp32D,_tag_fat(_tmpA1,sizeof(void*),2U));});});
# 367
({union Cyc_Absyn_AggrInfo _tmp32E=Cyc_Absyn_KnownAggr(adp);*info=_tmp32E;});}
# 356
;_pop_handler();}else{void*_tmp90=(void*)Cyc_Core_get_exn_thrown();void*_tmpA5=_tmp90;void*_tmpA6;if(((struct Cyc_Dict_Absent_exn_struct*)_tmpA5)->tag == Cyc_Dict_Absent){_LL9: _LLA:
# 371
({struct Cyc_Tcenv_Tenv*_tmp330=te;unsigned _tmp32F=loc;Cyc_Tc_tcAggrdecl(_tmp330,_tmp32F,({struct Cyc_Absyn_Aggrdecl*_tmpA7=_cycalloc(sizeof(*_tmpA7));_tmpA7->kind=k,_tmpA7->sc=Cyc_Absyn_Extern,_tmpA7->name=n,_tmpA7->tvs=0,_tmpA7->impl=0,_tmpA7->attributes=0,_tmpA7->expected_mem_kind=0;_tmpA7;}));});
adp=Cyc_Tcenv_lookup_aggrdecl(te,loc,n);
({union Cyc_Absyn_AggrInfo _tmp331=Cyc_Absyn_KnownAggr(adp);*info=_tmp331;});
# 375
if(*targs != 0){
({struct Cyc_Warn_String_Warn_Warg_struct _tmpA9=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2AC;_tmp2AC.tag=0U,({struct _fat_ptr _tmp332=({const char*_tmpAD="declare parameterized type ";_tag_fat(_tmpAD,sizeof(char),28U);});_tmp2AC.f1=_tmp332;});_tmp2AC;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpAA=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2AB;_tmp2AB.tag=1U,_tmp2AB.f1=n;_tmp2AB;});struct Cyc_Warn_String_Warn_Warg_struct _tmpAB=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2AA;_tmp2AA.tag=0U,({struct _fat_ptr _tmp333=({const char*_tmpAC=" before using";_tag_fat(_tmpAC,sizeof(char),14U);});_tmp2AA.f1=_tmp333;});_tmp2AA;});void*_tmpA8[3U];_tmpA8[0]=& _tmpA9,_tmpA8[1]=& _tmpAA,_tmpA8[2]=& _tmpAB;({unsigned _tmp334=loc;Cyc_Warn_err2(_tmp334,_tag_fat(_tmpA8,sizeof(void*),3U));});});
return env;}
# 379
goto _LL8;}else{_LLB: _tmpA6=_tmpA5;_LLC: {void*exn=_tmpA6;(int)_rethrow(exn);}}_LL8:;}}}
# 381
_tmp8B=*adp;goto _LL7;}}else{_LL6: _tmp8B=*(_tmp8A.KnownAggr).val;_LL7: {struct Cyc_Absyn_Aggrdecl*ad=_tmp8B;
# 383
env=Cyc_Tctyp_check_type_inst(env,targs,ad->tvs,expected_kind,allow_abs_aggr,ad->name,0);
# 385
if((allow_abs_aggr && ad->impl == 0)&& !
Cyc_Kinds_kind_leq(& Cyc_Kinds_ak,expected_kind))
# 390
ad->expected_mem_kind=1;
return env;}}_LL3:;}}
# 396
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_datatype(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*expected_kind,union Cyc_Absyn_DatatypeInfo*info,struct Cyc_List_List**targsp,int allow_abs_aggr){
# 399
struct Cyc_Tctyp_CVTEnv _tmpAE=env;struct Cyc_Tcenv_Tenv*_tmpB0;unsigned _tmpAF;_LL1: _tmpAF=_tmpAE.loc;_tmpB0=_tmpAE.te;_LL2: {unsigned loc=_tmpAF;struct Cyc_Tcenv_Tenv*te=_tmpB0;
struct Cyc_List_List*_tmpB1=*targsp;struct Cyc_List_List*targs=_tmpB1;
union Cyc_Absyn_DatatypeInfo _tmpB2=*info;union Cyc_Absyn_DatatypeInfo _stmttmpC=_tmpB2;union Cyc_Absyn_DatatypeInfo _tmpB3=_stmttmpC;struct Cyc_Absyn_Datatypedecl*_tmpB4;int _tmpB6;struct _tuple1*_tmpB5;if((_tmpB3.UnknownDatatype).tag == 1){_LL4: _tmpB5=((_tmpB3.UnknownDatatype).val).name;_tmpB6=((_tmpB3.UnknownDatatype).val).is_extensible;_LL5: {struct _tuple1*n=_tmpB5;int is_x=_tmpB6;
# 403
struct Cyc_Absyn_Datatypedecl**tudp;
{struct _handler_cons _tmpB7;_push_handler(& _tmpB7);{int _tmpB9=0;if(setjmp(_tmpB7.handler))_tmpB9=1;if(!_tmpB9){tudp=Cyc_Tcenv_lookup_datatypedecl(te,loc,n);;_pop_handler();}else{void*_tmpB8=(void*)Cyc_Core_get_exn_thrown();void*_tmpBA=_tmpB8;void*_tmpBB;if(((struct Cyc_Dict_Absent_exn_struct*)_tmpBA)->tag == Cyc_Dict_Absent){_LL9: _LLA:
# 407
({struct Cyc_Tcenv_Tenv*_tmp336=te;unsigned _tmp335=loc;Cyc_Tc_tcDatatypedecl(_tmp336,_tmp335,({struct Cyc_Absyn_Datatypedecl*_tmpBC=_cycalloc(sizeof(*_tmpBC));_tmpBC->sc=Cyc_Absyn_Extern,_tmpBC->name=n,_tmpBC->tvs=0,_tmpBC->fields=0,_tmpBC->is_extensible=is_x;_tmpBC;}));});
tudp=Cyc_Tcenv_lookup_datatypedecl(te,loc,n);
# 410
if(targs != 0){
({struct Cyc_Warn_String_Warn_Warg_struct _tmpBE=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2AF;_tmp2AF.tag=0U,({struct _fat_ptr _tmp337=({const char*_tmpC2="declare parameterized datatype ";_tag_fat(_tmpC2,sizeof(char),32U);});_tmp2AF.f1=_tmp337;});_tmp2AF;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpBF=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2AE;_tmp2AE.tag=1U,_tmp2AE.f1=n;_tmp2AE;});struct Cyc_Warn_String_Warn_Warg_struct _tmpC0=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2AD;_tmp2AD.tag=0U,({struct _fat_ptr _tmp338=({const char*_tmpC1=" before using";_tag_fat(_tmpC1,sizeof(char),14U);});_tmp2AD.f1=_tmp338;});_tmp2AD;});void*_tmpBD[3U];_tmpBD[0]=& _tmpBE,_tmpBD[1]=& _tmpBF,_tmpBD[2]=& _tmpC0;({unsigned _tmp339=loc;Cyc_Warn_err2(_tmp339,_tag_fat(_tmpBD,sizeof(void*),3U));});});
return env;}
# 414
goto _LL8;}else{_LLB: _tmpBB=_tmpBA;_LLC: {void*exn=_tmpBB;(int)_rethrow(exn);}}_LL8:;}}}
# 418
if(is_x && !(*tudp)->is_extensible)
({struct Cyc_Warn_String_Warn_Warg_struct _tmpC4=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B2;_tmp2B2.tag=0U,({struct _fat_ptr _tmp33A=({const char*_tmpC8="datatype ";_tag_fat(_tmpC8,sizeof(char),10U);});_tmp2B2.f1=_tmp33A;});_tmp2B2;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpC5=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2B1;_tmp2B1.tag=1U,_tmp2B1.f1=n;_tmp2B1;});struct Cyc_Warn_String_Warn_Warg_struct _tmpC6=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B0;_tmp2B0.tag=0U,({struct _fat_ptr _tmp33B=({const char*_tmpC7=" was not declared @extensible";_tag_fat(_tmpC7,sizeof(char),30U);});_tmp2B0.f1=_tmp33B;});_tmp2B0;});void*_tmpC3[3U];_tmpC3[0]=& _tmpC4,_tmpC3[1]=& _tmpC5,_tmpC3[2]=& _tmpC6;({unsigned _tmp33C=loc;Cyc_Warn_err2(_tmp33C,_tag_fat(_tmpC3,sizeof(void*),3U));});});
({union Cyc_Absyn_DatatypeInfo _tmp33D=Cyc_Absyn_KnownDatatype(tudp);*info=_tmp33D;});
_tmpB4=*tudp;goto _LL7;}}else{_LL6: _tmpB4=*(_tmpB3.KnownDatatype).val;_LL7: {struct Cyc_Absyn_Datatypedecl*tud=_tmpB4;
# 423
return Cyc_Tctyp_check_type_inst(env,targsp,tud->tvs,expected_kind,allow_abs_aggr,tud->name,0);}}_LL3:;}}
# 429
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_datatype_field(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*expected_kind,union Cyc_Absyn_DatatypeFieldInfo*info,struct Cyc_List_List**targsp,int allow_abs_aggr){
# 432
union Cyc_Absyn_DatatypeFieldInfo _tmpC9=*info;union Cyc_Absyn_DatatypeFieldInfo _stmttmpD=_tmpC9;union Cyc_Absyn_DatatypeFieldInfo _tmpCA=_stmttmpD;struct Cyc_Absyn_Datatypefield*_tmpCC;struct Cyc_Absyn_Datatypedecl*_tmpCB;int _tmpCF;struct _tuple1*_tmpCE;struct _tuple1*_tmpCD;if((_tmpCA.UnknownDatatypefield).tag == 1){_LL1: _tmpCD=((_tmpCA.UnknownDatatypefield).val).datatype_name;_tmpCE=((_tmpCA.UnknownDatatypefield).val).field_name;_tmpCF=((_tmpCA.UnknownDatatypefield).val).is_extensible;_LL2: {struct _tuple1*tname=_tmpCD;struct _tuple1*fname=_tmpCE;int is_x=_tmpCF;
# 435
struct Cyc_Absyn_Datatypedecl*tud=*Cyc_Tcenv_lookup_datatypedecl(env.te,env.loc,tname);
struct Cyc_Absyn_Datatypefield*tuf;
# 440
{struct Cyc_List_List*_tmpD0=(struct Cyc_List_List*)((struct Cyc_Core_Opt*)_check_null(tud->fields))->v;struct Cyc_List_List*fs=_tmpD0;for(0;1;fs=fs->tl){
if(fs == 0)({struct Cyc_Warn_String_Warn_Warg_struct _tmpD2=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B3;_tmp2B3.tag=0U,({struct _fat_ptr _tmp33E=({const char*_tmpD3="Tcutil found a bad datatypefield";_tag_fat(_tmpD3,sizeof(char),33U);});_tmp2B3.f1=_tmp33E;});_tmp2B3;});void*_tmpD1[1U];_tmpD1[0]=& _tmpD2;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpD1,sizeof(void*),1U));});
if(Cyc_Absyn_qvar_cmp(((struct Cyc_Absyn_Datatypefield*)fs->hd)->name,fname)== 0){
tuf=(struct Cyc_Absyn_Datatypefield*)fs->hd;
break;}}}
# 447
({union Cyc_Absyn_DatatypeFieldInfo _tmp33F=Cyc_Absyn_KnownDatatypefield(tud,tuf);*info=_tmp33F;});
_tmpCB=tud;_tmpCC=tuf;goto _LL4;}}else{_LL3: _tmpCB=((_tmpCA.KnownDatatypefield).val).f1;_tmpCC=((_tmpCA.KnownDatatypefield).val).f2;_LL4: {struct Cyc_Absyn_Datatypedecl*tud=_tmpCB;struct Cyc_Absyn_Datatypefield*tuf=_tmpCC;
# 450
return Cyc_Tctyp_check_type_inst(env,targsp,tud->tvs,expected_kind,allow_abs_aggr,tuf->name,0);}}_LL0:;}
# 455
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_unary_app(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*k,struct Cyc_List_List*ts,int put_in_effect,struct _fat_ptr s){
# 457
if(((int(*)(struct Cyc_List_List*x))Cyc_List_length)(ts)!= 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmpD5=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B5;_tmp2B5.tag=0U,_tmp2B5.f1=s;_tmp2B5;});struct Cyc_Warn_String_Warn_Warg_struct _tmpD6=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B4;_tmp2B4.tag=0U,({struct _fat_ptr _tmp340=({const char*_tmpD7=" has wrong number of arguments";_tag_fat(_tmpD7,sizeof(char),31U);});_tmp2B4.f1=_tmp340;});_tmp2B4;});void*_tmpD4[2U];_tmpD4[0]=& _tmpD5,_tmpD4[1]=& _tmpD6;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpD4,sizeof(void*),2U));});
return Cyc_Tctyp_i_check_type(env,k,(void*)((struct Cyc_List_List*)_check_null(ts))->hd,put_in_effect,1);}
# 462
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_type_app(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*expected_kind,void*c,struct Cyc_List_List**targsp,int put_in_effect,int allow_abs_aggr){
# 465
struct Cyc_Tctyp_CVTEnv _tmpD8=env;struct Cyc_Tcenv_Tenv*_tmpDA;unsigned _tmpD9;_LL1: _tmpD9=_tmpD8.loc;_tmpDA=_tmpD8.te;_LL2: {unsigned loc=_tmpD9;struct Cyc_Tcenv_Tenv*te=_tmpDA;
struct Cyc_List_List*_tmpDB=*targsp;struct Cyc_List_List*ts=_tmpDB;
void*_tmpDC=c;union Cyc_Absyn_DatatypeFieldInfo*_tmpDD;union Cyc_Absyn_DatatypeInfo*_tmpDE;union Cyc_Absyn_AggrInfo*_tmpDF;struct Cyc_List_List*_tmpE0;struct Cyc_Absyn_Enumdecl**_tmpE2;struct _tuple1*_tmpE1;switch(*((int*)_tmpDC)){case 1U: _LL4: _LL5:
# 469
 goto _LL7;case 2U: _LL6: _LL7: goto _LL9;case 0U: _LL8: _LL9: goto _LLB;case 7U: _LLA: _LLB:
 goto _LLD;case 6U: _LLC: _LLD: goto _LLF;case 5U: _LLE: _LLF: goto _LL11;case 12U: _LL10: _LL11:
 goto _LL13;case 11U: _LL12: _LL13: goto _LL15;case 14U: _LL14: _LL15: goto _LL17;case 17U: _LL16: _LL17:
# 473
 if(ts != 0)
({struct Cyc_Warn_Typ_Warn_Warg_struct _tmpE4=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2B7;_tmp2B7.tag=2U,({typeof((void*)((void*)({struct Cyc_Absyn_AppType_Absyn_Type_struct*_tmpE7=_cycalloc(sizeof(*_tmpE7));_tmpE7->tag=0U,_tmpE7->f1=c,_tmpE7->f2=ts;_tmpE7;})))_tmp341=(void*)((void*)({struct Cyc_Absyn_AppType_Absyn_Type_struct*_tmpE7=_cycalloc(sizeof(*_tmpE7));_tmpE7->tag=0U,_tmpE7->f1=c,_tmpE7->f2=ts;_tmpE7;}));_tmp2B7.f1=_tmp341;});_tmp2B7;});struct Cyc_Warn_String_Warn_Warg_struct _tmpE5=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B6;_tmp2B6.tag=0U,({struct _fat_ptr _tmp342=({const char*_tmpE6=" applied to argument(s)";_tag_fat(_tmpE6,sizeof(char),24U);});_tmp2B6.f1=_tmp342;});_tmp2B6;});void*_tmpE3[2U];_tmpE3[0]=& _tmpE4,_tmpE3[1]=& _tmpE5;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpE3,sizeof(void*),2U));});
return env;case 9U: _LL18: _LL19:
# 477
 for(0;ts != 0;ts=ts->tl){
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_ek,(void*)ts->hd,1,1);}
return env;case 15U: _LL1A: _tmpE1=((struct Cyc_Absyn_EnumCon_Absyn_TyCon_struct*)_tmpDC)->f1;_tmpE2=(struct Cyc_Absyn_Enumdecl**)&((struct Cyc_Absyn_EnumCon_Absyn_TyCon_struct*)_tmpDC)->f2;_LL1B: {struct _tuple1*n=_tmpE1;struct Cyc_Absyn_Enumdecl**edp=_tmpE2;
# 481
if(ts != 0)({struct Cyc_Warn_String_Warn_Warg_struct _tmpE9=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B8;_tmp2B8.tag=0U,({struct _fat_ptr _tmp343=({const char*_tmpEA="enum applied to argument(s)";_tag_fat(_tmpEA,sizeof(char),28U);});_tmp2B8.f1=_tmp343;});_tmp2B8;});void*_tmpE8[1U];_tmpE8[0]=& _tmpE9;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpE8,sizeof(void*),1U));});
if(*edp == 0 ||((struct Cyc_Absyn_Enumdecl*)_check_null(*edp))->fields == 0){
struct _handler_cons _tmpEB;_push_handler(& _tmpEB);{int _tmpED=0;if(setjmp(_tmpEB.handler))_tmpED=1;if(!_tmpED){({struct Cyc_Absyn_Enumdecl*_tmp344=*Cyc_Tcenv_lookup_enumdecl(te,loc,n);*edp=_tmp344;});;_pop_handler();}else{void*_tmpEC=(void*)Cyc_Core_get_exn_thrown();void*_tmpEE=_tmpEC;void*_tmpEF;if(((struct Cyc_Dict_Absent_exn_struct*)_tmpEE)->tag == Cyc_Dict_Absent){_LL2F: _LL30:
# 485
({struct Cyc_Tcenv_Tenv*_tmp346=te;unsigned _tmp345=loc;Cyc_Tc_tcEnumdecl(_tmp346,_tmp345,({struct Cyc_Absyn_Enumdecl*_tmpF0=_cycalloc(sizeof(*_tmpF0));_tmpF0->sc=Cyc_Absyn_Extern,_tmpF0->name=n,_tmpF0->fields=0;_tmpF0;}));});
({struct Cyc_Absyn_Enumdecl*_tmp347=*Cyc_Tcenv_lookup_enumdecl(te,loc,n);*edp=_tmp347;});
goto _LL2E;}else{_LL31: _tmpEF=_tmpEE;_LL32: {void*exn=_tmpEF;(int)_rethrow(exn);}}_LL2E:;}}}
# 490
return env;}case 16U: _LL1C: _tmpE0=((struct Cyc_Absyn_AnonEnumCon_Absyn_TyCon_struct*)_tmpDC)->f1;_LL1D: {struct Cyc_List_List*fs=_tmpE0;
# 492
if(ts != 0)
({struct Cyc_Warn_String_Warn_Warg_struct _tmpF2=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2B9;_tmp2B9.tag=0U,({struct _fat_ptr _tmp348=({const char*_tmpF3="enum applied to argument(s)";_tag_fat(_tmpF3,sizeof(char),28U);});_tmp2B9.f1=_tmp348;});_tmp2B9;});void*_tmpF1[1U];_tmpF1[0]=& _tmpF2;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmpF1,sizeof(void*),1U));});{
# 495
struct Cyc_List_List*prev_fields=0;
unsigned tag_count=0U;
for(0;fs != 0;fs=fs->tl){
struct Cyc_Absyn_Enumfield*_tmpF4=(struct Cyc_Absyn_Enumfield*)fs->hd;struct Cyc_Absyn_Enumfield*f=_tmpF4;
if(((int(*)(int(*compare)(struct _fat_ptr*,struct _fat_ptr*),struct Cyc_List_List*l,struct _fat_ptr*x))Cyc_List_mem)(Cyc_strptrcmp,prev_fields,(*f->name).f2))
({struct Cyc_Warn_String_Warn_Warg_struct _tmpF6=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2BB;_tmp2BB.tag=0U,({struct _fat_ptr _tmp349=({const char*_tmpF8="duplicate enum field name ";_tag_fat(_tmpF8,sizeof(char),27U);});_tmp2BB.f1=_tmp349;});_tmp2BB;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpF7=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2BA;_tmp2BA.tag=1U,_tmp2BA.f1=f->name;_tmp2BA;});void*_tmpF5[2U];_tmpF5[0]=& _tmpF6,_tmpF5[1]=& _tmpF7;({unsigned _tmp34A=f->loc;Cyc_Warn_err2(_tmp34A,_tag_fat(_tmpF5,sizeof(void*),2U));});});else{
# 502
prev_fields=({struct Cyc_List_List*_tmpF9=_cycalloc(sizeof(*_tmpF9));_tmpF9->hd=(*f->name).f2,_tmpF9->tl=prev_fields;_tmpF9;});}
if(f->tag == 0)
({struct Cyc_Absyn_Exp*_tmp34B=Cyc_Absyn_uint_exp(tag_count,f->loc);f->tag=_tmp34B;});else{
if(!Cyc_Tcutil_is_const_exp((struct Cyc_Absyn_Exp*)_check_null(f->tag)))
({struct Cyc_Warn_String_Warn_Warg_struct _tmpFB=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2BE;_tmp2BE.tag=0U,({struct _fat_ptr _tmp34C=({const char*_tmpFF="enum field ";_tag_fat(_tmpFF,sizeof(char),12U);});_tmp2BE.f1=_tmp34C;});_tmp2BE;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmpFC=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2BD;_tmp2BD.tag=1U,_tmp2BD.f1=f->name;_tmp2BD;});struct Cyc_Warn_String_Warn_Warg_struct _tmpFD=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2BC;_tmp2BC.tag=0U,({struct _fat_ptr _tmp34D=({const char*_tmpFE=": expression is not constant";_tag_fat(_tmpFE,sizeof(char),29U);});_tmp2BC.f1=_tmp34D;});_tmp2BC;});void*_tmpFA[3U];_tmpFA[0]=& _tmpFB,_tmpFA[1]=& _tmpFC,_tmpFA[2]=& _tmpFD;({unsigned _tmp34E=loc;Cyc_Warn_err2(_tmp34E,_tag_fat(_tmpFA,sizeof(void*),3U));});});}
tag_count=(unsigned)1 + (Cyc_Evexp_eval_const_uint_exp((struct Cyc_Absyn_Exp*)_check_null(f->tag))).f1;}
# 509
return env;}}case 10U: _LL1E: _LL1F:
 return({struct Cyc_Tctyp_CVTEnv _tmp350=env;struct Cyc_List_List*_tmp34F=ts;Cyc_Tctyp_i_check_unary_app(_tmp350,& Cyc_Kinds_tak,_tmp34F,1,({const char*_tmp100="regions";_tag_fat(_tmp100,sizeof(char),8U);}));});case 3U: _LL20: _LL21:
 return({struct Cyc_Tctyp_CVTEnv _tmp352=env;struct Cyc_List_List*_tmp351=ts;Cyc_Tctyp_i_check_unary_app(_tmp352,& Cyc_Kinds_trk,_tmp351,1,({const char*_tmp101="region_t";_tag_fat(_tmp101,sizeof(char),9U);}));});case 8U: _LL22: _LL23:
 return({struct Cyc_Tctyp_CVTEnv _tmp354=env;struct Cyc_List_List*_tmp353=ts;Cyc_Tctyp_i_check_unary_app(_tmp354,& Cyc_Kinds_trk,_tmp353,1,({const char*_tmp102="access";_tag_fat(_tmp102,sizeof(char),7U);}));});case 4U: _LL24: _LL25:
 return({struct Cyc_Tctyp_CVTEnv _tmp356=env;struct Cyc_List_List*_tmp355=ts;Cyc_Tctyp_i_check_unary_app(_tmp356,& Cyc_Kinds_ik,_tmp355,0,({const char*_tmp103="tag_t";_tag_fat(_tmp103,sizeof(char),6U);}));});case 13U: _LL26: _LL27:
 return({struct Cyc_Tctyp_CVTEnv _tmp358=env;struct Cyc_List_List*_tmp357=ts;Cyc_Tctyp_i_check_unary_app(_tmp358,& Cyc_Kinds_ik,_tmp357,0,({const char*_tmp104="@thin";_tag_fat(_tmp104,sizeof(char),6U);}));});case 20U: _LL28: _tmpDF=(union Cyc_Absyn_AggrInfo*)&((struct Cyc_Absyn_AggrCon_Absyn_TyCon_struct*)_tmpDC)->f1;_LL29: {union Cyc_Absyn_AggrInfo*info=_tmpDF;
# 516
return Cyc_Tctyp_i_check_aggr(env,expected_kind,info,targsp,allow_abs_aggr);}case 18U: _LL2A: _tmpDE=(union Cyc_Absyn_DatatypeInfo*)&((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)_tmpDC)->f1;_LL2B: {union Cyc_Absyn_DatatypeInfo*info=_tmpDE;
# 518
return Cyc_Tctyp_i_check_datatype(env,expected_kind,info,targsp,allow_abs_aggr);}default: _LL2C: _tmpDD=(union Cyc_Absyn_DatatypeFieldInfo*)&((struct Cyc_Absyn_DatatypeFieldCon_Absyn_TyCon_struct*)_tmpDC)->f1;_LL2D: {union Cyc_Absyn_DatatypeFieldInfo*info=_tmpDD;
# 520
return Cyc_Tctyp_i_check_datatype_field(env,expected_kind,info,targsp,allow_abs_aggr);}}_LL3:;}}struct _tuple16{enum Cyc_Absyn_Format_Type f1;void*f2;};struct _tuple17{struct Cyc_Absyn_Tqual f1;void*f2;};struct _tuple18{struct Cyc_Absyn_Tvar*f1;void*f2;};
# 525
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_i_check_type(struct Cyc_Tctyp_CVTEnv env,struct Cyc_Absyn_Kind*expected_kind,void*t,int put_in_effect,int allow_abs_aggr){
# 527
struct Cyc_Tctyp_CVTEnv _tmp105=env;struct Cyc_Tcenv_Tenv*_tmp107;unsigned _tmp106;_LL1: _tmp106=_tmp105.loc;_tmp107=_tmp105.te;_LL2: {unsigned loc=_tmp106;struct Cyc_Tcenv_Tenv*te=_tmp107;
{void*_tmp108=Cyc_Absyn_compress(t);void*_stmttmpE=_tmp108;void*_tmp109=_stmttmpE;void**_tmp10D;struct Cyc_Absyn_Typedefdecl**_tmp10C;struct Cyc_List_List**_tmp10B;struct _tuple1*_tmp10A;struct Cyc_List_List*_tmp10F;enum Cyc_Absyn_AggrKind _tmp10E;struct Cyc_List_List*_tmp110;struct Cyc_List_List**_tmp11D;struct Cyc_Absyn_Exp*_tmp11C;struct Cyc_List_List**_tmp11B;struct Cyc_Absyn_Exp*_tmp11A;struct Cyc_List_List*_tmp119;struct Cyc_List_List*_tmp118;struct Cyc_Absyn_VarargInfo*_tmp117;int _tmp116;struct Cyc_List_List*_tmp115;void*_tmp114;struct Cyc_Absyn_Tqual*_tmp113;void**_tmp112;struct Cyc_List_List**_tmp111;unsigned _tmp122;void*_tmp121;struct Cyc_Absyn_Exp**_tmp120;struct Cyc_Absyn_Tqual*_tmp11F;void*_tmp11E;struct Cyc_Absyn_Exp*_tmp123;struct Cyc_Absyn_Exp*_tmp124;void*_tmp12A;void*_tmp129;void*_tmp128;void*_tmp127;struct Cyc_Absyn_Tqual*_tmp126;void*_tmp125;void***_tmp12C;void*_tmp12B;struct Cyc_Absyn_Tvar*_tmp12D;void**_tmp12F;struct Cyc_Core_Opt**_tmp12E;struct Cyc_List_List**_tmp131;void*_tmp130;switch(*((int*)_tmp109)){case 0U: _LL4: _tmp130=(void*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp109)->f1;_tmp131=(struct Cyc_List_List**)&((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp109)->f2;_LL5: {void*c=_tmp130;struct Cyc_List_List**targsp=_tmp131;
# 530
env=Cyc_Tctyp_i_check_type_app(env,expected_kind,c,targsp,put_in_effect,allow_abs_aggr);
# 532
goto _LL3;}case 1U: _LL6: _tmp12E=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp109)->f1;_tmp12F=(void**)&((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp109)->f2;_LL7: {struct Cyc_Core_Opt**k=_tmp12E;void**ref=_tmp12F;
# 535
if(*k == 0 ||
 Cyc_Kinds_kind_leq(expected_kind,(struct Cyc_Absyn_Kind*)((struct Cyc_Core_Opt*)_check_null(*k))->v)&& !
Cyc_Kinds_kind_leq((struct Cyc_Absyn_Kind*)((struct Cyc_Core_Opt*)_check_null(*k))->v,expected_kind))
({struct Cyc_Core_Opt*_tmp359=Cyc_Kinds_kind_to_opt(expected_kind);*k=_tmp359;});
if(((env.fn_result && env.generalize_evars)&&(int)expected_kind->kind == (int)Cyc_Absyn_RgnKind)&& !te->tempest_generalize){
# 541
if((int)expected_kind->aliasqual == (int)Cyc_Absyn_Unique)
*ref=Cyc_Absyn_unique_rgn_type;else{
# 544
*ref=Cyc_Absyn_heap_rgn_type;}}else{
if((env.generalize_evars &&(int)expected_kind->kind != (int)Cyc_Absyn_BoolKind)&&(int)expected_kind->kind != (int)Cyc_Absyn_PtrBndKind){
# 548
struct Cyc_Absyn_Tvar*_tmp132=Cyc_Tcutil_new_tvar((void*)({struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*_tmp133=_cycalloc(sizeof(*_tmp133));_tmp133->tag=2U,_tmp133->f1=0,_tmp133->f2=expected_kind;_tmp133;}));struct Cyc_Absyn_Tvar*v=_tmp132;
({void*_tmp35A=Cyc_Absyn_var_type(v);*ref=_tmp35A;});
_tmp12D=v;goto _LL9;}else{
# 552
({struct Cyc_List_List*_tmp35B=Cyc_Tctyp_add_free_evar(env.free_evars,t,put_in_effect);env.free_evars=_tmp35B;});}}
# 554
goto _LL3;}case 2U: _LL8: _tmp12D=((struct Cyc_Absyn_VarType_Absyn_Type_struct*)_tmp109)->f1;_LL9: {struct Cyc_Absyn_Tvar*v=_tmp12D;
# 556
{void*_tmp134=Cyc_Kinds_compress_kb(v->kind);void*_stmttmpF=_tmp134;void*_tmp135=_stmttmpF;struct Cyc_Core_Opt**_tmp136;if(((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp135)->tag == 1U){_LL1D: _tmp136=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp135)->f1;_LL1E: {struct Cyc_Core_Opt**f=_tmp136;
({struct Cyc_Core_Opt*_tmp35D=({struct Cyc_Core_Opt*_tmp138=_cycalloc(sizeof(*_tmp138));({void*_tmp35C=(void*)({struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*_tmp137=_cycalloc(sizeof(*_tmp137));_tmp137->tag=2U,_tmp137->f1=0,_tmp137->f2=expected_kind;_tmp137;});_tmp138->v=_tmp35C;});_tmp138;});*f=_tmp35D;});goto _LL1C;}}else{_LL1F: _LL20:
 goto _LL1C;}_LL1C:;}
# 562
({struct Cyc_List_List*_tmp35E=Cyc_Tctyp_add_free_tvar(loc,env.kind_env,v);env.kind_env=_tmp35E;});
# 565
({struct Cyc_List_List*_tmp35F=Cyc_Tctyp_fast_add_free_tvar_bool(env.free_vars,v,put_in_effect);env.free_vars=_tmp35F;});
# 567
{void*_tmp139=Cyc_Kinds_compress_kb(v->kind);void*_stmttmp10=_tmp139;void*_tmp13A=_stmttmp10;struct Cyc_Absyn_Kind*_tmp13C;struct Cyc_Core_Opt**_tmp13B;if(((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp13A)->tag == 2U){_LL22: _tmp13B=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp13A)->f1;_tmp13C=((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp13A)->f2;_LL23: {struct Cyc_Core_Opt**f=_tmp13B;struct Cyc_Absyn_Kind*k=_tmp13C;
# 569
if(Cyc_Kinds_kind_leq(expected_kind,k))
({struct Cyc_Core_Opt*_tmp361=({struct Cyc_Core_Opt*_tmp13E=_cycalloc(sizeof(*_tmp13E));({void*_tmp360=(void*)({struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*_tmp13D=_cycalloc(sizeof(*_tmp13D));_tmp13D->tag=2U,_tmp13D->f1=0,_tmp13D->f2=expected_kind;_tmp13D;});_tmp13E->v=_tmp360;});_tmp13E;});*f=_tmp361;});
goto _LL21;}}else{_LL24: _LL25:
 goto _LL21;}_LL21:;}
# 574
goto _LL3;}case 10U: _LLA: _tmp12B=(((struct Cyc_Absyn_TypeDeclType_Absyn_Type_struct*)_tmp109)->f1)->r;_tmp12C=(void***)&((struct Cyc_Absyn_TypeDeclType_Absyn_Type_struct*)_tmp109)->f2;_LLB: {void*td=_tmp12B;void***topt=_tmp12C;
# 579
void*new_typ=Cyc_Tcutil_copy_type(Cyc_Absyn_compress(t));
{void*_tmp13F=td;struct Cyc_Absyn_Datatypedecl*_tmp140;struct Cyc_Absyn_Enumdecl*_tmp141;struct Cyc_Absyn_Aggrdecl*_tmp142;switch(*((int*)_tmp13F)){case 0U: _LL27: _tmp142=((struct Cyc_Absyn_Aggr_td_Absyn_Raw_typedecl_struct*)_tmp13F)->f1;_LL28: {struct Cyc_Absyn_Aggrdecl*ad=_tmp142;
# 582
if((env.te)->in_extern_c_include)
ad->sc=Cyc_Absyn_ExternC;
Cyc_Tc_tcAggrdecl(te,loc,ad);goto _LL26;}case 1U: _LL29: _tmp141=((struct Cyc_Absyn_Enum_td_Absyn_Raw_typedecl_struct*)_tmp13F)->f1;_LL2A: {struct Cyc_Absyn_Enumdecl*ed=_tmp141;
# 586
if((env.te)->in_extern_c_include)
ed->sc=Cyc_Absyn_ExternC;
Cyc_Tc_tcEnumdecl(te,loc,ed);goto _LL26;}default: _LL2B: _tmp140=((struct Cyc_Absyn_Datatype_td_Absyn_Raw_typedecl_struct*)_tmp13F)->f1;_LL2C: {struct Cyc_Absyn_Datatypedecl*dd=_tmp140;
# 590
Cyc_Tc_tcDatatypedecl(te,loc,dd);goto _LL26;}}_LL26:;}
# 592
({void**_tmp362=({void**_tmp143=_cycalloc(sizeof(*_tmp143));*_tmp143=new_typ;_tmp143;});*topt=_tmp362;});
return Cyc_Tctyp_i_check_type(env,expected_kind,new_typ,put_in_effect,allow_abs_aggr);}case 3U: _LLC: _tmp125=(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).elt_type;_tmp126=(struct Cyc_Absyn_Tqual*)&(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).elt_tq;_tmp127=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).ptr_atts).rgn;_tmp128=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).ptr_atts).nullable;_tmp129=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).ptr_atts).bounds;_tmp12A=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp109)->f1).ptr_atts).zero_term;_LLD: {void*t1=_tmp125;struct Cyc_Absyn_Tqual*tqp=_tmp126;void*rgn_type=_tmp127;void*nullable=_tmp128;void*b=_tmp129;void*zt=_tmp12A;
# 597
struct Cyc_Absyn_Kind*rgnk;
{enum Cyc_Absyn_AliasQual _tmp144=expected_kind->aliasqual;enum Cyc_Absyn_AliasQual _stmttmp11=_tmp144;enum Cyc_Absyn_AliasQual _tmp145=_stmttmp11;switch(_tmp145){case Cyc_Absyn_Aliasable: _LL2E: _LL2F:
 rgnk=& Cyc_Kinds_rk;goto _LL2D;case Cyc_Absyn_Unique: _LL30: _LL31:
 rgnk=& Cyc_Kinds_urk;goto _LL2D;case Cyc_Absyn_Top: _LL32: _LL33:
 goto _LL35;default: _LL34: _LL35: rgnk=& Cyc_Kinds_trk;goto _LL2D;}_LL2D:;}
# 603
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tak,t1,1,1);
env=Cyc_Tctyp_i_check_type(env,rgnk,rgn_type,1,1);
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_boolk,zt,0,1);
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_boolk,nullable,0,1);
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_ptrbk,b,0,allow_abs_aggr);
# 609
({int _tmp363=Cyc_Tcutil_extract_const_from_typedef(env.loc,tqp->print_const,t1);tqp->real_const=_tmp363;});
# 613
({void*_tmp364=zt;Cyc_Unify_unify(_tmp364,Cyc_Tcutil_is_char_type(t1)?Cyc_Absyn_true_type: Cyc_Absyn_false_type);});{
int is_zero_terminated=Cyc_Absyn_type2bool(0,zt);
if(is_zero_terminated && !Cyc_Tcutil_admits_zero(t1))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp147=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C1;_tmp2C1.tag=0U,({struct _fat_ptr _tmp365=({const char*_tmp14B="cannot have a pointer to zero-terminated ";_tag_fat(_tmp14B,sizeof(char),42U);});_tmp2C1.f1=_tmp365;});_tmp2C1;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp148=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2C0;_tmp2C0.tag=2U,_tmp2C0.f1=(void*)t1;_tmp2C0;});struct Cyc_Warn_String_Warn_Warg_struct _tmp149=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2BF;_tmp2BF.tag=0U,({struct _fat_ptr _tmp366=({const char*_tmp14A=" elements";_tag_fat(_tmp14A,sizeof(char),10U);});_tmp2BF.f1=_tmp366;});_tmp2BF;});void*_tmp146[3U];_tmp146[0]=& _tmp147,_tmp146[1]=& _tmp148,_tmp146[2]=& _tmp149;({unsigned _tmp367=loc;Cyc_Warn_err2(_tmp367,_tag_fat(_tmp146,sizeof(void*),3U));});});{
# 619
struct Cyc_Absyn_Exp*_tmp14C=({void*_tmp368=Cyc_Absyn_bounds_one();Cyc_Tcutil_get_bounds_exp(_tmp368,b);});struct Cyc_Absyn_Exp*e=_tmp14C;
if(e != 0){
struct _tuple12 _tmp14D=Cyc_Evexp_eval_const_uint_exp(e);struct _tuple12 _stmttmp12=_tmp14D;struct _tuple12 _tmp14E=_stmttmp12;int _tmp150;unsigned _tmp14F;_LL37: _tmp14F=_tmp14E.f1;_tmp150=_tmp14E.f2;_LL38: {unsigned sz=_tmp14F;int known=_tmp150;
if(is_zero_terminated &&(!known || sz < (unsigned)1))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp152=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C2;_tmp2C2.tag=0U,({struct _fat_ptr _tmp369=({const char*_tmp153="zero-terminated pointer cannot point to empty sequence";_tag_fat(_tmp153,sizeof(char),55U);});_tmp2C2.f1=_tmp369;});_tmp2C2;});void*_tmp151[1U];_tmp151[0]=& _tmp152;({unsigned _tmp36A=loc;Cyc_Warn_err2(_tmp36A,_tag_fat(_tmp151,sizeof(void*),1U));});});}}
# 625
goto _LL3;}}}case 9U: _LLE: _tmp124=((struct Cyc_Absyn_ValueofType_Absyn_Type_struct*)_tmp109)->f1;_LLF: {struct Cyc_Absyn_Exp*e=_tmp124;
# 630
({struct Cyc_Tcenv_Tenv*_tmp36B=Cyc_Tcenv_allow_valueof(te);Cyc_Tcexp_tcExp(_tmp36B,0,e);});
if(!Cyc_Tcutil_coerce_uint_type(e))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp155=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C3;_tmp2C3.tag=0U,({struct _fat_ptr _tmp36C=({const char*_tmp156="valueof_t requires an int expression";_tag_fat(_tmp156,sizeof(char),37U);});_tmp2C3.f1=_tmp36C;});_tmp2C3;});void*_tmp154[1U];_tmp154[0]=& _tmp155;({unsigned _tmp36D=loc;Cyc_Warn_err2(_tmp36D,_tag_fat(_tmp154,sizeof(void*),1U));});});
env=Cyc_Tctyp_i_check_type_level_exp(e,env);
goto _LL3;}case 11U: _LL10: _tmp123=((struct Cyc_Absyn_TypeofType_Absyn_Type_struct*)_tmp109)->f1;_LL11: {struct Cyc_Absyn_Exp*e=_tmp123;
# 639
({struct Cyc_Tcenv_Tenv*_tmp36E=Cyc_Tcenv_allow_valueof(te);Cyc_Tcexp_tcExp(_tmp36E,0,e);});
goto _LL3;}case 4U: _LL12: _tmp11E=(((struct Cyc_Absyn_ArrayType_Absyn_Type_struct*)_tmp109)->f1).elt_type;_tmp11F=(struct Cyc_Absyn_Tqual*)&(((struct Cyc_Absyn_ArrayType_Absyn_Type_struct*)_tmp109)->f1).tq;_tmp120=(struct Cyc_Absyn_Exp**)&(((struct Cyc_Absyn_ArrayType_Absyn_Type_struct*)_tmp109)->f1).num_elts;_tmp121=(((struct Cyc_Absyn_ArrayType_Absyn_Type_struct*)_tmp109)->f1).zero_term;_tmp122=(((struct Cyc_Absyn_ArrayType_Absyn_Type_struct*)_tmp109)->f1).zt_loc;_LL13: {void*t1=_tmp11E;struct Cyc_Absyn_Tqual*tqp=_tmp11F;struct Cyc_Absyn_Exp**eptr=_tmp120;void*zt=_tmp121;unsigned ztl=_tmp122;
# 644
struct Cyc_Absyn_Exp*_tmp157=*eptr;struct Cyc_Absyn_Exp*e=_tmp157;
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tmk,t1,1,allow_abs_aggr);
({int _tmp36F=Cyc_Tcutil_extract_const_from_typedef(loc,tqp->print_const,t1);tqp->real_const=_tmp36F;});{
# 648
int is_zero_terminated=Cyc_Tcutil_force_type2bool(0,zt);
if(is_zero_terminated){
# 651
if(!Cyc_Tcutil_admits_zero(t1))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp159=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C6;_tmp2C6.tag=0U,({struct _fat_ptr _tmp370=({const char*_tmp15D="cannot have a zero-terminated array of ";_tag_fat(_tmp15D,sizeof(char),40U);});_tmp2C6.f1=_tmp370;});_tmp2C6;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp15A=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2C5;_tmp2C5.tag=2U,_tmp2C5.f1=(void*)t1;_tmp2C5;});struct Cyc_Warn_String_Warn_Warg_struct _tmp15B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C4;_tmp2C4.tag=0U,({struct _fat_ptr _tmp371=({const char*_tmp15C=" elements";_tag_fat(_tmp15C,sizeof(char),10U);});_tmp2C4.f1=_tmp371;});_tmp2C4;});void*_tmp158[3U];_tmp158[0]=& _tmp159,_tmp158[1]=& _tmp15A,_tmp158[2]=& _tmp15B;({unsigned _tmp372=loc;Cyc_Warn_err2(_tmp372,_tag_fat(_tmp158,sizeof(void*),3U));});});}
# 656
if(e == 0){
# 659
if(!is_zero_terminated)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp15F=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C7;_tmp2C7.tag=0U,({struct _fat_ptr _tmp373=({const char*_tmp160="array bound defaults to 1 here";_tag_fat(_tmp160,sizeof(char),31U);});_tmp2C7.f1=_tmp373;});_tmp2C7;});void*_tmp15E[1U];_tmp15E[0]=& _tmp15F;({unsigned _tmp374=loc;Cyc_Warn_warn2(_tmp374,_tag_fat(_tmp15E,sizeof(void*),1U));});});
({struct Cyc_Absyn_Exp*_tmp375=e=Cyc_Absyn_uint_exp(1U,0U);*eptr=_tmp375;});}
# 663
({struct Cyc_Tcenv_Tenv*_tmp376=Cyc_Tcenv_allow_valueof(te);Cyc_Tcexp_tcExp(_tmp376,0,e);});
if(!Cyc_Tcutil_coerce_uint_type(e))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp162=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C8;_tmp2C8.tag=0U,({struct _fat_ptr _tmp377=({const char*_tmp163="array bounds expression is not an unsigned int";_tag_fat(_tmp163,sizeof(char),47U);});_tmp2C8.f1=_tmp377;});_tmp2C8;});void*_tmp161[1U];_tmp161[0]=& _tmp162;({unsigned _tmp378=loc;Cyc_Warn_err2(_tmp378,_tag_fat(_tmp161,sizeof(void*),1U));});});
env=Cyc_Tctyp_i_check_type_level_exp(e,env);{
struct _tuple12 _tmp164=Cyc_Evexp_eval_const_uint_exp(e);struct _tuple12 _stmttmp13=_tmp164;struct _tuple12 _tmp165=_stmttmp13;int _tmp167;unsigned _tmp166;_LL3A: _tmp166=_tmp165.f1;_tmp167=_tmp165.f2;_LL3B: {unsigned sz=_tmp166;int known=_tmp167;
# 669
if((is_zero_terminated && known)&& sz < (unsigned)1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp169=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2C9;_tmp2C9.tag=0U,({struct _fat_ptr _tmp379=({const char*_tmp16A="zero terminated array cannot have zero elements";_tag_fat(_tmp16A,sizeof(char),48U);});_tmp2C9.f1=_tmp379;});_tmp2C9;});void*_tmp168[1U];_tmp168[0]=& _tmp169;({unsigned _tmp37A=loc;Cyc_Warn_warn2(_tmp37A,_tag_fat(_tmp168,sizeof(void*),1U));});});
# 672
if((known && sz < (unsigned)1)&& Cyc_Flags_tovc_r){
({struct Cyc_Warn_String_Warn_Warg_struct _tmp16C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2CA;_tmp2CA.tag=0U,({struct _fat_ptr _tmp37B=({const char*_tmp16D="0-element arrays are supported only with gcc, changing to 1";_tag_fat(_tmp16D,sizeof(char),60U);});_tmp2CA.f1=_tmp37B;});_tmp2CA;});void*_tmp16B[1U];_tmp16B[0]=& _tmp16C;({unsigned _tmp37C=loc;Cyc_Warn_warn2(_tmp37C,_tag_fat(_tmp16B,sizeof(void*),1U));});});
({struct Cyc_Absyn_Exp*_tmp37D=Cyc_Absyn_uint_exp(1U,0U);*eptr=_tmp37D;});}
# 676
goto _LL3;}}}}case 5U: _LL14: _tmp111=(struct Cyc_List_List**)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).tvars;_tmp112=(void**)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).effect;_tmp113=(struct Cyc_Absyn_Tqual*)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).ret_tqual;_tmp114=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).ret_type;_tmp115=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).args;_tmp116=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).c_varargs;_tmp117=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).cyc_varargs;_tmp118=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).rgn_po;_tmp119=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).attributes;_tmp11A=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).requires_clause;_tmp11B=(struct Cyc_List_List**)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).requires_relns;_tmp11C=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).ensures_clause;_tmp11D=(struct Cyc_List_List**)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp109)->f1).ensures_relns;_LL15: {struct Cyc_List_List**btvs=_tmp111;void**eff=_tmp112;struct Cyc_Absyn_Tqual*rtq=_tmp113;void*tr=_tmp114;struct Cyc_List_List*args=_tmp115;int c_vararg=_tmp116;struct Cyc_Absyn_VarargInfo*cyc_vararg=_tmp117;struct Cyc_List_List*rgn_po=_tmp118;struct Cyc_List_List*atts=_tmp119;struct Cyc_Absyn_Exp*req=_tmp11A;struct Cyc_List_List**req_relns=_tmp11B;struct Cyc_Absyn_Exp*ens=_tmp11C;struct Cyc_List_List**ens_relns=_tmp11D;
# 684
int num_formats=0;
int is_cdecl=0;
int is_stdcall=0;
int is_fastcall=0;
enum Cyc_Absyn_Format_Type ft=0U;
int fmt_desc_arg=-1;
int fmt_arg_start=-1;
for(0;atts != 0;atts=atts->tl){
void*_tmp16E=(void*)atts->hd;void*_stmttmp14=_tmp16E;void*_tmp16F=_stmttmp14;int _tmp172;int _tmp171;enum Cyc_Absyn_Format_Type _tmp170;switch(*((int*)_tmp16F)){case 1U: _LL3D: _LL3E:
 is_stdcall=1;goto _LL3C;case 2U: _LL3F: _LL40:
 is_cdecl=1;goto _LL3C;case 3U: _LL41: _LL42:
 is_fastcall=1;goto _LL3C;case 19U: _LL43: _tmp170=((struct Cyc_Absyn_Format_att_Absyn_Attribute_struct*)_tmp16F)->f1;_tmp171=((struct Cyc_Absyn_Format_att_Absyn_Attribute_struct*)_tmp16F)->f2;_tmp172=((struct Cyc_Absyn_Format_att_Absyn_Attribute_struct*)_tmp16F)->f3;_LL44: {enum Cyc_Absyn_Format_Type fmttype=_tmp170;int i=_tmp171;int j=_tmp172;
# 697
++ num_formats;
ft=fmttype;
fmt_desc_arg=i;
fmt_arg_start=j;
goto _LL3C;}default: _LL45: _LL46:
# 703
 if(!Cyc_Atts_fntype_att((void*)atts->hd))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp174=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2CC;_tmp2CC.tag=0U,({struct _fat_ptr _tmp37E=({const char*_tmp176="bad function type attribute ";_tag_fat(_tmp176,sizeof(char),29U);});_tmp2CC.f1=_tmp37E;});_tmp2CC;});struct Cyc_Warn_Attribute_Warn_Warg_struct _tmp175=({struct Cyc_Warn_Attribute_Warn_Warg_struct _tmp2CB;_tmp2CB.tag=10U,_tmp2CB.f1=(void*)atts->hd;_tmp2CB;});void*_tmp173[2U];_tmp173[0]=& _tmp174,_tmp173[1]=& _tmp175;({unsigned _tmp37F=loc;Cyc_Warn_err2(_tmp37F,_tag_fat(_tmp173,sizeof(void*),2U));});});}_LL3C:;}
# 706
if((is_stdcall + is_cdecl)+ is_fastcall > 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp178=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2CD;_tmp2CD.tag=0U,({struct _fat_ptr _tmp380=({const char*_tmp179="function can't have multiple calling conventions";_tag_fat(_tmp179,sizeof(char),49U);});_tmp2CD.f1=_tmp380;});_tmp2CD;});void*_tmp177[1U];_tmp177[0]=& _tmp178;({unsigned _tmp381=loc;Cyc_Warn_err2(_tmp381,_tag_fat(_tmp177,sizeof(void*),1U));});});
if(num_formats > 1)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp17B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2CE;_tmp2CE.tag=0U,({struct _fat_ptr _tmp382=({const char*_tmp17C="function can't have multiple format attributes";_tag_fat(_tmp17C,sizeof(char),47U);});_tmp2CE.f1=_tmp382;});_tmp2CE;});void*_tmp17A[1U];_tmp17A[0]=& _tmp17B;({unsigned _tmp383=loc;Cyc_Warn_err2(_tmp383,_tag_fat(_tmp17A,sizeof(void*),1U));});});
# 713
Cyc_Tcutil_check_unique_tvars(loc,*btvs);
{struct Cyc_List_List*b=*btvs;for(0;b != 0;b=b->tl){
({int _tmp384=Cyc_Tcutil_new_tvar_id();((struct Cyc_Absyn_Tvar*)b->hd)->identity=_tmp384;});
({struct Cyc_List_List*_tmp385=Cyc_Tctyp_add_bound_tvar(env.kind_env,(struct Cyc_Absyn_Tvar*)b->hd);env.kind_env=_tmp385;});{
void*_tmp17D=Cyc_Kinds_compress_kb(((struct Cyc_Absyn_Tvar*)b->hd)->kind);void*_stmttmp15=_tmp17D;void*_tmp17E=_stmttmp15;if(((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp17E)->tag == 0U){if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp17E)->f1)->kind == Cyc_Absyn_MemKind){_LL48: _LL49:
# 719
({struct Cyc_Warn_String_Warn_Warg_struct _tmp180=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D0;_tmp2D0.tag=0U,({struct _fat_ptr _tmp386=({const char*_tmp182="function abstracts Mem type variable ";_tag_fat(_tmp182,sizeof(char),38U);});_tmp2D0.f1=_tmp386;});_tmp2D0;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp181=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp2CF;_tmp2CF.tag=7U,_tmp2CF.f1=(struct Cyc_Absyn_Tvar*)b->hd;_tmp2CF;});void*_tmp17F[2U];_tmp17F[0]=& _tmp180,_tmp17F[1]=& _tmp181;({unsigned _tmp387=loc;Cyc_Warn_err2(_tmp387,_tag_fat(_tmp17F,sizeof(void*),2U));});});goto _LL47;}else{goto _LL4A;}}else{_LL4A: _LL4B:
 goto _LL47;}_LL47:;}}}{
# 725
struct Cyc_Tctyp_CVTEnv new_env=({struct Cyc_Tctyp_CVTEnv _tmp2E1;_tmp2E1.loc=loc,_tmp2E1.te=te,_tmp2E1.kind_env=env.kind_env,_tmp2E1.fn_result=1,_tmp2E1.generalize_evars=env.generalize_evars,_tmp2E1.free_vars=0,_tmp2E1.free_evars=0;_tmp2E1;});
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_tmk,tr,1,1);
({int _tmp388=Cyc_Tcutil_extract_const_from_typedef(loc,rtq->print_const,tr);rtq->real_const=_tmp388;});
new_env.fn_result=0;
# 731
{struct Cyc_List_List*a=args;for(0;a != 0;a=a->tl){
struct _tuple9*_tmp183=(struct _tuple9*)a->hd;struct _tuple9*trip=_tmp183;
void*_tmp184=(*trip).f3;void*t=_tmp184;
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_tmk,t,1,1);{
int _tmp185=Cyc_Tcutil_extract_const_from_typedef(loc,((*trip).f2).print_const,t);int ec=_tmp185;
((*trip).f2).real_const=ec;
# 739
if(Cyc_Tcutil_is_array_type(t)){
# 741
void*_tmp186=Cyc_Absyn_new_evar(0,0);void*ptr_rgn=_tmp186;
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_rk,ptr_rgn,1,1);
({void*_tmp389=Cyc_Tcutil_promote_array(t,ptr_rgn,0);(*trip).f3=_tmp389;});}}}}
# 747
if(cyc_vararg != 0){
if(c_vararg)({struct Cyc_Warn_String_Warn_Warg_struct _tmp188=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D1;_tmp2D1.tag=0U,({struct _fat_ptr _tmp38A=({const char*_tmp189="both c_vararg and cyc_vararg";_tag_fat(_tmp189,sizeof(char),29U);});_tmp2D1.f1=_tmp38A;});_tmp2D1;});void*_tmp187[1U];_tmp187[0]=& _tmp188;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp187,sizeof(void*),1U));});{
struct Cyc_Absyn_VarargInfo _tmp18A=*cyc_vararg;struct Cyc_Absyn_VarargInfo _stmttmp16=_tmp18A;struct Cyc_Absyn_VarargInfo _tmp18B=_stmttmp16;int _tmp18D;void*_tmp18C;_LL4D: _tmp18C=_tmp18B.type;_tmp18D=_tmp18B.inject;_LL4E: {void*vt=_tmp18C;int vi=_tmp18D;
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_tmk,vt,1,1);
({int _tmp38B=Cyc_Tcutil_extract_const_from_typedef(loc,(cyc_vararg->tq).print_const,vt);(cyc_vararg->tq).real_const=_tmp38B;});
# 753
if(vi){
void*_tmp18E=Cyc_Absyn_compress(vt);void*_stmttmp17=_tmp18E;void*_tmp18F=_stmttmp17;void*_tmp192;void*_tmp191;void*_tmp190;if(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp18F)->tag == 3U){_LL50: _tmp190=(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp18F)->f1).elt_type;_tmp191=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp18F)->f1).ptr_atts).bounds;_tmp192=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp18F)->f1).ptr_atts).zero_term;_LL51: {void*et=_tmp190;void*bs=_tmp191;void*zt=_tmp192;
# 756
{void*_tmp193=Cyc_Absyn_compress(et);void*_stmttmp18=_tmp193;void*_tmp194=_stmttmp18;if(((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp194)->tag == 0U){if(((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp194)->f1)->tag == 18U){_LL55: _LL56:
# 758
 if(Cyc_Tcutil_force_type2bool(0,zt))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp196=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D2;_tmp2D2.tag=0U,({struct _fat_ptr _tmp38C=({const char*_tmp197="can't inject into a zeroterm pointer";_tag_fat(_tmp197,sizeof(char),37U);});_tmp2D2.f1=_tmp38C;});_tmp2D2;});void*_tmp195[1U];_tmp195[0]=& _tmp196;({unsigned _tmp38D=loc;Cyc_Warn_err2(_tmp38D,_tag_fat(_tmp195,sizeof(void*),1U));});});
if(!({void*_tmp38E=Cyc_Absyn_bounds_one();Cyc_Unify_unify(_tmp38E,bs);}))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp199=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D3;_tmp2D3.tag=0U,({struct _fat_ptr _tmp38F=({const char*_tmp19A="can't inject into a fat pointer to datatype";_tag_fat(_tmp19A,sizeof(char),44U);});_tmp2D3.f1=_tmp38F;});_tmp2D3;});void*_tmp198[1U];_tmp198[0]=& _tmp199;({unsigned _tmp390=loc;Cyc_Warn_err2(_tmp390,_tag_fat(_tmp198,sizeof(void*),1U));});});
goto _LL54;}else{goto _LL57;}}else{_LL57: _LL58:
({struct Cyc_Warn_String_Warn_Warg_struct _tmp19C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D4;_tmp2D4.tag=0U,({struct _fat_ptr _tmp391=({const char*_tmp19D="can't inject a non-datatype type";_tag_fat(_tmp19D,sizeof(char),33U);});_tmp2D4.f1=_tmp391;});_tmp2D4;});void*_tmp19B[1U];_tmp19B[0]=& _tmp19C;({unsigned _tmp392=loc;Cyc_Warn_err2(_tmp392,_tag_fat(_tmp19B,sizeof(void*),1U));});});goto _LL54;}_LL54:;}
# 765
goto _LL4F;}}else{_LL52: _LL53:
({struct Cyc_Warn_String_Warn_Warg_struct _tmp19F=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D5;_tmp2D5.tag=0U,({struct _fat_ptr _tmp393=({const char*_tmp1A0="expecting a datatype pointer type";_tag_fat(_tmp1A0,sizeof(char),34U);});_tmp2D5.f1=_tmp393;});_tmp2D5;});void*_tmp19E[1U];_tmp19E[0]=& _tmp19F;({unsigned _tmp394=loc;Cyc_Warn_err2(_tmp394,_tag_fat(_tmp19E,sizeof(void*),1U));});});goto _LL4F;}_LL4F:;}}}}
# 770
if(num_formats > 0){
int _tmp1A1=((int(*)(struct Cyc_List_List*x))Cyc_List_length)(args);int num_args=_tmp1A1;
if((((fmt_desc_arg < 0 || fmt_desc_arg > num_args)|| fmt_arg_start < 0)||
# 774
(cyc_vararg == 0 && !c_vararg)&& fmt_arg_start != 0)||
(cyc_vararg != 0 || c_vararg)&& fmt_arg_start != num_args + 1)
# 777
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1A3=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D6;_tmp2D6.tag=0U,({struct _fat_ptr _tmp395=({const char*_tmp1A4="bad format descriptor";_tag_fat(_tmp1A4,sizeof(char),22U);});_tmp2D6.f1=_tmp395;});_tmp2D6;});void*_tmp1A2[1U];_tmp1A2[0]=& _tmp1A3;({unsigned _tmp396=loc;Cyc_Warn_err2(_tmp396,_tag_fat(_tmp1A2,sizeof(void*),1U));});});else{
# 780
struct _tuple9 _tmp1A5=*((struct _tuple9*(*)(struct Cyc_List_List*x,int n))Cyc_List_nth)(args,fmt_desc_arg - 1);struct _tuple9 _stmttmp19=_tmp1A5;struct _tuple9 _tmp1A6=_stmttmp19;void*_tmp1A7;_LL5A: _tmp1A7=_tmp1A6.f3;_LL5B: {void*t=_tmp1A7;
# 782
{void*_tmp1A8=Cyc_Absyn_compress(t);void*_stmttmp1A=_tmp1A8;void*_tmp1A9=_stmttmp1A;void*_tmp1AB;void*_tmp1AA;if(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp1A9)->tag == 3U){_LL5D: _tmp1AA=(((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp1A9)->f1).elt_type;_tmp1AB=((((struct Cyc_Absyn_PointerType_Absyn_Type_struct*)_tmp1A9)->f1).ptr_atts).bounds;_LL5E: {void*et=_tmp1AA;void*b=_tmp1AB;
# 785
if(!Cyc_Tcutil_is_char_type(et))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1AD=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D7;_tmp2D7.tag=0U,({struct _fat_ptr _tmp397=({const char*_tmp1AE="format descriptor is not a string";_tag_fat(_tmp1AE,sizeof(char),34U);});_tmp2D7.f1=_tmp397;});_tmp2D7;});void*_tmp1AC[1U];_tmp1AC[0]=& _tmp1AD;({unsigned _tmp398=loc;Cyc_Warn_err2(_tmp398,_tag_fat(_tmp1AC,sizeof(void*),1U));});});else{
# 788
struct Cyc_Absyn_Exp*_tmp1AF=({void*_tmp399=Cyc_Absyn_bounds_one();Cyc_Tcutil_get_bounds_exp(_tmp399,b);});struct Cyc_Absyn_Exp*e=_tmp1AF;
if(e == 0 && c_vararg)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1B1=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D8;_tmp2D8.tag=0U,({struct _fat_ptr _tmp39A=({const char*_tmp1B2="format descriptor is not a char * type";_tag_fat(_tmp1B2,sizeof(char),39U);});_tmp2D8.f1=_tmp39A;});_tmp2D8;});void*_tmp1B0[1U];_tmp1B0[0]=& _tmp1B1;({unsigned _tmp39B=loc;Cyc_Warn_err2(_tmp39B,_tag_fat(_tmp1B0,sizeof(void*),1U));});});else{
if(e != 0 && !c_vararg)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1B4=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2D9;_tmp2D9.tag=0U,({struct _fat_ptr _tmp39C=({const char*_tmp1B5="format descriptor is not a char ? type";_tag_fat(_tmp1B5,sizeof(char),39U);});_tmp2D9.f1=_tmp39C;});_tmp2D9;});void*_tmp1B3[1U];_tmp1B3[0]=& _tmp1B4;({unsigned _tmp39D=loc;Cyc_Warn_err2(_tmp39D,_tag_fat(_tmp1B3,sizeof(void*),1U));});});}}
# 794
goto _LL5C;}}else{_LL5F: _LL60:
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1B7=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2DA;_tmp2DA.tag=0U,({struct _fat_ptr _tmp39E=({const char*_tmp1B8="format descriptor is not a string type";_tag_fat(_tmp1B8,sizeof(char),39U);});_tmp2DA.f1=_tmp39E;});_tmp2DA;});void*_tmp1B6[1U];_tmp1B6[0]=& _tmp1B7;({unsigned _tmp39F=loc;Cyc_Warn_err2(_tmp39F,_tag_fat(_tmp1B6,sizeof(void*),1U));});});goto _LL5C;}_LL5C:;}
# 797
if(fmt_arg_start != 0 && !c_vararg){
# 801
int problem;
{struct _tuple16 _tmp1B9=({struct _tuple16 _tmp2DB;_tmp2DB.f1=ft,({void*_tmp3A0=Cyc_Absyn_compress(Cyc_Tcutil_pointer_elt_type(((struct Cyc_Absyn_VarargInfo*)_check_null(cyc_vararg))->type));_tmp2DB.f2=_tmp3A0;});_tmp2DB;});struct _tuple16 _stmttmp1B=_tmp1B9;struct _tuple16 _tmp1BA=_stmttmp1B;struct Cyc_Absyn_Datatypedecl*_tmp1BB;struct Cyc_Absyn_Datatypedecl*_tmp1BC;switch(_tmp1BA.f1){case Cyc_Absyn_Printf_ft: if(((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->tag == 0U){if(((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->tag == 18U){if(((((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->f1).KnownDatatype).tag == 2){_LL62: _tmp1BC=*((((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->f1).KnownDatatype).val;_LL63: {struct Cyc_Absyn_Datatypedecl*tud=_tmp1BC;
# 804
problem=({struct _tuple1*_tmp3A1=tud->name;Cyc_Absyn_qvar_cmp(_tmp3A1,Cyc_Absyn_datatype_print_arg_qvar());})!= 0;
goto _LL61;}}else{goto _LL66;}}else{goto _LL66;}}else{goto _LL66;}case Cyc_Absyn_Scanf_ft: if(((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->tag == 0U){if(((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->tag == 18U){if(((((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->f1).KnownDatatype).tag == 2){_LL64: _tmp1BB=*((((struct Cyc_Absyn_DatatypeCon_Absyn_TyCon_struct*)((struct Cyc_Absyn_AppType_Absyn_Type_struct*)_tmp1BA.f2)->f1)->f1).KnownDatatype).val;_LL65: {struct Cyc_Absyn_Datatypedecl*tud=_tmp1BB;
# 807
problem=({struct _tuple1*_tmp3A2=tud->name;Cyc_Absyn_qvar_cmp(_tmp3A2,Cyc_Absyn_datatype_scanf_arg_qvar());})!= 0;
goto _LL61;}}else{goto _LL66;}}else{goto _LL66;}}else{goto _LL66;}default: _LL66: _LL67:
# 810
 problem=1;
goto _LL61;}_LL61:;}
# 813
if(problem)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1BE=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2DC;_tmp2DC.tag=0U,({struct _fat_ptr _tmp3A3=({const char*_tmp1BF="format attribute and vararg types don't match";_tag_fat(_tmp1BF,sizeof(char),46U);});_tmp2DC.f1=_tmp3A3;});_tmp2DC;});void*_tmp1BD[1U];_tmp1BD[0]=& _tmp1BE;({unsigned _tmp3A4=loc;Cyc_Warn_err2(_tmp3A4,_tag_fat(_tmp1BD,sizeof(void*),1U));});});}}}}
# 821
{struct Cyc_List_List*rpo=rgn_po;for(0;rpo != 0;rpo=rpo->tl){
struct _tuple0*_tmp1C0=(struct _tuple0*)rpo->hd;struct _tuple0*_stmttmp1C=_tmp1C0;struct _tuple0*_tmp1C1=_stmttmp1C;void*_tmp1C3;void*_tmp1C2;_LL69: _tmp1C2=_tmp1C1->f1;_tmp1C3=_tmp1C1->f2;_LL6A: {void*r1=_tmp1C2;void*r2=_tmp1C3;
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_ek,r1,1,1);
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_trk,r2,1,1);}}}{
# 829
struct Cyc_Tcenv_Fenv*_tmp1C4=Cyc_Tcenv_bogus_fenv(tr,args);struct Cyc_Tcenv_Fenv*fenv=_tmp1C4;
struct Cyc_Tcenv_Tenv*_tmp1C5=new_env.te;struct Cyc_Tcenv_Tenv*old_te=_tmp1C5;
({struct Cyc_Tcenv_Tenv*_tmp3A5=({struct Cyc_Tcenv_Tenv*_tmp1C6=_cycalloc(sizeof(*_tmp1C6));_tmp1C6->ns=te->ns,_tmp1C6->ae=te->ae,_tmp1C6->le=fenv,_tmp1C6->allow_valueof=1,_tmp1C6->in_extern_c_include=te->in_extern_c_include,_tmp1C6->in_tempest=te->in_tempest,_tmp1C6->tempest_generalize=te->tempest_generalize,_tmp1C6->in_extern_c_inc_repeat=te->in_extern_c_inc_repeat;_tmp1C6;});new_env.te=_tmp3A5;});{
# 835
struct _tuple15 _tmp1C7=({struct Cyc_Tctyp_CVTEnv _tmp3A7=new_env;struct _fat_ptr _tmp3A6=({const char*_tmp200="@requires";_tag_fat(_tmp200,sizeof(char),10U);});Cyc_Tctyp_check_clause(_tmp3A7,_tmp3A6,req);});struct _tuple15 _stmttmp1D=_tmp1C7;struct _tuple15 _tmp1C8=_stmttmp1D;struct Cyc_List_List*_tmp1CA;struct Cyc_Tctyp_CVTEnv _tmp1C9;_LL6C: _tmp1C9=_tmp1C8.f1;_tmp1CA=_tmp1C8.f2;_LL6D: {struct Cyc_Tctyp_CVTEnv nenv=_tmp1C9;struct Cyc_List_List*req_rs=_tmp1CA;
new_env=nenv;
*req_relns=req_rs;
((void(*)(void(*f)(struct Cyc_List_List*,struct Cyc_Relations_Reln*),struct Cyc_List_List*env,struct Cyc_List_List*x))Cyc_List_iter_c)(Cyc_Tcutil_replace_rops,args,req_rs);{
struct _tuple15 _tmp1CB=({struct Cyc_Tctyp_CVTEnv _tmp3A9=new_env;struct _fat_ptr _tmp3A8=({const char*_tmp1FF="@ensures";_tag_fat(_tmp1FF,sizeof(char),9U);});Cyc_Tctyp_check_clause(_tmp3A9,_tmp3A8,ens);});struct _tuple15 _stmttmp1E=_tmp1CB;struct _tuple15 _tmp1CC=_stmttmp1E;struct Cyc_List_List*_tmp1CE;struct Cyc_Tctyp_CVTEnv _tmp1CD;_LL6F: _tmp1CD=_tmp1CC.f1;_tmp1CE=_tmp1CC.f2;_LL70: {struct Cyc_Tctyp_CVTEnv nenv=_tmp1CD;struct Cyc_List_List*ens_rs=_tmp1CE;
new_env=nenv;
*ens_relns=ens_rs;
((void(*)(void(*f)(struct Cyc_List_List*,struct Cyc_Relations_Reln*),struct Cyc_List_List*env,struct Cyc_List_List*x))Cyc_List_iter_c)(Cyc_Tcutil_replace_rops,args,ens_rs);
new_env.te=old_te;
# 845
if(*eff != 0)
new_env=Cyc_Tctyp_i_check_type(new_env,& Cyc_Kinds_ek,(void*)_check_null(*eff),1,1);else{
# 848
struct Cyc_List_List*effect=0;
# 853
{struct Cyc_List_List*tvs=new_env.free_vars;for(0;tvs != 0;tvs=tvs->tl){
struct _tuple14 _tmp1CF=*((struct _tuple14*)tvs->hd);struct _tuple14 _stmttmp1F=_tmp1CF;struct _tuple14 _tmp1D0=_stmttmp1F;int _tmp1D2;struct Cyc_Absyn_Tvar*_tmp1D1;_LL72: _tmp1D1=_tmp1D0.f1;_tmp1D2=_tmp1D0.f2;_LL73: {struct Cyc_Absyn_Tvar*tv=_tmp1D1;int put_in_eff=_tmp1D2;
if(!put_in_eff)continue;{
void*_tmp1D3=Cyc_Kinds_compress_kb(tv->kind);void*_stmttmp20=_tmp1D3;void*_tmp1D4=_stmttmp20;struct Cyc_Core_Opt**_tmp1D5;struct Cyc_Absyn_Kind*_tmp1D6;struct Cyc_Core_Opt**_tmp1D7;struct Cyc_Absyn_Kind*_tmp1D9;struct Cyc_Core_Opt**_tmp1D8;switch(*((int*)_tmp1D4)){case 2U: _LL75: _tmp1D8=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1D4)->f1;_tmp1D9=((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1D4)->f2;if((int)_tmp1D9->kind == (int)Cyc_Absyn_RgnKind){_LL76: {struct Cyc_Core_Opt**f=_tmp1D8;struct Cyc_Absyn_Kind*r=_tmp1D9;
# 859
if((int)r->aliasqual == (int)Cyc_Absyn_Top)
({struct Cyc_Core_Opt*_tmp3AA=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_rk);*f=_tmp3AA;});else{
# 862
({struct Cyc_Core_Opt*_tmp3AB=Cyc_Kinds_kind_to_bound_opt(r);*f=_tmp3AB;});}
_tmp1D6=r;goto _LL78;}}else{switch(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1D4)->f2)->kind){case Cyc_Absyn_BoolKind: _LL79: _LL7A:
# 866
 goto _LL7C;case Cyc_Absyn_PtrBndKind: _LL7B: _LL7C:
 goto _LL7E;case Cyc_Absyn_IntKind: _LL7D: _LL7E:
 goto _LL80;case Cyc_Absyn_EffKind: _LL85: _tmp1D7=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1D4)->f1;_LL86: {struct Cyc_Core_Opt**f=_tmp1D7;
# 873
({struct Cyc_Core_Opt*_tmp3AC=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_ek);*f=_tmp3AC;});goto _LL88;}default: goto _LL8B;}}case 0U: _LL77: _tmp1D6=((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp1D4)->f1;if((int)_tmp1D6->kind == (int)Cyc_Absyn_RgnKind){_LL78: {struct Cyc_Absyn_Kind*r=_tmp1D6;
# 865
effect=({struct Cyc_List_List*_tmp1DA=_cycalloc(sizeof(*_tmp1DA));({void*_tmp3AD=Cyc_Absyn_access_eff(Cyc_Absyn_var_type(tv));_tmp1DA->hd=_tmp3AD;}),_tmp1DA->tl=effect;_tmp1DA;});goto _LL74;}}else{switch(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp1D4)->f1)->kind){case Cyc_Absyn_BoolKind: _LL7F: _LL80:
# 869
 goto _LL82;case Cyc_Absyn_PtrBndKind: _LL81: _LL82:
 goto _LL84;case Cyc_Absyn_IntKind: _LL83: _LL84:
 goto _LL74;case Cyc_Absyn_EffKind: _LL87: _LL88:
# 875
 effect=({struct Cyc_List_List*_tmp1DB=_cycalloc(sizeof(*_tmp1DB));({void*_tmp3AE=Cyc_Absyn_var_type(tv);_tmp1DB->hd=_tmp3AE;}),_tmp1DB->tl=effect;_tmp1DB;});goto _LL74;default: _LL8B: _LL8C:
# 880
 effect=({struct Cyc_List_List*_tmp1DE=_cycalloc(sizeof(*_tmp1DE));({void*_tmp3AF=Cyc_Absyn_regionsof_eff(Cyc_Absyn_var_type(tv));_tmp1DE->hd=_tmp3AF;}),_tmp1DE->tl=effect;_tmp1DE;});goto _LL74;}}default: _LL89: _tmp1D5=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp1D4)->f1;_LL8A: {struct Cyc_Core_Opt**f=_tmp1D5;
# 877
({struct Cyc_Core_Opt*_tmp3B1=({struct Cyc_Core_Opt*_tmp1DD=_cycalloc(sizeof(*_tmp1DD));({void*_tmp3B0=(void*)({struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*_tmp1DC=_cycalloc(sizeof(*_tmp1DC));_tmp1DC->tag=2U,_tmp1DC->f1=0,_tmp1DC->f2=& Cyc_Kinds_ak;_tmp1DC;});_tmp1DD->v=_tmp3B0;});_tmp1DD;});*f=_tmp3B1;});goto _LL8C;}}_LL74:;}}}}
# 884
{struct Cyc_List_List*ts=new_env.free_evars;for(0;ts != 0;ts=ts->tl){
struct _tuple13 _tmp1DF=*((struct _tuple13*)ts->hd);struct _tuple13 _stmttmp21=_tmp1DF;struct _tuple13 _tmp1E0=_stmttmp21;int _tmp1E2;void*_tmp1E1;_LL8E: _tmp1E1=_tmp1E0.f1;_tmp1E2=_tmp1E0.f2;_LL8F: {void*tv=_tmp1E1;int put_in_eff=_tmp1E2;
if(!put_in_eff)continue;{
struct Cyc_Absyn_Kind*_tmp1E3=Cyc_Tcutil_type_kind(tv);struct Cyc_Absyn_Kind*_stmttmp22=_tmp1E3;struct Cyc_Absyn_Kind*_tmp1E4=_stmttmp22;switch(((struct Cyc_Absyn_Kind*)_tmp1E4)->kind){case Cyc_Absyn_RgnKind: _LL91: _LL92:
 effect=({struct Cyc_List_List*_tmp1E5=_cycalloc(sizeof(*_tmp1E5));({void*_tmp3B2=Cyc_Absyn_access_eff(tv);_tmp1E5->hd=_tmp3B2;}),_tmp1E5->tl=effect;_tmp1E5;});goto _LL90;case Cyc_Absyn_EffKind: _LL93: _LL94:
 effect=({struct Cyc_List_List*_tmp1E6=_cycalloc(sizeof(*_tmp1E6));_tmp1E6->hd=tv,_tmp1E6->tl=effect;_tmp1E6;});goto _LL90;case Cyc_Absyn_IntKind: _LL95: _LL96:
 goto _LL90;default: _LL97: _LL98:
 effect=({struct Cyc_List_List*_tmp1E7=_cycalloc(sizeof(*_tmp1E7));({void*_tmp3B3=Cyc_Absyn_regionsof_eff(tv);_tmp1E7->hd=_tmp3B3;}),_tmp1E7->tl=effect;_tmp1E7;});goto _LL90;}_LL90:;}}}}
# 894
({void*_tmp3B4=Cyc_Absyn_join_eff(effect);*eff=_tmp3B4;});}
# 900
if(*btvs != 0){
struct Cyc_List_List*bs=*btvs;for(0;bs != 0;bs=bs->tl){
void*_tmp1E8=Cyc_Kinds_compress_kb(((struct Cyc_Absyn_Tvar*)bs->hd)->kind);void*_stmttmp23=_tmp1E8;void*_tmp1E9=_stmttmp23;struct Cyc_Absyn_Kind*_tmp1EB;struct Cyc_Core_Opt**_tmp1EA;struct Cyc_Core_Opt**_tmp1EC;switch(*((int*)_tmp1E9)){case 1U: _LL9A: _tmp1EC=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp1E9)->f1;_LL9B: {struct Cyc_Core_Opt**f=_tmp1EC;
# 904
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1EE=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2DF;_tmp2DF.tag=0U,({struct _fat_ptr _tmp3B5=({const char*_tmp1F2="type variable ";_tag_fat(_tmp1F2,sizeof(char),15U);});_tmp2DF.f1=_tmp3B5;});_tmp2DF;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp1EF=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp2DE;_tmp2DE.tag=7U,_tmp2DE.f1=(struct Cyc_Absyn_Tvar*)bs->hd;_tmp2DE;});struct Cyc_Warn_String_Warn_Warg_struct _tmp1F0=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2DD;_tmp2DD.tag=0U,({struct _fat_ptr _tmp3B6=({const char*_tmp1F1=" unconstrained, assuming boxed";_tag_fat(_tmp1F1,sizeof(char),31U);});_tmp2DD.f1=_tmp3B6;});_tmp2DD;});void*_tmp1ED[3U];_tmp1ED[0]=& _tmp1EE,_tmp1ED[1]=& _tmp1EF,_tmp1ED[2]=& _tmp1F0;({unsigned _tmp3B7=loc;Cyc_Warn_warn2(_tmp3B7,_tag_fat(_tmp1ED,sizeof(void*),3U));});});
({struct Cyc_Core_Opt*_tmp3B8=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_bk);*f=_tmp3B8;});
goto _LL99;}case 2U: _LL9C: _tmp1EA=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1E9)->f1;_tmp1EB=((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp1E9)->f2;_LL9D: {struct Cyc_Core_Opt**f=_tmp1EA;struct Cyc_Absyn_Kind*k=_tmp1EB;
# 908
{struct Cyc_Absyn_Kind*_tmp1F3=k;switch(((struct Cyc_Absyn_Kind*)_tmp1F3)->kind){case Cyc_Absyn_BoxKind: if(((struct Cyc_Absyn_Kind*)_tmp1F3)->aliasqual == Cyc_Absyn_Top){_LLA3: _LLA4:
 goto _LLA6;}else{goto _LLB3;}case Cyc_Absyn_MemKind: switch(((struct Cyc_Absyn_Kind*)_tmp1F3)->aliasqual){case Cyc_Absyn_Top: _LLA5: _LLA6:
 goto _LLA8;case Cyc_Absyn_Aliasable: _LLA7: _LLA8:
 goto _LLAA;case Cyc_Absyn_Unique: _LLAD: _LLAE:
# 915
 goto _LLB0;default: goto _LLB3;}case Cyc_Absyn_AnyKind: switch(((struct Cyc_Absyn_Kind*)_tmp1F3)->aliasqual){case Cyc_Absyn_Top: _LLA9: _LLAA:
# 912
 goto _LLAC;case Cyc_Absyn_Aliasable: _LLAB: _LLAC:
# 914
({struct Cyc_Core_Opt*_tmp3B9=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_bk);*f=_tmp3B9;});goto _LLA2;case Cyc_Absyn_Unique: _LLAF: _LLB0:
# 917
({struct Cyc_Core_Opt*_tmp3BA=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_ubk);*f=_tmp3BA;});goto _LLA2;default: goto _LLB3;}case Cyc_Absyn_RgnKind: if(((struct Cyc_Absyn_Kind*)_tmp1F3)->aliasqual == Cyc_Absyn_Top){_LLB1: _LLB2:
# 919
({struct Cyc_Core_Opt*_tmp3BB=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_rk);*f=_tmp3BB;});goto _LLA2;}else{goto _LLB3;}default: _LLB3: _LLB4:
# 921
({struct Cyc_Core_Opt*_tmp3BC=Cyc_Kinds_kind_to_bound_opt(k);*f=_tmp3BC;});goto _LLA2;}_LLA2:;}
# 923
goto _LL99;}default: if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp1E9)->f1)->kind == Cyc_Absyn_MemKind){_LL9E: _LL9F:
# 925
({struct Cyc_Warn_String_Warn_Warg_struct _tmp1F5=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E0;_tmp2E0.tag=0U,({struct _fat_ptr _tmp3BD=({const char*_tmp1F6="functions can't abstract M types";_tag_fat(_tmp1F6,sizeof(char),33U);});_tmp2E0.f1=_tmp3BD;});_tmp2E0;});void*_tmp1F4[1U];_tmp1F4[0]=& _tmp1F5;({unsigned _tmp3BE=loc;Cyc_Warn_err2(_tmp3BE,_tag_fat(_tmp1F4,sizeof(void*),1U));});});goto _LL99;}else{_LLA0: _LLA1:
 goto _LL99;}}_LL99:;}}
# 929
({struct Cyc_List_List*_tmp3BF=((struct Cyc_List_List*(*)(struct Cyc_List_List*tvs,struct Cyc_List_List*btvs,struct Cyc_Absyn_Tvar*(*get)(struct Cyc_Absyn_Tvar*)))Cyc_Tctyp_remove_bound_tvars)(new_env.kind_env,*btvs,(struct Cyc_Absyn_Tvar*(*)(struct Cyc_Absyn_Tvar*))Cyc_Core_identity);env.kind_env=_tmp3BF;});
({struct Cyc_List_List*_tmp3C0=((struct Cyc_List_List*(*)(struct Cyc_List_List*tvs,struct Cyc_List_List*btvs,struct Cyc_Absyn_Tvar*(*get)(struct _tuple14*)))Cyc_Tctyp_remove_bound_tvars)(new_env.free_vars,*btvs,(struct Cyc_Absyn_Tvar*(*)(struct _tuple14*))Cyc_Core_fst);new_env.free_vars=_tmp3C0;});
# 932
{struct Cyc_List_List*tvs=new_env.free_vars;for(0;tvs != 0;tvs=tvs->tl){
struct _tuple14 _tmp1F7=*((struct _tuple14*)tvs->hd);struct _tuple14 _stmttmp24=_tmp1F7;struct _tuple14 _tmp1F8=_stmttmp24;int _tmp1FA;struct Cyc_Absyn_Tvar*_tmp1F9;_LLB6: _tmp1F9=_tmp1F8.f1;_tmp1FA=_tmp1F8.f2;_LLB7: {struct Cyc_Absyn_Tvar*t=_tmp1F9;int b=_tmp1FA;
({struct Cyc_List_List*_tmp3C1=Cyc_Tctyp_fast_add_free_tvar_bool(env.free_vars,t,b);env.free_vars=_tmp3C1;});}}}
# 937
{struct Cyc_List_List*evs=new_env.free_evars;for(0;evs != 0;evs=evs->tl){
struct _tuple13 _tmp1FB=*((struct _tuple13*)evs->hd);struct _tuple13 _stmttmp25=_tmp1FB;struct _tuple13 _tmp1FC=_stmttmp25;int _tmp1FE;void*_tmp1FD;_LLB9: _tmp1FD=_tmp1FC.f1;_tmp1FE=_tmp1FC.f2;_LLBA: {void*e=_tmp1FD;int b=_tmp1FE;
({struct Cyc_List_List*_tmp3C2=Cyc_Tctyp_add_free_evar(env.free_evars,e,b);env.free_evars=_tmp3C2;});}}}
# 941
goto _LL3;}}}}}}}case 6U: _LL16: _tmp110=((struct Cyc_Absyn_TupleType_Absyn_Type_struct*)_tmp109)->f1;_LL17: {struct Cyc_List_List*tq_ts=_tmp110;
# 944
for(0;tq_ts != 0;tq_ts=tq_ts->tl){
struct _tuple17*_tmp201=(struct _tuple17*)tq_ts->hd;struct _tuple17*p=_tmp201;
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tmk,(*p).f2,1,0);
({int _tmp3C3=
Cyc_Tcutil_extract_const_from_typedef(loc,((*p).f1).print_const,(*p).f2);
# 947
((*p).f1).real_const=_tmp3C3;});}
# 950
goto _LL3;}case 7U: _LL18: _tmp10E=((struct Cyc_Absyn_AnonAggrType_Absyn_Type_struct*)_tmp109)->f1;_tmp10F=((struct Cyc_Absyn_AnonAggrType_Absyn_Type_struct*)_tmp109)->f2;_LL19: {enum Cyc_Absyn_AggrKind k=_tmp10E;struct Cyc_List_List*fs=_tmp10F;
# 954
struct Cyc_List_List*prev_fields=0;
for(0;fs != 0;fs=fs->tl){
struct Cyc_Absyn_Aggrfield*_tmp202=(struct Cyc_Absyn_Aggrfield*)fs->hd;struct Cyc_Absyn_Aggrfield*_stmttmp26=_tmp202;struct Cyc_Absyn_Aggrfield*_tmp203=_stmttmp26;struct Cyc_Absyn_Exp*_tmp209;struct Cyc_List_List*_tmp208;struct Cyc_Absyn_Exp*_tmp207;void*_tmp206;struct Cyc_Absyn_Tqual*_tmp205;struct _fat_ptr*_tmp204;_LLBC: _tmp204=_tmp203->name;_tmp205=(struct Cyc_Absyn_Tqual*)& _tmp203->tq;_tmp206=_tmp203->type;_tmp207=_tmp203->width;_tmp208=_tmp203->attributes;_tmp209=_tmp203->requires_clause;_LLBD: {struct _fat_ptr*fn=_tmp204;struct Cyc_Absyn_Tqual*tqp=_tmp205;void*t=_tmp206;struct Cyc_Absyn_Exp*width=_tmp207;struct Cyc_List_List*atts=_tmp208;struct Cyc_Absyn_Exp*requires_clause=_tmp209;
if(((int(*)(int(*compare)(struct _fat_ptr*,struct _fat_ptr*),struct Cyc_List_List*l,struct _fat_ptr*x))Cyc_List_mem)(Cyc_strptrcmp,prev_fields,fn))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp20B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E3;_tmp2E3.tag=0U,({struct _fat_ptr _tmp3C4=({const char*_tmp20D="duplicate field ";_tag_fat(_tmp20D,sizeof(char),17U);});_tmp2E3.f1=_tmp3C4;});_tmp2E3;});struct Cyc_Warn_String_Warn_Warg_struct _tmp20C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E2;_tmp2E2.tag=0U,_tmp2E2.f1=*fn;_tmp2E2;});void*_tmp20A[2U];_tmp20A[0]=& _tmp20B,_tmp20A[1]=& _tmp20C;({unsigned _tmp3C5=loc;Cyc_Warn_err2(_tmp3C5,_tag_fat(_tmp20A,sizeof(void*),2U));});});
if(({struct _fat_ptr _tmp3C6=(struct _fat_ptr)*fn;Cyc_strcmp(_tmp3C6,({const char*_tmp20E="";_tag_fat(_tmp20E,sizeof(char),1U);}));})!= 0)
prev_fields=({struct Cyc_List_List*_tmp20F=_cycalloc(sizeof(*_tmp20F));_tmp20F->hd=fn,_tmp20F->tl=prev_fields;_tmp20F;});
env=Cyc_Tctyp_i_check_type(env,& Cyc_Kinds_tmk,t,1,0);
({int _tmp3C7=Cyc_Tcutil_extract_const_from_typedef(loc,tqp->print_const,t);tqp->real_const=_tmp3C7;});
Cyc_Tcutil_check_bitfield(loc,t,width,fn);
Cyc_Atts_check_field_atts(loc,fn,atts);
if(requires_clause != 0){
# 967
if((int)k != (int)1U)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp211=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E4;_tmp2E4.tag=0U,({struct _fat_ptr _tmp3C8=({const char*_tmp212="@requires clause is only allowed on union members";_tag_fat(_tmp212,sizeof(char),50U);});_tmp2E4.f1=_tmp3C8;});_tmp2E4;});void*_tmp210[1U];_tmp210[0]=& _tmp211;({unsigned _tmp3C9=loc;Cyc_Warn_err2(_tmp3C9,_tag_fat(_tmp210,sizeof(void*),1U));});});
({struct Cyc_Tcenv_Tenv*_tmp3CA=Cyc_Tcenv_allow_valueof(te);Cyc_Tcexp_tcExp(_tmp3CA,0,requires_clause);});
if(!Cyc_Tcutil_is_integral(requires_clause))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp214=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E7;_tmp2E7.tag=0U,({struct _fat_ptr _tmp3CB=({const char*_tmp218="@requires clause has type ";_tag_fat(_tmp218,sizeof(char),27U);});_tmp2E7.f1=_tmp3CB;});_tmp2E7;});struct Cyc_Warn_TypOpt_Warn_Warg_struct _tmp215=({struct Cyc_Warn_TypOpt_Warn_Warg_struct _tmp2E6;_tmp2E6.tag=3U,_tmp2E6.f1=(void*)requires_clause->topt;_tmp2E6;});struct Cyc_Warn_String_Warn_Warg_struct _tmp216=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E5;_tmp2E5.tag=0U,({
struct _fat_ptr _tmp3CC=({const char*_tmp217=" instead of integral type";_tag_fat(_tmp217,sizeof(char),26U);});_tmp2E5.f1=_tmp3CC;});_tmp2E5;});void*_tmp213[3U];_tmp213[0]=& _tmp214,_tmp213[1]=& _tmp215,_tmp213[2]=& _tmp216;({unsigned _tmp3CD=loc;Cyc_Warn_err2(_tmp3CD,_tag_fat(_tmp213,sizeof(void*),3U));});});
env=Cyc_Tctyp_i_check_type_level_exp(requires_clause,env);}}}
# 976
goto _LL3;}default: _LL1A: _tmp10A=((struct Cyc_Absyn_TypedefType_Absyn_Type_struct*)_tmp109)->f1;_tmp10B=(struct Cyc_List_List**)&((struct Cyc_Absyn_TypedefType_Absyn_Type_struct*)_tmp109)->f2;_tmp10C=(struct Cyc_Absyn_Typedefdecl**)&((struct Cyc_Absyn_TypedefType_Absyn_Type_struct*)_tmp109)->f3;_tmp10D=(void**)&((struct Cyc_Absyn_TypedefType_Absyn_Type_struct*)_tmp109)->f4;_LL1B: {struct _tuple1*tdn=_tmp10A;struct Cyc_List_List**targs_ref=_tmp10B;struct Cyc_Absyn_Typedefdecl**tdopt=_tmp10C;void**toptp=_tmp10D;
# 980
struct Cyc_Absyn_Typedefdecl*td;
{struct _handler_cons _tmp219;_push_handler(& _tmp219);{int _tmp21B=0;if(setjmp(_tmp219.handler))_tmp21B=1;if(!_tmp21B){td=Cyc_Tcenv_lookup_typedefdecl(te,loc,tdn);;_pop_handler();}else{void*_tmp21A=(void*)Cyc_Core_get_exn_thrown();void*_tmp21C=_tmp21A;void*_tmp21D;if(((struct Cyc_Dict_Absent_exn_struct*)_tmp21C)->tag == Cyc_Dict_Absent){_LLBF: _LLC0:
# 983
({struct Cyc_Warn_String_Warn_Warg_struct _tmp21F=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2E9;_tmp2E9.tag=0U,({struct _fat_ptr _tmp3CE=({const char*_tmp221="unbound typedef name ";_tag_fat(_tmp221,sizeof(char),22U);});_tmp2E9.f1=_tmp3CE;});_tmp2E9;});struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp220=({struct Cyc_Warn_Qvar_Warn_Warg_struct _tmp2E8;_tmp2E8.tag=1U,_tmp2E8.f1=tdn;_tmp2E8;});void*_tmp21E[2U];_tmp21E[0]=& _tmp21F,_tmp21E[1]=& _tmp220;({unsigned _tmp3CF=loc;Cyc_Warn_err2(_tmp3CF,_tag_fat(_tmp21E,sizeof(void*),2U));});});
return env;}else{_LLC1: _tmp21D=_tmp21C;_LLC2: {void*exn=_tmp21D;(int)_rethrow(exn);}}_LLBE:;}}}
# 986
*tdopt=td;
env=Cyc_Tctyp_check_type_inst(env,targs_ref,td->tvs,expected_kind,allow_abs_aggr,tdn,td);
# 989
if(td->defn != 0){
struct Cyc_List_List*inst=0;
struct Cyc_List_List*_tmp222=td->tvs;struct Cyc_List_List*tvs=_tmp222;
struct Cyc_List_List*_tmp223=*targs_ref;struct Cyc_List_List*ts=_tmp223;
for(0;tvs != 0;(tvs=tvs->tl,ts=ts->tl)){
inst=({struct Cyc_List_List*_tmp225=_cycalloc(sizeof(*_tmp225));({struct _tuple18*_tmp3D0=({struct _tuple18*_tmp224=_cycalloc(sizeof(*_tmp224));_tmp224->f1=(struct Cyc_Absyn_Tvar*)tvs->hd,_tmp224->f2=(void*)((struct Cyc_List_List*)_check_null(ts))->hd;_tmp224;});_tmp225->hd=_tmp3D0;}),_tmp225->tl=inst;_tmp225;});}
({void*_tmp3D1=Cyc_Tcutil_substitute(inst,(void*)_check_null(td->defn));*toptp=_tmp3D1;});}
# 997
goto _LL3;}}_LL3:;}
# 999
if(!({struct Cyc_Absyn_Kind*_tmp3D2=Cyc_Tcutil_type_kind(t);Cyc_Kinds_kind_leq(_tmp3D2,expected_kind);}))
({struct Cyc_Warn_String_Warn_Warg_struct _tmp227=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2EF;_tmp2EF.tag=0U,({struct _fat_ptr _tmp3D3=({const char*_tmp22F="type ";_tag_fat(_tmp22F,sizeof(char),6U);});_tmp2EF.f1=_tmp3D3;});_tmp2EF;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp228=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2EE;_tmp2EE.tag=2U,_tmp2EE.f1=(void*)t;_tmp2EE;});struct Cyc_Warn_String_Warn_Warg_struct _tmp229=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2ED;_tmp2ED.tag=0U,({struct _fat_ptr _tmp3D4=({const char*_tmp22E=" has kind ";_tag_fat(_tmp22E,sizeof(char),11U);});_tmp2ED.f1=_tmp3D4;});_tmp2ED;});struct Cyc_Warn_Kind_Warn_Warg_struct _tmp22A=({struct Cyc_Warn_Kind_Warn_Warg_struct _tmp2EC;_tmp2EC.tag=9U,({struct Cyc_Absyn_Kind*_tmp3D5=Cyc_Tcutil_type_kind(t);_tmp2EC.f1=_tmp3D5;});_tmp2EC;});struct Cyc_Warn_String_Warn_Warg_struct _tmp22B=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2EB;_tmp2EB.tag=0U,({
struct _fat_ptr _tmp3D6=({const char*_tmp22D=" but as used here needs kind ";_tag_fat(_tmp22D,sizeof(char),30U);});_tmp2EB.f1=_tmp3D6;});_tmp2EB;});struct Cyc_Warn_Kind_Warn_Warg_struct _tmp22C=({struct Cyc_Warn_Kind_Warn_Warg_struct _tmp2EA;_tmp2EA.tag=9U,_tmp2EA.f1=expected_kind;_tmp2EA;});void*_tmp226[6U];_tmp226[0]=& _tmp227,_tmp226[1]=& _tmp228,_tmp226[2]=& _tmp229,_tmp226[3]=& _tmp22A,_tmp226[4]=& _tmp22B,_tmp226[5]=& _tmp22C;({unsigned _tmp3D7=loc;Cyc_Warn_err2(_tmp3D7,_tag_fat(_tmp226,sizeof(void*),6U));});});
return env;}}
# 1005
static struct Cyc_Tctyp_CVTEnv Cyc_Tctyp_check_valid_type(struct Cyc_Tctyp_CVTEnv cvt,struct Cyc_Absyn_Kind*expected_kind,int allow_abs_aggr,void*t){
# 1007
struct Cyc_List_List*_tmp230=cvt.kind_env;struct Cyc_List_List*kind_env=_tmp230;
cvt=Cyc_Tctyp_i_check_type(cvt,expected_kind,t,1,allow_abs_aggr);
# 1010
{struct Cyc_List_List*vs=cvt.free_vars;for(0;vs != 0;vs=vs->tl){
({struct Cyc_List_List*_tmp3D8=Cyc_Tctyp_fast_add_free_tvar(kind_env,(*((struct _tuple14*)vs->hd)).f1);cvt.kind_env=_tmp3D8;});}}
# 1016
{struct Cyc_List_List*evs=cvt.free_evars;for(0;evs != 0;evs=evs->tl){
struct _tuple13 _tmp231=*((struct _tuple13*)evs->hd);struct _tuple13 _stmttmp27=_tmp231;struct _tuple13 _tmp232=_stmttmp27;void*_tmp233;_LL1: _tmp233=_tmp232.f1;_LL2: {void*e=_tmp233;
void*_tmp234=Cyc_Absyn_compress(e);void*_stmttmp28=_tmp234;void*_tmp235=_stmttmp28;struct Cyc_Core_Opt**_tmp236;if(((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp235)->tag == 1U){_LL4: _tmp236=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Evar_Absyn_Type_struct*)_tmp235)->f4;_LL5: {struct Cyc_Core_Opt**s=_tmp236;
# 1020
if(*s == 0)
({struct Cyc_Core_Opt*_tmp3D9=({struct Cyc_Core_Opt*_tmp237=_cycalloc(sizeof(*_tmp237));_tmp237->v=kind_env;_tmp237;});*s=_tmp3D9;});else{
# 1024
struct Cyc_List_List*_tmp238=0;struct Cyc_List_List*result=_tmp238;
{struct Cyc_List_List*_tmp239=(struct Cyc_List_List*)((struct Cyc_Core_Opt*)_check_null(*s))->v;struct Cyc_List_List*tvs=_tmp239;for(0;tvs != 0;tvs=tvs->tl){
if(((int(*)(int(*compare)(struct Cyc_Absyn_Tvar*,struct Cyc_Absyn_Tvar*),struct Cyc_List_List*l,struct Cyc_Absyn_Tvar*x))Cyc_List_mem)(Cyc_Tcutil_fast_tvar_cmp,kind_env,(struct Cyc_Absyn_Tvar*)tvs->hd))
result=({struct Cyc_List_List*_tmp23A=_cycalloc(sizeof(*_tmp23A));_tmp23A->hd=(struct Cyc_Absyn_Tvar*)tvs->hd,_tmp23A->tl=result;_tmp23A;});}}
({struct Cyc_Core_Opt*_tmp3DA=({struct Cyc_Core_Opt*_tmp23B=_cycalloc(sizeof(*_tmp23B));_tmp23B->v=result;_tmp23B;});*s=_tmp3DA;});}
# 1030
goto _LL3;}}else{_LL6: _LL7:
 goto _LL3;}_LL3:;}}}
# 1034
return cvt;}
# 1041
void Cyc_Tctyp_check_valid_toplevel_type(unsigned loc,struct Cyc_Tcenv_Tenv*te,void*t){
int generalize_evars=
Cyc_Tcutil_is_function_type(t)&&(!te->in_tempest || te->tempest_generalize);
struct Cyc_Absyn_Kind*expected_kind=Cyc_Tcutil_is_function_type(t)?& Cyc_Kinds_tak:& Cyc_Kinds_tmk;
struct Cyc_List_List*_tmp23C=Cyc_Tcenv_lookup_type_vars(te);struct Cyc_List_List*te_vars=_tmp23C;
struct Cyc_Tctyp_CVTEnv _tmp23D=({struct Cyc_Tctyp_CVTEnv _tmp3DC=({struct Cyc_Tctyp_CVTEnv _tmp2FC;_tmp2FC.loc=loc,_tmp2FC.te=te,_tmp2FC.kind_env=te_vars,_tmp2FC.fn_result=0,_tmp2FC.generalize_evars=generalize_evars,_tmp2FC.free_vars=0,_tmp2FC.free_evars=0;_tmp2FC;});struct Cyc_Absyn_Kind*_tmp3DB=expected_kind;Cyc_Tctyp_check_valid_type(_tmp3DC,_tmp3DB,1,t);});struct Cyc_Tctyp_CVTEnv cvt=_tmp23D;
# 1049
struct Cyc_List_List*_tmp23E=((struct Cyc_List_List*(*)(struct Cyc_Absyn_Tvar*(*f)(struct _tuple14*),struct Cyc_List_List*x))Cyc_List_map)((struct Cyc_Absyn_Tvar*(*)(struct _tuple14*))Cyc_Core_fst,cvt.free_vars);struct Cyc_List_List*free_tvars=_tmp23E;
struct Cyc_List_List*_tmp23F=cvt.free_evars;struct Cyc_List_List*free_evars=_tmp23F;
# 1052
if(te_vars != 0){
struct Cyc_List_List*_tmp240=0;struct Cyc_List_List*res=_tmp240;
{struct Cyc_List_List*_tmp241=free_tvars;struct Cyc_List_List*fs=_tmp241;for(0;(unsigned)fs;fs=fs->tl){
struct Cyc_List_List*_tmp242=te_vars;struct Cyc_List_List*ts=_tmp242;
for(0;ts != 0;ts=ts->tl){
if(Cyc_Absyn_tvar_cmp((struct Cyc_Absyn_Tvar*)fs->hd,(struct Cyc_Absyn_Tvar*)ts->hd)== 0)
break;}
if(ts == 0)
res=({struct Cyc_List_List*_tmp243=_cycalloc(sizeof(*_tmp243));_tmp243->hd=(struct Cyc_Absyn_Tvar*)fs->hd,_tmp243->tl=res;_tmp243;});}}
# 1062
free_tvars=((struct Cyc_List_List*(*)(struct Cyc_List_List*x))Cyc_List_imp_rev)(res);}
# 1067
{struct Cyc_List_List*x=free_tvars;for(0;x != 0;x=x->tl){
void*_tmp244=Cyc_Kinds_compress_kb(((struct Cyc_Absyn_Tvar*)x->hd)->kind);void*_stmttmp29=_tmp244;void*_tmp245=_stmttmp29;enum Cyc_Absyn_AliasQual _tmp246;struct Cyc_Absyn_Kind*_tmp248;struct Cyc_Core_Opt**_tmp247;struct Cyc_Core_Opt**_tmp249;switch(*((int*)_tmp245)){case 1U: _LL1: _tmp249=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Unknown_kb_Absyn_KindBound_struct*)_tmp245)->f1;_LL2: {struct Cyc_Core_Opt**f=_tmp249;
({struct Cyc_Core_Opt*_tmp3DD=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_bk);*f=_tmp3DD;});goto _LL0;}case 2U: _LL3: _tmp247=(struct Cyc_Core_Opt**)&((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp245)->f1;_tmp248=((struct Cyc_Absyn_Less_kb_Absyn_KindBound_struct*)_tmp245)->f2;_LL4: {struct Cyc_Core_Opt**f=_tmp247;struct Cyc_Absyn_Kind*k=_tmp248;
# 1071
{struct Cyc_Absyn_Kind*_tmp24A=k;switch(((struct Cyc_Absyn_Kind*)_tmp24A)->kind){case Cyc_Absyn_BoxKind: if(((struct Cyc_Absyn_Kind*)_tmp24A)->aliasqual == Cyc_Absyn_Top){_LLA: _LLB:
 goto _LLD;}else{goto _LL14;}case Cyc_Absyn_MemKind: switch(((struct Cyc_Absyn_Kind*)_tmp24A)->aliasqual){case Cyc_Absyn_Top: _LLC: _LLD:
 goto _LLF;case Cyc_Absyn_Aliasable: _LLE: _LLF:
# 1075
({struct Cyc_Core_Opt*_tmp3DE=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_bk);*f=_tmp3DE;});goto _LL9;case Cyc_Absyn_Unique: _LL10: _LL11:
# 1077
({struct Cyc_Core_Opt*_tmp3DF=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_ubk);*f=_tmp3DF;});goto _LL9;default: goto _LL14;}case Cyc_Absyn_RgnKind: if(((struct Cyc_Absyn_Kind*)_tmp24A)->aliasqual == Cyc_Absyn_Top){_LL12: _LL13:
# 1079
({struct Cyc_Core_Opt*_tmp3E0=Cyc_Kinds_kind_to_bound_opt(& Cyc_Kinds_rk);*f=_tmp3E0;});goto _LL9;}else{goto _LL14;}default: _LL14: _LL15:
# 1081
({struct Cyc_Core_Opt*_tmp3E1=Cyc_Kinds_kind_to_bound_opt(k);*f=_tmp3E1;});goto _LL9;}_LL9:;}
# 1083
goto _LL0;}default: if(((struct Cyc_Absyn_Kind*)((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp245)->f1)->kind == Cyc_Absyn_MemKind){_LL5: _tmp246=(((struct Cyc_Absyn_Eq_kb_Absyn_KindBound_struct*)_tmp245)->f1)->aliasqual;_LL6: {enum Cyc_Absyn_AliasQual a=_tmp246;
# 1085
({struct Cyc_Warn_String_Warn_Warg_struct _tmp24C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2F3;_tmp2F3.tag=0U,({struct _fat_ptr _tmp3E2=({const char*_tmp252="type variable ";_tag_fat(_tmp252,sizeof(char),15U);});_tmp2F3.f1=_tmp3E2;});_tmp2F3;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp24D=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp2F2;_tmp2F2.tag=7U,_tmp2F2.f1=(struct Cyc_Absyn_Tvar*)x->hd;_tmp2F2;});struct Cyc_Warn_String_Warn_Warg_struct _tmp24E=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2F1;_tmp2F1.tag=0U,({struct _fat_ptr _tmp3E3=({const char*_tmp251=" cannot have kind ";_tag_fat(_tmp251,sizeof(char),19U);});_tmp2F1.f1=_tmp3E3;});_tmp2F1;});struct Cyc_Warn_Kind_Warn_Warg_struct _tmp24F=({struct Cyc_Warn_Kind_Warn_Warg_struct _tmp2F0;_tmp2F0.tag=9U,({struct Cyc_Absyn_Kind*_tmp3E4=({struct Cyc_Absyn_Kind*_tmp250=_cycalloc(sizeof(*_tmp250));_tmp250->kind=Cyc_Absyn_MemKind,_tmp250->aliasqual=a;_tmp250;});_tmp2F0.f1=_tmp3E4;});_tmp2F0;});void*_tmp24B[4U];_tmp24B[0]=& _tmp24C,_tmp24B[1]=& _tmp24D,_tmp24B[2]=& _tmp24E,_tmp24B[3]=& _tmp24F;({unsigned _tmp3E5=loc;Cyc_Warn_err2(_tmp3E5,_tag_fat(_tmp24B,sizeof(void*),4U));});});
goto _LL0;}}else{_LL7: _LL8:
 goto _LL0;}}_LL0:;}}
# 1091
if(free_tvars != 0 || free_evars != 0){
{void*_tmp253=Cyc_Absyn_compress(t);void*_stmttmp2A=_tmp253;void*_tmp254=_stmttmp2A;struct Cyc_List_List**_tmp255;if(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp254)->tag == 5U){_LL17: _tmp255=(struct Cyc_List_List**)&(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp254)->f1).tvars;_LL18: {struct Cyc_List_List**btvs=_tmp255;
# 1094
if(*btvs == 0){
# 1096
({struct Cyc_List_List*_tmp3E6=((struct Cyc_List_List*(*)(int(*f)(struct Cyc_Absyn_Tvar*),struct Cyc_List_List*x))Cyc_List_filter)(Cyc_CurRgn_is_not_curr_rgn,free_tvars);*btvs=_tmp3E6;});
free_tvars=0;}
# 1099
goto _LL16;}}else{_LL19: _LL1A:
 goto _LL16;}_LL16:;}
# 1102
if(free_tvars != 0)
({struct Cyc_Warn_String_Warn_Warg_struct _tmp257=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2F7;_tmp2F7.tag=0U,({struct _fat_ptr _tmp3E7=({const char*_tmp25C="unbound type variable ";_tag_fat(_tmp25C,sizeof(char),23U);});_tmp2F7.f1=_tmp3E7;});_tmp2F7;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp258=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp2F6;_tmp2F6.tag=7U,_tmp2F6.f1=(struct Cyc_Absyn_Tvar*)free_tvars->hd;_tmp2F6;});struct Cyc_Warn_String_Warn_Warg_struct _tmp259=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2F5;_tmp2F5.tag=0U,({struct _fat_ptr _tmp3E8=({const char*_tmp25B=" when checking ";_tag_fat(_tmp25B,sizeof(char),16U);});_tmp2F5.f1=_tmp3E8;});_tmp2F5;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp25A=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2F4;_tmp2F4.tag=2U,_tmp2F4.f1=(void*)t;_tmp2F4;});void*_tmp256[4U];_tmp256[0]=& _tmp257,_tmp256[1]=& _tmp258,_tmp256[2]=& _tmp259,_tmp256[3]=& _tmp25A;({unsigned _tmp3E9=loc;Cyc_Warn_err2(_tmp3E9,_tag_fat(_tmp256,sizeof(void*),4U));});});
if(!Cyc_Tcutil_is_function_type(t)|| !te->in_tempest)
Cyc_Tctyp_check_free_evars(free_evars,t,loc);}{
# 1108
void*_tmp25D=Cyc_Absyn_compress(t);void*_stmttmp2B=_tmp25D;void*_tmp25E=_stmttmp2B;struct Cyc_List_List*_tmp25F;if(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp25E)->tag == 5U){_LL1C: _tmp25F=(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp25E)->f1).tvars;_LL1D: {struct Cyc_List_List*btvs=_tmp25F;
# 1110
if(((int(*)(int(*pred)(struct Cyc_Absyn_Tvar*),struct Cyc_List_List*x))Cyc_List_exists)(Cyc_CurRgn_is_curr_rgn,btvs))
# 1112
({struct Cyc_Warn_String_Warn_Warg_struct _tmp261=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2FB;_tmp2FB.tag=0U,({struct _fat_ptr _tmp3EA=({const char*_tmp266="cannot abstract current region ";_tag_fat(_tmp266,sizeof(char),32U);});_tmp2FB.f1=_tmp3EA;});_tmp2FB;});struct Cyc_Warn_String_Warn_Warg_struct _tmp262=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2FA;_tmp2FA.tag=0U,_tmp2FA.f1=Cyc_CurRgn_curr_rgn_name;_tmp2FA;});struct Cyc_Warn_String_Warn_Warg_struct _tmp263=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2F9;_tmp2F9.tag=0U,({struct _fat_ptr _tmp3EB=({const char*_tmp265=" in ";_tag_fat(_tmp265,sizeof(char),5U);});_tmp2F9.f1=_tmp3EB;});_tmp2F9;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp264=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2F8;_tmp2F8.tag=2U,_tmp2F8.f1=(void*)t;_tmp2F8;});void*_tmp260[4U];_tmp260[0]=& _tmp261,_tmp260[1]=& _tmp262,_tmp260[2]=& _tmp263,_tmp260[3]=& _tmp264;({unsigned _tmp3EC=loc;Cyc_Warn_err2(_tmp3EC,_tag_fat(_tmp260,sizeof(void*),4U));});});
goto _LL1B;}}else{_LL1E: _LL1F:
 goto _LL1B;}_LL1B:;}}
# 1122
void Cyc_Tctyp_check_fndecl_valid_type(unsigned loc,struct Cyc_Tcenv_Tenv*te,struct Cyc_Absyn_Fndecl*fd){
void*t=Cyc_Tcutil_fndecl2type(fd);
# 1125
Cyc_Tctyp_check_valid_toplevel_type(loc,te,t);
{void*_tmp267=Cyc_Absyn_compress(t);void*_stmttmp2C=_tmp267;void*_tmp268=_stmttmp2C;struct Cyc_Absyn_FnInfo _tmp269;if(((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp268)->tag == 5U){_LL1: _tmp269=((struct Cyc_Absyn_FnType_Absyn_Type_struct*)_tmp268)->f1;_LL2: {struct Cyc_Absyn_FnInfo i=_tmp269;
# 1128
struct Cyc_List_List*_tmp26A=(fd->i).attributes;struct Cyc_List_List*atts=_tmp26A;
fd->i=i;
(fd->i).attributes=atts;
({int _tmp3ED=
Cyc_Tcutil_extract_const_from_typedef(loc,(i.ret_tqual).print_const,i.ret_type);
# 1131
((fd->i).ret_tqual).real_const=_tmp3ED;});
# 1133
goto _LL0;}}else{_LL3: _LL4:
({struct Cyc_Warn_String_Warn_Warg_struct _tmp26C=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2FD;_tmp2FD.tag=0U,({struct _fat_ptr _tmp3EE=({const char*_tmp26D="check_fndecl_valid_type: not a FnType";_tag_fat(_tmp26D,sizeof(char),38U);});_tmp2FD.f1=_tmp3EE;});_tmp2FD;});void*_tmp26B[1U];_tmp26B[0]=& _tmp26C;((int(*)(struct _fat_ptr))Cyc_Warn_impos2)(_tag_fat(_tmp26B,sizeof(void*),1U));});}_LL0:;}{
# 1136
struct Cyc_List_List*_tmp26E=0;struct Cyc_List_List*argnames=_tmp26E;
{struct Cyc_List_List*_tmp26F=(fd->i).args;struct Cyc_List_List*args=_tmp26F;for(0;args != 0;args=args->tl){
argnames=({struct Cyc_List_List*_tmp270=_cycalloc(sizeof(*_tmp270));_tmp270->hd=(struct _fat_ptr*)_check_null((*((struct _tuple9*)args->hd)).f1),_tmp270->tl=argnames;_tmp270;});}}
({struct Cyc_List_List*_tmp3F0=argnames;unsigned _tmp3EF=loc;Cyc_Tcutil_check_unique_vars(_tmp3F0,_tmp3EF,({const char*_tmp271="function declaration has repeated parameter";_tag_fat(_tmp271,sizeof(char),44U);}));});
# 1141
fd->cached_type=t;}}
# 1146
void Cyc_Tctyp_check_type(unsigned loc,struct Cyc_Tcenv_Tenv*te,struct Cyc_List_List*bound_tvars,struct Cyc_Absyn_Kind*expected_kind,int allow_evars,int allow_abs_aggr,void*t){
# 1149
struct Cyc_Tctyp_CVTEnv _tmp272=({struct Cyc_Tctyp_CVTEnv _tmp3F3=({struct Cyc_Tctyp_CVTEnv _tmp302;_tmp302.loc=loc,_tmp302.te=te,_tmp302.kind_env=bound_tvars,_tmp302.fn_result=0,_tmp302.generalize_evars=0,_tmp302.free_vars=0,_tmp302.free_evars=0;_tmp302;});struct Cyc_Absyn_Kind*_tmp3F2=expected_kind;int _tmp3F1=allow_abs_aggr;Cyc_Tctyp_check_valid_type(_tmp3F3,_tmp3F2,_tmp3F1,t);});struct Cyc_Tctyp_CVTEnv cvt=_tmp272;
# 1151
struct Cyc_List_List*_tmp273=((struct Cyc_List_List*(*)(struct Cyc_List_List*tvs,struct Cyc_List_List*btvs,struct Cyc_Absyn_Tvar*(*get)(struct _tuple14*)))Cyc_Tctyp_remove_bound_tvars)(cvt.free_vars,bound_tvars,(struct Cyc_Absyn_Tvar*(*)(struct _tuple14*))Cyc_Core_fst);struct Cyc_List_List*free_tvs=_tmp273;
for(0;free_tvs != 0;free_tvs=free_tvs->tl){
({struct Cyc_Warn_String_Warn_Warg_struct _tmp275=({struct Cyc_Warn_String_Warn_Warg_struct _tmp301;_tmp301.tag=0U,({struct _fat_ptr _tmp3F4=({const char*_tmp27A="unbound type variable ";_tag_fat(_tmp27A,sizeof(char),23U);});_tmp301.f1=_tmp3F4;});_tmp301;});struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp276=({struct Cyc_Warn_Tvar_Warn_Warg_struct _tmp300;_tmp300.tag=7U,_tmp300.f1=(*((struct _tuple14*)free_tvs->hd)).f1;_tmp300;});struct Cyc_Warn_String_Warn_Warg_struct _tmp277=({struct Cyc_Warn_String_Warn_Warg_struct _tmp2FF;_tmp2FF.tag=0U,({struct _fat_ptr _tmp3F5=({const char*_tmp279=" in type ";_tag_fat(_tmp279,sizeof(char),10U);});_tmp2FF.f1=_tmp3F5;});_tmp2FF;});struct Cyc_Warn_Typ_Warn_Warg_struct _tmp278=({struct Cyc_Warn_Typ_Warn_Warg_struct _tmp2FE;_tmp2FE.tag=2U,_tmp2FE.f1=(void*)t;_tmp2FE;});void*_tmp274[4U];_tmp274[0]=& _tmp275,_tmp274[1]=& _tmp276,_tmp274[2]=& _tmp277,_tmp274[3]=& _tmp278;({unsigned _tmp3F6=loc;Cyc_Warn_err2(_tmp3F6,_tag_fat(_tmp274,sizeof(void*),4U));});});}
if(!allow_evars)
Cyc_Tctyp_check_free_evars(cvt.free_evars,t,loc);}