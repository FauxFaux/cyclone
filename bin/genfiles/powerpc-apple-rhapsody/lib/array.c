#ifndef _SETJMP_H_
#define _SETJMP_H_
#ifndef _jmp_buf_def_
#define _jmp_buf_def_
typedef int jmp_buf[192];
#endif
extern int setjmp(jmp_buf);
#endif
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
 struct Cyc_Core_NewRegion{struct _DynRegionHandle*dynregion;};struct Cyc_Core_Opt{
void*v;};extern char Cyc_Core_Invalid_argument[17];struct Cyc_Core_Invalid_argument_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Failure[8];struct Cyc_Core_Failure_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Impossible[11];struct Cyc_Core_Impossible_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Not_found[10];struct Cyc_Core_Not_found_struct{
char*tag;};extern char Cyc_Core_Unreachable[12];struct Cyc_Core_Unreachable_struct{
char*tag;struct _dyneither_ptr f1;};extern char Cyc_Core_Open_Region[12];struct Cyc_Core_Open_Region_struct{
char*tag;};extern char Cyc_Core_Free_Region[12];struct Cyc_Core_Free_Region_struct{
char*tag;};struct Cyc_List_List{void*hd;struct Cyc_List_List*tl;};extern char Cyc_List_List_mismatch[
14];struct Cyc_List_List_mismatch_struct{char*tag;};extern char Cyc_List_Nth[4];
struct Cyc_List_Nth_struct{char*tag;};struct _dyneither_ptr Cyc_List_to_array(
struct Cyc_List_List*x);struct Cyc_List_List*Cyc_List_from_array(struct
_dyneither_ptr arr);void Cyc_Array_qsort(int(*)(void**,void**),struct
_dyneither_ptr x,int len);void Cyc_Array_msort(int(*)(void**,void**),struct
_dyneither_ptr x,int len);struct _dyneither_ptr Cyc_Array_from_list(struct Cyc_List_List*
l);struct Cyc_List_List*Cyc_Array_to_list(struct _dyneither_ptr x);struct
_dyneither_ptr Cyc_Array_copy(struct _dyneither_ptr x);struct _dyneither_ptr Cyc_Array_map(
void*(*f)(void*),struct _dyneither_ptr x);struct _dyneither_ptr Cyc_Array_map_c(void*(*
f)(void*,void*),void*env,struct _dyneither_ptr x);void Cyc_Array_imp_map(void*(*f)(
void*),struct _dyneither_ptr x);void Cyc_Array_imp_map_c(void*(*f)(void*,void*),
void*env,struct _dyneither_ptr x);extern char Cyc_Array_Array_mismatch[15];struct Cyc_Array_Array_mismatch_struct{
char*tag;};struct _dyneither_ptr Cyc_Array_map2(void*(*f)(void*,void*),struct
_dyneither_ptr x,struct _dyneither_ptr y);void Cyc_Array_app(void*(*f)(void*),struct
_dyneither_ptr x);void Cyc_Array_app_c(void*(*f)(void*,void*),void*env,struct
_dyneither_ptr x);void Cyc_Array_iter(void(*f)(void*),struct _dyneither_ptr x);void
Cyc_Array_iter_c(void(*f)(void*,void*),void*env,struct _dyneither_ptr x);void Cyc_Array_app2(
void*(*f)(void*,void*),struct _dyneither_ptr x,struct _dyneither_ptr y);void Cyc_Array_app2_c(
void*(*f)(void*,void*,void*),void*env,struct _dyneither_ptr x,struct _dyneither_ptr
y);void Cyc_Array_iter2(void(*f)(void*,void*),struct _dyneither_ptr x,struct
_dyneither_ptr y);void Cyc_Array_iter2_c(void(*f)(void*,void*,void*),void*env,
struct _dyneither_ptr x,struct _dyneither_ptr y);void*Cyc_Array_fold_left(void*(*f)(
void*,void*),void*accum,struct _dyneither_ptr x);void*Cyc_Array_fold_left_c(void*(*
f)(void*,void*,void*),void*env,void*accum,struct _dyneither_ptr x);void*Cyc_Array_fold_right(
void*(*f)(void*,void*),struct _dyneither_ptr x,void*accum);void*Cyc_Array_fold_right_c(
void*(*f)(void*,void*,void*),void*env,struct _dyneither_ptr x,void*accum);struct
_dyneither_ptr Cyc_Array_rev_copy(struct _dyneither_ptr x);void Cyc_Array_imp_rev(
struct _dyneither_ptr x);int Cyc_Array_forall(int(*pred)(void*),struct
_dyneither_ptr x);int Cyc_Array_forall_c(int(*pred)(void*,void*),void*env,struct
_dyneither_ptr x);int Cyc_Array_exists(int(*pred)(void*),struct _dyneither_ptr x);
int Cyc_Array_exists_c(int(*pred)(void*,void*),void*env,struct _dyneither_ptr x);
struct _tuple0{void*f1;void*f2;};struct _dyneither_ptr Cyc_Array_zip(struct
_dyneither_ptr x,struct _dyneither_ptr y);struct _tuple1{struct _dyneither_ptr f1;
struct _dyneither_ptr f2;};struct _tuple1 Cyc_Array_split(struct _dyneither_ptr x);int
Cyc_Array_memq(struct _dyneither_ptr l,void*x);int Cyc_Array_mem(int(*cmp)(void*,
void*),struct _dyneither_ptr l,void*x);struct _dyneither_ptr Cyc_Array_extract(
struct _dyneither_ptr x,int start,int*len_opt);void Cyc_Array_qsort(int(*less_eq)(
void**,void**),struct _dyneither_ptr arr,int len);void Cyc_Array_qsort(int(*less_eq)(
void**,void**),struct _dyneither_ptr arr,int len){int base_ofs=0;void*temp;int sp[40];
int sp_ofs;int i;int j;int limit_ofs;if((base_ofs < 0  || base_ofs + len > 
_get_dyneither_size(arr,sizeof(void*))) || len < 0){struct Cyc_Core_Invalid_argument_struct
_tmp39;const char*_tmp38;struct Cyc_Core_Invalid_argument_struct*_tmp37;(int)
_throw((void*)((_tmp37=_cycalloc(sizeof(*_tmp37)),((_tmp37[0]=((_tmp39.tag=Cyc_Core_Invalid_argument,((
_tmp39.f1=((_tmp38="Array::qsort",_tag_dyneither(_tmp38,sizeof(char),13))),
_tmp39)))),_tmp37)))));}limit_ofs=base_ofs + len;sp_ofs=0;for(0;1;0){if(limit_ofs
- base_ofs > 3){temp=*((void**)_check_dyneither_subscript(arr,sizeof(void*),(
limit_ofs - base_ofs)/ 2 + base_ofs));*((void**)_check_dyneither_subscript(arr,
sizeof(void*),(limit_ofs - base_ofs)/ 2 + base_ofs))=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs))=temp;i=base_ofs + 1;j=
limit_ofs - 1;if(less_eq((void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,
sizeof(void*),i),sizeof(void*),1),(void**)_untag_dyneither_ptr(
_dyneither_ptr_plus(arr,sizeof(void*),j),sizeof(void*),1))> 0){temp=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),i));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),i))=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j))=temp;}if(less_eq((void**)
_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),base_ofs),sizeof(void*),
1),(void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),j),sizeof(
void*),1))> 0){temp=*((void**)_check_dyneither_subscript(arr,sizeof(void*),
base_ofs));*((void**)_check_dyneither_subscript(arr,sizeof(void*),base_ofs))=*((
void**)_check_dyneither_subscript(arr,sizeof(void*),j));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j))=temp;}if(less_eq((void**)
_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),i),sizeof(void*),1),(
void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),base_ofs),
sizeof(void*),1))> 0){temp=*((void**)_check_dyneither_subscript(arr,sizeof(void*),
i));*((void**)_check_dyneither_subscript(arr,sizeof(void*),i))=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs))=temp;}for(0;1;0){do{++ i;}while(
less_eq((void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),i),
sizeof(void*),1),(void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(
void*),base_ofs),sizeof(void*),1))< 0);do{-- j;}while(less_eq((void**)
_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),j),sizeof(void*),1),(
void**)_untag_dyneither_ptr(_dyneither_ptr_plus(arr,sizeof(void*),base_ofs),
sizeof(void*),1))> 0);if(i > j)break;temp=*((void**)_check_dyneither_subscript(
arr,sizeof(void*),i));*((void**)_check_dyneither_subscript(arr,sizeof(void*),i))=*((
void**)_check_dyneither_subscript(arr,sizeof(void*),j));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j))=temp;}temp=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),base_ofs))=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j))=temp;if(j - base_ofs > limit_ofs - 
i){sp[_check_known_subscript_notnull(40,sp_ofs)]=base_ofs;sp[
_check_known_subscript_notnull(40,sp_ofs + 1)]=j;base_ofs=i;}else{sp[
_check_known_subscript_notnull(40,sp_ofs)]=i;sp[_check_known_subscript_notnull(
40,sp_ofs + 1)]=limit_ofs;limit_ofs=j;}sp_ofs +=2;}else{for((j=base_ofs,i=j + 1);i < 
limit_ofs;(j=i,i ++)){for(0;less_eq((void**)_untag_dyneither_ptr(
_dyneither_ptr_plus(arr,sizeof(void*),j),sizeof(void*),1),(void**)
_untag_dyneither_ptr(_dyneither_ptr_plus(_dyneither_ptr_plus(arr,sizeof(void*),j),
sizeof(void*),1),sizeof(void*),1))> 0;-- j){temp=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j))=*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j + 1));*((void**)
_check_dyneither_subscript(arr,sizeof(void*),j + 1))=temp;if(j == base_ofs)break;}}
if(sp_ofs != 0){sp_ofs -=2;base_ofs=sp[_check_known_subscript_notnull(40,sp_ofs)];
limit_ofs=sp[_check_known_subscript_notnull(40,sp_ofs + 1)];}else{break;}}}}void
Cyc_Array_msort(int(*less_eq)(void**,void**),struct _dyneither_ptr arr,int len);
static void _tmp45(struct _dyneither_ptr*arr,unsigned int*_tmp44,unsigned int*
_tmp43,void***_tmp41){for(*_tmp44=0;*_tmp44 < *_tmp43;(*_tmp44)++){(*_tmp41)[*
_tmp44]=*((void**)_check_dyneither_subscript(*arr,sizeof(void*),0));}}void Cyc_Array_msort(
int(*less_eq)(void**,void**),struct _dyneither_ptr arr,int len){if(len > 
_get_dyneither_size(arr,sizeof(void*)) || len < 0){struct Cyc_Core_Invalid_argument_struct
_tmp3F;const char*_tmp3E;struct Cyc_Core_Invalid_argument_struct*_tmp3D;(int)
_throw((void*)((_tmp3D=_cycalloc(sizeof(*_tmp3D)),((_tmp3D[0]=((_tmp3F.tag=Cyc_Core_Invalid_argument,((
_tmp3F.f1=((_tmp3E="Array::msort",_tag_dyneither(_tmp3E,sizeof(char),13))),
_tmp3F)))),_tmp3D)))));}{unsigned int _tmp44;unsigned int _tmp43;struct
_dyneither_ptr _tmp42;void**_tmp41;unsigned int _tmp40;struct _dyneither_ptr from=(
_tmp40=(unsigned int)len,((_tmp41=(void**)_cycalloc(_check_times(sizeof(void*),
_tmp40)),((_tmp42=_tag_dyneither(_tmp41,sizeof(void*),_tmp40),((((_tmp43=_tmp40,
_tmp45(& arr,& _tmp44,& _tmp43,& _tmp41))),_tmp42)))))));struct _dyneither_ptr to=arr;
struct _dyneither_ptr swap;int swapped=0;int stepsize;int start;int lstart;int lend;int
rstart;int rend;int dest;for(stepsize=1;stepsize < len;stepsize=stepsize * 2){swap=
from;from=to;to=swap;dest=0;for(start=0;start < len;start=start + stepsize * 2){
lstart=start;rstart=start + stepsize < len?start + stepsize: len;lend=rstart;rend=
start + stepsize * 2 < len?start + stepsize * 2: len;while(lstart < lend  && rstart < 
rend){if(less_eq((void**)_untag_dyneither_ptr(_dyneither_ptr_plus(from,sizeof(
void*),lstart),sizeof(void*),1),(void**)_untag_dyneither_ptr(_dyneither_ptr_plus(
from,sizeof(void*),rstart),sizeof(void*),1))<= 0)*((void**)
_check_dyneither_subscript(to,sizeof(void*),dest ++))=*((void**)
_check_dyneither_subscript(from,sizeof(void*),lstart ++));else{*((void**)
_check_dyneither_subscript(to,sizeof(void*),dest ++))=*((void**)
_check_dyneither_subscript(from,sizeof(void*),rstart ++));}}while(lstart < lend){*((
void**)_check_dyneither_subscript(to,sizeof(void*),dest ++))=*((void**)
_check_dyneither_subscript(from,sizeof(void*),lstart ++));}while(rstart < rend){*((
void**)_check_dyneither_subscript(to,sizeof(void*),dest ++))=*((void**)
_check_dyneither_subscript(from,sizeof(void*),rstart ++));}}}if(swapped){int i=0;
for(0;i < len;++ i){*((void**)_check_dyneither_subscript(from,sizeof(void*),i))=*((
void**)_check_dyneither_subscript(to,sizeof(void*),i));}}};}struct _dyneither_ptr
Cyc_Array_from_list(struct Cyc_List_List*x);struct _dyneither_ptr Cyc_Array_from_list(
struct Cyc_List_List*x){return Cyc_List_to_array(x);}struct Cyc_List_List*Cyc_Array_to_list(
struct _dyneither_ptr x);struct Cyc_List_List*Cyc_Array_to_list(struct
_dyneither_ptr x){return Cyc_List_from_array(x);}struct _dyneither_ptr Cyc_Array_copy(
struct _dyneither_ptr x);static void _tmp4B(struct _dyneither_ptr*x,unsigned int*
_tmp4A,unsigned int*_tmp49,void***_tmp47){for(*_tmp4A=0;*_tmp4A < *_tmp49;(*
_tmp4A)++){(*_tmp47)[*_tmp4A]=((void**)(*x).curr)[(int)*_tmp4A];}}struct
_dyneither_ptr Cyc_Array_copy(struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));unsigned int _tmp4A;unsigned int _tmp49;
struct _dyneither_ptr _tmp48;void**_tmp47;unsigned int _tmp46;return(_tmp46=(
unsigned int)sx,((_tmp47=(void**)_cycalloc(_check_times(sizeof(void*),_tmp46)),((
_tmp48=_tag_dyneither(_tmp47,sizeof(void*),_tmp46),((((_tmp49=_tmp46,_tmp4B(& x,&
_tmp4A,& _tmp49,& _tmp47))),_tmp48)))))));}struct _dyneither_ptr Cyc_Array_map(void*(*
f)(void*),struct _dyneither_ptr x);static void _tmp51(struct _dyneither_ptr*x,void*(**
f)(void*),unsigned int*_tmp50,unsigned int*_tmp4F,void***_tmp4D){for(*_tmp50=0;*
_tmp50 < *_tmp4F;(*_tmp50)++){(*_tmp4D)[*_tmp50]=(*f)(((void**)(*x).curr)[(int)*
_tmp50]);}}struct _dyneither_ptr Cyc_Array_map(void*(*f)(void*),struct
_dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));unsigned int
_tmp50;unsigned int _tmp4F;struct _dyneither_ptr _tmp4E;void**_tmp4D;unsigned int
_tmp4C;return(_tmp4C=(unsigned int)sx,((_tmp4D=(void**)_cycalloc(_check_times(
sizeof(void*),_tmp4C)),((_tmp4E=_tag_dyneither(_tmp4D,sizeof(void*),_tmp4C),((((
_tmp4F=_tmp4C,_tmp51(& x,& f,& _tmp50,& _tmp4F,& _tmp4D))),_tmp4E)))))));}struct
_dyneither_ptr Cyc_Array_map_c(void*(*f)(void*,void*),void*env,struct
_dyneither_ptr x);static void _tmp57(struct _dyneither_ptr*x,void*(**f)(void*,void*),
void**env,unsigned int*_tmp56,unsigned int*_tmp55,void***_tmp53){for(*_tmp56=0;*
_tmp56 < *_tmp55;(*_tmp56)++){(*_tmp53)[*_tmp56]=(*f)(*env,((void**)(*x).curr)[(
int)*_tmp56]);}}struct _dyneither_ptr Cyc_Array_map_c(void*(*f)(void*,void*),void*
env,struct _dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));
unsigned int _tmp56;unsigned int _tmp55;struct _dyneither_ptr _tmp54;void**_tmp53;
unsigned int _tmp52;return(_tmp52=(unsigned int)sx,((_tmp53=(void**)_cycalloc(
_check_times(sizeof(void*),_tmp52)),((_tmp54=_tag_dyneither(_tmp53,sizeof(void*),
_tmp52),((((_tmp55=_tmp52,_tmp57(& x,& f,& env,& _tmp56,& _tmp55,& _tmp53))),_tmp54)))))));}
void Cyc_Array_imp_map(void*(*f)(void*),struct _dyneither_ptr x);void Cyc_Array_imp_map(
void*(*f)(void*),struct _dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(
void*));int i=0;for(0;i < sx;++ i){*((void**)_check_dyneither_subscript(x,sizeof(
void*),i))=f(*((void**)_check_dyneither_subscript(x,sizeof(void*),i)));}}void Cyc_Array_imp_map_c(
void*(*f)(void*,void*),void*env,struct _dyneither_ptr x);void Cyc_Array_imp_map_c(
void*(*f)(void*,void*),void*env,struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));int i=0;for(0;i < sx;++ i){*((void**)
_check_dyneither_subscript(x,sizeof(void*),i))=f(env,*((void**)
_check_dyneither_subscript(x,sizeof(void*),i)));}}char Cyc_Array_Array_mismatch[
15]="Array_mismatch";static struct Cyc_Array_Array_mismatch_struct Cyc_Array_Array_mismatch_val={
Cyc_Array_Array_mismatch};struct _dyneither_ptr Cyc_Array_map2(void*(*f)(void*,
void*),struct _dyneither_ptr x,struct _dyneither_ptr y);static void _tmp5D(struct
_dyneither_ptr*y,struct _dyneither_ptr*x,void*(**f)(void*,void*),unsigned int*
_tmp5C,unsigned int*_tmp5B,void***_tmp59){for(*_tmp5C=0;*_tmp5C < *_tmp5B;(*
_tmp5C)++){(*_tmp59)[*_tmp5C]=(*f)(((void**)(*x).curr)[(int)*_tmp5C],*((void**)
_check_dyneither_subscript(*y,sizeof(void*),(int)*_tmp5C)));}}struct
_dyneither_ptr Cyc_Array_map2(void*(*f)(void*,void*),struct _dyneither_ptr x,struct
_dyneither_ptr y){int sx=(int)_get_dyneither_size(x,sizeof(void*));if(sx != 
_get_dyneither_size(y,sizeof(void*)))(int)_throw((void*)& Cyc_Array_Array_mismatch_val);{
unsigned int _tmp5C;unsigned int _tmp5B;struct _dyneither_ptr _tmp5A;void**_tmp59;
unsigned int _tmp58;return(_tmp58=(unsigned int)sx,((_tmp59=(void**)_cycalloc(
_check_times(sizeof(void*),_tmp58)),((_tmp5A=_tag_dyneither(_tmp59,sizeof(void*),
_tmp58),((((_tmp5B=_tmp58,_tmp5D(& y,& x,& f,& _tmp5C,& _tmp5B,& _tmp59))),_tmp5A)))))));};}
void Cyc_Array_app(void*(*f)(void*),struct _dyneither_ptr x);void Cyc_Array_app(void*(*
f)(void*),struct _dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));
int i=0;for(0;i < sx;++ i){f(*((void**)_check_dyneither_subscript(x,sizeof(void*),i)));}}
void Cyc_Array_app_c(void*(*f)(void*,void*),void*env,struct _dyneither_ptr x);void
Cyc_Array_app_c(void*(*f)(void*,void*),void*env,struct _dyneither_ptr x){int sx=(
int)_get_dyneither_size(x,sizeof(void*));int i=0;for(0;i < sx;++ i){f(env,*((void**)
_check_dyneither_subscript(x,sizeof(void*),i)));}}void Cyc_Array_iter(void(*f)(
void*),struct _dyneither_ptr x);void Cyc_Array_iter(void(*f)(void*),struct
_dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));int i=0;for(0;i < 
sx;++ i){f(*((void**)_check_dyneither_subscript(x,sizeof(void*),i)));}}void Cyc_Array_iter_c(
void(*f)(void*,void*),void*env,struct _dyneither_ptr x);void Cyc_Array_iter_c(void(*
f)(void*,void*),void*env,struct _dyneither_ptr x){int sx=(int)_get_dyneither_size(x,
sizeof(void*));int i=0;for(0;i < sx;++ i){f(env,*((void**)
_check_dyneither_subscript(x,sizeof(void*),i)));}}void Cyc_Array_app2(void*(*f)(
void*,void*),struct _dyneither_ptr x,struct _dyneither_ptr y);void Cyc_Array_app2(
void*(*f)(void*,void*),struct _dyneither_ptr x,struct _dyneither_ptr y){int sx=(int)
_get_dyneither_size(x,sizeof(void*));if(sx != _get_dyneither_size(y,sizeof(void*)))(
int)_throw((void*)& Cyc_Array_Array_mismatch_val);{int i=0;for(0;i < sx;++ i){f(*((
void**)_check_dyneither_subscript(x,sizeof(void*),i)),*((void**)
_check_dyneither_subscript(y,sizeof(void*),i)));}};}void Cyc_Array_app2_c(void*(*
f)(void*,void*,void*),void*env,struct _dyneither_ptr x,struct _dyneither_ptr y);void
Cyc_Array_app2_c(void*(*f)(void*,void*,void*),void*env,struct _dyneither_ptr x,
struct _dyneither_ptr y){int sx=(int)_get_dyneither_size(x,sizeof(void*));if(sx != 
_get_dyneither_size(y,sizeof(void*)))(int)_throw((void*)& Cyc_Array_Array_mismatch_val);{
int i=0;for(0;i < sx;++ i){f(env,*((void**)_check_dyneither_subscript(x,sizeof(void*),
i)),*((void**)_check_dyneither_subscript(y,sizeof(void*),i)));}};}void Cyc_Array_iter2(
void(*f)(void*,void*),struct _dyneither_ptr x,struct _dyneither_ptr y);void Cyc_Array_iter2(
void(*f)(void*,void*),struct _dyneither_ptr x,struct _dyneither_ptr y){int sx=(int)
_get_dyneither_size(x,sizeof(void*));if(sx != _get_dyneither_size(y,sizeof(void*)))(
int)_throw((void*)& Cyc_Array_Array_mismatch_val);{int i=0;for(0;i < sx;++ i){f(*((
void**)_check_dyneither_subscript(x,sizeof(void*),i)),*((void**)
_check_dyneither_subscript(y,sizeof(void*),i)));}};}void Cyc_Array_iter2_c(void(*
f)(void*,void*,void*),void*env,struct _dyneither_ptr x,struct _dyneither_ptr y);void
Cyc_Array_iter2_c(void(*f)(void*,void*,void*),void*env,struct _dyneither_ptr x,
struct _dyneither_ptr y){int sx=(int)_get_dyneither_size(x,sizeof(void*));if(sx != 
_get_dyneither_size(y,sizeof(void*)))(int)_throw((void*)& Cyc_Array_Array_mismatch_val);{
int i=0;for(0;i < sx;++ i){f(env,*((void**)_check_dyneither_subscript(x,sizeof(void*),
i)),*((void**)_check_dyneither_subscript(y,sizeof(void*),i)));}};}void*Cyc_Array_fold_left(
void*(*f)(void*,void*),void*accum,struct _dyneither_ptr x);void*Cyc_Array_fold_left(
void*(*f)(void*,void*),void*accum,struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));{int i=0;for(0;i < sx;++ i){accum=f(accum,*((
void**)_check_dyneither_subscript(x,sizeof(void*),i)));}}return accum;}void*Cyc_Array_fold_left_c(
void*(*f)(void*,void*,void*),void*env,void*accum,struct _dyneither_ptr x);void*Cyc_Array_fold_left_c(
void*(*f)(void*,void*,void*),void*env,void*accum,struct _dyneither_ptr x){int sx=(
int)_get_dyneither_size(x,sizeof(void*));{int i=0;for(0;i < sx;++ i){accum=f(env,
accum,*((void**)_check_dyneither_subscript(x,sizeof(void*),i)));}}return accum;}
void*Cyc_Array_fold_right(void*(*f)(void*,void*),struct _dyneither_ptr x,void*
accum);void*Cyc_Array_fold_right(void*(*f)(void*,void*),struct _dyneither_ptr x,
void*accum){{int i=(int)(_get_dyneither_size(x,sizeof(void*))- 1);for(0;i >= 0;-- i){
accum=f(*((void**)_check_dyneither_subscript(x,sizeof(void*),i)),accum);}}return
accum;}void*Cyc_Array_fold_right_c(void*(*f)(void*,void*,void*),void*env,struct
_dyneither_ptr x,void*accum);void*Cyc_Array_fold_right_c(void*(*f)(void*,void*,
void*),void*env,struct _dyneither_ptr x,void*accum){{int i=(int)(
_get_dyneither_size(x,sizeof(void*))- 1);for(0;i >= 0;-- i){accum=f(env,*((void**)
_check_dyneither_subscript(x,sizeof(void*),i)),accum);}}return accum;}struct
_dyneither_ptr Cyc_Array_rev_copy(struct _dyneither_ptr x);static void _tmp63(struct
_dyneither_ptr*x,int*n,unsigned int*_tmp62,unsigned int*_tmp61,void***_tmp5F){
for(*_tmp62=0;*_tmp62 < *_tmp61;(*_tmp62)++){(*_tmp5F)[*_tmp62]=*((void**)
_check_dyneither_subscript(*x,sizeof(void*),(int)(*n - *_tmp62)));}}struct
_dyneither_ptr Cyc_Array_rev_copy(struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));int n=sx - 1;unsigned int _tmp62;unsigned int
_tmp61;struct _dyneither_ptr _tmp60;void**_tmp5F;unsigned int _tmp5E;return(_tmp5E=(
unsigned int)sx,((_tmp5F=(void**)_cycalloc(_check_times(sizeof(void*),_tmp5E)),((
_tmp60=_tag_dyneither(_tmp5F,sizeof(void*),_tmp5E),((((_tmp61=_tmp5E,_tmp63(& x,&
n,& _tmp62,& _tmp61,& _tmp5F))),_tmp60)))))));}void Cyc_Array_imp_rev(struct
_dyneither_ptr x);void Cyc_Array_imp_rev(struct _dyneither_ptr x){void*temp;int i=0;
int j=(int)(_get_dyneither_size(x,sizeof(void*))- 1);while(i < j){temp=*((void**)
_check_dyneither_subscript(x,sizeof(void*),i));*((void**)
_check_dyneither_subscript(x,sizeof(void*),i))=*((void**)
_check_dyneither_subscript(x,sizeof(void*),j));*((void**)
_check_dyneither_subscript(x,sizeof(void*),j))=temp;++ i;-- j;}}int Cyc_Array_forall(
int(*pred)(void*),struct _dyneither_ptr x);int Cyc_Array_forall(int(*pred)(void*),
struct _dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));{int i=0;
for(0;i < sx;++ i){if(!pred(*((void**)_check_dyneither_subscript(x,sizeof(void*),i))))
return 0;}}return 1;}int Cyc_Array_forall_c(int(*pred)(void*,void*),void*env,struct
_dyneither_ptr x);int Cyc_Array_forall_c(int(*pred)(void*,void*),void*env,struct
_dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(void*));{int i=0;for(0;i
< sx;++ i){if(!pred(env,*((void**)_check_dyneither_subscript(x,sizeof(void*),i))))
return 0;}}return 1;}int Cyc_Array_exists(int(*pred)(void*),struct _dyneither_ptr x);
int Cyc_Array_exists(int(*pred)(void*),struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));{int i=0;for(0;i < sx;++ i){if(pred(*((void**)
_check_dyneither_subscript(x,sizeof(void*),i))))return 1;}}return 0;}int Cyc_Array_exists_c(
int(*pred)(void*,void*),void*env,struct _dyneither_ptr x);int Cyc_Array_exists_c(
int(*pred)(void*,void*),void*env,struct _dyneither_ptr x){int sx=(int)
_get_dyneither_size(x,sizeof(void*));{int i=0;for(0;i < sx;++ i){if(pred(env,*((
void**)_check_dyneither_subscript(x,sizeof(void*),i))))return 1;}}return 0;}struct
_dyneither_ptr Cyc_Array_zip(struct _dyneither_ptr x,struct _dyneither_ptr y);static
void _tmp6A(struct _dyneither_ptr*y,struct _dyneither_ptr*x,unsigned int*_tmp69,
unsigned int*_tmp68,struct _tuple0**_tmp66){for(*_tmp69=0;*_tmp69 < *_tmp68;(*
_tmp69)++){struct _tuple0 _tmp64;(*_tmp66)[*_tmp69]=((_tmp64.f1=((void**)(*x).curr)[(
int)*_tmp69],((_tmp64.f2=*((void**)_check_dyneither_subscript(*y,sizeof(void*),(
int)*_tmp69)),_tmp64))));}}struct _dyneither_ptr Cyc_Array_zip(struct
_dyneither_ptr x,struct _dyneither_ptr y){int sx=(int)_get_dyneither_size(x,sizeof(
void*));if(sx != _get_dyneither_size(y,sizeof(void*)))(int)_throw((void*)& Cyc_Array_Array_mismatch_val);{
unsigned int _tmp69;unsigned int _tmp68;struct _dyneither_ptr _tmp67;struct _tuple0*
_tmp66;unsigned int _tmp65;return(_tmp65=(unsigned int)sx,((_tmp66=(struct _tuple0*)
_cycalloc(_check_times(sizeof(struct _tuple0),_tmp65)),((_tmp67=_tag_dyneither(
_tmp66,sizeof(struct _tuple0),_tmp65),((((_tmp68=_tmp65,_tmp6A(& y,& x,& _tmp69,&
_tmp68,& _tmp66))),_tmp67)))))));};}struct _tuple1 Cyc_Array_split(struct
_dyneither_ptr x);static void _tmp70(struct _dyneither_ptr*x,unsigned int*_tmp6F,
unsigned int*_tmp6E,void***_tmp6C){for(*_tmp6F=0;*_tmp6F < *_tmp6E;(*_tmp6F)++){(*
_tmp6C)[*_tmp6F]=(((struct _tuple0*)(*x).curr)[(int)*_tmp6F]).f1;}}static void
_tmp76(struct _dyneither_ptr*x,unsigned int*_tmp75,unsigned int*_tmp74,void***
_tmp72){for(*_tmp75=0;*_tmp75 < *_tmp74;(*_tmp75)++){(*_tmp72)[*_tmp75]=(((struct
_tuple0*)(*x).curr)[(int)*_tmp75]).f2;}}struct _tuple1 Cyc_Array_split(struct
_dyneither_ptr x){int sx=(int)_get_dyneither_size(x,sizeof(struct _tuple0));
unsigned int _tmp81;void**_tmp80;struct _dyneither_ptr _tmp7F;unsigned int _tmp7E;
unsigned int _tmp7D;unsigned int _tmp7C;void**_tmp7B;struct _dyneither_ptr _tmp7A;
unsigned int _tmp79;unsigned int _tmp78;struct _tuple1 _tmp77;return(_tmp77.f1=((
_tmp81=(unsigned int)sx,((_tmp80=(void**)_cycalloc(_check_times(sizeof(void*),
_tmp81)),((_tmp7F=_tag_dyneither(_tmp80,sizeof(void*),_tmp81),((((_tmp7E=_tmp81,
_tmp70(& x,& _tmp7D,& _tmp7E,& _tmp80))),_tmp7F)))))))),((_tmp77.f2=((_tmp7C=(
unsigned int)sx,((_tmp7B=(void**)_cycalloc(_check_times(sizeof(void*),_tmp7C)),((
_tmp7A=_tag_dyneither(_tmp7B,sizeof(void*),_tmp7C),((((_tmp79=_tmp7C,_tmp76(& x,&
_tmp78,& _tmp79,& _tmp7B))),_tmp7A)))))))),_tmp77)));}int Cyc_Array_memq(struct
_dyneither_ptr l,void*x);int Cyc_Array_memq(struct _dyneither_ptr l,void*x){int s=(
int)_get_dyneither_size(l,sizeof(void*));{int i=0;for(0;i < s;++ i){if(*((void**)
_check_dyneither_subscript(l,sizeof(void*),i))== x)return 1;}}return 0;}int Cyc_Array_mem(
int(*compare)(void*,void*),struct _dyneither_ptr l,void*x);int Cyc_Array_mem(int(*
compare)(void*,void*),struct _dyneither_ptr l,void*x){int s=(int)
_get_dyneither_size(l,sizeof(void*));{int i=0;for(0;i < s;++ i){if(0 == compare(*((
void**)_check_dyneither_subscript(l,sizeof(void*),i)),x))return 1;}}return 0;}
struct _dyneither_ptr Cyc_Array_extract(struct _dyneither_ptr x,int start,int*n_opt);
static void _tmp8D(struct _dyneither_ptr*x,int*start,unsigned int*_tmp8C,
unsigned int*_tmp8B,void***_tmp89){for(*_tmp8C=0;*_tmp8C < *_tmp8B;(*_tmp8C)++){(*
_tmp89)[*_tmp8C]=*((void**)_check_dyneither_subscript(*x,sizeof(void*),(int)(*
start + *_tmp8C)));}}struct _dyneither_ptr Cyc_Array_extract(struct _dyneither_ptr x,
int start,int*n_opt){int sx=(int)_get_dyneither_size(x,sizeof(void*));int n=n_opt == 
0?sx - start:*n_opt;if((start < 0  || n <= 0) || start + (n_opt == 0?0: n)> sx){struct
Cyc_Core_Invalid_argument_struct _tmp87;const char*_tmp86;struct Cyc_Core_Invalid_argument_struct*
_tmp85;(int)_throw((void*)((_tmp85=_cycalloc(sizeof(*_tmp85)),((_tmp85[0]=((
_tmp87.tag=Cyc_Core_Invalid_argument,((_tmp87.f1=((_tmp86="Array::extract",
_tag_dyneither(_tmp86,sizeof(char),15))),_tmp87)))),_tmp85)))));}{unsigned int
_tmp8C;unsigned int _tmp8B;struct _dyneither_ptr _tmp8A;void**_tmp89;unsigned int
_tmp88;return(_tmp88=(unsigned int)n,((_tmp89=(void**)_cycalloc(_check_times(
sizeof(void*),_tmp88)),((_tmp8A=_tag_dyneither(_tmp89,sizeof(void*),_tmp88),((((
_tmp8B=_tmp88,_tmp8D(& x,& start,& _tmp8C,& _tmp8B,& _tmp89))),_tmp8A)))))));};}