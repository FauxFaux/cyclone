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
char*tag;struct _dyneither_ptr f1;};extern struct _RegionHandle*Cyc_Core_heap_region;
extern char Cyc_Core_Open_Region[12];struct Cyc_Core_Open_Region_struct{char*tag;};
extern char Cyc_Core_Free_Region[12];struct Cyc_Core_Free_Region_struct{char*tag;};
struct Cyc_List_List{void*hd;struct Cyc_List_List*tl;};int Cyc_List_length(struct
Cyc_List_List*x);extern char Cyc_List_List_mismatch[14];struct Cyc_List_List_mismatch_struct{
char*tag;};extern char Cyc_List_Nth[4];struct Cyc_List_Nth_struct{char*tag;};struct
Cyc_Iter_Iter{void*env;int(*next)(void*env,void*dest);};int Cyc_Iter_next(struct
Cyc_Iter_Iter,void*);struct Cyc___cycFILE;struct Cyc_Cstdio___abstractFILE;struct
Cyc_String_pa_struct{int tag;struct _dyneither_ptr f1;};struct Cyc_Int_pa_struct{int
tag;unsigned long f1;};struct Cyc_Double_pa_struct{int tag;double f1;};struct Cyc_LongDouble_pa_struct{
int tag;long double f1;};struct Cyc_ShortPtr_pa_struct{int tag;short*f1;};struct Cyc_IntPtr_pa_struct{
int tag;unsigned long*f1;};struct Cyc_ShortPtr_sa_struct{int tag;short*f1;};struct
Cyc_UShortPtr_sa_struct{int tag;unsigned short*f1;};struct Cyc_IntPtr_sa_struct{
int tag;int*f1;};struct Cyc_UIntPtr_sa_struct{int tag;unsigned int*f1;};struct Cyc_StringPtr_sa_struct{
int tag;struct _dyneither_ptr f1;};struct Cyc_DoublePtr_sa_struct{int tag;double*f1;}
;struct Cyc_FloatPtr_sa_struct{int tag;float*f1;};struct Cyc_CharPtr_sa_struct{int
tag;struct _dyneither_ptr f1;};int Cyc_getw(struct Cyc___cycFILE*);extern char Cyc_FileCloseError[
15];struct Cyc_FileCloseError_struct{char*tag;};extern char Cyc_FileOpenError[14];
struct Cyc_FileOpenError_struct{char*tag;struct _dyneither_ptr f1;};struct Cyc_Dict_T;
struct Cyc_Dict_Dict{int(*rel)(void*,void*);struct _RegionHandle*r;struct Cyc_Dict_T*
t;};extern char Cyc_Dict_Present[8];struct Cyc_Dict_Present_struct{char*tag;};
extern char Cyc_Dict_Absent[7];struct Cyc_Dict_Absent_struct{char*tag;};struct Cyc_Dict_Dict
Cyc_Dict_empty(int(*cmp)(void*,void*));struct Cyc_Dict_Dict Cyc_Dict_rempty(struct
_RegionHandle*,int(*cmp)(void*,void*));struct Cyc_Dict_Dict Cyc_Dict_rshare(struct
_RegionHandle*,struct Cyc_Dict_Dict);int Cyc_Dict_is_empty(struct Cyc_Dict_Dict d);
int Cyc_Dict_cardinality(struct Cyc_Dict_Dict d);int Cyc_Dict_member(struct Cyc_Dict_Dict
d,void*k);struct Cyc_Dict_Dict Cyc_Dict_insert(struct Cyc_Dict_Dict d,void*k,void*v);
struct Cyc_Dict_Dict Cyc_Dict_insert_new(struct Cyc_Dict_Dict d,void*k,void*v);
struct Cyc_Dict_Dict Cyc_Dict_inserts(struct Cyc_Dict_Dict d,struct Cyc_List_List*l);
struct Cyc_Dict_Dict Cyc_Dict_singleton(int(*cmp)(void*,void*),void*k,void*v);
struct Cyc_Dict_Dict Cyc_Dict_rsingleton(struct _RegionHandle*,int(*cmp)(void*,void*),
void*k,void*v);void*Cyc_Dict_lookup(struct Cyc_Dict_Dict d,void*k);void**Cyc_Dict_lookup_opt(
struct Cyc_Dict_Dict d,void*k);int Cyc_Dict_lookup_bool(struct Cyc_Dict_Dict d,void*k,
void**ans);void*Cyc_Dict_fold(void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict d,
void*accum);void*Cyc_Dict_fold_c(void*(*f)(void*,void*,void*,void*),void*env,
struct Cyc_Dict_Dict d,void*accum);void Cyc_Dict_app(void*(*f)(void*,void*),struct
Cyc_Dict_Dict d);void Cyc_Dict_app_c(void*(*f)(void*,void*,void*),void*env,struct
Cyc_Dict_Dict d);void Cyc_Dict_iter(void(*f)(void*,void*),struct Cyc_Dict_Dict d);
void Cyc_Dict_iter_c(void(*f)(void*,void*,void*),void*env,struct Cyc_Dict_Dict d);
void Cyc_Dict_iter2(void(*f)(void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2);void Cyc_Dict_iter2_c(void(*f)(void*,void*,void*),void*env,struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2);void*Cyc_Dict_fold2_c(void*(*f)(void*,void*,void*,void*,
void*),void*env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2,void*accum);struct
Cyc_Dict_Dict Cyc_Dict_rcopy(struct _RegionHandle*,struct Cyc_Dict_Dict);struct Cyc_Dict_Dict
Cyc_Dict_copy(struct Cyc_Dict_Dict);struct Cyc_Dict_Dict Cyc_Dict_map(void*(*f)(
void*),struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rmap(struct
_RegionHandle*,void*(*f)(void*),struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_map_c(
void*(*f)(void*,void*),void*env,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rmap_c(
struct _RegionHandle*,void*(*f)(void*,void*),void*env,struct Cyc_Dict_Dict d);
struct Cyc_Dict_Dict Cyc_Dict_union_two_c(void*(*f)(void*,void*,void*,void*),void*
env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct Cyc_Dict_Dict Cyc_Dict_intersect(
void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct
Cyc_Dict_Dict Cyc_Dict_intersect_c(void*(*f)(void*,void*,void*,void*),void*env,
struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);int Cyc_Dict_forall_c(int(*f)(void*,
void*,void*),void*env,struct Cyc_Dict_Dict d);int Cyc_Dict_forall_intersect(int(*f)(
void*,void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct _tuple0{
void*f1;void*f2;};struct _tuple0*Cyc_Dict_rchoose(struct _RegionHandle*r,struct Cyc_Dict_Dict
d);struct _tuple0*Cyc_Dict_rchoose(struct _RegionHandle*,struct Cyc_Dict_Dict d);
struct Cyc_List_List*Cyc_Dict_to_list(struct Cyc_Dict_Dict d);struct Cyc_List_List*
Cyc_Dict_rto_list(struct _RegionHandle*,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict
Cyc_Dict_filter(int(*f)(void*,void*),struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict
Cyc_Dict_rfilter(struct _RegionHandle*,int(*f)(void*,void*),struct Cyc_Dict_Dict d);
struct Cyc_Dict_Dict Cyc_Dict_filter_c(int(*f)(void*,void*,void*),void*env,struct
Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rfilter_c(struct _RegionHandle*,int(*
f)(void*,void*,void*),void*env,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_difference(
struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct Cyc_Dict_Dict Cyc_Dict_rdifference(
struct _RegionHandle*,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct Cyc_Dict_Dict
Cyc_Dict_delete(struct Cyc_Dict_Dict,void*);struct Cyc_Dict_Dict Cyc_Dict_rdelete(
struct _RegionHandle*,struct Cyc_Dict_Dict,void*);struct Cyc_Dict_Dict Cyc_Dict_rdelete_same(
struct Cyc_Dict_Dict,void*);struct Cyc_Iter_Iter Cyc_Dict_make_iter(struct
_RegionHandle*rgn,struct Cyc_Dict_Dict d);void*Cyc_Dict_marshal(struct
_RegionHandle*rgn,void*env,void*(*write_key)(void*,struct Cyc___cycFILE*,void*),
void*(*write_val)(void*,struct Cyc___cycFILE*,void*),struct Cyc___cycFILE*fp,
struct Cyc_Dict_Dict dict);struct Cyc_Dict_Dict Cyc_Dict_unmarshal(struct
_RegionHandle*rgn,void*env,int(*cmp)(void*,void*),void*(*read_key)(void*,struct
Cyc___cycFILE*),void*(*read_val)(void*,struct Cyc___cycFILE*),struct Cyc___cycFILE*
fp);char Cyc_Dict_Absent[7]="Absent";char Cyc_Dict_Present[8]="Present";struct Cyc_Dict_Absent_struct
Cyc_Dict_Absent_val={Cyc_Dict_Absent};struct Cyc_Dict_Present_struct Cyc_Dict_Present_val={
Cyc_Dict_Present};enum Cyc_Dict_Color{Cyc_Dict_R  = 0,Cyc_Dict_B  = 1};struct Cyc_Dict_T{
enum Cyc_Dict_Color color;struct Cyc_Dict_T*left;struct Cyc_Dict_T*right;struct
_tuple0 key_val;};struct Cyc_Dict_Dict;struct Cyc_Dict_Dict Cyc_Dict_rempty(struct
_RegionHandle*r,int(*comp)(void*,void*));struct Cyc_Dict_Dict Cyc_Dict_rempty(
struct _RegionHandle*r,int(*comp)(void*,void*)){struct Cyc_Dict_Dict _tmp124;return(
_tmp124.rel=comp,((_tmp124.r=r,((_tmp124.t=0,_tmp124)))));}struct Cyc_Dict_Dict
Cyc_Dict_empty(int(*comp)(void*,void*));struct Cyc_Dict_Dict Cyc_Dict_empty(int(*
comp)(void*,void*)){return Cyc_Dict_rempty(Cyc_Core_heap_region,comp);}struct Cyc_Dict_Dict
Cyc_Dict_rsingleton(struct _RegionHandle*r,int(*comp)(void*,void*),void*key,void*
data);struct Cyc_Dict_Dict Cyc_Dict_rsingleton(struct _RegionHandle*r,int(*comp)(
void*,void*),void*key,void*data){struct Cyc_Dict_T*_tmp12A;struct _tuple0 _tmp129;
struct Cyc_Dict_Dict _tmp128;return(_tmp128.rel=comp,((_tmp128.r=r,((_tmp128.t=((
_tmp12A=_region_malloc(r,sizeof(*_tmp12A)),((_tmp12A->color=Cyc_Dict_B,((_tmp12A->left=
0,((_tmp12A->right=0,((_tmp12A->key_val=((_tmp129.f1=key,((_tmp129.f2=data,
_tmp129)))),_tmp12A)))))))))),_tmp128)))));}struct Cyc_Dict_Dict Cyc_Dict_singleton(
int(*comp)(void*,void*),void*key,void*data);struct Cyc_Dict_Dict Cyc_Dict_singleton(
int(*comp)(void*,void*),void*key,void*data){return Cyc_Dict_rsingleton(Cyc_Core_heap_region,
comp,key,data);}struct Cyc_Dict_Dict Cyc_Dict_rshare(struct _RegionHandle*r,struct
Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rshare(struct _RegionHandle*r,struct
Cyc_Dict_Dict d){struct Cyc_Dict_Dict _tmp12B;return(_tmp12B.rel=d.rel,((_tmp12B.r=
r,((_tmp12B.t=(struct Cyc_Dict_T*)d.t,_tmp12B)))));}int Cyc_Dict_is_empty(struct
Cyc_Dict_Dict d);int Cyc_Dict_is_empty(struct Cyc_Dict_Dict d){return d.t == 0;}int Cyc_Dict_member(
struct Cyc_Dict_Dict d,void*key);int Cyc_Dict_member(struct Cyc_Dict_Dict d,void*key){
int(*_tmp7)(void*,void*)=d.rel;struct Cyc_Dict_T*_tmp8=d.t;while(_tmp8 != 0){int
_tmp9=_tmp7(key,(_tmp8->key_val).f1);if(_tmp9 < 0)_tmp8=_tmp8->left;else{if(_tmp9
> 0)_tmp8=_tmp8->right;else{return 1;}}}return 0;}void*Cyc_Dict_lookup(struct Cyc_Dict_Dict
d,void*key);void*Cyc_Dict_lookup(struct Cyc_Dict_Dict d,void*key){int(*_tmpA)(void*,
void*)=d.rel;struct Cyc_Dict_T*_tmpB=d.t;while(_tmpB != 0){int _tmpC=_tmpA(key,(
_tmpB->key_val).f1);if(_tmpC < 0)_tmpB=_tmpB->left;else{if(_tmpC > 0)_tmpB=_tmpB->right;
else{return(_tmpB->key_val).f2;}}}(int)_throw((void*)& Cyc_Dict_Absent_val);}void**
Cyc_Dict_lookup_opt(struct Cyc_Dict_Dict d,void*key);void**Cyc_Dict_lookup_opt(
struct Cyc_Dict_Dict d,void*key){int(*_tmpD)(void*,void*)=d.rel;struct Cyc_Dict_T*
_tmpE=d.t;while(_tmpE != 0){int _tmpF=_tmpD(key,(_tmpE->key_val).f1);if(_tmpF < 0)
_tmpE=_tmpE->left;else{if(_tmpF > 0)_tmpE=_tmpE->right;else{return(void**)&(_tmpE->key_val).f2;}}}
return 0;}int Cyc_Dict_lookup_bool(struct Cyc_Dict_Dict d,void*key,void**ans_place);
int Cyc_Dict_lookup_bool(struct Cyc_Dict_Dict d,void*key,void**ans_place){int(*
_tmp10)(void*,void*)=d.rel;struct Cyc_Dict_T*_tmp11=d.t;while(_tmp11 != 0){int
_tmp12=_tmp10(key,(_tmp11->key_val).f1);if(_tmp12 < 0)_tmp11=_tmp11->left;else{
if(_tmp12 > 0)_tmp11=_tmp11->right;else{*ans_place=(_tmp11->key_val).f2;return 1;}}}
return 0;}struct _tuple1{enum Cyc_Dict_Color f1;struct Cyc_Dict_T*f2;struct Cyc_Dict_T*
f3;struct _tuple0 f4;};static struct Cyc_Dict_T*Cyc_Dict_balance(struct _RegionHandle*
r,struct _tuple1 quad);static struct Cyc_Dict_T*Cyc_Dict_balance(struct _RegionHandle*
r,struct _tuple1 quad){struct _tuple1 _tmp13=quad;enum Cyc_Dict_Color _tmp14;struct
Cyc_Dict_T*_tmp15;struct Cyc_Dict_T _tmp16;enum Cyc_Dict_Color _tmp17;struct Cyc_Dict_T*
_tmp18;struct Cyc_Dict_T _tmp19;enum Cyc_Dict_Color _tmp1A;struct Cyc_Dict_T*_tmp1B;
struct Cyc_Dict_T*_tmp1C;struct _tuple0 _tmp1D;struct Cyc_Dict_T*_tmp1E;struct
_tuple0 _tmp1F;struct Cyc_Dict_T*_tmp20;struct _tuple0 _tmp21;enum Cyc_Dict_Color
_tmp22;struct Cyc_Dict_T*_tmp23;struct Cyc_Dict_T _tmp24;enum Cyc_Dict_Color _tmp25;
struct Cyc_Dict_T*_tmp26;struct Cyc_Dict_T*_tmp27;struct Cyc_Dict_T _tmp28;enum Cyc_Dict_Color
_tmp29;struct Cyc_Dict_T*_tmp2A;struct Cyc_Dict_T*_tmp2B;struct _tuple0 _tmp2C;
struct _tuple0 _tmp2D;struct Cyc_Dict_T*_tmp2E;struct _tuple0 _tmp2F;enum Cyc_Dict_Color
_tmp30;struct Cyc_Dict_T*_tmp31;struct Cyc_Dict_T*_tmp32;struct Cyc_Dict_T _tmp33;
enum Cyc_Dict_Color _tmp34;struct Cyc_Dict_T*_tmp35;struct Cyc_Dict_T _tmp36;enum 
Cyc_Dict_Color _tmp37;struct Cyc_Dict_T*_tmp38;struct Cyc_Dict_T*_tmp39;struct
_tuple0 _tmp3A;struct Cyc_Dict_T*_tmp3B;struct _tuple0 _tmp3C;struct _tuple0 _tmp3D;
enum Cyc_Dict_Color _tmp3E;struct Cyc_Dict_T*_tmp3F;struct Cyc_Dict_T*_tmp40;struct
Cyc_Dict_T _tmp41;enum Cyc_Dict_Color _tmp42;struct Cyc_Dict_T*_tmp43;struct Cyc_Dict_T*
_tmp44;struct Cyc_Dict_T _tmp45;enum Cyc_Dict_Color _tmp46;struct Cyc_Dict_T*_tmp47;
struct Cyc_Dict_T*_tmp48;struct _tuple0 _tmp49;struct _tuple0 _tmp4A;struct _tuple0
_tmp4B;enum Cyc_Dict_Color _tmp4C;struct Cyc_Dict_T*_tmp4D;struct Cyc_Dict_T*_tmp4E;
struct _tuple0 _tmp4F;_LL1: _tmp14=_tmp13.f1;if(_tmp14 != Cyc_Dict_B)goto _LL3;_tmp15=
_tmp13.f2;if(_tmp15 == 0)goto _LL3;_tmp16=*_tmp15;_tmp17=_tmp16.color;if(_tmp17 != 
Cyc_Dict_R)goto _LL3;_tmp18=_tmp16.left;if(_tmp18 == 0)goto _LL3;_tmp19=*_tmp18;
_tmp1A=_tmp19.color;if(_tmp1A != Cyc_Dict_R)goto _LL3;_tmp1B=_tmp19.left;_tmp1C=
_tmp19.right;_tmp1D=_tmp19.key_val;_tmp1E=_tmp16.right;_tmp1F=_tmp16.key_val;
_tmp20=_tmp13.f3;_tmp21=_tmp13.f4;_LL2: {struct Cyc_Dict_T*_tmp130;struct Cyc_Dict_T*
_tmp12F;struct Cyc_Dict_T*_tmp12E;return(_tmp12E=_region_malloc(r,sizeof(*_tmp12E)),((
_tmp12E->color=Cyc_Dict_R,((_tmp12E->left=((_tmp130=_region_malloc(r,sizeof(*
_tmp130)),((_tmp130->color=Cyc_Dict_B,((_tmp130->left=_tmp1B,((_tmp130->right=
_tmp1C,((_tmp130->key_val=_tmp1D,_tmp130)))))))))),((_tmp12E->right=((_tmp12F=
_region_malloc(r,sizeof(*_tmp12F)),((_tmp12F->color=Cyc_Dict_B,((_tmp12F->left=
_tmp1E,((_tmp12F->right=_tmp20,((_tmp12F->key_val=_tmp21,_tmp12F)))))))))),((
_tmp12E->key_val=_tmp1F,_tmp12E)))))))));}_LL3: _tmp22=_tmp13.f1;if(_tmp22 != Cyc_Dict_B)
goto _LL5;_tmp23=_tmp13.f2;if(_tmp23 == 0)goto _LL5;_tmp24=*_tmp23;_tmp25=_tmp24.color;
if(_tmp25 != Cyc_Dict_R)goto _LL5;_tmp26=_tmp24.left;_tmp27=_tmp24.right;if(_tmp27
== 0)goto _LL5;_tmp28=*_tmp27;_tmp29=_tmp28.color;if(_tmp29 != Cyc_Dict_R)goto _LL5;
_tmp2A=_tmp28.left;_tmp2B=_tmp28.right;_tmp2C=_tmp28.key_val;_tmp2D=_tmp24.key_val;
_tmp2E=_tmp13.f3;_tmp2F=_tmp13.f4;_LL4: {struct Cyc_Dict_T*_tmp135;struct Cyc_Dict_T*
_tmp134;struct Cyc_Dict_T*_tmp133;return(_tmp133=_region_malloc(r,sizeof(*_tmp133)),((
_tmp133->color=Cyc_Dict_R,((_tmp133->left=((_tmp135=_region_malloc(r,sizeof(*
_tmp135)),((_tmp135->color=Cyc_Dict_B,((_tmp135->left=_tmp26,((_tmp135->right=
_tmp2A,((_tmp135->key_val=_tmp2D,_tmp135)))))))))),((_tmp133->right=((_tmp134=
_region_malloc(r,sizeof(*_tmp134)),((_tmp134->color=Cyc_Dict_B,((_tmp134->left=
_tmp2B,((_tmp134->right=_tmp2E,((_tmp134->key_val=_tmp2F,_tmp134)))))))))),((
_tmp133->key_val=_tmp2C,_tmp133)))))))));}_LL5: _tmp30=_tmp13.f1;if(_tmp30 != Cyc_Dict_B)
goto _LL7;_tmp31=_tmp13.f2;_tmp32=_tmp13.f3;if(_tmp32 == 0)goto _LL7;_tmp33=*_tmp32;
_tmp34=_tmp33.color;if(_tmp34 != Cyc_Dict_R)goto _LL7;_tmp35=_tmp33.left;if(_tmp35
== 0)goto _LL7;_tmp36=*_tmp35;_tmp37=_tmp36.color;if(_tmp37 != Cyc_Dict_R)goto _LL7;
_tmp38=_tmp36.left;_tmp39=_tmp36.right;_tmp3A=_tmp36.key_val;_tmp3B=_tmp33.right;
_tmp3C=_tmp33.key_val;_tmp3D=_tmp13.f4;_LL6: {struct Cyc_Dict_T*_tmp13A;struct Cyc_Dict_T*
_tmp139;struct Cyc_Dict_T*_tmp138;return(_tmp138=_region_malloc(r,sizeof(*_tmp138)),((
_tmp138->color=Cyc_Dict_R,((_tmp138->left=((_tmp13A=_region_malloc(r,sizeof(*
_tmp13A)),((_tmp13A->color=Cyc_Dict_B,((_tmp13A->left=_tmp31,((_tmp13A->right=
_tmp38,((_tmp13A->key_val=_tmp3D,_tmp13A)))))))))),((_tmp138->right=((_tmp139=
_region_malloc(r,sizeof(*_tmp139)),((_tmp139->color=Cyc_Dict_B,((_tmp139->left=
_tmp39,((_tmp139->right=_tmp3B,((_tmp139->key_val=_tmp3C,_tmp139)))))))))),((
_tmp138->key_val=_tmp3A,_tmp138)))))))));}_LL7: _tmp3E=_tmp13.f1;if(_tmp3E != Cyc_Dict_B)
goto _LL9;_tmp3F=_tmp13.f2;_tmp40=_tmp13.f3;if(_tmp40 == 0)goto _LL9;_tmp41=*_tmp40;
_tmp42=_tmp41.color;if(_tmp42 != Cyc_Dict_R)goto _LL9;_tmp43=_tmp41.left;_tmp44=
_tmp41.right;if(_tmp44 == 0)goto _LL9;_tmp45=*_tmp44;_tmp46=_tmp45.color;if(_tmp46
!= Cyc_Dict_R)goto _LL9;_tmp47=_tmp45.left;_tmp48=_tmp45.right;_tmp49=_tmp45.key_val;
_tmp4A=_tmp41.key_val;_tmp4B=_tmp13.f4;_LL8: {struct Cyc_Dict_T*_tmp13F;struct Cyc_Dict_T*
_tmp13E;struct Cyc_Dict_T*_tmp13D;return(_tmp13D=_region_malloc(r,sizeof(*_tmp13D)),((
_tmp13D->color=Cyc_Dict_R,((_tmp13D->left=((_tmp13F=_region_malloc(r,sizeof(*
_tmp13F)),((_tmp13F->color=Cyc_Dict_B,((_tmp13F->left=_tmp3F,((_tmp13F->right=
_tmp43,((_tmp13F->key_val=_tmp4B,_tmp13F)))))))))),((_tmp13D->right=((_tmp13E=
_region_malloc(r,sizeof(*_tmp13E)),((_tmp13E->color=Cyc_Dict_B,((_tmp13E->left=
_tmp47,((_tmp13E->right=_tmp48,((_tmp13E->key_val=_tmp49,_tmp13E)))))))))),((
_tmp13D->key_val=_tmp4A,_tmp13D)))))))));}_LL9: _tmp4C=_tmp13.f1;_tmp4D=_tmp13.f2;
_tmp4E=_tmp13.f3;_tmp4F=_tmp13.f4;_LLA: {struct Cyc_Dict_T*_tmp140;return(_tmp140=
_region_malloc(r,sizeof(*_tmp140)),((_tmp140->color=_tmp4C,((_tmp140->left=
_tmp4D,((_tmp140->right=_tmp4E,((_tmp140->key_val=_tmp4F,_tmp140)))))))));}_LL0:;}
static struct Cyc_Dict_T*Cyc_Dict_ins(struct _RegionHandle*r,int(*rel)(void*,void*),
struct _tuple0 key_val,struct Cyc_Dict_T*t);static struct Cyc_Dict_T*Cyc_Dict_ins(
struct _RegionHandle*r,int(*rel)(void*,void*),struct _tuple0 key_val,struct Cyc_Dict_T*
t){struct Cyc_Dict_T*_tmp5D=t;struct Cyc_Dict_T _tmp5E;enum Cyc_Dict_Color _tmp5F;
struct Cyc_Dict_T*_tmp60;struct Cyc_Dict_T*_tmp61;struct _tuple0 _tmp62;_LLC: if(
_tmp5D != 0)goto _LLE;_LLD: {struct Cyc_Dict_T*_tmp141;return(_tmp141=
_region_malloc(r,sizeof(*_tmp141)),((_tmp141->color=Cyc_Dict_R,((_tmp141->left=0,((
_tmp141->right=0,((_tmp141->key_val=key_val,_tmp141)))))))));}_LLE: if(_tmp5D == 0)
goto _LLB;_tmp5E=*_tmp5D;_tmp5F=_tmp5E.color;_tmp60=_tmp5E.left;_tmp61=_tmp5E.right;
_tmp62=_tmp5E.key_val;_LLF: {int _tmp64=rel(key_val.f1,_tmp62.f1);if(_tmp64 < 0){
struct _tuple1 _tmp142;return Cyc_Dict_balance(r,((_tmp142.f1=_tmp5F,((_tmp142.f2=
Cyc_Dict_ins(r,rel,key_val,_tmp60),((_tmp142.f3=_tmp61,((_tmp142.f4=_tmp62,
_tmp142)))))))));}else{if(_tmp64 > 0){struct _tuple1 _tmp143;return Cyc_Dict_balance(
r,((_tmp143.f1=_tmp5F,((_tmp143.f2=_tmp60,((_tmp143.f3=Cyc_Dict_ins(r,rel,
key_val,_tmp61),((_tmp143.f4=_tmp62,_tmp143)))))))));}else{struct Cyc_Dict_T*
_tmp144;return(_tmp144=_region_malloc(r,sizeof(*_tmp144)),((_tmp144->color=
_tmp5F,((_tmp144->left=_tmp60,((_tmp144->right=_tmp61,((_tmp144->key_val=key_val,
_tmp144)))))))));}}}_LLB:;}struct Cyc_Dict_Dict Cyc_Dict_insert(struct Cyc_Dict_Dict
d,void*key,void*data);struct Cyc_Dict_Dict Cyc_Dict_insert(struct Cyc_Dict_Dict d,
void*key,void*data){struct _tuple0 _tmp145;struct Cyc_Dict_T*_tmp68=Cyc_Dict_ins(d.r,
d.rel,((_tmp145.f1=key,((_tmp145.f2=data,_tmp145)))),d.t);((struct Cyc_Dict_T*)
_check_null(_tmp68))->color=Cyc_Dict_B;{struct Cyc_Dict_Dict _tmp146;struct Cyc_Dict_Dict
_tmp69=(_tmp146.rel=d.rel,((_tmp146.r=d.r,((_tmp146.t=_tmp68,_tmp146)))));return
_tmp69;};}struct Cyc_Dict_Dict Cyc_Dict_insert_new(struct Cyc_Dict_Dict d,void*key,
void*data);struct Cyc_Dict_Dict Cyc_Dict_insert_new(struct Cyc_Dict_Dict d,void*key,
void*data){if(Cyc_Dict_member(d,key))(int)_throw((void*)& Cyc_Dict_Absent_val);
return Cyc_Dict_insert(d,key,data);}struct Cyc_Dict_Dict Cyc_Dict_inserts(struct Cyc_Dict_Dict
d,struct Cyc_List_List*kds);struct Cyc_Dict_Dict Cyc_Dict_inserts(struct Cyc_Dict_Dict
d,struct Cyc_List_List*kds){for(0;kds != 0;kds=kds->tl){d=Cyc_Dict_insert(d,(((
struct _tuple0*)kds->hd)[_check_known_subscript_notnull(1,0)]).f1,(((struct
_tuple0*)kds->hd)[_check_known_subscript_notnull(1,0)]).f2);}return d;}static void*
Cyc_Dict_fold_tree(void*(*f)(void*,void*,void*),struct Cyc_Dict_T*t,void*accum);
static void*Cyc_Dict_fold_tree(void*(*f)(void*,void*,void*),struct Cyc_Dict_T*t,
void*accum){struct Cyc_Dict_T _tmp6D;struct Cyc_Dict_T*_tmp6E;struct Cyc_Dict_T*
_tmp6F;struct _tuple0 _tmp70;void*_tmp71;void*_tmp72;struct Cyc_Dict_T*_tmp6C=t;
_tmp6D=*_tmp6C;_tmp6E=_tmp6D.left;_tmp6F=_tmp6D.right;_tmp70=_tmp6D.key_val;
_tmp71=_tmp70.f1;_tmp72=_tmp70.f2;if(_tmp6E != 0)accum=Cyc_Dict_fold_tree(f,(
struct Cyc_Dict_T*)_tmp6E,accum);accum=f(_tmp71,_tmp72,accum);if(_tmp6F != 0)accum=
Cyc_Dict_fold_tree(f,(struct Cyc_Dict_T*)_tmp6F,accum);return accum;}void*Cyc_Dict_fold(
void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict d,void*accum);void*Cyc_Dict_fold(
void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict d,void*accum){if(d.t == 0)return
accum;return Cyc_Dict_fold_tree(f,(struct Cyc_Dict_T*)d.t,accum);}static void*Cyc_Dict_fold_tree_c(
void*(*f)(void*,void*,void*,void*),void*env,struct Cyc_Dict_T*t,void*accum);
static void*Cyc_Dict_fold_tree_c(void*(*f)(void*,void*,void*,void*),void*env,
struct Cyc_Dict_T*t,void*accum){struct Cyc_Dict_T _tmp74;struct Cyc_Dict_T*_tmp75;
struct Cyc_Dict_T*_tmp76;struct _tuple0 _tmp77;void*_tmp78;void*_tmp79;struct Cyc_Dict_T*
_tmp73=t;_tmp74=*_tmp73;_tmp75=_tmp74.left;_tmp76=_tmp74.right;_tmp77=_tmp74.key_val;
_tmp78=_tmp77.f1;_tmp79=_tmp77.f2;if(_tmp75 != 0)accum=Cyc_Dict_fold_tree_c(f,env,(
struct Cyc_Dict_T*)_tmp75,accum);accum=f(env,_tmp78,_tmp79,accum);if(_tmp76 != 0)
accum=Cyc_Dict_fold_tree_c(f,env,(struct Cyc_Dict_T*)_tmp76,accum);return accum;}
void*Cyc_Dict_fold_c(void*(*f)(void*,void*,void*,void*),void*env,struct Cyc_Dict_Dict
d,void*accum);void*Cyc_Dict_fold_c(void*(*f)(void*,void*,void*,void*),void*env,
struct Cyc_Dict_Dict d,void*accum){if(d.t == 0)return accum;return Cyc_Dict_fold_tree_c(
f,env,(struct Cyc_Dict_T*)d.t,accum);}static void Cyc_Dict_app_tree(void*(*f)(void*,
void*),struct Cyc_Dict_T*t);static void Cyc_Dict_app_tree(void*(*f)(void*,void*),
struct Cyc_Dict_T*t){struct Cyc_Dict_T _tmp7B;struct Cyc_Dict_T*_tmp7C;struct Cyc_Dict_T*
_tmp7D;struct _tuple0 _tmp7E;void*_tmp7F;void*_tmp80;struct Cyc_Dict_T*_tmp7A=t;
_tmp7B=*_tmp7A;_tmp7C=_tmp7B.left;_tmp7D=_tmp7B.right;_tmp7E=_tmp7B.key_val;
_tmp7F=_tmp7E.f1;_tmp80=_tmp7E.f2;if(_tmp7C != 0)Cyc_Dict_app_tree(f,(struct Cyc_Dict_T*)
_tmp7C);f(_tmp7F,_tmp80);if(_tmp7D != 0)Cyc_Dict_app_tree(f,(struct Cyc_Dict_T*)
_tmp7D);}void Cyc_Dict_app(void*(*f)(void*,void*),struct Cyc_Dict_Dict d);void Cyc_Dict_app(
void*(*f)(void*,void*),struct Cyc_Dict_Dict d){if(d.t != 0)Cyc_Dict_app_tree(f,(
struct Cyc_Dict_T*)d.t);}static void Cyc_Dict_app_tree_c(void*(*f)(void*,void*,void*),
void*env,struct Cyc_Dict_T*t);static void Cyc_Dict_app_tree_c(void*(*f)(void*,void*,
void*),void*env,struct Cyc_Dict_T*t){struct Cyc_Dict_T _tmp82;struct Cyc_Dict_T*
_tmp83;struct Cyc_Dict_T*_tmp84;struct _tuple0 _tmp85;void*_tmp86;void*_tmp87;
struct Cyc_Dict_T*_tmp81=t;_tmp82=*_tmp81;_tmp83=_tmp82.left;_tmp84=_tmp82.right;
_tmp85=_tmp82.key_val;_tmp86=_tmp85.f1;_tmp87=_tmp85.f2;if(_tmp83 != 0)Cyc_Dict_app_tree_c(
f,env,(struct Cyc_Dict_T*)_tmp83);f(env,_tmp86,_tmp87);if(_tmp84 != 0)Cyc_Dict_app_tree_c(
f,env,(struct Cyc_Dict_T*)_tmp84);}void Cyc_Dict_app_c(void*(*f)(void*,void*,void*),
void*env,struct Cyc_Dict_Dict d);void Cyc_Dict_app_c(void*(*f)(void*,void*,void*),
void*env,struct Cyc_Dict_Dict d){if(d.t != 0)Cyc_Dict_app_tree_c(f,env,(struct Cyc_Dict_T*)
d.t);}static void Cyc_Dict_iter_tree(void(*f)(void*,void*),struct Cyc_Dict_T*t);
static void Cyc_Dict_iter_tree(void(*f)(void*,void*),struct Cyc_Dict_T*t){struct Cyc_Dict_T
_tmp89;struct Cyc_Dict_T*_tmp8A;struct Cyc_Dict_T*_tmp8B;struct _tuple0 _tmp8C;void*
_tmp8D;void*_tmp8E;struct Cyc_Dict_T*_tmp88=t;_tmp89=*_tmp88;_tmp8A=_tmp89.left;
_tmp8B=_tmp89.right;_tmp8C=_tmp89.key_val;_tmp8D=_tmp8C.f1;_tmp8E=_tmp8C.f2;if(
_tmp8A != 0)Cyc_Dict_iter_tree(f,(struct Cyc_Dict_T*)_tmp8A);f(_tmp8D,_tmp8E);if(
_tmp8B != 0)Cyc_Dict_iter_tree(f,(struct Cyc_Dict_T*)_tmp8B);}void Cyc_Dict_iter(
void(*f)(void*,void*),struct Cyc_Dict_Dict d);void Cyc_Dict_iter(void(*f)(void*,
void*),struct Cyc_Dict_Dict d){if(d.t != 0)Cyc_Dict_iter_tree(f,(struct Cyc_Dict_T*)
d.t);}static void Cyc_Dict_iter_tree_c(void(*f)(void*,void*,void*),void*env,struct
Cyc_Dict_T*t);static void Cyc_Dict_iter_tree_c(void(*f)(void*,void*,void*),void*
env,struct Cyc_Dict_T*t){struct Cyc_Dict_T _tmp90;struct Cyc_Dict_T*_tmp91;struct Cyc_Dict_T*
_tmp92;struct _tuple0 _tmp93;void*_tmp94;void*_tmp95;struct Cyc_Dict_T*_tmp8F=t;
_tmp90=*_tmp8F;_tmp91=_tmp90.left;_tmp92=_tmp90.right;_tmp93=_tmp90.key_val;
_tmp94=_tmp93.f1;_tmp95=_tmp93.f2;if(_tmp91 != 0)Cyc_Dict_iter_tree_c(f,env,(
struct Cyc_Dict_T*)_tmp91);f(env,_tmp94,_tmp95);if(_tmp92 != 0)Cyc_Dict_iter_tree_c(
f,env,(struct Cyc_Dict_T*)_tmp92);}void Cyc_Dict_iter_c(void(*f)(void*,void*,void*),
void*env,struct Cyc_Dict_Dict d);void Cyc_Dict_iter_c(void(*f)(void*,void*,void*),
void*env,struct Cyc_Dict_Dict d){if(d.t != 0)Cyc_Dict_iter_tree_c(f,env,(struct Cyc_Dict_T*)
d.t);}static void Cyc_Dict_count_elem(int*cnt,void*a,void*b);static void Cyc_Dict_count_elem(
int*cnt,void*a,void*b){*cnt=*cnt + 1;}int Cyc_Dict_cardinality(struct Cyc_Dict_Dict
d);int Cyc_Dict_cardinality(struct Cyc_Dict_Dict d){int num=0;((void(*)(void(*f)(int*,
void*,void*),int*env,struct Cyc_Dict_Dict d))Cyc_Dict_iter_c)(Cyc_Dict_count_elem,&
num,d);return num;}struct _tuple2{void(*f1)(void*,void*);struct Cyc_Dict_Dict f2;};
static void Cyc_Dict_iter2_f(struct _tuple2*env,void*a,void*b1);static void Cyc_Dict_iter2_f(
struct _tuple2*env,void*a,void*b1){struct _tuple2 _tmp97;void(*_tmp98)(void*,void*);
struct Cyc_Dict_Dict _tmp99;struct _tuple2*_tmp96=env;_tmp97=*_tmp96;_tmp98=_tmp97.f1;
_tmp99=_tmp97.f2;_tmp98(b1,Cyc_Dict_lookup(_tmp99,a));}void Cyc_Dict_iter2(void(*
f)(void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);void Cyc_Dict_iter2(
void(*f)(void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2){struct _tuple2
_tmp147;struct _tuple2 _tmp9A=(_tmp147.f1=f,((_tmp147.f2=d2,_tmp147)));((void(*)(
void(*f)(struct _tuple2*,void*,void*),struct _tuple2*env,struct Cyc_Dict_Dict d))Cyc_Dict_iter_c)(
Cyc_Dict_iter2_f,& _tmp9A,d1);}struct _tuple3{void(*f1)(void*,void*,void*);struct
Cyc_Dict_Dict f2;void*f3;};static void Cyc_Dict_iter2_c_f(struct _tuple3*env,void*a,
void*b1);static void Cyc_Dict_iter2_c_f(struct _tuple3*env,void*a,void*b1){struct
_tuple3 _tmp9D;void(*_tmp9E)(void*,void*,void*);struct Cyc_Dict_Dict _tmp9F;void*
_tmpA0;struct _tuple3*_tmp9C=env;_tmp9D=*_tmp9C;_tmp9E=_tmp9D.f1;_tmp9F=_tmp9D.f2;
_tmpA0=_tmp9D.f3;_tmp9E(_tmpA0,b1,Cyc_Dict_lookup(_tmp9F,a));}void Cyc_Dict_iter2_c(
void(*f)(void*,void*,void*),void*inner_env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2);void Cyc_Dict_iter2_c(void(*f)(void*,void*,void*),void*inner_env,struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2){struct _tuple3 _tmp148;struct _tuple3 _tmpA1=(_tmp148.f1=f,((
_tmp148.f2=d2,((_tmp148.f3=inner_env,_tmp148)))));((void(*)(void(*f)(struct
_tuple3*,void*,void*),struct _tuple3*env,struct Cyc_Dict_Dict d))Cyc_Dict_iter_c)(
Cyc_Dict_iter2_c_f,& _tmpA1,d1);}struct _tuple4{void*(*f1)(void*,void*,void*,void*,
void*);struct Cyc_Dict_Dict f2;void*f3;};static void*Cyc_Dict_fold2_c_f(struct
_tuple4*env,void*a,void*b1,void*accum);static void*Cyc_Dict_fold2_c_f(struct
_tuple4*env,void*a,void*b1,void*accum){struct _tuple4 _tmpA4;void*(*_tmpA5)(void*,
void*,void*,void*,void*);struct Cyc_Dict_Dict _tmpA6;void*_tmpA7;struct _tuple4*
_tmpA3=env;_tmpA4=*_tmpA3;_tmpA5=_tmpA4.f1;_tmpA6=_tmpA4.f2;_tmpA7=_tmpA4.f3;
return _tmpA5(_tmpA7,a,b1,Cyc_Dict_lookup(_tmpA6,a),accum);}void*Cyc_Dict_fold2_c(
void*(*f)(void*,void*,void*,void*,void*),void*inner_env,struct Cyc_Dict_Dict d1,
struct Cyc_Dict_Dict d2,void*accum);void*Cyc_Dict_fold2_c(void*(*f)(void*,void*,
void*,void*,void*),void*inner_env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2,
void*accum){struct _tuple4 _tmp149;struct _tuple4 _tmpA8=(_tmp149.f1=f,((_tmp149.f2=
d2,((_tmp149.f3=inner_env,_tmp149)))));return((void*(*)(void*(*f)(struct _tuple4*,
void*,void*,void*),struct _tuple4*env,struct Cyc_Dict_Dict d,void*accum))Cyc_Dict_fold_c)(
Cyc_Dict_fold2_c_f,& _tmpA8,d1,accum);}static struct Cyc_Dict_T*Cyc_Dict_copy_tree(
struct _RegionHandle*r2,struct Cyc_Dict_T*t);static struct Cyc_Dict_T*Cyc_Dict_copy_tree(
struct _RegionHandle*r2,struct Cyc_Dict_T*t){if(t == 0)return 0;else{enum Cyc_Dict_Color
_tmpAB;struct Cyc_Dict_T*_tmpAC;struct Cyc_Dict_T*_tmpAD;struct _tuple0 _tmpAE;
struct Cyc_Dict_T _tmpAA=*t;_tmpAB=_tmpAA.color;_tmpAC=_tmpAA.left;_tmpAD=_tmpAA.right;
_tmpAE=_tmpAA.key_val;{struct Cyc_Dict_T*_tmpAF=Cyc_Dict_copy_tree(r2,_tmpAC);
struct Cyc_Dict_T*_tmpB0=Cyc_Dict_copy_tree(r2,_tmpAD);struct Cyc_Dict_T*_tmp14A;
return(_tmp14A=_region_malloc(r2,sizeof(*_tmp14A)),((_tmp14A->color=_tmpAB,((
_tmp14A->left=_tmpAF,((_tmp14A->right=_tmpB0,((_tmp14A->key_val=_tmpAE,_tmp14A)))))))));};}}
struct Cyc_Dict_Dict Cyc_Dict_rcopy(struct _RegionHandle*r2,struct Cyc_Dict_Dict d);
struct Cyc_Dict_Dict Cyc_Dict_rcopy(struct _RegionHandle*r2,struct Cyc_Dict_Dict d){
struct Cyc_Dict_Dict _tmp14B;return(_tmp14B.rel=d.rel,((_tmp14B.r=r2,((_tmp14B.t=
Cyc_Dict_copy_tree(r2,d.t),_tmp14B)))));}struct Cyc_Dict_Dict Cyc_Dict_copy(struct
Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_copy(struct Cyc_Dict_Dict d){return Cyc_Dict_rcopy(
Cyc_Core_heap_region,d);}static struct Cyc_Dict_T*Cyc_Dict_map_tree(struct
_RegionHandle*r,void*(*f)(void*),struct Cyc_Dict_T*t);static struct Cyc_Dict_T*Cyc_Dict_map_tree(
struct _RegionHandle*r,void*(*f)(void*),struct Cyc_Dict_T*t){struct Cyc_Dict_T
_tmpB4;enum Cyc_Dict_Color _tmpB5;struct Cyc_Dict_T*_tmpB6;struct Cyc_Dict_T*_tmpB7;
struct _tuple0 _tmpB8;void*_tmpB9;void*_tmpBA;struct Cyc_Dict_T*_tmpB3=t;_tmpB4=*
_tmpB3;_tmpB5=_tmpB4.color;_tmpB6=_tmpB4.left;_tmpB7=_tmpB4.right;_tmpB8=_tmpB4.key_val;
_tmpB9=_tmpB8.f1;_tmpBA=_tmpB8.f2;{struct Cyc_Dict_T*_tmpBB=_tmpB6 == 0?0: Cyc_Dict_map_tree(
r,f,(struct Cyc_Dict_T*)_tmpB6);void*_tmpBC=f(_tmpBA);struct Cyc_Dict_T*_tmpBD=
_tmpB7 == 0?0: Cyc_Dict_map_tree(r,f,(struct Cyc_Dict_T*)_tmpB7);struct _tuple0
_tmp14E;struct Cyc_Dict_T*_tmp14D;return(_tmp14D=_region_malloc(r,sizeof(*_tmp14D)),((
_tmp14D->color=_tmpB5,((_tmp14D->left=_tmpBB,((_tmp14D->right=_tmpBD,((_tmp14D->key_val=((
_tmp14E.f1=_tmpB9,((_tmp14E.f2=_tmpBC,_tmp14E)))),_tmp14D)))))))));};}struct Cyc_Dict_Dict
Cyc_Dict_rmap(struct _RegionHandle*r,void*(*f)(void*),struct Cyc_Dict_Dict d);
struct Cyc_Dict_Dict Cyc_Dict_rmap(struct _RegionHandle*r,void*(*f)(void*),struct
Cyc_Dict_Dict d){if(d.t == 0){struct Cyc_Dict_Dict _tmp14F;return(_tmp14F.rel=d.rel,((
_tmp14F.r=r,((_tmp14F.t=0,_tmp14F)))));}{struct Cyc_Dict_Dict _tmp150;return(
_tmp150.rel=d.rel,((_tmp150.r=r,((_tmp150.t=Cyc_Dict_map_tree(r,f,(struct Cyc_Dict_T*)
d.t),_tmp150)))));};}struct Cyc_Dict_Dict Cyc_Dict_map(void*(*f)(void*),struct Cyc_Dict_Dict
d);struct Cyc_Dict_Dict Cyc_Dict_map(void*(*f)(void*),struct Cyc_Dict_Dict d){return
Cyc_Dict_rmap(Cyc_Core_heap_region,f,d);}static struct Cyc_Dict_T*Cyc_Dict_map_tree_c(
struct _RegionHandle*r,void*(*f)(void*,void*),void*env,struct Cyc_Dict_T*t);static
struct Cyc_Dict_T*Cyc_Dict_map_tree_c(struct _RegionHandle*r,void*(*f)(void*,void*),
void*env,struct Cyc_Dict_T*t){struct Cyc_Dict_T _tmpC3;enum Cyc_Dict_Color _tmpC4;
struct Cyc_Dict_T*_tmpC5;struct Cyc_Dict_T*_tmpC6;struct _tuple0 _tmpC7;void*_tmpC8;
void*_tmpC9;struct Cyc_Dict_T*_tmpC2=t;_tmpC3=*_tmpC2;_tmpC4=_tmpC3.color;_tmpC5=
_tmpC3.left;_tmpC6=_tmpC3.right;_tmpC7=_tmpC3.key_val;_tmpC8=_tmpC7.f1;_tmpC9=
_tmpC7.f2;{struct Cyc_Dict_T*_tmpCA=_tmpC5 == 0?0: Cyc_Dict_map_tree_c(r,f,env,(
struct Cyc_Dict_T*)_tmpC5);void*_tmpCB=f(env,_tmpC9);struct Cyc_Dict_T*_tmpCC=
_tmpC6 == 0?0: Cyc_Dict_map_tree_c(r,f,env,(struct Cyc_Dict_T*)_tmpC6);struct
_tuple0 _tmp153;struct Cyc_Dict_T*_tmp152;return(_tmp152=_region_malloc(r,sizeof(*
_tmp152)),((_tmp152->color=_tmpC4,((_tmp152->left=_tmpCA,((_tmp152->right=_tmpCC,((
_tmp152->key_val=((_tmp153.f1=_tmpC8,((_tmp153.f2=_tmpCB,_tmp153)))),_tmp152)))))))));};}
struct Cyc_Dict_Dict Cyc_Dict_rmap_c(struct _RegionHandle*r,void*(*f)(void*,void*),
void*env,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rmap_c(struct
_RegionHandle*r,void*(*f)(void*,void*),void*env,struct Cyc_Dict_Dict d){if(d.t == 0){
struct Cyc_Dict_Dict _tmp154;return(_tmp154.rel=d.rel,((_tmp154.r=r,((_tmp154.t=0,
_tmp154)))));}{struct Cyc_Dict_Dict _tmp155;return(_tmp155.rel=d.rel,((_tmp155.r=r,((
_tmp155.t=Cyc_Dict_map_tree_c(r,f,env,(struct Cyc_Dict_T*)d.t),_tmp155)))));};}
struct Cyc_Dict_Dict Cyc_Dict_map_c(void*(*f)(void*,void*),void*env,struct Cyc_Dict_Dict
d);struct Cyc_Dict_Dict Cyc_Dict_map_c(void*(*f)(void*,void*),void*env,struct Cyc_Dict_Dict
d){return Cyc_Dict_rmap_c(Cyc_Core_heap_region,f,env,d);}struct _tuple0*Cyc_Dict_rchoose(
struct _RegionHandle*r,struct Cyc_Dict_Dict d);struct _tuple0*Cyc_Dict_rchoose(
struct _RegionHandle*r,struct Cyc_Dict_Dict d){if(d.t == 0)(int)_throw((void*)& Cyc_Dict_Absent_val);{
struct _tuple0*_tmp156;return(_tmp156=_region_malloc(r,sizeof(*_tmp156)),((
_tmp156->f1=((d.t)->key_val).f1,((_tmp156->f2=((d.t)->key_val).f2,_tmp156)))));};}
static int Cyc_Dict_forall_tree_c(int(*f)(void*,void*,void*),void*env,struct Cyc_Dict_T*
t);static int Cyc_Dict_forall_tree_c(int(*f)(void*,void*,void*),void*env,struct Cyc_Dict_T*
t){struct Cyc_Dict_T _tmpD3;struct Cyc_Dict_T*_tmpD4;struct Cyc_Dict_T*_tmpD5;struct
_tuple0 _tmpD6;void*_tmpD7;void*_tmpD8;struct Cyc_Dict_T*_tmpD2=t;_tmpD3=*_tmpD2;
_tmpD4=_tmpD3.left;_tmpD5=_tmpD3.right;_tmpD6=_tmpD3.key_val;_tmpD7=_tmpD6.f1;
_tmpD8=_tmpD6.f2;return((_tmpD4 == 0  || Cyc_Dict_forall_tree_c(f,env,(struct Cyc_Dict_T*)
_tmpD4)) && f(env,_tmpD7,_tmpD8)) && (_tmpD5 == 0  || Cyc_Dict_forall_tree_c(f,env,(
struct Cyc_Dict_T*)_tmpD5));}int Cyc_Dict_forall_c(int(*f)(void*,void*,void*),void*
env,struct Cyc_Dict_Dict d);int Cyc_Dict_forall_c(int(*f)(void*,void*,void*),void*
env,struct Cyc_Dict_Dict d){if(d.t == 0)return 1;return Cyc_Dict_forall_tree_c(f,env,(
struct Cyc_Dict_T*)d.t);}struct _tuple5{int(*f1)(void*,void*,void*);struct Cyc_Dict_Dict
f2;};static int Cyc_Dict_forall_intersect_f(struct _tuple5*env,void*a,void*b);
static int Cyc_Dict_forall_intersect_f(struct _tuple5*env,void*a,void*b){struct
_tuple5 _tmpDA;int(*_tmpDB)(void*,void*,void*);struct Cyc_Dict_Dict _tmpDC;struct
_tuple5*_tmpD9=env;_tmpDA=*_tmpD9;_tmpDB=_tmpDA.f1;_tmpDC=_tmpDA.f2;if(Cyc_Dict_member(
_tmpDC,a))return _tmpDB(a,b,Cyc_Dict_lookup(_tmpDC,a));return 1;}int Cyc_Dict_forall_intersect(
int(*f)(void*,void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);int Cyc_Dict_forall_intersect(
int(*f)(void*,void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2){struct
_tuple5 _tmp157;struct _tuple5 _tmpDD=(_tmp157.f1=f,((_tmp157.f2=d2,_tmp157)));
return((int(*)(int(*f)(struct _tuple5*,void*,void*),struct _tuple5*env,struct Cyc_Dict_Dict
d))Cyc_Dict_forall_c)(Cyc_Dict_forall_intersect_f,& _tmpDD,d1);}struct _tuple6{
void*(*f1)(void*,void*,void*,void*);void*f2;};static struct Cyc_Dict_Dict*Cyc_Dict_union_f(
struct _tuple6*env,void*a,void*b,struct Cyc_Dict_Dict*d1);static struct Cyc_Dict_Dict*
Cyc_Dict_union_f(struct _tuple6*env,void*a,void*b,struct Cyc_Dict_Dict*d1){if(Cyc_Dict_member(*
d1,a)){void*_tmpDF=Cyc_Dict_lookup(*d1,a);void*_tmpE0=((*env).f1)((*env).f2,a,
_tmpDF,b);if(_tmpE0 != _tmpDF)*d1=Cyc_Dict_insert(*d1,a,_tmpE0);return d1;}*d1=Cyc_Dict_insert(*
d1,a,b);return d1;}struct Cyc_Dict_Dict Cyc_Dict_union_two_c(void*(*f)(void*,void*,
void*,void*),void*env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);struct Cyc_Dict_Dict
Cyc_Dict_union_two_c(void*(*f)(void*,void*,void*,void*),void*env,struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2){if((int)d1.t == (int)d2.t)return d1;if(d1.t == 0)return d2;
if(d2.t == 0)return d1;{struct _tuple6 _tmp158;struct _tuple6 _tmpE1=(_tmp158.f1=f,((
_tmp158.f2=env,_tmp158)));((struct Cyc_Dict_Dict*(*)(struct Cyc_Dict_Dict*(*f)(
struct _tuple6*,void*,void*,struct Cyc_Dict_Dict*),struct _tuple6*env,struct Cyc_Dict_Dict
d,struct Cyc_Dict_Dict*accum))Cyc_Dict_fold_c)(Cyc_Dict_union_f,& _tmpE1,d2,& d1);
return d1;};}struct Cyc_Dict_Dict Cyc_Dict_intersect_c(void*(*f)(void*,void*,void*,
void*),void*env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2);static void _tmp15D(
struct Cyc_Dict_Dict*d2,unsigned int*_tmp15C,unsigned int*_tmp15B,struct Cyc_Dict_T***
_tmp15A){for(*_tmp15C=0;*_tmp15C < *_tmp15B;(*_tmp15C)++){(*_tmp15A)[*_tmp15C]=(
struct Cyc_Dict_T*)(*d2).t;}}static void _tmp163(struct _dyneither_ptr*queue,struct
Cyc_Dict_Dict*d2,unsigned int*_tmp162,unsigned int*_tmp161,struct Cyc_Dict_T***
_tmp15F){for(*_tmp162=0;*_tmp162 < *_tmp161;(*_tmp162)++){(*_tmp15F)[*_tmp162]=*
_tmp162 < _get_dyneither_size(*queue,sizeof(struct Cyc_Dict_T*))?((struct Cyc_Dict_T**)(*
queue).curr)[(int)*_tmp162]:(struct Cyc_Dict_T*)(*d2).t;}}struct Cyc_Dict_Dict Cyc_Dict_intersect_c(
void*(*f)(void*,void*,void*,void*),void*env,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2){if((int)d1.t == (int)d2.t  || d2.t == 0)return d2;{struct Cyc_Dict_T*_tmpE3=0;
struct _RegionHandle _tmpE4=_new_region("temp");struct _RegionHandle*temp=& _tmpE4;
_push_region(temp);{unsigned int _tmp15C;unsigned int _tmp15B;struct Cyc_Dict_T**
_tmp15A;unsigned int _tmp159;struct _dyneither_ptr queue=_tag_dyneither(((_tmp159=(
unsigned int)16,((_tmp15A=(struct Cyc_Dict_T**)_region_malloc(temp,_check_times(
sizeof(struct Cyc_Dict_T*),_tmp159)),((((_tmp15B=_tmp159,_tmp15D(& d2,& _tmp15C,&
_tmp15B,& _tmp15A))),_tmp15A)))))),sizeof(struct Cyc_Dict_T*),(unsigned int)16);
int ind=0;while(ind != - 1){struct Cyc_Dict_T _tmpE6;struct Cyc_Dict_T*_tmpE7;struct
Cyc_Dict_T*_tmpE8;struct _tuple0 _tmpE9;void*_tmpEA;void*_tmpEB;struct Cyc_Dict_T*
_tmpE5=*((struct Cyc_Dict_T**)_check_dyneither_subscript(queue,sizeof(struct Cyc_Dict_T*),
ind --));_tmpE6=*_tmpE5;_tmpE7=_tmpE6.left;_tmpE8=_tmpE6.right;_tmpE9=_tmpE6.key_val;
_tmpEA=_tmpE9.f1;_tmpEB=_tmpE9.f2;if(ind + 2 >= _get_dyneither_size(queue,sizeof(
struct Cyc_Dict_T*))){unsigned int _tmp162;unsigned int _tmp161;struct
_dyneither_ptr _tmp160;struct Cyc_Dict_T**_tmp15F;unsigned int _tmp15E;queue=((
_tmp15E=_get_dyneither_size(queue,sizeof(struct Cyc_Dict_T*))* 2,((_tmp15F=(
struct Cyc_Dict_T**)_region_malloc(temp,_check_times(sizeof(struct Cyc_Dict_T*),
_tmp15E)),((_tmp160=_tag_dyneither(_tmp15F,sizeof(struct Cyc_Dict_T*),_tmp15E),((((
_tmp161=_tmp15E,_tmp163(& queue,& d2,& _tmp162,& _tmp161,& _tmp15F))),_tmp160))))))));}
if(_tmpE7 != 0)*((struct Cyc_Dict_T**)_check_dyneither_subscript(queue,sizeof(
struct Cyc_Dict_T*),++ ind))=(struct Cyc_Dict_T*)_tmpE7;if(_tmpE8 != 0)*((struct Cyc_Dict_T**)
_check_dyneither_subscript(queue,sizeof(struct Cyc_Dict_T*),++ ind))=(struct Cyc_Dict_T*)
_tmpE8;if(Cyc_Dict_member(d1,_tmpEA)){struct _tuple0 _tmp164;_tmpE3=Cyc_Dict_ins(
d2.r,d2.rel,((_tmp164.f1=_tmpEA,((_tmp164.f2=f(env,_tmpEA,Cyc_Dict_lookup(d1,
_tmpEA),_tmpEB),_tmp164)))),_tmpE3);}}}{struct Cyc_Dict_Dict _tmp165;struct Cyc_Dict_Dict
_tmpF5=(_tmp165.rel=d2.rel,((_tmp165.r=d2.r,((_tmp165.t=_tmpE3,_tmp165)))));
_npop_handler(0);return _tmpF5;};;_pop_region(temp);};}static void*Cyc_Dict_intersect_f(
void*(*f)(void*,void*,void*),void*a,void*b1,void*b2);static void*Cyc_Dict_intersect_f(
void*(*f)(void*,void*,void*),void*a,void*b1,void*b2){return f(a,b1,b2);}struct Cyc_Dict_Dict
Cyc_Dict_intersect(void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2);struct Cyc_Dict_Dict Cyc_Dict_intersect(void*(*f)(void*,void*,void*),struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2){return((struct Cyc_Dict_Dict(*)(void*(*f)(void*(*)(void*,
void*,void*),void*,void*,void*),void*(*env)(void*,void*,void*),struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2))Cyc_Dict_intersect_c)(Cyc_Dict_intersect_f,f,d1,d2);}
static struct Cyc_List_List*Cyc_Dict_to_list_f(struct _RegionHandle*r,void*k,void*v,
struct Cyc_List_List*accum);static struct Cyc_List_List*Cyc_Dict_to_list_f(struct
_RegionHandle*r,void*k,void*v,struct Cyc_List_List*accum){struct _tuple0*_tmp168;
struct Cyc_List_List*_tmp167;return(_tmp167=_region_malloc(r,sizeof(*_tmp167)),((
_tmp167->hd=((_tmp168=_region_malloc(r,sizeof(*_tmp168)),((_tmp168->f1=k,((
_tmp168->f2=v,_tmp168)))))),((_tmp167->tl=accum,_tmp167)))));}struct Cyc_List_List*
Cyc_Dict_rto_list(struct _RegionHandle*r,struct Cyc_Dict_Dict d);struct Cyc_List_List*
Cyc_Dict_rto_list(struct _RegionHandle*r,struct Cyc_Dict_Dict d){return((struct Cyc_List_List*(*)(
struct Cyc_List_List*(*f)(struct _RegionHandle*,void*,void*,struct Cyc_List_List*),
struct _RegionHandle*env,struct Cyc_Dict_Dict d,struct Cyc_List_List*accum))Cyc_Dict_fold_c)(
Cyc_Dict_to_list_f,r,d,0);}struct Cyc_List_List*Cyc_Dict_to_list(struct Cyc_Dict_Dict
d);struct Cyc_List_List*Cyc_Dict_to_list(struct Cyc_Dict_Dict d){return Cyc_Dict_rto_list(
Cyc_Core_heap_region,d);}struct _tuple7{int(*f1)(void*,void*);struct _RegionHandle*
f2;};static struct Cyc_Dict_Dict*Cyc_Dict_filter_f(struct _tuple7*env,void*x,void*y,
struct Cyc_Dict_Dict*acc);static struct Cyc_Dict_Dict*Cyc_Dict_filter_f(struct
_tuple7*env,void*x,void*y,struct Cyc_Dict_Dict*acc){struct _tuple7 _tmpF9;int(*
_tmpFA)(void*,void*);struct _RegionHandle*_tmpFB;struct _tuple7*_tmpF8=env;_tmpF9=*
_tmpF8;_tmpFA=_tmpF9.f1;_tmpFB=_tmpF9.f2;if(_tmpFA(x,y))*acc=Cyc_Dict_insert(*
acc,x,y);return acc;}struct Cyc_Dict_Dict Cyc_Dict_rfilter(struct _RegionHandle*r2,
int(*f)(void*,void*),struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_rfilter(
struct _RegionHandle*r2,int(*f)(void*,void*),struct Cyc_Dict_Dict d){struct _tuple7
_tmp169;struct _tuple7 _tmpFC=(_tmp169.f1=f,((_tmp169.f2=r2,_tmp169)));struct Cyc_Dict_Dict
_tmpFD=Cyc_Dict_rempty(r2,d.rel);return*((struct Cyc_Dict_Dict*(*)(struct Cyc_Dict_Dict*(*
f)(struct _tuple7*,void*,void*,struct Cyc_Dict_Dict*),struct _tuple7*env,struct Cyc_Dict_Dict
d,struct Cyc_Dict_Dict*accum))Cyc_Dict_fold_c)(Cyc_Dict_filter_f,& _tmpFC,d,&
_tmpFD);}struct Cyc_Dict_Dict Cyc_Dict_filter(int(*f)(void*,void*),struct Cyc_Dict_Dict
d);struct Cyc_Dict_Dict Cyc_Dict_filter(int(*f)(void*,void*),struct Cyc_Dict_Dict d){
return Cyc_Dict_rfilter(Cyc_Core_heap_region,f,d);}struct _tuple8{int(*f1)(void*,
void*,void*);void*f2;struct _RegionHandle*f3;};static struct Cyc_Dict_Dict*Cyc_Dict_filter_c_f(
struct _tuple8*env,void*x,void*y,struct Cyc_Dict_Dict*acc);static struct Cyc_Dict_Dict*
Cyc_Dict_filter_c_f(struct _tuple8*env,void*x,void*y,struct Cyc_Dict_Dict*acc){
struct _tuple8 _tmp100;int(*_tmp101)(void*,void*,void*);void*_tmp102;struct
_RegionHandle*_tmp103;struct _tuple8*_tmpFF=env;_tmp100=*_tmpFF;_tmp101=_tmp100.f1;
_tmp102=_tmp100.f2;_tmp103=_tmp100.f3;if(_tmp101(_tmp102,x,y))*acc=Cyc_Dict_insert(*
acc,x,y);return acc;}struct Cyc_Dict_Dict Cyc_Dict_rfilter_c(struct _RegionHandle*r2,
int(*f)(void*,void*,void*),void*f_env,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict
Cyc_Dict_rfilter_c(struct _RegionHandle*r2,int(*f)(void*,void*,void*),void*f_env,
struct Cyc_Dict_Dict d){struct _tuple8 _tmp16A;struct _tuple8 _tmp104=(_tmp16A.f1=f,((
_tmp16A.f2=f_env,((_tmp16A.f3=r2,_tmp16A)))));struct Cyc_Dict_Dict _tmp105=Cyc_Dict_rempty(
r2,d.rel);return*((struct Cyc_Dict_Dict*(*)(struct Cyc_Dict_Dict*(*f)(struct
_tuple8*,void*,void*,struct Cyc_Dict_Dict*),struct _tuple8*env,struct Cyc_Dict_Dict
d,struct Cyc_Dict_Dict*accum))Cyc_Dict_fold_c)(Cyc_Dict_filter_c_f,& _tmp104,d,&
_tmp105);}struct Cyc_Dict_Dict Cyc_Dict_filter_c(int(*f)(void*,void*,void*),void*
f_env,struct Cyc_Dict_Dict d);struct Cyc_Dict_Dict Cyc_Dict_filter_c(int(*f)(void*,
void*,void*),void*f_env,struct Cyc_Dict_Dict d){return Cyc_Dict_rfilter_c(Cyc_Core_heap_region,
f,f_env,d);}static int Cyc_Dict_difference_f(struct Cyc_Dict_Dict*d,void*x,void*y);
static int Cyc_Dict_difference_f(struct Cyc_Dict_Dict*d,void*x,void*y){return !Cyc_Dict_member(*
d,x);}struct Cyc_Dict_Dict Cyc_Dict_rdifference(struct _RegionHandle*r2,struct Cyc_Dict_Dict
d1,struct Cyc_Dict_Dict d2);struct Cyc_Dict_Dict Cyc_Dict_rdifference(struct
_RegionHandle*r2,struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict d2){return((struct Cyc_Dict_Dict(*)(
struct _RegionHandle*r2,int(*f)(struct Cyc_Dict_Dict*,void*,void*),struct Cyc_Dict_Dict*
f_env,struct Cyc_Dict_Dict d))Cyc_Dict_rfilter_c)(r2,Cyc_Dict_difference_f,& d2,d1);}
struct Cyc_Dict_Dict Cyc_Dict_difference(struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2);struct Cyc_Dict_Dict Cyc_Dict_difference(struct Cyc_Dict_Dict d1,struct Cyc_Dict_Dict
d2){return Cyc_Dict_rdifference(Cyc_Core_heap_region,d1,d2);}struct _tuple9{int(*
f1)(void*,void*);void*f2;};static int Cyc_Dict_delete_f(struct _tuple9*env,void*x,
void*y);static int Cyc_Dict_delete_f(struct _tuple9*env,void*x,void*y){struct
_tuple9 _tmp108;int(*_tmp109)(void*,void*);void*_tmp10A;struct _tuple9*_tmp107=env;
_tmp108=*_tmp107;_tmp109=_tmp108.f1;_tmp10A=_tmp108.f2;return _tmp109(_tmp10A,x)
!= 0;}struct Cyc_Dict_Dict Cyc_Dict_rdelete(struct _RegionHandle*r2,struct Cyc_Dict_Dict
d,void*x);struct Cyc_Dict_Dict Cyc_Dict_rdelete(struct _RegionHandle*r2,struct Cyc_Dict_Dict
d,void*x){if(!Cyc_Dict_member(d,x))return Cyc_Dict_rcopy(r2,d);{struct _tuple9
_tmp16B;struct _tuple9 _tmp10B=(_tmp16B.f1=d.rel,((_tmp16B.f2=x,_tmp16B)));return((
struct Cyc_Dict_Dict(*)(struct _RegionHandle*r2,int(*f)(struct _tuple9*,void*,void*),
struct _tuple9*f_env,struct Cyc_Dict_Dict d))Cyc_Dict_rfilter_c)(r2,Cyc_Dict_delete_f,&
_tmp10B,d);};}struct Cyc_Dict_Dict Cyc_Dict_rdelete_same(struct Cyc_Dict_Dict d,void*
x);struct Cyc_Dict_Dict Cyc_Dict_rdelete_same(struct Cyc_Dict_Dict d,void*x){if(!Cyc_Dict_member(
d,x))return d;{struct _tuple9 _tmp16C;struct _tuple9 _tmp10D=(_tmp16C.f1=d.rel,((
_tmp16C.f2=x,_tmp16C)));return((struct Cyc_Dict_Dict(*)(struct _RegionHandle*r2,
int(*f)(struct _tuple9*,void*,void*),struct _tuple9*f_env,struct Cyc_Dict_Dict d))
Cyc_Dict_rfilter_c)(d.r,Cyc_Dict_delete_f,& _tmp10D,d);};}struct Cyc_Dict_Dict Cyc_Dict_delete(
struct Cyc_Dict_Dict d,void*x);struct Cyc_Dict_Dict Cyc_Dict_delete(struct Cyc_Dict_Dict
d,void*x){return Cyc_Dict_rdelete(Cyc_Core_heap_region,d,x);}struct _tuple10{
struct _dyneither_ptr f1;int f2;};int Cyc_Dict_iter_f(struct _tuple10*stk,struct
_tuple0*dest);int Cyc_Dict_iter_f(struct _tuple10*stk,struct _tuple0*dest){struct
_tuple10 _tmp110;struct _dyneither_ptr _tmp111;int _tmp112;int*_tmp113;struct
_tuple10*_tmp10F=stk;_tmp110=*_tmp10F;_tmp111=_tmp110.f1;_tmp112=_tmp110.f2;
_tmp113=(int*)&(*_tmp10F).f2;{int _tmp114=*_tmp113;if(_tmp114 == - 1)return 0;{
struct Cyc_Dict_T*_tmp115=*((struct Cyc_Dict_T**)_check_dyneither_subscript(
_tmp111,sizeof(struct Cyc_Dict_T*),_tmp114));*dest=((struct Cyc_Dict_T*)
_check_null(_tmp115))->key_val;-- _tmp114;if((unsigned int)_tmp115->left)*((
struct Cyc_Dict_T**)_check_dyneither_subscript(_tmp111,sizeof(struct Cyc_Dict_T*),
++ _tmp114))=_tmp115->left;if((unsigned int)_tmp115->right)*((struct Cyc_Dict_T**)
_check_dyneither_subscript(_tmp111,sizeof(struct Cyc_Dict_T*),++ _tmp114))=_tmp115->right;*
_tmp113=_tmp114;return 1;};};}struct Cyc_Iter_Iter Cyc_Dict_make_iter(struct
_RegionHandle*rgn,struct Cyc_Dict_Dict d);static void _tmp172(unsigned int*_tmp171,
unsigned int*_tmp170,struct Cyc_Dict_T***_tmp16E,struct Cyc_Dict_T**_tmp116){for(*
_tmp171=0;*_tmp171 < *_tmp170;(*_tmp171)++){(*_tmp16E)[*_tmp171]=*_tmp116;}}
struct Cyc_Iter_Iter Cyc_Dict_make_iter(struct _RegionHandle*rgn,struct Cyc_Dict_Dict
d){int half_max_size=1;struct Cyc_Dict_T*_tmp116=d.t;while(_tmp116 != 0){_tmp116=
_tmp116->left;++ half_max_size;}_tmp116=d.t;{unsigned int _tmp171;unsigned int
_tmp170;struct _dyneither_ptr _tmp16F;struct Cyc_Dict_T**_tmp16E;unsigned int
_tmp16D;struct _dyneither_ptr _tmp117=(_tmp16D=(unsigned int)(2 * half_max_size),((
_tmp16E=(struct Cyc_Dict_T**)_region_malloc(rgn,_check_times(sizeof(struct Cyc_Dict_T*),
_tmp16D)),((_tmp16F=_tag_dyneither(_tmp16E,sizeof(struct Cyc_Dict_T*),_tmp16D),((((
_tmp170=_tmp16D,_tmp172(& _tmp171,& _tmp170,& _tmp16E,& _tmp116))),_tmp16F)))))));
struct _tuple10*_tmp175;struct Cyc_Iter_Iter _tmp174;return(_tmp174.env=(void*)((
_tmp175=_region_malloc(rgn,sizeof(*_tmp175)),((_tmp175->f1=_tmp117,((_tmp175->f2=(
unsigned int)_tmp116?0: - 1,_tmp175)))))),((_tmp174.next=(int(*)(void*env,void*
dest))Cyc_Dict_iter_f,_tmp174)));};}void*Cyc_Dict_marshal(struct _RegionHandle*
rgn,void*env,void*(*write_key)(void*,struct Cyc___cycFILE*,void*),void*(*
write_val)(void*,struct Cyc___cycFILE*,void*),struct Cyc___cycFILE*fp,struct Cyc_Dict_Dict
dict);void*Cyc_Dict_marshal(struct _RegionHandle*rgn,void*env,void*(*write_key)(
void*,struct Cyc___cycFILE*,void*),void*(*write_val)(void*,struct Cyc___cycFILE*,
void*),struct Cyc___cycFILE*fp,struct Cyc_Dict_Dict dict){struct Cyc_List_List*
dict_list=Cyc_Dict_rto_list(rgn,dict);int len=((int(*)(struct Cyc_List_List*x))Cyc_List_length)(
dict_list);{struct Cyc_Core_Failure_struct _tmp17B;const char*_tmp17A;struct Cyc_Core_Failure_struct*
_tmp179;(int)_throw((void*)((_tmp179=_cycalloc(sizeof(*_tmp179)),((_tmp179[0]=((
_tmp17B.tag=Cyc_Core_Failure,((_tmp17B.f1=((_tmp17A="Dict::marshal: Write failure",
_tag_dyneither(_tmp17A,sizeof(char),29))),_tmp17B)))),_tmp179)))));}while(
dict_list != 0){env=((void*(*)(void*,struct Cyc___cycFILE*,struct _tuple0*))
write_key)(env,fp,(struct _tuple0*)dict_list->hd);env=((void*(*)(void*,struct Cyc___cycFILE*,
struct _tuple0*))write_val)(env,fp,(struct _tuple0*)dict_list->hd);dict_list=
dict_list->tl;}return env;}struct Cyc_Dict_Dict Cyc_Dict_unmarshal(struct
_RegionHandle*rgn,void*env,int(*cmp)(void*,void*),void*(*read_key)(void*,struct
Cyc___cycFILE*),void*(*read_val)(void*,struct Cyc___cycFILE*),struct Cyc___cycFILE*
fp);struct Cyc_Dict_Dict Cyc_Dict_unmarshal(struct _RegionHandle*rgn,void*env,int(*
cmp)(void*,void*),void*(*read_key)(void*,struct Cyc___cycFILE*),void*(*read_val)(
void*,struct Cyc___cycFILE*),struct Cyc___cycFILE*fp){struct Cyc_Dict_Dict dict=Cyc_Dict_empty(
cmp);int len=Cyc_getw(fp);if(len == - 1){struct Cyc_Core_Failure_struct _tmp181;const
char*_tmp180;struct Cyc_Core_Failure_struct*_tmp17F;(int)_throw((void*)((_tmp17F=
_cycalloc(sizeof(*_tmp17F)),((_tmp17F[0]=((_tmp181.tag=Cyc_Core_Failure,((
_tmp181.f1=((_tmp180="Dict::unmarshal: list length is -1",_tag_dyneither(_tmp180,
sizeof(char),35))),_tmp181)))),_tmp17F)))));}{int i=0;for(0;i < len;++ i){void*key=
read_key(env,fp);void*val=read_val(env,fp);dict=Cyc_Dict_insert(dict,key,val);}}
return dict;}