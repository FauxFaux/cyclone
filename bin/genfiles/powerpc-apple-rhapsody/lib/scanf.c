#include <setjmp.h>
/* This is a C header file to be used by the output of the Cyclone to
   C translator.  The corresponding definitions are in file
   lib/runtime_cyc.c
*/
#ifndef _CYC_INCLUDE_H_
#define _CYC_INCLUDE_H_

#ifdef NO_CYC_PREFIX
#define ADD_PREFIX(x) x
#else
#define ADD_PREFIX(x) Cyc_##x
#endif

#ifndef offsetof
/* should be size_t, but int is fine. */
#define offsetof(t,n) ((int)(&(((t *)0)->n)))
#endif

/* Tagged arrays */
struct _dyneither_ptr {
  unsigned char *curr; 
  unsigned char *base; 
  unsigned char *last_plus_one; 
};  

/* Discriminated Unions */
struct _xtunion_struct { char *tag; };

/* Need one of these per thread (we don't have threads)
   The runtime maintains a stack that contains either _handler_cons
   structs or _RegionHandle structs.  The tag is 0 for a handler_cons
   and 1 for a region handle.  */
struct _RuntimeStack {
  int tag; /* 0 for an exception handler, 1 for a region handle */
  struct _RuntimeStack *next;
};

/* Regions */
struct _RegionPage {
#ifdef CYC_REGION_PROFILE
  unsigned total_bytes;
  unsigned free_bytes;
#endif
  struct _RegionPage *next;
  char data[1];  /*FJS: used to be size 0, but that's forbidden in ansi c*/
};

struct _RegionHandle {
  struct _RuntimeStack s;
  struct _RegionPage *curr;
  char               *offset;
  char               *last_plus_one;
  struct _DynRegionHandle *sub_regions;
#ifdef CYC_REGION_PROFILE
  const char         *name;
#endif
};

struct _DynRegionFrame {
  struct _RuntimeStack s;
  struct _DynRegionHandle *x;
};

extern struct _RegionHandle _new_region(const char *);
extern void * _region_malloc(struct _RegionHandle *, unsigned);
extern void * _region_calloc(struct _RegionHandle *, unsigned t, unsigned n);
extern void   _free_region(struct _RegionHandle *);
extern void   _reset_region(struct _RegionHandle *);
extern struct _RegionHandle *_open_dynregion(struct _DynRegionFrame *f,
                                             struct _DynRegionHandle *h);
extern void   _pop_dynregion();

/* Exceptions */
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

/* Built-in Exceptions */
struct Cyc_Null_Exception_struct { char *tag; };
struct Cyc_Array_bounds_struct { char *tag; };
struct Cyc_Match_Exception_struct { char *tag; };
struct Cyc_Bad_alloc_struct { char *tag; };
extern char Cyc_Null_Exception[];
extern char Cyc_Array_bounds[];
extern char Cyc_Match_Exception[];
extern char Cyc_Bad_alloc[];

/* Built-in Run-time Checks and company */
#ifdef __APPLE__
#define _INLINE_FUNCTIONS
#endif

#ifdef CYC_ANSI_OUTPUT
#define _INLINE  
#define _INLINE_FUNCTIONS
#else
#define _INLINE inline
#endif

#ifdef VC_C
#define _CYC_U_LONG_LONG_T __int64
#else
#ifdef GCC_C
#define _CYC_U_LONG_LONG_T unsigned long long
#else
#define _CYC_U_LONG_LONG_T unsigned long long
#endif
#endif

#ifdef NO_CYC_NULL_CHECKS
#define _check_null(ptr) (ptr)
#else
#ifdef _INLINE_FUNCTIONS
static _INLINE void *
_check_null(void *ptr) {
  void*_check_null_temp = (void*)(ptr);
  if (!_check_null_temp) _throw_null();
  return _check_null_temp;
}
#else
#define _check_null(ptr) \
  ({ void*_check_null_temp = (void*)(ptr); \
     if (!_check_null_temp) _throw_null(); \
     _check_null_temp; })
#endif
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  ((char *)ptr) + (elt_sz)*(index); })
#else
#ifdef _INLINE_FUNCTIONS
static _INLINE char *
_check_known_subscript_null(void *ptr, unsigned bound, unsigned elt_sz, unsigned index) {
  void*_cks_ptr = (void*)(ptr);
  unsigned _cks_bound = (bound);
  unsigned _cks_elt_sz = (elt_sz);
  unsigned _cks_index = (index);
  if (!_cks_ptr) _throw_null();
  if (_cks_index >= _cks_bound) _throw_arraybounds();
  return ((char *)_cks_ptr) + _cks_elt_sz*_cks_index;
}
#else
#define _check_known_subscript_null(ptr,bound,elt_sz,index) ({ \
  void*_cks_ptr = (void*)(ptr); \
  unsigned _cks_bound = (bound); \
  unsigned _cks_elt_sz = (elt_sz); \
  unsigned _cks_index = (index); \
  if (!_cks_ptr) _throw_null(); \
  if (_cks_index >= _cks_bound) _throw_arraybounds(); \
  ((char *)_cks_ptr) + _cks_elt_sz*_cks_index; })
#endif
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _check_known_subscript_notnull(bound,index) (index)
#else
#ifdef _INLINE_FUNCTIONS
static _INLINE unsigned
_check_known_subscript_notnull(unsigned bound,unsigned index) { 
  unsigned _cksnn_bound = (bound); 
  unsigned _cksnn_index = (index); 
  if (_cksnn_index >= _cksnn_bound) _throw_arraybounds(); 
  return _cksnn_index;
}
#else
#define _check_known_subscript_notnull(bound,index) ({ \
  unsigned _cksnn_bound = (bound); \
  unsigned _cksnn_index = (index); \
  if (_cksnn_index >= _cksnn_bound) _throw_arraybounds(); \
  _cksnn_index; })
#endif
#endif

/* Add i to zero-terminated pointer x.  Checks for x being null and
   ensures that x[0..i-1] are not 0. */
#ifdef NO_CYC_BOUNDS_CHECK
#define _zero_arr_plus_char(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_short(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_int(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_float(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_double(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_longdouble(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#define _zero_arr_plus_voidstar(orig_x,orig_sz,orig_i) ((orig_x)+(orig_i))
#else
static _INLINE char *
_zero_arr_plus_char(char *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE short *
_zero_arr_plus_short(short *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE int *
_zero_arr_plus_int(int *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE float *
_zero_arr_plus_float(float *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE double *
_zero_arr_plus_double(double *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE long double *
_zero_arr_plus_longdouble(long double *orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
static _INLINE void *
_zero_arr_plus_voidstar(void **orig_x, int orig_sz, int orig_i) {
  unsigned int _czs_temp;
  if ((orig_x) == 0) _throw_null();
  if (orig_i < 0) _throw_arraybounds();
  for (_czs_temp=orig_sz; _czs_temp < orig_i; _czs_temp++)
    if (orig_x[_czs_temp] == 0) _throw_arraybounds();
  return orig_x + orig_i;
}
#endif


/* Calculates the number of elements in a zero-terminated, thin array.
   If non-null, the array is guaranteed to have orig_offset elements. */
static _INLINE int
_get_zero_arr_size_char(const char *orig_x, unsigned int orig_offset) {
  const char *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_short(const short *orig_x, unsigned int orig_offset) {
  const short *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_int(const int *orig_x, unsigned int orig_offset) {
  const int *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_float(const float *orig_x, unsigned int orig_offset) {
  const float *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_double(const double *orig_x, unsigned int orig_offset) {
  const double *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_longdouble(const long double *orig_x, unsigned int orig_offset) {
  const long double *_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}
static _INLINE int
_get_zero_arr_size_voidstar(const void **orig_x, unsigned int orig_offset) {
  const void **_gres_x = orig_x;
  unsigned int _gres = 0;
  if (_gres_x != 0) {
     _gres = orig_offset;
     _gres_x += orig_offset - 1;
     while (*_gres_x != 0) { _gres_x++; _gres++; }
  }
  return _gres; 
}


/* Does in-place addition of a zero-terminated pointer (x += e and ++x).  
   Note that this expands to call _zero_arr_plus. */
/*#define _zero_arr_inplace_plus(x,orig_i) ({ \
  typedef _zap_tx = (*x); \
  _zap_tx **_zap_x = &((_zap_tx*)x); \
  *_zap_x = _zero_arr_plus(*_zap_x,1,(orig_i)); })
  */
static _INLINE void 
_zero_arr_inplace_plus_char(char *x, int orig_i) {
  char **_zap_x = &x;
  *_zap_x = _zero_arr_plus_char(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_short(short *x, int orig_i) {
  short **_zap_x = &x;
  *_zap_x = _zero_arr_plus_short(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_int(int *x, int orig_i) {
  int **_zap_x = &x;
  *_zap_x = _zero_arr_plus_int(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_float(float *x, int orig_i) {
  float **_zap_x = &x;
  *_zap_x = _zero_arr_plus_float(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_double(double *x, int orig_i) {
  double **_zap_x = &x;
  *_zap_x = _zero_arr_plus_double(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_longdouble(long double *x, int orig_i) {
  long double **_zap_x = &x;
  *_zap_x = _zero_arr_plus_longdouble(*_zap_x,1,orig_i);
}
static _INLINE void 
_zero_arr_inplace_plus_voidstar(void **x, int orig_i) {
  void ***_zap_x = &x;
  *_zap_x = _zero_arr_plus_voidstar(*_zap_x,1,orig_i);
}




/* Does in-place increment of a zero-terminated pointer (e.g., x++).
   Note that this expands to call _zero_arr_plus. */
/*#define _zero_arr_inplace_plus_post(x,orig_i) ({ \
  typedef _zap_tx = (*x); \
  _zap_tx **_zap_x = &((_zap_tx*)x); \
  _zap_tx *_zap_res = *_zap_x; \
  *_zap_x = _zero_arr_plus(_zap_res,1,(orig_i)); \
  _zap_res; })*/
  
static _INLINE char *
_zero_arr_inplace_plus_post_char(char *x, int orig_i){
  char ** _zap_x = &x;
  char * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_char(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE short *
_zero_arr_inplace_plus_post_short(short *x, int orig_i){
  short **_zap_x = &x;
  short * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_short(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE int *
_zero_arr_inplace_plus_post_int(int *x, int orig_i){
  int **_zap_x = &x;
  int * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_int(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE float *
_zero_arr_inplace_plus_post_float(float *x, int orig_i){
  float **_zap_x = &x;
  float * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_float(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE double *
_zero_arr_inplace_plus_post_double(double *x, int orig_i){
  double **_zap_x = &x;
  double * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_double(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE long double *
_zero_arr_inplace_plus_post_longdouble(long double *x, int orig_i){
  long double **_zap_x = &x;
  long double * _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_longdouble(_zap_res,1,orig_i);
  return _zap_res;
}
static _INLINE void **
_zero_arr_inplace_plus_post_voidstar(void **x, int orig_i){
  void ***_zap_x = &x;
  void ** _zap_res = *_zap_x;
  *_zap_x = _zero_arr_plus_voidstar(_zap_res,1,orig_i);
  return _zap_res;
}



/* functions for dealing with dynamically sized pointers */
#ifdef NO_CYC_BOUNDS_CHECKS
#ifdef _INLINE_FUNCTIONS
static _INLINE unsigned char *
_check_dyneither_subscript(struct _dyneither_ptr arr,unsigned elt_sz,unsigned index) {
  struct _dyneither_ptr _cus_arr = (arr);
  unsigned _cus_elt_sz = (elt_sz);
  unsigned _cus_index = (index);
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index;
  return _cus_ans;
}
#else
#define _check_dyneither_subscript(arr,elt_sz,index) ({ \
  struct _dyneither_ptr _cus_arr = (arr); \
  unsigned _cus_elt_sz = (elt_sz); \
  unsigned _cus_index = (index); \
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index; \
  _cus_ans; })
#endif
#else
#ifdef _INLINE_FUNCTIONS
static _INLINE unsigned char *
_check_dyneither_subscript(struct _dyneither_ptr arr,unsigned elt_sz,unsigned index) {
  struct _dyneither_ptr _cus_arr = (arr);
  unsigned _cus_elt_sz = (elt_sz);
  unsigned _cus_index = (index);
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index;
  /* JGM: not needed! if (!_cus_arr.base) _throw_null(); */ 
  if (_cus_ans < _cus_arr.base || _cus_ans >= _cus_arr.last_plus_one)
    _throw_arraybounds();
  return _cus_ans;
}
#else
#define _check_dyneither_subscript(arr,elt_sz,index) ({ \
  struct _dyneither_ptr _cus_arr = (arr); \
  unsigned _cus_elt_sz = (elt_sz); \
  unsigned _cus_index = (index); \
  unsigned char *_cus_ans = _cus_arr.curr + _cus_elt_sz * _cus_index; \
  /* JGM: not needed! if (!_cus_arr.base) _throw_null();*/ \
  if (_cus_ans < _cus_arr.base || _cus_ans >= _cus_arr.last_plus_one) \
    _throw_arraybounds(); \
  _cus_ans; })
#endif
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE struct _dyneither_ptr
_tag_dyneither(const void *tcurr,unsigned elt_sz,unsigned num_elts) {
  struct _dyneither_ptr _tag_arr_ans;
  _tag_arr_ans.base = _tag_arr_ans.curr = (void*)(tcurr);
  _tag_arr_ans.last_plus_one = _tag_arr_ans.base + (elt_sz) * (num_elts);
  return _tag_arr_ans;
}
#else
#define _tag_dyneither(tcurr,elt_sz,num_elts) ({ \
  struct _dyneither_ptr _tag_arr_ans; \
  _tag_arr_ans.base = _tag_arr_ans.curr = (void*)(tcurr); \
  _tag_arr_ans.last_plus_one = _tag_arr_ans.base + (elt_sz) * (num_elts); \
  _tag_arr_ans; })
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE struct _dyneither_ptr *
_init_dyneither_ptr(struct _dyneither_ptr *arr_ptr,
                    void *arr, unsigned elt_sz, unsigned num_elts) {
  struct _dyneither_ptr *_itarr_ptr = (arr_ptr);
  void* _itarr = (arr);
  _itarr_ptr->base = _itarr_ptr->curr = _itarr;
  _itarr_ptr->last_plus_one = ((unsigned char *)_itarr) + (elt_sz) * (num_elts);
  return _itarr_ptr;
}
#else
#define _init_dyneither_ptr(arr_ptr,arr,elt_sz,num_elts) ({ \
  struct _dyneither_ptr *_itarr_ptr = (arr_ptr); \
  void* _itarr = (arr); \
  _itarr_ptr->base = _itarr_ptr->curr = _itarr; \
  _itarr_ptr->last_plus_one = ((char *)_itarr) + (elt_sz) * (num_elts); \
  _itarr_ptr; })
#endif

#ifdef NO_CYC_BOUNDS_CHECKS
#define _untag_dyneither_ptr(arr,elt_sz,num_elts) ((arr).curr)
#else
#ifdef _INLINE_FUNCTIONS
static _INLINE unsigned char *
_untag_dyneither_ptr(struct _dyneither_ptr arr, 
                     unsigned elt_sz,unsigned num_elts) {
  struct _dyneither_ptr _arr = (arr);
  unsigned char *_curr = _arr.curr;
  if (_curr < _arr.base || _curr + (elt_sz) * (num_elts) > _arr.last_plus_one)
    _throw_arraybounds();
  return _curr;
}
#else
#define _untag_dyneither_ptr(arr,elt_sz,num_elts) ({ \
  struct _dyneither_ptr _arr = (arr); \
  unsigned char *_curr = _arr.curr; \
  if (_curr < _arr.base || _curr + (elt_sz) * (num_elts) > _arr.last_plus_one)\
    _throw_arraybounds(); \
  _curr; })
#endif
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE unsigned
_get_dyneither_size(struct _dyneither_ptr arr,unsigned elt_sz) {
  struct _dyneither_ptr _get_arr_size_temp = (arr);
  unsigned char *_get_arr_size_curr=_get_arr_size_temp.curr;
  unsigned char *_get_arr_size_last=_get_arr_size_temp.last_plus_one;
  return (_get_arr_size_curr < _get_arr_size_temp.base ||
          _get_arr_size_curr >= _get_arr_size_last) ? 0 :
    ((_get_arr_size_last - _get_arr_size_curr) / (elt_sz));
}
#else
#define _get_dyneither_size(arr,elt_sz) \
  ({struct _dyneither_ptr _get_arr_size_temp = (arr); \
    unsigned char *_get_arr_size_curr=_get_arr_size_temp.curr; \
    unsigned char *_get_arr_size_last=_get_arr_size_temp.last_plus_one; \
    (_get_arr_size_curr < _get_arr_size_temp.base || \
     _get_arr_size_curr >= _get_arr_size_last) ? 0 : \
    ((_get_arr_size_last - _get_arr_size_curr) / (elt_sz));})
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE struct _dyneither_ptr
_dyneither_ptr_plus(struct _dyneither_ptr arr,unsigned elt_sz,int change) {
  struct _dyneither_ptr _ans = (arr);
  _ans.curr += ((int)(elt_sz))*(change);
  return _ans;
}
#else
#define _dyneither_ptr_plus(arr,elt_sz,change) ({ \
  struct _dyneither_ptr _ans = (arr); \
  _ans.curr += ((int)(elt_sz))*(change); \
  _ans; })
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE struct _dyneither_ptr
_dyneither_ptr_inplace_plus(struct _dyneither_ptr *arr_ptr,unsigned elt_sz,
                            int change) {
  struct _dyneither_ptr * _arr_ptr = (arr_ptr);
  _arr_ptr->curr += ((int)(elt_sz))*(change);
  return *_arr_ptr;
}
#else
#define _dyneither_ptr_inplace_plus(arr_ptr,elt_sz,change) ({ \
  struct _dyneither_ptr * _arr_ptr = (arr_ptr); \
  _arr_ptr->curr += ((int)(elt_sz))*(change); \
  *_arr_ptr; })
#endif

#ifdef _INLINE_FUNCTIONS
static _INLINE struct _dyneither_ptr
_dyneither_ptr_inplace_plus_post(struct _dyneither_ptr *arr_ptr,unsigned elt_sz,int change) {
  struct _dyneither_ptr * _arr_ptr = (arr_ptr);
  struct _dyneither_ptr _ans = *_arr_ptr;
  _arr_ptr->curr += ((int)(elt_sz))*(change);
  return _ans;
}
#else
#define _dyneither_ptr_inplace_plus_post(arr_ptr,elt_sz,change) ({ \
  struct _dyneither_ptr * _arr_ptr = (arr_ptr); \
  struct _dyneither_ptr _ans = *_arr_ptr; \
  _arr_ptr->curr += ((int)(elt_sz))*(change); \
  _ans; })
#endif

/* Decrease the upper bound on a fat pointer by numelts where sz is
   the size of the pointer's type.  Note that this can't be a macro
   if we're to get initializers right. */
static struct 
_dyneither_ptr _dyneither_ptr_decrease_size(struct _dyneither_ptr x,
                                            unsigned int sz,
                                            unsigned int numelts) {
  x.last_plus_one -= sz * numelts; 
  return x; 
}

/* Allocation */
extern void* GC_malloc(int);
extern void* GC_malloc_atomic(int);
extern void* GC_calloc(unsigned,unsigned);
extern void* GC_calloc_atomic(unsigned,unsigned);

static _INLINE void* _cycalloc(int n) {
  void * ans = (void *)GC_malloc(n);
  if(!ans)
    _throw_badalloc();
  return ans;
}
static _INLINE void* _cycalloc_atomic(int n) {
  void * ans = (void *)GC_malloc_atomic(n);
  if(!ans)
    _throw_badalloc();
  return ans;
}
static _INLINE void* _cyccalloc(unsigned n, unsigned s) {
  void* ans = (void*)GC_calloc(n,s);
  if (!ans)
    _throw_badalloc();
  return ans;
}
static _INLINE void* _cyccalloc_atomic(unsigned n, unsigned s) {
  void* ans = (void*)GC_calloc_atomic(n,s);
  if (!ans)
    _throw_badalloc();
  return ans;
}
#define MAX_MALLOC_SIZE (1 << 28)
static _INLINE unsigned int _check_times(unsigned x, unsigned y) {
  _CYC_U_LONG_LONG_T whole_ans = 
    ((_CYC_U_LONG_LONG_T)x)*((_CYC_U_LONG_LONG_T)y);
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

/* the next two routines swap [x] and [y]; not thread safe! */
static _INLINE void _swap_word(void *x, void *y) {
  unsigned long *lx = (unsigned long *)x, *ly = (unsigned long *)y, tmp;
  tmp = *lx;
  *lx = *ly;
  *ly = tmp;
}
static _INLINE void _swap_dyneither(struct _dyneither_ptr *x, 
				   struct _dyneither_ptr *y) {
  struct _dyneither_ptr tmp = *x;
  *x = *y;
  *y = tmp;
}
 struct Cyc_Core_NewRegion{struct _DynRegionHandle*dynregion;};struct Cyc___cycFILE;
extern struct Cyc___cycFILE*Cyc_stdin;struct Cyc_String_pa_struct{int tag;struct
_dyneither_ptr f1;};struct Cyc_Int_pa_struct{int tag;unsigned long f1;};struct Cyc_Double_pa_struct{
int tag;double f1;};struct Cyc_LongDouble_pa_struct{int tag;long double f1;};struct
Cyc_ShortPtr_pa_struct{int tag;short*f1;};struct Cyc_IntPtr_pa_struct{int tag;
unsigned long*f1;};int Cyc_fgetc(struct Cyc___cycFILE*);struct Cyc_ShortPtr_sa_struct{
int tag;short*f1;};struct Cyc_UShortPtr_sa_struct{int tag;unsigned short*f1;};
struct Cyc_IntPtr_sa_struct{int tag;int*f1;};struct Cyc_UIntPtr_sa_struct{int tag;
unsigned int*f1;};struct Cyc_StringPtr_sa_struct{int tag;struct _dyneither_ptr f1;};
struct Cyc_DoublePtr_sa_struct{int tag;double*f1;};struct Cyc_FloatPtr_sa_struct{
int tag;float*f1;};struct Cyc_CharPtr_sa_struct{int tag;struct _dyneither_ptr f1;};
int Cyc_getc(struct Cyc___cycFILE*);int Cyc_sscanf(struct _dyneither_ptr,struct
_dyneither_ptr,struct _dyneither_ptr);int Cyc_ungetc(int,struct Cyc___cycFILE*);int
Cyc_vsscanf(struct _dyneither_ptr,struct _dyneither_ptr,struct _dyneither_ptr);
extern char Cyc_FileCloseError[15];struct Cyc_FileCloseError_struct{char*tag;};
extern char Cyc_FileOpenError[14];struct Cyc_FileOpenError_struct{char*tag;struct
_dyneither_ptr f1;};int isspace(int);int isupper(int);double atof(const char*);long
strtol(char*,char**,int);unsigned long strtoul(char*,char**,int);struct Cyc_Core_Opt{
void*v;};extern char Cyc_Core_Invalid_argument[17];struct Cyc_Core_Invalid_argument_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Failure[8];struct Cyc_Core_Failure_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Impossible[11];struct Cyc_Core_Impossible_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Not_found[10];struct Cyc_Core_Not_found_struct{
char*tag;};extern char Cyc_Core_Unreachable[12];struct Cyc_Core_Unreachable_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Open_Region[12];struct Cyc_Core_Open_Region_struct{
char*tag;};extern char Cyc_Core_Free_Region[12];struct Cyc_Core_Free_Region_struct{
char*tag;};static struct _dyneither_ptr Cyc___sccl(char*tab,struct _dyneither_ptr fmt);
static short*Cyc_va_arg_short_ptr(void*a);static short*Cyc_va_arg_short_ptr(void*a){
void*_tmp0=a;short*_tmp2;unsigned short*_tmp4;_LL1: {struct Cyc_ShortPtr_sa_struct*
_tmp1=(struct Cyc_ShortPtr_sa_struct*)_tmp0;if(_tmp1->tag != 0)goto _LL3;else{_tmp2=
_tmp1->f1;}}_LL2: return _tmp2;_LL3: {struct Cyc_UShortPtr_sa_struct*_tmp3=(struct
Cyc_UShortPtr_sa_struct*)_tmp0;if(_tmp3->tag != 1)goto _LL5;else{_tmp4=_tmp3->f1;}}
_LL4: return(short*)_tmp4;_LL5:;_LL6: {struct Cyc_Core_Invalid_argument_struct
_tmp49;const char*_tmp48;struct Cyc_Core_Invalid_argument_struct*_tmp47;(int)
_throw((void*)((_tmp47=_cycalloc(sizeof(*_tmp47)),((_tmp47[0]=((_tmp49.tag=Cyc_Core_Invalid_argument,((
_tmp49.f1=((_tmp48="scan expects short pointer",_tag_dyneither(_tmp48,sizeof(
char),27))),_tmp49)))),_tmp47)))));}_LL0:;}static int*Cyc_va_arg_int_ptr(void*a);
static int*Cyc_va_arg_int_ptr(void*a){void*_tmp8=a;int*_tmpA;unsigned int*_tmpC;
_LL8: {struct Cyc_IntPtr_sa_struct*_tmp9=(struct Cyc_IntPtr_sa_struct*)_tmp8;if(
_tmp9->tag != 2)goto _LLA;else{_tmpA=_tmp9->f1;}}_LL9: return _tmpA;_LLA: {struct Cyc_UIntPtr_sa_struct*
_tmpB=(struct Cyc_UIntPtr_sa_struct*)_tmp8;if(_tmpB->tag != 3)goto _LLC;else{_tmpC=
_tmpB->f1;}}_LLB: return(int*)_tmpC;_LLC:;_LLD: {struct Cyc_Core_Invalid_argument_struct
_tmp4F;const char*_tmp4E;struct Cyc_Core_Invalid_argument_struct*_tmp4D;(int)
_throw((void*)((_tmp4D=_cycalloc(sizeof(*_tmp4D)),((_tmp4D[0]=((_tmp4F.tag=Cyc_Core_Invalid_argument,((
_tmp4F.f1=((_tmp4E="scan expects int pointer",_tag_dyneither(_tmp4E,sizeof(char),
25))),_tmp4F)))),_tmp4D)))));}_LL7:;}static struct _dyneither_ptr Cyc_va_arg_string_ptr(
void*a);static struct _dyneither_ptr Cyc_va_arg_string_ptr(void*a){void*_tmp10=a;
struct _dyneither_ptr _tmp12;struct _dyneither_ptr _tmp14;_LLF: {struct Cyc_StringPtr_sa_struct*
_tmp11=(struct Cyc_StringPtr_sa_struct*)_tmp10;if(_tmp11->tag != 4)goto _LL11;else{
_tmp12=_tmp11->f1;}}_LL10: return _dyneither_ptr_decrease_size(_tmp12,sizeof(char),
1);_LL11: {struct Cyc_CharPtr_sa_struct*_tmp13=(struct Cyc_CharPtr_sa_struct*)
_tmp10;if(_tmp13->tag != 7)goto _LL13;else{_tmp14=_tmp13->f1;}}_LL12: return _tmp14;
_LL13:;_LL14: {struct Cyc_Core_Invalid_argument_struct _tmp55;const char*_tmp54;
struct Cyc_Core_Invalid_argument_struct*_tmp53;(int)_throw((void*)((_tmp53=
_cycalloc(sizeof(*_tmp53)),((_tmp53[0]=((_tmp55.tag=Cyc_Core_Invalid_argument,((
_tmp55.f1=((_tmp54="scan expects char pointer",_tag_dyneither(_tmp54,sizeof(char),
26))),_tmp55)))),_tmp53)))));}_LLE:;}static double*Cyc_va_arg_double_ptr(void*a);
static double*Cyc_va_arg_double_ptr(void*a){void*_tmp18=a;double*_tmp1A;_LL16: {
struct Cyc_DoublePtr_sa_struct*_tmp19=(struct Cyc_DoublePtr_sa_struct*)_tmp18;if(
_tmp19->tag != 5)goto _LL18;else{_tmp1A=_tmp19->f1;}}_LL17: return _tmp1A;_LL18:;
_LL19: {struct Cyc_Core_Invalid_argument_struct _tmp5B;const char*_tmp5A;struct Cyc_Core_Invalid_argument_struct*
_tmp59;(int)_throw((void*)((_tmp59=_cycalloc(sizeof(*_tmp59)),((_tmp59[0]=((
_tmp5B.tag=Cyc_Core_Invalid_argument,((_tmp5B.f1=((_tmp5A="scan expects double pointer",
_tag_dyneither(_tmp5A,sizeof(char),28))),_tmp5B)))),_tmp59)))));}_LL15:;}static
float*Cyc_va_arg_float_ptr(void*a);static float*Cyc_va_arg_float_ptr(void*a){void*
_tmp1E=a;float*_tmp20;_LL1B: {struct Cyc_FloatPtr_sa_struct*_tmp1F=(struct Cyc_FloatPtr_sa_struct*)
_tmp1E;if(_tmp1F->tag != 6)goto _LL1D;else{_tmp20=_tmp1F->f1;}}_LL1C: return _tmp20;
_LL1D:;_LL1E: {struct Cyc_Core_Invalid_argument_struct _tmp61;const char*_tmp60;
struct Cyc_Core_Invalid_argument_struct*_tmp5F;(int)_throw((void*)((_tmp5F=
_cycalloc(sizeof(*_tmp5F)),((_tmp5F[0]=((_tmp61.tag=Cyc_Core_Invalid_argument,((
_tmp61.f1=((_tmp60="scan expects float pointer",_tag_dyneither(_tmp60,sizeof(
char),27))),_tmp61)))),_tmp5F)))));}_LL1A:;}static struct _dyneither_ptr Cyc_va_arg_char_ptr(
void*a);static struct _dyneither_ptr Cyc_va_arg_char_ptr(void*a){void*_tmp24=a;
struct _dyneither_ptr _tmp26;struct _dyneither_ptr _tmp28;_LL20: {struct Cyc_CharPtr_sa_struct*
_tmp25=(struct Cyc_CharPtr_sa_struct*)_tmp24;if(_tmp25->tag != 7)goto _LL22;else{
_tmp26=_tmp25->f1;}}_LL21: return _tmp26;_LL22: {struct Cyc_StringPtr_sa_struct*
_tmp27=(struct Cyc_StringPtr_sa_struct*)_tmp24;if(_tmp27->tag != 4)goto _LL24;else{
_tmp28=_tmp27->f1;}}_LL23: return _dyneither_ptr_decrease_size(_tmp28,sizeof(char),
1);_LL24:;_LL25: {struct Cyc_Core_Invalid_argument_struct _tmp67;const char*_tmp66;
struct Cyc_Core_Invalid_argument_struct*_tmp65;(int)_throw((void*)((_tmp65=
_cycalloc(sizeof(*_tmp65)),((_tmp65[0]=((_tmp67.tag=Cyc_Core_Invalid_argument,((
_tmp67.f1=((_tmp66="scan expects char pointer",_tag_dyneither(_tmp66,sizeof(char),
26))),_tmp67)))),_tmp65)))));}_LL1F:;}int Cyc__IO_vfscanf(int(*_IO_getc)(void*),
int(*_IO_ungetc)(int,void*),int(*_IO_peekc)(void*),void*fp,struct _dyneither_ptr
fmt0,struct _dyneither_ptr ap,int*errp);int Cyc__IO_vfscanf(int(*_IO_getc)(void*),
int(*_IO_ungetc)(int,void*),int(*_IO_peekc)(void*),void*fp,struct _dyneither_ptr
fmt0,struct _dyneither_ptr ap,int*errp){struct _dyneither_ptr fmt=fmt0;int c;
long long width;struct _dyneither_ptr p=_tag_dyneither(0,0,0);int n;int flags=0;
struct _dyneither_ptr p0=_tag_dyneither(0,0,0);int nassigned;int nread;int base=0;int
use_strtoul=0;char ccltab[256];char buf[351];{unsigned int _tmp43=350;unsigned int i;
for(i=0;i < _tmp43;i ++){buf[i]='0';}buf[_tmp43]=(char)0;}{int seen_eof=0;static
short basefix[17]={10,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};nassigned=0;nread=0;
for(0;1;0){c=(int)*((const char*)_check_dyneither_subscript(
_dyneither_ptr_inplace_plus_post(& fmt,sizeof(char),1),sizeof(char),0));if(c == 0)
goto done;if(isspace(c)){for(0;1;0){c=_IO_getc(fp);if(c == - 1){++ seen_eof;break;}
if(!isspace(c)){_IO_ungetc(c,fp);break;}++ nread;}continue;}if(c != '%')goto
literal;width=(long long)0;flags=0;again: c=(int)*((const char*)
_check_dyneither_subscript(_dyneither_ptr_inplace_plus_post(& fmt,sizeof(char),1),
sizeof(char),0));switch(c){case '%': _LL26: literal: n=_IO_getc(fp);if(n == - 1)goto
eof_failure;if(n != c){_IO_ungetc(n,fp);goto match_failure;}++ nread;continue;case '*':
_LL27: if(flags)goto control_failure;flags=8;goto again;case 'l': _LL28: if(flags & ~(8
| 64))goto control_failure;flags |=1;goto again;case 'L': _LL29: if(flags & ~(8 | 64))
goto control_failure;flags |=2;goto again;case 'h': _LL2A: if(flags & ~(8 | 64))goto
control_failure;flags |=4;goto again;case '0': _LL2B: goto _LL2C;case '1': _LL2C: goto
_LL2D;case '2': _LL2D: goto _LL2E;case '3': _LL2E: goto _LL2F;case '4': _LL2F: goto _LL30;
case '5': _LL30: goto _LL31;case '6': _LL31: goto _LL32;case '7': _LL32: goto _LL33;case '8':
_LL33: goto _LL34;case '9': _LL34: if(flags & ~(8 | 64))goto control_failure;flags |=64;
width=(width * 10 + c)- '0';goto again;case 'D': _LL35: flags |=1;goto _LL36;case 'd':
_LL36: c=3;use_strtoul=0;base=10;break;case 'i': _LL37: c=3;use_strtoul=0;base=0;
break;case 'O': _LL38: flags |=1;goto _LL39;case 'o': _LL39: c=3;use_strtoul=1;base=8;
break;case 'u': _LL3A: c=3;use_strtoul=1;base=10;break;case 'X': _LL3B: goto _LL3C;case
'x': _LL3C: flags |=256;c=3;use_strtoul=1;base=16;break;case 'E': _LL3D: goto _LL3E;
case 'F': _LL3E: goto _LL3F;case 'e': _LL3F: goto _LL40;case 'f': _LL40: goto _LL41;case 'g':
_LL41: c=4;break;case 's': _LL42: c=2;break;case '[': _LL43: fmt=Cyc___sccl(ccltab,fmt);
flags |=32;c=1;break;case 'c': _LL44: flags |=32;c=0;break;case 'p': _LL45: flags |=16 | 
256;c=3;use_strtoul=1;base=16;break;case 'n': _LL46: if(flags & 8)continue;if(flags & 
4)*Cyc_va_arg_short_ptr(*((void**)_check_dyneither_subscript(ap,sizeof(void*),0)))=(
short)nread;else{if(flags & 1)*Cyc_va_arg_int_ptr(*((void**)
_check_dyneither_subscript(ap,sizeof(void*),0)))=(long)nread;else{*Cyc_va_arg_int_ptr(*((
void**)_check_dyneither_subscript(ap,sizeof(void*),0)))=(int)nread;}}
_dyneither_ptr_inplace_plus(& ap,sizeof(void*),1);continue;case '\000': _LL47:
nassigned=- 1;goto done;default: _LL48: if(isupper(c))flags |=1;c=3;use_strtoul=0;
base=10;break;}if(_IO_peekc(fp)== - 1)goto eof_failure;if((flags & 32)== 0){n=
_IO_peekc(fp);while(isspace(n)){n=_IO_getc(fp);++ nread;n=_IO_peekc(fp);if(n == - 1)
goto eof_failure;}}switch(c){case 0: _LL4A: if(width == 0)width=(long long)1;if(flags
& 8){long long sum=(long long)0;for(0;width > 0;0){n=_IO_getc(fp);if(n == - 1  && 
width != 0)goto eof_failure;else{if(n == - 1){++ seen_eof;break;}}++ sum;-- width;}
nread +=sum;}else{long long sum=(long long)0;struct _dyneither_ptr _tmp2C=Cyc_va_arg_char_ptr(*((
void**)_check_dyneither_subscript(ap,sizeof(void*),0)));
_dyneither_ptr_inplace_plus(& ap,sizeof(void*),1);for(0;width > 0;0){n=_IO_getc(fp);
if(n == - 1  && width != 0)goto eof_failure;else{if(n == - 1){++ seen_eof;break;}}*((
char*)_check_dyneither_subscript(_tmp2C,sizeof(char),0))=(char)n;
_dyneither_ptr_inplace_plus(& _tmp2C,sizeof(char),1);++ sum;-- width;}nread +=sum;++
nassigned;}break;case 1: _LL4B: if(width == 0)width=(long long)~ 0;if(flags & 8){n=0;{
int c=_IO_peekc(fp);while((int)ccltab[_check_known_subscript_notnull(256,(int)((
char)c))]){++ n;_IO_getc(fp);if(-- width == 0)break;if((c=_IO_peekc(fp))== - 1){if(n
== 0)goto eof_failure;++ seen_eof;break;}}if(n == 0)goto match_failure;};}else{
struct _dyneither_ptr p4=(struct _dyneither_ptr)Cyc_va_arg_string_ptr(*((void**)
_check_dyneither_subscript(ap,sizeof(void*),0)));_dyneither_ptr_inplace_plus(& ap,
sizeof(void*),1);{struct _dyneither_ptr p5=p4;int c=_IO_peekc(fp);while((int)ccltab[
_check_known_subscript_notnull(256,(int)((char)c))]){if(_get_dyneither_size(p5,
sizeof(char))== 0)goto eof_failure;*((char*)_check_dyneither_subscript(p5,sizeof(
char),0))=(char)c;_dyneither_ptr_inplace_plus(& p5,sizeof(char),1);_IO_getc(fp);
if(-- width == 0)break;if((c=_IO_peekc(fp))== - 1){if(p5.curr == p0.curr)goto
eof_failure;++ seen_eof;break;}}n=(p5.curr - p4.curr)/ sizeof(char);if(n == 0)goto
match_failure;if(_get_dyneither_size(p5,sizeof(char))== 0)goto eof_failure;*((
char*)_check_dyneither_subscript(p5,sizeof(char),0))='\000';++ nassigned;};}nread
+=n;break;case 2: _LL4C: if(width == 0)width=(long long)~ 0;if(flags & 8){n=0;{int c=
_IO_peekc(fp);while(!isspace((int)((unsigned char)c))){++ n;_IO_getc(fp);if(--
width == 0)break;if((c=_IO_peekc(fp))== - 1){++ seen_eof;break;}}nread +=n;};}else{
struct _dyneither_ptr _tmp2D=Cyc_va_arg_string_ptr(*((void**)
_check_dyneither_subscript(ap,sizeof(void*),0)));_dyneither_ptr_inplace_plus(& ap,
sizeof(void*),1);{struct _dyneither_ptr _tmp2E=_tmp2D;int c=_IO_peekc(fp);while(!
isspace((int)((unsigned char)c))){c=_IO_getc(fp);if(_get_dyneither_size(_tmp2E,
sizeof(char))== 0)goto eof_failure;*((char*)_check_dyneither_subscript(_tmp2E,
sizeof(char),0))=(char)c;_dyneither_ptr_inplace_plus(& _tmp2E,sizeof(char),1);if(
-- width == 0)break;if((c=_IO_peekc(fp))== - 1){++ seen_eof;break;}}if(
_get_dyneither_size(_tmp2E,sizeof(char))== 0)goto eof_failure;*((char*)
_check_dyneither_subscript(_tmp2E,sizeof(char),0))='\000';nread +=(_tmp2E.curr - 
_tmp2D.curr)/ sizeof(char);++ nassigned;};}continue;case 3: _LL4D: if(width == 0  || 
width > sizeof(buf)- 1)width=(long long)(sizeof(buf)- 1);flags |=(64 | 128)| 512;{
char*_tmp68;for(p=((_tmp68=buf,_tag_dyneither(_tmp68,sizeof(char),
_get_zero_arr_size_char(_tmp68,351))));width != 0;-- width){c=(int)((unsigned char)
_IO_peekc(fp));switch(c){case '0': _LL4F: if(base == 0){base=8;flags |=256;}if(flags & 
512)flags &=~((64 | 512)| 128);else{flags &=~((64 | 256)| 128);}goto ok;case '1': _LL50:
goto _LL51;case '2': _LL51: goto _LL52;case '3': _LL52: goto _LL53;case '4': _LL53: goto
_LL54;case '5': _LL54: goto _LL55;case '6': _LL55: goto _LL56;case '7': _LL56: base=(int)
basefix[_check_known_subscript_notnull(17,base)];flags &=~((64 | 256)| 128);goto ok;
case '8': _LL57: goto _LL58;case '9': _LL58: base=(int)basefix[
_check_known_subscript_notnull(17,base)];if(base <= 8)break;flags &=~((64 | 256)| 
128);goto ok;case 'A': _LL59: goto _LL5A;case 'B': _LL5A: goto _LL5B;case 'C': _LL5B: goto
_LL5C;case 'D': _LL5C: goto _LL5D;case 'E': _LL5D: goto _LL5E;case 'F': _LL5E: goto _LL5F;
case 'a': _LL5F: goto _LL60;case 'b': _LL60: goto _LL61;case 'c': _LL61: goto _LL62;case 'd':
_LL62: goto _LL63;case 'e': _LL63: goto _LL64;case 'f': _LL64: if(base <= 10)break;flags &=
~((64 | 256)| 128);goto ok;case '+': _LL65: goto _LL66;case '-': _LL66: if(flags & 64){
flags &=~ 64;goto ok;}break;case 'x': _LL67: goto _LL68;case 'X': _LL68:{char*_tmp69;if(
flags & 256  && p.curr == (_dyneither_ptr_plus(((_tmp69=buf,_tag_dyneither(_tmp69,
sizeof(char),_get_zero_arr_size_char(_tmp69,351)))),sizeof(char),1)).curr){base=
16;flags &=~ 256;goto ok;}}break;default: _LL69: break;}break;ok: {char _tmp6C;char
_tmp6B;struct _dyneither_ptr _tmp6A;(_tmp6A=_dyneither_ptr_inplace_plus_post(& p,
sizeof(char),1),((_tmp6B=*((char*)_check_dyneither_subscript(_tmp6A,sizeof(char),
0)),((_tmp6C=(char)c,((_get_dyneither_size(_tmp6A,sizeof(char))== 1  && (_tmp6B == '\000'
 && _tmp6C != '\000')?_throw_arraybounds(): 1,*((char*)_tmp6A.curr)=_tmp6C)))))));}
_IO_getc(fp);if(_IO_peekc(fp)== - 1){++ seen_eof;break;}}}if(flags & 128){if(p.curr
> buf){_dyneither_ptr_inplace_plus(& p,sizeof(char),-1);_IO_ungetc((int)*((char*)
_check_dyneither_subscript(p,sizeof(char),0)),fp);}goto match_failure;}c=(int)*((
char*)_check_dyneither_subscript(p,sizeof(char),- 1));if(c == 'x'  || c == 'X'){
_dyneither_ptr_inplace_plus(& p,sizeof(char),-1);_IO_ungetc(c,fp);}if((flags & 8)
== 0){unsigned long res;{char _tmp6F;char _tmp6E;struct _dyneither_ptr _tmp6D;(_tmp6D=
p,((_tmp6E=*((char*)_check_dyneither_subscript(_tmp6D,sizeof(char),0)),((_tmp6F='\000',((
_get_dyneither_size(_tmp6D,sizeof(char))== 1  && (_tmp6E == '\000'  && _tmp6F != '\000')?
_throw_arraybounds(): 1,*((char*)_tmp6D.curr)=_tmp6F)))))));}if(use_strtoul)res=
strtoul((char*)buf,0,base);else{res=(unsigned long)strtol((char*)buf,0,base);}
if(flags & 16)*Cyc_va_arg_int_ptr(*((void**)_check_dyneither_subscript(ap,sizeof(
void*),0)))=(int)res;else{if(flags & 4)*Cyc_va_arg_short_ptr(*((void**)
_check_dyneither_subscript(ap,sizeof(void*),0)))=(short)res;else{if(flags & 1)*
Cyc_va_arg_int_ptr(*((void**)_check_dyneither_subscript(ap,sizeof(void*),0)))=(
int)res;else{*Cyc_va_arg_int_ptr(*((void**)_check_dyneither_subscript(ap,sizeof(
void*),0)))=(int)res;}}}_dyneither_ptr_inplace_plus(& ap,sizeof(void*),1);++
nassigned;}{char*_tmp70;nread +=(p.curr - ((_tmp70=buf,_tag_dyneither(_tmp70,
sizeof(char),_get_zero_arr_size_char(_tmp70,351)))).curr)/ sizeof(char);}break;
case 4: _LL4E: if(width == 0  || width > sizeof(buf)- 1)width=(long long)(sizeof(buf)- 
1);flags |=((64 | 128)| 256)| 512;{char*_tmp71;for(p=((_tmp71=buf,_tag_dyneither(
_tmp71,sizeof(char),_get_zero_arr_size_char(_tmp71,351))));width != 0;-- width){c=
_IO_peekc(fp);switch(c){case '0': _LL6C: goto _LL6D;case '1': _LL6D: goto _LL6E;case '2':
_LL6E: goto _LL6F;case '3': _LL6F: goto _LL70;case '4': _LL70: goto _LL71;case '5': _LL71:
goto _LL72;case '6': _LL72: goto _LL73;case '7': _LL73: goto _LL74;case '8': _LL74: goto
_LL75;case '9': _LL75: flags &=~(64 | 128);goto fok;case '+': _LL76: goto _LL77;case '-':
_LL77: if(flags & 64){flags &=~ 64;goto fok;}break;case '.': _LL78: if(flags & 256){flags
&=~(64 | 256);goto fok;}break;case 'e': _LL79: goto _LL7A;case 'E': _LL7A: if((flags & (
128 | 512))== 512){flags=(flags & ~(512 | 256)| 64)| 128;goto fok;}break;default:
_LL7B: break;}break;fok: {char _tmp74;char _tmp73;struct _dyneither_ptr _tmp72;(
_tmp72=_dyneither_ptr_inplace_plus_post(& p,sizeof(char),1),((_tmp73=*((char*)
_check_dyneither_subscript(_tmp72,sizeof(char),0)),((_tmp74=(char)c,((
_get_dyneither_size(_tmp72,sizeof(char))== 1  && (_tmp73 == '\000'  && _tmp74 != '\000')?
_throw_arraybounds(): 1,*((char*)_tmp72.curr)=_tmp74)))))));}_IO_getc(fp);if(
_IO_peekc(fp)== - 1){++ seen_eof;break;}}}if(flags & 128){if(flags & 512){while(p.curr
> buf){_dyneither_ptr_inplace_plus(& p,sizeof(char),-1);_IO_ungetc((int)*((char*)
_check_dyneither_subscript(p,sizeof(char),0)),fp);}goto match_failure;}
_dyneither_ptr_inplace_plus(& p,sizeof(char),-1);c=(int)*((char*)
_check_dyneither_subscript(p,sizeof(char),0));if(c != 'e'  && c != 'E'){_IO_ungetc(
c,fp);_dyneither_ptr_inplace_plus(& p,sizeof(char),-1);c=(int)*((char*)
_check_dyneither_subscript(p,sizeof(char),0));}_IO_ungetc(c,fp);}if((flags & 8)== 
0){double res;{char _tmp77;char _tmp76;struct _dyneither_ptr _tmp75;(_tmp75=p,((
_tmp76=*((char*)_check_dyneither_subscript(_tmp75,sizeof(char),0)),((_tmp77='\000',((
_get_dyneither_size(_tmp75,sizeof(char))== 1  && (_tmp76 == '\000'  && _tmp77 != '\000')?
_throw_arraybounds(): 1,*((char*)_tmp75.curr)=_tmp77)))))));}res=atof((const char*)
buf);if(flags & 1)*Cyc_va_arg_double_ptr(*((void**)_check_dyneither_subscript(ap,
sizeof(void*),0)))=res;else{*Cyc_va_arg_float_ptr(*((void**)
_check_dyneither_subscript(ap,sizeof(void*),0)))=(float)res;}
_dyneither_ptr_inplace_plus(& ap,sizeof(void*),1);++ nassigned;}{char*_tmp78;nread
+=(p.curr - ((_tmp78=buf,_tag_dyneither(_tmp78,sizeof(char),
_get_zero_arr_size_char(_tmp78,351)))).curr)/ sizeof(char);}break;default: _LL6B: {
struct Cyc_Core_Impossible_struct _tmp7E;const char*_tmp7D;struct Cyc_Core_Impossible_struct*
_tmp7C;(int)_throw((void*)((_tmp7C=_cycalloc(sizeof(*_tmp7C)),((_tmp7C[0]=((
_tmp7E.tag=Cyc_Core_Impossible,((_tmp7E.f1=((_tmp7D="scanf3",_tag_dyneither(
_tmp7D,sizeof(char),7))),_tmp7E)))),_tmp7C)))));}}}eof_failure: ++ seen_eof;
input_failure: if(nassigned == 0)nassigned=- 1;control_failure: match_failure: if((
unsigned int)errp)*errp |=2;done: if((unsigned int)errp  && seen_eof)*errp |=1;
return nassigned;};}static struct _dyneither_ptr Cyc___sccl(char*tab,struct
_dyneither_ptr fmt);static struct _dyneither_ptr Cyc___sccl(char*tab,struct
_dyneither_ptr fmt){int c;int n;int v;c=(int)*((const char*)
_check_dyneither_subscript(_dyneither_ptr_inplace_plus_post(& fmt,sizeof(char),1),
sizeof(char),0));if(c == '^'){v=1;c=(int)*((const char*)_check_dyneither_subscript(
_dyneither_ptr_inplace_plus_post(& fmt,sizeof(char),1),sizeof(char),0));}else{v=0;}
for(n=0;n < 256;++ n){tab[_check_known_subscript_notnull(256,n)]=(char)v;}if(c == 0)
return _dyneither_ptr_plus(fmt,sizeof(char),- 1);v=1 - v;for(0;1;0){tab[
_check_known_subscript_notnull(256,c)]=(char)v;doswitch: n=(int)*((const char*)
_check_dyneither_subscript(_dyneither_ptr_inplace_plus_post(& fmt,sizeof(char),1),
sizeof(char),0));switch(n){case 0: _LL7E: return _dyneither_ptr_plus(fmt,sizeof(char),
- 1);case '-': _LL7F: n=(int)*((const char*)_check_dyneither_subscript(fmt,sizeof(
char),0));if(n == ']'  || n < c){c=(int)'-';break;}_dyneither_ptr_inplace_plus(& fmt,
sizeof(char),1);do{tab[_check_known_subscript_notnull(256,++ c)]=(char)v;}while(c
< n);goto doswitch;break;case ']': _LL80: return fmt;default: _LL81: c=n;break;}}}
static int Cyc_string_getc(struct _dyneither_ptr*sptr);static int Cyc_string_getc(
struct _dyneither_ptr*sptr){char c;struct _dyneither_ptr s=*sptr;if((s.curr == (
_tag_dyneither(0,0,0)).curr  || _get_dyneither_size(s,sizeof(char))== 0) || (c=*((
const char*)_check_dyneither_subscript(s,sizeof(char),0)))== '\000')return - 1;*
sptr=_dyneither_ptr_plus(s,sizeof(char),1);return(int)c;}static int Cyc_string_ungetc(
int ignore,struct _dyneither_ptr*sptr);static int Cyc_string_ungetc(int ignore,struct
_dyneither_ptr*sptr){*sptr=_dyneither_ptr_plus(*sptr,sizeof(char),- 1);return 0;}
static int Cyc_string_peekc(struct _dyneither_ptr*sptr);static int Cyc_string_peekc(
struct _dyneither_ptr*sptr){char c;struct _dyneither_ptr s=*sptr;if((s.curr == (
_tag_dyneither(0,0,0)).curr  || _get_dyneither_size(s,sizeof(char))== 0) || (c=*((
const char*)_check_dyneither_subscript(s,sizeof(char),0)))== '\000')return - 1;
return(int)c;}int Cyc_vsscanf(struct _dyneither_ptr src1,struct _dyneither_ptr fmt,
struct _dyneither_ptr ap);int Cyc_vsscanf(struct _dyneither_ptr src1,struct
_dyneither_ptr fmt,struct _dyneither_ptr ap){struct _dyneither_ptr src=(struct
_dyneither_ptr)src1;int err=0;return((int(*)(int(*_IO_getc)(struct _dyneither_ptr*),
int(*_IO_ungetc)(int,struct _dyneither_ptr*),int(*_IO_peekc)(struct _dyneither_ptr*),
struct _dyneither_ptr*fp,struct _dyneither_ptr fmt0,struct _dyneither_ptr ap,int*errp))
Cyc__IO_vfscanf)(Cyc_string_getc,Cyc_string_ungetc,Cyc_string_peekc,& src,fmt,ap,(
int*)& err);}int Cyc_sscanf(struct _dyneither_ptr src,struct _dyneither_ptr fmt,struct
_dyneither_ptr ap);int Cyc_sscanf(struct _dyneither_ptr src,struct _dyneither_ptr fmt,
struct _dyneither_ptr ap){return Cyc_vsscanf(src,fmt,ap);}int Cyc_peekc(struct Cyc___cycFILE*
stream);int Cyc_peekc(struct Cyc___cycFILE*stream){int c=Cyc_fgetc(stream);Cyc_ungetc(
c,stream);return c;}int Cyc_vfscanf(struct Cyc___cycFILE*stream,struct
_dyneither_ptr fmt,struct _dyneither_ptr ap);int Cyc_vfscanf(struct Cyc___cycFILE*
stream,struct _dyneither_ptr fmt,struct _dyneither_ptr ap){int err=0;return((int(*)(
int(*_IO_getc)(struct Cyc___cycFILE*),int(*_IO_ungetc)(int,struct Cyc___cycFILE*),
int(*_IO_peekc)(struct Cyc___cycFILE*),struct Cyc___cycFILE*fp,struct
_dyneither_ptr fmt0,struct _dyneither_ptr ap,int*errp))Cyc__IO_vfscanf)(Cyc_getc,
Cyc_ungetc,Cyc_peekc,stream,fmt,ap,(int*)& err);}int Cyc_fscanf(struct Cyc___cycFILE*
stream,struct _dyneither_ptr fmt,struct _dyneither_ptr ap);int Cyc_fscanf(struct Cyc___cycFILE*
stream,struct _dyneither_ptr fmt,struct _dyneither_ptr ap){return Cyc_vfscanf(stream,
fmt,ap);}int Cyc_scanf(struct _dyneither_ptr fmt,struct _dyneither_ptr ap);int Cyc_scanf(
struct _dyneither_ptr fmt,struct _dyneither_ptr ap){return Cyc_vfscanf(Cyc_stdin,fmt,
ap);}
