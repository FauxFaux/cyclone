extern void __assert_fail (__const char *__assertion, __const char *__file,
                           unsigned int __line, __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert_perror_fail (int __errnum, __const char *__file,
                                  unsigned int __line,
                                  __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert (const char *__assertion, const char *__file, int __line)
             __attribute__ ((__noreturn__));
typedef unsigned int size_t;
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
typedef unsigned long long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned int __nlink_t;
typedef long int __off_t;
typedef long long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef int __daddr_t;
typedef long int __swblk_t;
typedef int __key_t;
typedef int __clockid_t;
typedef int __timer_t;
typedef long int __blksize_t;
typedef long int __blkcnt_t;
typedef long long int __blkcnt64_t;
typedef unsigned long int __fsblkcnt_t;
typedef unsigned long long int __fsblkcnt64_t;
typedef unsigned long int __fsfilcnt_t;
typedef unsigned long long int __fsfilcnt64_t;
typedef int __ssize_t;
typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;
typedef int __intptr_t;
typedef unsigned int __socklen_t;
typedef struct _IO_FILE FILE;
typedef struct _IO_FILE __FILE;
typedef long int wchar_t;
typedef unsigned int wint_t;
typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    char __wchb[4];
  } __value;
} __mbstate_t;
typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
enum
{
  __GCONV_OK = 0,
  __GCONV_NOCONV,
  __GCONV_NODB,
  __GCONV_NOMEM,
  __GCONV_EMPTY_INPUT,
  __GCONV_FULL_OUTPUT,
  __GCONV_ILLEGAL_INPUT,
  __GCONV_INCOMPLETE_INPUT,
  __GCONV_ILLEGAL_DESCRIPTOR,
  __GCONV_INTERNAL_ERROR
};
enum
{
  __GCONV_IS_LAST = 0x0001,
  __GCONV_IGNORE_ERRORS = 0x0002
};
struct __gconv_step;
struct __gconv_step_data;
struct __gconv_loaded_object;
struct __gconv_trans_data;
typedef int (*__gconv_fct) (struct __gconv_step *, struct __gconv_step_data *,
                            __const unsigned char **, __const unsigned char *,
                            unsigned char **, size_t *, int, int);
typedef wint_t (*__gconv_btowc_fct) (struct __gconv_step *, unsigned char);
typedef int (*__gconv_init_fct) (struct __gconv_step *);
typedef void (*__gconv_end_fct) (struct __gconv_step *);
typedef int (*__gconv_trans_fct) (struct __gconv_step *,
                                  struct __gconv_step_data *, void *,
                                  __const unsigned char *,
                                  __const unsigned char **,
                                  __const unsigned char *, unsigned char **,
                                  size_t *);
typedef int (*__gconv_trans_context_fct) (void *, __const unsigned char *,
                                          __const unsigned char *,
                                          unsigned char *, unsigned char *);
typedef int (*__gconv_trans_query_fct) (__const char *, __const char ***,
                                        size_t *);
typedef int (*__gconv_trans_init_fct) (void **, const char *);
typedef void (*__gconv_trans_end_fct) (void *);
struct __gconv_trans_data
{
  __gconv_trans_fct __trans_fct;
  __gconv_trans_context_fct __trans_context_fct;
  __gconv_trans_end_fct __trans_end_fct;
  void *__data;
  struct __gconv_trans_data *__next;
};
struct __gconv_step
{
  struct __gconv_loaded_object *__shlib_handle;
  __const char *__modname;
  int __counter;
  char *__from_name;
  char *__to_name;
  __gconv_fct __fct;
  __gconv_btowc_fct __btowc_fct;
  __gconv_init_fct __init_fct;
  __gconv_end_fct __end_fct;
  int __min_needed_from;
  int __max_needed_from;
  int __min_needed_to;
  int __max_needed_to;
  int __stateful;
  void *__data;
};
struct __gconv_step_data
{
  unsigned char *__outbuf;
  unsigned char *__outbufend;
  int __flags;
  int __invocation_counter;
  int __internal_use;
  __mbstate_t *__statep;
  __mbstate_t __state;
  struct __gconv_trans_data *__trans;
};
typedef struct __gconv_info
{
  size_t __nsteps;
  struct __gconv_step *__steps;
  __extension__ struct __gconv_step_data __data [0];
} *__gconv_t;
typedef union
{
  struct __gconv_info __cd;
  struct
  {
    struct __gconv_info __cd;
    struct __gconv_step_data __data;
  } __combined;
} _G_iconv_t;
typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));
typedef __builtin_va_list __gnuc_va_list;
struct _IO_jump_t; struct _IO_FILE;
typedef void _IO_lock_t;
struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;
  int _pos;
};
enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
struct _IO_FILE {
  int _flags;
  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;
  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;
  struct _IO_marker *_markers;
  struct _IO_FILE *_chain;
  int _fileno;
  int _flags2;
  __off_t _old_offset;
  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];
  _IO_lock_t *_lock;
  __off64_t _offset;
  void *__pad1;
  void *__pad2;
  int _mode;
  char _unused2[15 * sizeof (int) - 2 * sizeof (void *)];
};
typedef struct _IO_FILE _IO_FILE;
struct _IO_FILE_plus;
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);
typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
                                 size_t __n);
typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);
typedef int __io_close_fn (void *__cookie);
extern int __underflow (_IO_FILE *) ;
extern int __uflow (_IO_FILE *) ;
extern int __overflow (_IO_FILE *, int) ;
extern wint_t __wunderflow (_IO_FILE *) ;
extern wint_t __wuflow (_IO_FILE *) ;
extern wint_t __woverflow (_IO_FILE *, wint_t) ;
extern int _IO_getc (_IO_FILE *__fp) ;
extern int _IO_putc (int __c, _IO_FILE *__fp) ;
extern int _IO_feof (_IO_FILE *__fp) ;
extern int _IO_ferror (_IO_FILE *__fp) ;
extern int _IO_peekc_locked (_IO_FILE *__fp) ;
extern void _IO_flockfile (_IO_FILE *) ;
extern void _IO_funlockfile (_IO_FILE *) ;
extern int _IO_ftrylockfile (_IO_FILE *) ;
extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
                        __gnuc_va_list, int *__restrict) ;
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
                         __gnuc_va_list) ;
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t) ;
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t) ;
extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int) ;
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int) ;
extern void _IO_free_backup_area (_IO_FILE *) ;
typedef _G_fpos_t fpos_t;
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;
extern int remove (__const char *__filename) ;
extern int rename (__const char *__old, __const char *__new) ;
extern FILE *tmpfile (void) ;
extern char *tmpnam (char *__s) ;
extern char *tmpnam_r (char *__s) ;
extern char *tempnam (__const char *__dir, __const char *__pfx)
             __attribute__ ((__malloc__));
extern int fclose (FILE *__stream) ;
extern int fflush (FILE *__stream) ;
extern int fflush_unlocked (FILE *__stream) ;
extern FILE *fopen (__const char *__restrict __filename,
                    __const char *__restrict __modes) ;
extern FILE *freopen (__const char *__restrict __filename,
                      __const char *__restrict __modes,
                      FILE *__restrict __stream) ;
extern FILE *fdopen (int __fd, __const char *__modes) ;
extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) ;
extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
                    int __modes, size_t __n) ;
extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
                       size_t __size) ;
extern void setlinebuf (FILE *__stream) ;
extern int fprintf (FILE *__restrict __stream,
                    __const char *__restrict __format, ...) ;
extern int printf (__const char *__restrict __format, ...) ;
extern int sprintf (char *__restrict __s,
                    __const char *__restrict __format, ...) ;
extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
                     __gnuc_va_list __arg) ;
extern int vprintf (__const char *__restrict __format, __gnuc_va_list __arg)
            ;
extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
                     __gnuc_va_list __arg) ;
extern int snprintf (char *__restrict __s, size_t __maxlen,
                     __const char *__restrict __format, ...)
             __attribute__ ((__format__ (__printf__, 3, 4)));
extern int vsnprintf (char *__restrict __s, size_t __maxlen,
                      __const char *__restrict __format, __gnuc_va_list __arg)
             __attribute__ ((__format__ (__printf__, 3, 0)));
extern int fscanf (FILE *__restrict __stream,
                   __const char *__restrict __format, ...) ;
extern int scanf (__const char *__restrict __format, ...) ;
extern int sscanf (__const char *__restrict __s,
                   __const char *__restrict __format, ...) ;
extern int fgetc (FILE *__stream) ;
extern int getc (FILE *__stream) ;
extern int getchar (void) ;
extern int getc_unlocked (FILE *__stream) ;
extern int getchar_unlocked (void) ;
extern int fgetc_unlocked (FILE *__stream) ;
extern int fputc (int __c, FILE *__stream) ;
extern int putc (int __c, FILE *__stream) ;
extern int putchar (int __c) ;
extern int fputc_unlocked (int __c, FILE *__stream) ;
extern int putc_unlocked (int __c, FILE *__stream) ;
extern int putchar_unlocked (int __c) ;
extern int getw (FILE *__stream) ;
extern int putw (int __w, FILE *__stream) ;
extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
            ;
extern char *gets (char *__s) ;
extern int fputs (__const char *__restrict __s, FILE *__restrict __stream)
            ;
extern int puts (__const char *__s) ;
extern int ungetc (int __c, FILE *__stream) ;
extern size_t fread (void *__restrict __ptr, size_t __size,
                     size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
                      size_t __n, FILE *__restrict __s) ;
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
                              size_t __n, FILE *__restrict __stream) ;
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
                               size_t __n, FILE *__restrict __stream) ;
extern int fseek (FILE *__stream, long int __off, int __whence) ;
extern long int ftell (FILE *__stream) ;
extern void rewind (FILE *__stream) ;
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos)
            ;
extern int fsetpos (FILE *__stream, __const fpos_t *__pos) ;
extern void clearerr (FILE *__stream) ;
extern int feof (FILE *__stream) ;
extern int ferror (FILE *__stream) ;
extern void clearerr_unlocked (FILE *__stream) ;
extern int feof_unlocked (FILE *__stream) ;
extern int ferror_unlocked (FILE *__stream) ;
extern void perror (__const char *__s) ;
extern int sys_nerr;
extern __const char *__const sys_errlist[];
extern int fileno (FILE *__stream) ;
extern int fileno_unlocked (FILE *__stream) ;
extern FILE *popen (__const char *__command, __const char *__modes) ;
extern int pclose (FILE *__stream) ;
extern char *ctermid (char *__s) ;
extern void flockfile (FILE *__stream) ;
extern int ftrylockfile (FILE *__stream) ;
extern void funlockfile (FILE *__stream) ;
extern __inline int
vprintf (__const char *__restrict __fmt, __gnuc_va_list __arg)
{
  return vfprintf (stdout, __fmt, __arg);
}
extern __inline int
getchar (void)
{
  return _IO_getc (stdin);
}
extern __inline int
getc_unlocked (FILE *__fp)
{
  return ((__fp)->_IO_read_ptr >= (__fp)->_IO_read_end ? __uflow (__fp) : *(unsigned char *) (__fp)->_IO_read_ptr++);
}
extern __inline int
getchar_unlocked (void)
{
  return ((stdin)->_IO_read_ptr >= (stdin)->_IO_read_end ? __uflow (stdin) : *(unsigned char *) (stdin)->_IO_read_ptr++);
}
extern __inline int
putchar (int __c)
{
  return _IO_putc (__c, stdout);
}
extern __inline int
fputc_unlocked (int __c, FILE *__stream)
{
  return (((__stream)->_IO_write_ptr >= (__stream)->_IO_write_end) ? __overflow (__stream, (unsigned char) (__c)) : (unsigned char) (*(__stream)->_IO_write_ptr++ = (__c)));
}
extern __inline int
putc_unlocked (int __c, FILE *__stream)
{
  return (((__stream)->_IO_write_ptr >= (__stream)->_IO_write_end) ? __overflow (__stream, (unsigned char) (__c)) : (unsigned char) (*(__stream)->_IO_write_ptr++ = (__c)));
}
extern __inline int
putchar_unlocked (int __c)
{
  return (((stdout)->_IO_write_ptr >= (stdout)->_IO_write_end) ? __overflow (stdout, (unsigned char) (__c)) : (unsigned char) (*(stdout)->_IO_write_ptr++ = (__c)));
}
extern __inline int
feof_unlocked (FILE *__stream)
{
  return (((__stream)->_flags & 0x10) != 0);
}
extern __inline int
ferror_unlocked (FILE *__stream)
{
  return (((__stream)->_flags & 0x20) != 0);
}
extern void *memcpy (void *__restrict __dest,
                     __const void *__restrict __src, size_t __n) ;
extern void *memmove (void *__dest, __const void *__src, size_t __n)
            ;
extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
                      int __c, size_t __n)
            ;
extern void *memset (void *__s, int __c, size_t __n) ;
extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
             __attribute__ ((__pure__));
extern void *memchr (__const void *__s, int __c, size_t __n)
              __attribute__ ((__pure__));
extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
            ;
extern char *strncpy (char *__restrict __dest,
                      __const char *__restrict __src, size_t __n) ;
extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
            ;
extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
                      size_t __n) ;
extern int strcmp (__const char *__s1, __const char *__s2)
             __attribute__ ((__pure__));
extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
             __attribute__ ((__pure__));
extern int strcoll (__const char *__s1, __const char *__s2)
             __attribute__ ((__pure__));
extern size_t strxfrm (char *__restrict __dest,
                       __const char *__restrict __src, size_t __n) ;
extern char *strdup (__const char *__s) __attribute__ ((__malloc__));
extern char *strchr (__const char *__s, int __c) __attribute__ ((__pure__));
extern char *strrchr (__const char *__s, int __c) __attribute__ ((__pure__));
extern size_t strcspn (__const char *__s, __const char *__reject)
             __attribute__ ((__pure__));
extern size_t strspn (__const char *__s, __const char *__accept)
             __attribute__ ((__pure__));
extern char *strpbrk (__const char *__s, __const char *__accept)
             __attribute__ ((__pure__));
extern char *strstr (__const char *__haystack, __const char *__needle)
             __attribute__ ((__pure__));
extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
            ;
extern char *__strtok_r (char *__restrict __s,
                         __const char *__restrict __delim,
                         char **__restrict __save_ptr) ;
extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
                       char **__restrict __save_ptr) ;
extern size_t strlen (__const char *__s) __attribute__ ((__pure__));
extern char *strerror (int __errnum) ;
extern char *strerror_r (int __errnum, char *__buf, size_t __buflen) ;
extern void __bzero (void *__s, size_t __n) ;
extern void bcopy (__const void *__src, void *__dest, size_t __n) ;
extern void bzero (void *__s, size_t __n) ;
extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
             __attribute__ ((__pure__));
extern char *index (__const char *__s, int __c) __attribute__ ((__pure__));
extern char *rindex (__const char *__s, int __c) __attribute__ ((__pure__));
extern int ffs (int __i) __attribute__ ((__const__));
extern int strcasecmp (__const char *__s1, __const char *__s2)
             __attribute__ ((__pure__));
extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
             __attribute__ ((__pure__));
extern char *strsep (char **__restrict __stringp,
                     __const char *__restrict __delim) ;
extern void *__rawmemchr (const void *__s, int __c);
extern __inline char *__strcpy_small (char *, __uint16_t, __uint16_t,
                                      __uint32_t, __uint32_t, size_t);
extern __inline char *
__strcpy_small (char *__dest,
                __uint16_t __src0_2, __uint16_t __src4_2,
                __uint32_t __src0_4, __uint32_t __src4_4,
                size_t __srclen)
{
  union {
    __uint32_t __ui;
    __uint16_t __usi;
    unsigned char __uc;
  } *__u = (void *) __dest;
  switch ((unsigned int) __srclen)
    {
    case 1:
      __u->__uc = '\0';
      break;
    case 2:
      __u->__usi = __src0_2;
      break;
    case 3:
      __u->__usi = __src0_2;
      __u = __extension__ ((void *) __u + 2);
      __u->__uc = '\0';
      break;
    case 4:
      __u->__ui = __src0_4;
      break;
    case 5:
      __u->__ui = __src0_4;
      __u = __extension__ ((void *) __u + 4);
      __u->__uc = '\0';
      break;
    case 6:
      __u->__ui = __src0_4;
      __u = __extension__ ((void *) __u + 4);
      __u->__usi = __src4_2;
      break;
    case 7:
      __u->__ui = __src0_4;
      __u = __extension__ ((void *) __u + 4);
      __u->__usi = __src4_2;
      __u = __extension__ ((void *) __u + 2);
      __u->__uc = '\0';
      break;
    case 8:
      __u->__ui = __src0_4;
      __u = __extension__ ((void *) __u + 4);
      __u->__ui = __src4_4;
      break;
    }
  return __dest;
}
extern __inline size_t __strcspn_c1 (__const char *__s, int __reject);
extern __inline size_t
__strcspn_c1 (__const char *__s, int __reject)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject)
    ++__result;
  return __result;
}
extern __inline size_t __strcspn_c2 (__const char *__s, int __reject1,
                                     int __reject2);
extern __inline size_t
__strcspn_c2 (__const char *__s, int __reject1, int __reject2)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
         && __s[__result] != __reject2)
    ++__result;
  return __result;
}
extern __inline size_t __strcspn_c3 (__const char *__s, int __reject1,
                                     int __reject2, int __reject3);
extern __inline size_t
__strcspn_c3 (__const char *__s, int __reject1, int __reject2,
              int __reject3)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
         && __s[__result] != __reject2 && __s[__result] != __reject3)
    ++__result;
  return __result;
}
extern __inline size_t __strspn_c1 (__const char *__s, int __accept);
extern __inline size_t
__strspn_c1 (__const char *__s, int __accept)
{
  register size_t __result = 0;
  while (__s[__result] == __accept)
    ++__result;
  return __result;
}
extern __inline size_t __strspn_c2 (__const char *__s, int __accept1,
                                    int __accept2);
extern __inline size_t
__strspn_c2 (__const char *__s, int __accept1, int __accept2)
{
  register size_t __result = 0;
  while (__s[__result] == __accept1 || __s[__result] == __accept2)
    ++__result;
  return __result;
}
extern __inline size_t __strspn_c3 (__const char *__s, int __accept1,
                                    int __accept2, int __accept3);
extern __inline size_t
__strspn_c3 (__const char *__s, int __accept1, int __accept2, int __accept3)
{
  register size_t __result = 0;
  while (__s[__result] == __accept1 || __s[__result] == __accept2
         || __s[__result] == __accept3)
    ++__result;
  return __result;
}
extern __inline char *__strpbrk_c2 (__const char *__s, int __accept1,
                                     int __accept2);
extern __inline char *
__strpbrk_c2 (__const char *__s, int __accept1, int __accept2)
{
  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2)
    ++__s;
  return *__s == '\0' ? ((void *)0) : (char *) (size_t) __s;
}
extern __inline char *__strpbrk_c3 (__const char *__s, int __accept1,
                                     int __accept2, int __accept3);
extern __inline char *
__strpbrk_c3 (__const char *__s, int __accept1, int __accept2,
              int __accept3)
{
  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2
         && *__s != __accept3)
    ++__s;
  return *__s == '\0' ? ((void *)0) : (char *) (size_t) __s;
}
extern __inline char *__strtok_r_1c (char *__s, char __sep, char **__nextp);
extern __inline char *
__strtok_r_1c (char *__s, char __sep, char **__nextp)
{
  char *__result;
  if (__s == ((void *)0))
    __s = *__nextp;
  while (*__s == __sep)
    ++__s;
  __result = ((void *)0);
  if (*__s != '\0')
    {
      __result = __s++;
      while (*__s != '\0')
        if (*__s++ == __sep)
          {
            __s[-1] = '\0';
            break;
          }
      *__nextp = __s;
    }
  return __result;
}
extern char *__strsep_g (char **__stringp, __const char *__delim);
extern __inline char *__strsep_1c (char **__s, char __reject);
extern __inline char *
__strsep_1c (char **__s, char __reject)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0) && (*__s = (__extension__ (__builtin_constant_p (__reject) && (__reject) == '\0' ? (char *) __rawmemchr (__retval, __reject) : strchr (__retval, __reject)))) != ((void *)0))
    *(*__s)++ = '\0';
  return __retval;
}
extern __inline char *__strsep_2c (char **__s, char __reject1, char __reject2);
extern __inline char *
__strsep_2c (char **__s, char __reject1, char __reject2)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0))
    {
      register char *__cp = __retval;
      while (1)
        {
          if (*__cp == '\0')
            {
              __cp = ((void *)0);
          break;
            }
          if (*__cp == __reject1 || *__cp == __reject2)
            {
              *__cp++ = '\0';
              break;
            }
          ++__cp;
        }
      *__s = __cp;
    }
  return __retval;
}
extern __inline char *__strsep_3c (char **__s, char __reject1, char __reject2,
                                   char __reject3);
extern __inline char *
__strsep_3c (char **__s, char __reject1, char __reject2, char __reject3)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0))
    {
      register char *__cp = __retval;
      while (1)
        {
          if (*__cp == '\0')
            {
              __cp = ((void *)0);
          break;
            }
          if (*__cp == __reject1 || *__cp == __reject2 || *__cp == __reject3)
            {
              *__cp++ = '\0';
              break;
            }
          ++__cp;
        }
      *__s = __cp;
    }
  return __retval;
}
extern void *malloc (size_t __size) __attribute__ ((__malloc__));
extern void *calloc (size_t __nmemb, size_t __size)
             __attribute__ ((__malloc__));
extern char *__strdup (__const char *__string) __attribute__ ((__malloc__));
extern char *__strndup (__const char *__string, size_t __n)
             __attribute__ ((__malloc__));
typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
__extension__
typedef long long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
__extension__
typedef unsigned long long int uint64_t;
typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;
__extension__
typedef long long int int_least64_t;
typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
__extension__
typedef unsigned long long int uint_least64_t;
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;
typedef unsigned char uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
typedef int intptr_t;
typedef unsigned int uintptr_t;
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
typedef struct
  {
    int quot;
    int rem;
  } div_t;
typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;
extern size_t __ctype_get_mb_cur_max (void) ;
extern double atof (__const char *__nptr) __attribute__ ((__pure__));
extern int atoi (__const char *__nptr) __attribute__ ((__pure__));
extern long int atol (__const char *__nptr) __attribute__ ((__pure__));
__extension__ extern long long int atoll (__const char *__nptr)
             __attribute__ ((__pure__));
extern double strtod (__const char *__restrict __nptr,
                      char **__restrict __endptr) ;
extern long int strtol (__const char *__restrict __nptr,
                        char **__restrict __endptr, int __base) ;
extern unsigned long int strtoul (__const char *__restrict __nptr,
                                  char **__restrict __endptr, int __base)
            ;
__extension__
extern long long int strtoq (__const char *__restrict __nptr,
                             char **__restrict __endptr, int __base) ;
__extension__
extern unsigned long long int strtouq (__const char *__restrict __nptr,
                                       char **__restrict __endptr, int __base)
            ;
__extension__
extern long long int strtoll (__const char *__restrict __nptr,
                              char **__restrict __endptr, int __base) ;
__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
                                        char **__restrict __endptr, int __base)
            ;
extern double __strtod_internal (__const char *__restrict __nptr,
                                 char **__restrict __endptr, int __group)
            ;
extern float __strtof_internal (__const char *__restrict __nptr,
                                char **__restrict __endptr, int __group)
            ;
extern long double __strtold_internal (__const char *__restrict __nptr,
                                       char **__restrict __endptr,
                                       int __group) ;
extern long int __strtol_internal (__const char *__restrict __nptr,
                                   char **__restrict __endptr,
                                   int __base, int __group) ;
extern unsigned long int __strtoul_internal (__const char *__restrict __nptr,
                                             char **__restrict __endptr,
                                             int __base, int __group) ;
__extension__
extern long long int __strtoll_internal (__const char *__restrict __nptr,
                                         char **__restrict __endptr,
                                         int __base, int __group) ;
__extension__
extern unsigned long long int __strtoull_internal (__const char *
                                                   __restrict __nptr,
                                                   char **__restrict __endptr,
                                                   int __base, int __group)
            ;
extern __inline double
strtod (__const char *__restrict __nptr, char **__restrict __endptr)
{
  return __strtod_internal (__nptr, __endptr, 0);
}
extern __inline long int
strtol (__const char *__restrict __nptr, char **__restrict __endptr,
        int __base)
{
  return __strtol_internal (__nptr, __endptr, __base, 0);
}
extern __inline unsigned long int
strtoul (__const char *__restrict __nptr, char **__restrict __endptr,
         int __base)
{
  return __strtoul_internal (__nptr, __endptr, __base, 0);
}
__extension__ extern __inline long long int
strtoq (__const char *__restrict __nptr, char **__restrict __endptr,
        int __base)
{
  return __strtoll_internal (__nptr, __endptr, __base, 0);
}
__extension__ extern __inline unsigned long long int
strtouq (__const char *__restrict __nptr, char **__restrict __endptr,
         int __base)
{
  return __strtoull_internal (__nptr, __endptr, __base, 0);
}
__extension__ extern __inline long long int
strtoll (__const char *__restrict __nptr, char **__restrict __endptr,
         int __base)
{
  return __strtoll_internal (__nptr, __endptr, __base, 0);
}
__extension__ extern __inline unsigned long long int
strtoull (__const char * __restrict __nptr, char **__restrict __endptr,
          int __base)
{
  return __strtoull_internal (__nptr, __endptr, __base, 0);
}
extern __inline double
atof (__const char *__nptr)
{
  return strtod (__nptr, (char **) ((void *)0));
}
extern __inline int
atoi (__const char *__nptr)
{
  return (int) strtol (__nptr, (char **) ((void *)0), 10);
}
extern __inline long int
atol (__const char *__nptr)
{
  return strtol (__nptr, (char **) ((void *)0), 10);
}
__extension__ extern __inline long long int
atoll (__const char *__nptr)
{
  return strtoll (__nptr, (char **) ((void *)0), 10);
}
extern char *l64a (long int __n) ;
extern long int a64l (__const char *__s) __attribute__ ((__pure__));
typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;
typedef __loff_t loff_t;
typedef __ino_t ino_t;
typedef __dev_t dev_t;
typedef __gid_t gid_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __uid_t uid_t;
typedef __off_t off_t;
typedef __pid_t pid_t;
typedef __id_t id_t;
typedef __ssize_t ssize_t;
typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;
typedef __key_t key_t;
typedef __time_t time_t;
typedef __clockid_t clockid_t;
typedef __timer_t timer_t;
typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));
typedef int register_t __attribute__ ((__mode__ (__word__)));
typedef int __sig_atomic_t;
typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
typedef __sigset_t sigset_t;
struct timespec
  {
    __time_t tv_sec;
    long int tv_nsec;
  };
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
typedef __suseconds_t suseconds_t;
typedef long int __fd_mask;
typedef struct
  {
    __fd_mask __fds_bits[1024 / (8 * sizeof (__fd_mask))];
  } fd_set;
typedef __fd_mask fd_mask;
extern int select (int __nfds, fd_set *__restrict __readfds,
                   fd_set *__restrict __writefds,
                   fd_set *__restrict __exceptfds,
                   struct timeval *__restrict __timeout) ;
typedef __blkcnt_t blkcnt_t;
typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;
struct __sched_param
  {
    int __sched_priority;
  };
struct _pthread_fastlock
{
  long int __status;
  int __spinlock;
};
typedef struct _pthread_descr_struct *_pthread_descr;
typedef struct __pthread_attr_s
{
  int __detachstate;
  int __schedpolicy;
  struct __sched_param __schedparam;
  int __inheritsched;
  int __scope;
  size_t __guardsize;
  int __stackaddr_set;
  void *__stackaddr;
  size_t __stacksize;
} pthread_attr_t;
__extension__ typedef long long __pthread_cond_align_t;
typedef struct
{
  struct _pthread_fastlock __c_lock;
  _pthread_descr __c_waiting;
  char __padding[48 - sizeof (struct _pthread_fastlock)
                 - sizeof (_pthread_descr) - sizeof (__pthread_cond_align_t)];
  __pthread_cond_align_t __align;
} pthread_cond_t;
typedef struct
{
  int __dummy;
} pthread_condattr_t;
typedef unsigned int pthread_key_t;
typedef struct
{
  int __m_reserved;
  int __m_count;
  _pthread_descr __m_owner;
  int __m_kind;
  struct _pthread_fastlock __m_lock;
} pthread_mutex_t;
typedef struct
{
  int __mutexkind;
} pthread_mutexattr_t;
typedef int pthread_once_t;
typedef unsigned long int pthread_t;
extern long int random (void) ;
extern void srandom (unsigned int __seed) ;
extern char *initstate (unsigned int __seed, char *__statebuf,
                        size_t __statelen) ;
extern char *setstate (char *__statebuf) ;
struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };
extern int random_r (struct random_data *__restrict __buf,
                     int32_t *__restrict __result) ;
extern int srandom_r (unsigned int __seed, struct random_data *__buf) ;
extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
                        size_t __statelen,
                        struct random_data *__restrict __buf) ;
extern int setstate_r (char *__restrict __statebuf,
                       struct random_data *__restrict __buf) ;
extern int rand (void) ;
extern void srand (unsigned int __seed) ;
extern int rand_r (unsigned int *__seed) ;
extern double drand48 (void) ;
extern double erand48 (unsigned short int __xsubi[3]) ;
extern long int lrand48 (void) ;
extern long int nrand48 (unsigned short int __xsubi[3]) ;
extern long int mrand48 (void) ;
extern long int jrand48 (unsigned short int __xsubi[3]) ;
extern void srand48 (long int __seedval) ;
extern unsigned short int *seed48 (unsigned short int __seed16v[3]) ;
extern void lcong48 (unsigned short int __param[7]) ;
struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;
  };
extern int drand48_r (struct drand48_data *__restrict __buffer,
                      double *__restrict __result) ;
extern int erand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      double *__restrict __result) ;
extern int lrand48_r (struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int nrand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int mrand48_r (struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int jrand48_r (unsigned short int __xsubi[3],
                      struct drand48_data *__restrict __buffer,
                      long int *__restrict __result) ;
extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
            ;
extern int seed48_r (unsigned short int __seed16v[3],
                     struct drand48_data *__buffer) ;
extern int lcong48_r (unsigned short int __param[7],
                      struct drand48_data *__buffer) ;
extern void *realloc (void *__ptr, size_t __size) __attribute__ ((__malloc__));
extern void free (void *__ptr) ;
extern void cfree (void *__ptr) ;
extern void *alloca (size_t __size) ;
extern void *valloc (size_t __size) __attribute__ ((__malloc__));
extern void abort (void) __attribute__ ((__noreturn__));
extern int atexit (void (*__func) (void)) ;
extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
            ;
extern void exit (int __status) __attribute__ ((__noreturn__));
extern char *getenv (__const char *__name) ;
extern char *__secure_getenv (__const char *__name) ;
extern int putenv (char *__string) ;
extern int setenv (__const char *__name, __const char *__value, int __replace)
            ;
extern int unsetenv (__const char *__name) ;
extern int clearenv (void) ;
extern char *mktemp (char *__template) ;
extern int mkstemp (char *__template) ;
extern char *mkdtemp (char *__template) ;
extern int system (__const char *__command) ;
extern char *realpath (__const char *__restrict __name,
                       char *__restrict __resolved) ;
typedef int (*__compar_fn_t) (__const void *, __const void *);
extern void *bsearch (__const void *__key, __const void *__base,
                      size_t __nmemb, size_t __size, __compar_fn_t __compar);
extern void qsort (void *__base, size_t __nmemb, size_t __size,
                   __compar_fn_t __compar);
extern int abs (int __x) __attribute__ ((__const__));
extern long int labs (long int __x) __attribute__ ((__const__));
extern div_t div (int __numer, int __denom)
             __attribute__ ((__const__));
extern ldiv_t ldiv (long int __numer, long int __denom)
             __attribute__ ((__const__));
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign) ;
extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign) ;
extern char *gcvt (double __value, int __ndigit, char *__buf) ;
extern char *qecvt (long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign) ;
extern char *qfcvt (long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign) ;
extern char *qgcvt (long double __value, int __ndigit, char *__buf) ;
extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf,
                   size_t __len) ;
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
                   int *__restrict __sign, char *__restrict __buf,
                   size_t __len) ;
extern int qecvt_r (long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign,
                    char *__restrict __buf, size_t __len) ;
extern int qfcvt_r (long double __value, int __ndigit,
                    int *__restrict __decpt, int *__restrict __sign,
                    char *__restrict __buf, size_t __len) ;
extern int mblen (__const char *__s, size_t __n) ;
extern int mbtowc (wchar_t *__restrict __pwc,
                   __const char *__restrict __s, size_t __n) ;
extern int wctomb (char *__s, wchar_t __wchar) ;
extern size_t mbstowcs (wchar_t *__restrict __pwcs,
                        __const char *__restrict __s, size_t __n) ;
extern size_t wcstombs (char *__restrict __s,
                        __const wchar_t *__restrict __pwcs, size_t __n)
            ;
extern int rpmatch (__const char *__response) ;
extern int getloadavg (double __loadavg[], int __nelem) ;
typedef unsigned char bool;
typedef struct region_ *region;
extern region permanent;
void region_init(void);
region newregion(void);
region newsubregion(region parent);
typedef int type_t;
void *__rc_typed_ralloc(region r, size_t size, type_t type);
void *__rc_typed_rarrayalloc(region r, size_t n, size_t size, type_t type);
void *__rc_typed_rarrayextend(region r, void *old, size_t n, size_t size, type_t type);
void typed_rarraycopy(void *to, void *from, size_t n, size_t size, type_t type);
void *__rc_ralloc_small0(region r, size_t size);
char *__rc_rstralloc(region r, size_t size);
char *__rc_rstralloc0(region r, size_t size);
char *__rc_rstrdup(region r, const char *s);
char *__rc_rstrextend(region r, const char *old, size_t newsize);
char *__rc_rstrextend0(region r, const char *old, size_t newsize);
void deleteregion(region r);
void deleteregion_ptr(region *r);
void deleteregion_array(int n, region *regions);
region regionof(void *ptr);
typedef void (*nomem_handler)(void);
nomem_handler set_nomem_handler(nomem_handler newhandler);
void findrefs(region r, void *from, void *to);
void findgrefs(region r);
void findrrefs(region r, region from);
typedef unsigned int *bitset;
unsigned int sizeof_bitset(unsigned int nbits);
bitset bitset_new(region r, unsigned int nbits);
bitset bitset_copy(region r, bitset b);
void bitset_assign(bitset b1, bitset b2);
bool bitset_empty(bitset b);
bool bitset_empty_range(bitset b, unsigned int first, unsigned int last);
bool bitset_full_range(bitset b, unsigned int first, unsigned int last);
void bitset_insert_all(bitset b);
bool bitset_insert(bitset b, unsigned int elt);
bool bitset_remove(bitset b, unsigned int elt);
bool bitset_member(bitset b, unsigned int elt);
bool bitset_intersect(bitset b1, const bitset b2);
void bitset_print(bitset b);
unsigned long bitset_hash(bitset b);
bool bitset_eq(bitset b1, bitset b2);
typedef __gnuc_va_list va_list;
typedef struct growbuf *growbuf;
growbuf growbuf_new(region, int);
void growbuf_reset(growbuf);
int gprintf(growbuf, const char *, ...);
int gvprintf(growbuf, const char *, va_list);
char *growbuf_contents(growbuf);
bool growbuf_empty(growbuf);
typedef struct Qtype *qtype;
typedef struct Abstract_loc *aloc;
typedef struct Effect *effect;
typedef struct Store *store;
typedef struct Rinfo *rinfo;
typedef struct Drinfo *drinfo;
typedef struct ExprDrinfoPair *exprdrinfo;
typedef int (*printf_func)(const char *fmt, ...);
typedef enum {p_neg = -1, p_non = 0, p_pos = 1, p_sub = 2} polarity;
typedef enum {
  eff_any = 0,
  eff_rwr,
  eff_r,
  eff_wr,
  eff_alloc,
  eff_last = eff_alloc } eff_kind;
void load_config_file_quals(const char *);
typedef struct {
  char *data;
  int length;
} cstring;
cstring make_cstring(region r, const char *s, int l);
cstring str2cstring(region r, const char *s);
typedef long long largest_int;
typedef unsigned long long largest_uint;
typedef int (*dd_cmp_fn) (void*, void*);
typedef struct dd_list *dd_list;
typedef struct dd_list_pos
{
  struct dd_list_pos *next;
  struct dd_list_pos *previous;
  void *data;
} *dd_list_pos;
dd_list dd_new_list(region r);
void dd_add_first(region r, dd_list l, void *data);
void dd_add_last(region r, dd_list l, void *data);
void dd_insert_before(region r, dd_list_pos where, void *data);
void dd_insert_after(region r, dd_list_pos where, void *data);
void dd_remove(dd_list_pos what);
dd_list_pos dd_first(dd_list l);
dd_list_pos dd_last(dd_list l);
unsigned long dd_length(dd_list l);
void dd_append(dd_list l1, dd_list l2);
dd_list dd_copy(region r, dd_list l);
void dd_free_list(dd_list l, void (*delete)(dd_list_pos p));
dd_list_pos dd_find(dd_list l, void *find);
dd_list_pos dd_search(dd_list l, dd_cmp_fn f, void *find);
void dd_sort (dd_list l, dd_cmp_fn f);
void dd_remove_all_matches_from(dd_list_pos begin, dd_cmp_fn f, void* find);
void dd_remove_dups(dd_list l, dd_cmp_fn f);
static inline dd_list dd_fix_null(region r, dd_list l)
{
  if (l == ((void *)0))
    return dd_new_list(r);
  else
    return l;
}
typedef int (*set_cmp_fn)(void *e1, void *e2);
typedef struct Location
{
  char *filename;
  unsigned long lineno;
  unsigned long filepos;
  bool in_system_header;
  int location_index;
} *location;
unsigned long location_hash(location loc);
bool location_eq(location loc1, location loc2);
int location_cmp(location loc1, location loc2);
int location_index(location loc);
typedef dd_list loc_set; typedef dd_list_pos loc_set_scanner; static inline loc_set empty_loc_set (region r) { return dd_new_list(r); } static inline loc_set loc_set_copy(region r, loc_set s) { if (s == ((void *)0)) return ((void *)0); else return dd_copy(r, s); } static inline bool loc_set_empty(loc_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool loc_set_member(loc_set s, location elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) location_cmp, (void *) elt) != ((void *)0); } static inline int loc_set_size(loc_set s) { if (s == ((void *)0)) return 0; else return dd_length(s); } static inline bool loc_set_insert(region r, loc_set *s, location elt) { *s = dd_fix_null(r, *s); if (! loc_set_member(*s, elt)) { dd_add_first(r, *s, (void *) elt); return 1; } return 0; } static inline bool loc_set_insert_last(region r, loc_set *s, location elt) { *s = dd_fix_null(r, *s); if (! loc_set_member(*s, elt)) { dd_add_last(r, *s, (void *) elt); return 1; } return 0; } static inline bool loc_set_insert_nocheck(region r, loc_set *s, location elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool loc_set_insert_last_nocheck(region r, loc_set *s, location elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline void loc_set_remove(loc_set *s, location elt) { if (*s) dd_remove_all_matches_from(dd_first(*s), (dd_cmp_fn) location_cmp, (void *) elt); } static inline loc_set loc_set_union(loc_set s1, loc_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); dd_remove_dups(s1, (dd_cmp_fn)location_cmp); return s1; } static inline loc_set loc_set_union_nocheck(loc_set s1, loc_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool loc_set_single(loc_set s) { return loc_set_size(s) == 1; } static inline void loc_set_sort(loc_set s) { if (s) dd_sort(s, (set_cmp_fn) location_cmp); } static inline void loc_set_remove_dups(int (*cmp)(location, location), loc_set s) { if (s) dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void loc_set_scan(loc_set s, loc_set_scanner *ss) { if (s) *ss = dd_first(s); else *ss = ((void *)0); } static inline location loc_set_next(loc_set_scanner *ss) { location result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((location)((*ss)->data)); *ss = ((*ss)->next); return result; };typedef struct type_qualifier *qual;
int cmp_qual(qual left, qual right);
typedef dd_list qual_set; typedef dd_list_pos qual_set_scanner; static inline qual_set empty_qual_set (region r) { return dd_new_list(r); } static inline qual_set qual_set_copy(region r, qual_set s) { if (s == ((void *)0)) return ((void *)0); else return dd_copy(r, s); } static inline bool qual_set_empty(qual_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool qual_set_member(qual_set s, qual elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) cmp_qual, (void *) elt) != ((void *)0); } static inline int qual_set_size(qual_set s) { if (s == ((void *)0)) return 0; else return dd_length(s); } static inline bool qual_set_insert(region r, qual_set *s, qual elt) { *s = dd_fix_null(r, *s); if (! qual_set_member(*s, elt)) { dd_add_first(r, *s, (void *) elt); return 1; } return 0; } static inline bool qual_set_insert_last(region r, qual_set *s, qual elt) { *s = dd_fix_null(r, *s); if (! qual_set_member(*s, elt)) { dd_add_last(r, *s, (void *) elt); return 1; } return 0; } static inline bool qual_set_insert_nocheck(region r, qual_set *s, qual elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool qual_set_insert_last_nocheck(region r, qual_set *s, qual elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline void qual_set_remove(qual_set *s, qual elt) { if (*s) dd_remove_all_matches_from(dd_first(*s), (dd_cmp_fn) cmp_qual, (void *) elt); } static inline qual_set qual_set_union(qual_set s1, qual_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); dd_remove_dups(s1, (dd_cmp_fn)cmp_qual); return s1; } static inline qual_set qual_set_union_nocheck(qual_set s1, qual_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool qual_set_single(qual_set s) { return qual_set_size(s) == 1; } static inline void qual_set_sort(qual_set s) { if (s) dd_sort(s, (set_cmp_fn) cmp_qual); } static inline void qual_set_remove_dups(int (*cmp)(qual, qual), qual_set s) { if (s) dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void qual_set_scan(qual_set s, qual_set_scanner *ss) { if (s) *ss = dd_first(s); else *ss = ((void *)0); } static inline qual qual_set_next(qual_set_scanner *ss) { qual result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((qual)((*ss)->data)); *ss = ((*ss)->next); return result; };
typedef struct Qual_edge* qual_edge;
typedef dd_list qual_edge_set; typedef dd_list_pos qual_edge_set_scanner; static inline qual_edge_set empty_qual_edge_set(region r) { return dd_new_list(r); } static inline qual_edge_set qual_edge_set_copy(region r, qual_edge_set s) { if (s == ((void *)0)) return ((void *)0); return dd_copy(r, s); } static inline bool qual_edge_set_empty(qual_edge_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool qual_edge_set_member(int (*cmp)(qual_edge, qual_edge), qual_edge_set s, qual_edge elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) cmp, (void *) elt) != ((void *)0); } static inline int qual_edge_set_size(qual_edge_set s) { if (s == ((void *)0)) return 0; return dd_length(s); } static inline bool qual_edge_set_insert(region r, qual_edge_set *s, qual_edge elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool qual_edge_set_insert_last(region r, qual_edge_set *s, qual_edge elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline qual_edge_set qual_edge_set_union(qual_edge_set s1, qual_edge_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool qual_edge_set_single(qual_edge_set s) { return qual_edge_set_size(s) == 1; } static inline void qual_edge_set_sort(int (*cmp)(qual_edge, qual_edge), qual_edge_set s) { if (s == ((void *)0)) return; dd_sort(s, (set_cmp_fn) cmp); } static inline void qual_edge_set_remove_dups(int (*cmp)(qual_edge, qual_edge), qual_edge_set s) { if (s == ((void *)0)) return; dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void qual_edge_set_scan(qual_edge_set s, qual_edge_set_scanner *ss) { if (s == ((void *)0)) *ss = ((void *)0); else *ss = dd_first(s); } static inline qual_edge qual_edge_set_next(qual_edge_set_scanner *ss) { qual_edge result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((qual_edge)((*ss)->data)); *ss = ((*ss)->next); return result; };
extern int flag_print_quals_graph;
typedef enum { level_value, level_ref } level_qual_t;
typedef enum { sign_pos, sign_neg, sign_eq } sign_qual_t;
typedef enum { flow_up, flow_down } flow_qual_t;
typedef unsigned int gate_word;
typedef struct Qual_gate {
  bool forward:1;
  bool backward:1;
  gate_word mask[(((32) + (sizeof (gate_word) * 8) - 1) / (sizeof (gate_word) * 8))];
} qual_gate;
extern qual const_qual;
extern qual nonconst_qual;
extern qual volatile_qual;
extern qual restrict_qual;
extern qual noreturn_qual;
extern qual init_qual;
extern qual noninit_qual;
extern bool exists_effect_qual;
extern bool exists_ptrflow_qual;
extern bool exists_fieldflow_qual;
extern bool exists_fieldptrflow_qual;
extern bool used_fs_qual;
qual_gate open_qgate;
qual_gate fi_qgate;
qual_gate fs_qgate;
qual_gate effect_qgate;
qual_gate ptrflow_down_pos_qgate;
qual_gate ptrflow_down_neg_qgate;
qual_gate ptrflow_up_pos_qgate;
qual_gate ptrflow_up_neg_qgate;
qual_gate fieldflow_down_pos_qgate;
qual_gate fieldflow_down_neg_qgate;
qual_gate fieldflow_up_pos_qgate;
qual_gate fieldflow_up_neg_qgate;
qual_gate fieldptrflow_down_pos_qgate;
qual_gate fieldptrflow_down_neg_qgate;
qual_gate fieldptrflow_up_pos_qgate;
qual_gate fieldptrflow_up_neg_qgate;
typedef void (*qual_traverse_fn)(qual q, void *arg);
typedef void (*edge_traverse_fn)(qual left, qual right, location loc,
                                 polarity p,bool first,bool bidi,void *arg);
const char *polarity_to_string(polarity p);
void init_quals(void);
void end_define_pos(void);
void finish_quals(void);
void dispose_quals(void);
void begin_po_qual(void);
void end_po_qual(void);
void set_po_flow_sensitive(void);
void set_po_nonprop(void);
void set_po_effect(void);
qual add_qual(const char *name);
void add_qual_lt(qual left, qual right);
void add_color_qual(qual q, const char *color);
void add_level_qual(qual q, level_qual_t lev);
void add_sign_qual(qual q, sign_qual_t sign);
void add_ptrflow_qual(qual q, flow_qual_t f);
void add_fieldflow_qual(qual q, flow_qual_t f);
void add_fieldptrflow_qual(qual q, flow_qual_t f);
qual_set get_constant_quals (void);
qual find_qual(const char *name);
level_qual_t level_qual(qual q);
sign_qual_t sign_qual(qual q);
bool flow_sensitive_qual(qual q);
bool nonprop_qual(qual q);
bool variable_qual(qual q);
bool constant_qual(qual q);
qual make_qvar(const char *name, location loc, bool preferred, bool global);
qual make_fresh_qvar(const char *base_name, location loc);
qual_set get_variable_quals (void);
bool store_aloc_qual(qual q, store s, aloc al);
void mk_no_qual_qual(location loc, qual q);
void set_global_qual (qual q, bool global);
bool global_qual(qual q);
bool preferred_qual(qual q);
location location_qual(qual q);
qual_edge_set lb_qual(region r, qual q);
qual_edge_set cs_lb_qual(region r, qual q);
qual_edge_set ub_qual(region r, qual q);
qual_edge_set cs_ub_qual(region r, qual q);
int error_count_qual(qual q);
void scan_qual_bounds(qual q, qual_traverse_fn f, void *arg);
void scan_qual_bounds_sorted(qual q, qual_traverse_fn f, void *arg);
qual qual_edge_other_end (qual_edge qe, qual q);
location qual_edge_loc (qual_edge qe);
bool mkleq_qual(location loc, qual_gate *qgate, qual left, qual right);
bool mkeq_qual(location loc, qual_gate *qgate, qual left, qual right);
bool mkinst_qual(location loc, qual_gate *qgate, qual left, qual right, polarity p);
bool unify_qual(location loc, qual left, qual right);
bool cond_mkleq_qual(location loc, qual_gate *qgate1, qual l1, qual r1,
                     qual_gate *qgate2, qual l2, qual r2);
bool copy_constraint (qual_edge e, qual q1, qual q2);
bool eq_qual(qual left, qual right);
bool leq_qual(qual left, qual right);
bool cs_leq_qual(qual left, qual right);
bool has_lb_qual(qual q, qual bound);
bool cs_has_lb_qual(qual q, qual bound);
bool has_ub_qual(qual q, qual bound);
bool cs_has_ub_qual(qual q, qual bound);
bool has_qual(qual q, qual bound);
bool cs_has_qual(qual q, qual bound);
bool has_fs_qual(qual q);
bool may_leq_qual (qual q1, qual q2);
bool cs_may_leq_qual (qual q1, qual q2);
typedef int (*pr_qual_fn)(printf_func, qual);
int print_qual(printf_func, qual);
int cs_print_qual(printf_func, qual);
int print_qual_raw(printf_func, qual);
const char *name_qual(qual q);
bool set_qual_name(qual q, const char *name);
const char *unique_name_qual(qual q);
const char *color_qual(qual q);
void print_qual_graph(qual q, const char *file, bool po_bounds_only);
void traverse_shortest_path_edges(qual q, qual c, edge_traverse_fn f,
                                  void *arg);
void traverse_error_path_edges(qual q,edge_traverse_fn f,void* arg);
void cs_traverse_error_path_edges(qual q,edge_traverse_fn f,void* arg);
extern int num_hotspots;
extern qual *hotspots;
void reset_flow_sensitive_quals(void);
typedef struct Aloctype *aloctype;
typedef struct Alocreftype *alocreftype;
typedef struct Alocfntype *alocfntype;
typedef struct Effecttype *effecttype;
typedef struct Rinf_const *rinf_const;
extern effecttype effecttype_empty;
void init_aloctypes(void);
aloctype make_aloctype(alocreftype tau, alocfntype fn);
aloctype make_aloctype_fresh(void);
void unify_aloctype(aloctype al1, aloctype al2);
void mkleq_aloctype(aloctype al1, aloctype al2);
alocreftype proj_tau(aloctype al);
alocfntype proj_lam(aloctype al);
int print_aloctype(printf_func pf, aloctype al, int depth);
alocreftype alocreftype_fresh(void);
alocreftype alocreftype_var(const char * name);
alocreftype alocreftype_ref(aloctype pointsto);
void unify_alocreftype(alocreftype alref1, alocreftype alref2);
void mkleq_alocreftype(alocreftype alref1, alocreftype alref2);
aloctype deref(alocreftype tau);
int print_alocreftype(printf_func pf, alocreftype alref, int depth);
alocfntype alocfntype_fresh(void);
alocfntype alocfntype_var(const char * name);
alocfntype alocfntype_fn(aloctype * params, effecttype eff, aloctype returns);
void unify_alocfntype(alocfntype alfn1, alocfntype alfn2);
void mkleq_alocfntype(alocfntype alfn1, alocfntype alfn2);
int print_alocfntype(printf_func pf, alocfntype alfn, int depth);
effecttype effecttype_read(alocreftype base);
effecttype effecttype_write(alocreftype base);
effecttype effecttype_any(alocreftype base);
effecttype effecttype_read_reach(alocreftype base);
effecttype effecttype_write_reach(alocreftype base);
effecttype effecttype_any_reach(alocreftype base);
effecttype effecttype_union(effecttype e1, effecttype e2);
effecttype effecttype_inter(effecttype e1, effecttype e2);
effecttype effecttype_var(const char * name);
effecttype effecttype_fresh(void);
void mkleq_effecttype(effecttype e1, effecttype e2);
void mkeq_effecttype(effecttype e1, effecttype e2);
void unify_effecttype(effecttype e1, effecttype e2);
int print_effecttype(printf_func pf, effecttype e);
rinf_const mk_rinf_const(location loc,
                         alocreftype r_al, alocreftype old_al,
                         alocreftype top_al,
                         effecttype r_type, effecttype pointsto_type,
                         effecttype r_body, effecttype body, effecttype env);
void check_rinf_consts(void);
bool is_rinf_satisfied(rinf_const c);
bool is_rinf_used(rinf_const c);
int print_rinf_info(printf_func pf, rinf_const c);
extern void __assert_fail (__const char *__assertion, __const char *__file,
                           unsigned int __line, __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert_perror_fail (int __errnum, __const char *__file,
                                  unsigned int __line,
                                  __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert (const char *__assertion, const char *__file, int __line)
             __attribute__ ((__noreturn__));
struct array;
struct array *new_array(region r, size_t initialsize,
                        size_t typesize, type_t typeinfo);
void *array_extend(struct array *a, int by);
void array_reset(struct array *a);
size_t array_length(struct array *a);
void *array_data(struct array *a);
typedef struct wchar_array_a *wchar_array; wchar_array new_wchar_array(region r, size_t initialsize); wchar_t *wchar_array_extend(wchar_array a, int by); void wchar_array_reset(wchar_array a); size_t wchar_array_length(wchar_array a); wchar_t *wchar_array_data(wchar_array a);
extern void __assert_fail (__const char *__assertion, __const char *__file,
                           unsigned int __line, __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert_perror_fail (int __errnum, __const char *__file,
                                  unsigned int __line,
                                  __const char *__function)
             __attribute__ ((__noreturn__));
extern void __assert (const char *__assertion, const char *__file, int __line)
             __attribute__ ((__noreturn__));
void *xmalloc(size_t size);
void *xrealloc(void *p, size_t newsize);
unsigned long align_to(unsigned long n, unsigned long alignment);
unsigned long lcm(unsigned long x, unsigned long y);
void __fail(const char *file, unsigned int line,
            const char *func, const char *fmt, ...) __attribute__ ((__noreturn__));
void __user_error(const char *file, unsigned int line,
                  const char *func, const char *fmt, ...)
  __attribute__ ((__noreturn__));
char *rstrcat(region, const char *, const char *);
char *rstrscat(region, ...);
const char *inttostr(region r, int);
char *rsprintf(region r, const char *fmt, ...);
char *rvsprintf(region r, const char *fmt, va_list args);
char *ptr_to_ascii(void *ptr);
unsigned long ptr_hash(void *ptr);
bool ptr_eq(void *ptr1, void *ptr2);
unsigned long string_hash(const char *str);
bool string_eq(const char *s1, const char *s2);
int ptr_cmp(void *ptr1, void *ptr2);
const char *name_with_loc(region r, const char *name, location loc);
typedef int id_declaration_list;
typedef struct typelist *typelist;
typedef struct type *type;
typedef struct known_cst *known_cst;
typedef struct AST_node *node;
typedef struct AST_declaration *declaration;
typedef struct AST_statement *statement;
typedef struct AST_expression *expression;
typedef struct AST_type_element *type_element;
typedef struct AST_declarator *declarator;
typedef struct AST_label *label;
typedef struct AST_asm_decl *asm_decl;
typedef struct AST_data_decl *data_decl;
typedef struct AST_extension_decl *extension_decl;
typedef struct AST_ellipsis_decl *ellipsis_decl;
typedef struct AST_enumerator *enumerator;
typedef struct AST_oldidentifier_decl *oldidentifier_decl;
typedef struct AST_function_decl *function_decl;
typedef struct AST_implicit_decl *implicit_decl;
typedef struct AST_variable_decl *variable_decl;
typedef struct AST_field_decl *field_decl;
typedef struct AST_asttype *asttype;
typedef struct AST_typename *typename;
typedef struct AST_type_variable *type_variable;
typedef struct AST_typeof_expr *typeof_expr;
typedef struct AST_typeof_type *typeof_type;
typedef struct AST_attribute *attribute;
typedef struct AST_rid *rid;
typedef struct AST_user_qual *user_qual;
typedef struct AST_qualifier *qualifier;
typedef struct AST_tag_ref *tag_ref;
typedef struct AST_function_declarator *function_declarator;
typedef struct AST_pointer_declarator *pointer_declarator;
typedef struct AST_array_declarator *array_declarator;
typedef struct AST_identifier_declarator *identifier_declarator;
typedef struct AST_asm_stmt *asm_stmt;
typedef struct AST_compound_stmt *compound_stmt;
typedef struct AST_if_stmt *if_stmt;
typedef struct AST_labeled_stmt *labeled_stmt;
typedef struct AST_expression_stmt *expression_stmt;
typedef struct AST_breakable_stmt *breakable_stmt;
typedef struct AST_conditional_stmt *conditional_stmt;
typedef struct AST_switch_stmt *switch_stmt;
typedef struct AST_for_stmt *for_stmt;
typedef struct AST_break_stmt *break_stmt;
typedef struct AST_continue_stmt *continue_stmt;
typedef struct AST_return_stmt *return_stmt;
typedef struct AST_goto_stmt *goto_stmt;
typedef struct AST_computed_goto_stmt *computed_goto_stmt;
typedef struct AST_empty_stmt *empty_stmt;
typedef struct AST_assert_type_stmt *assert_type_stmt;
typedef struct AST_change_type_stmt *change_type_stmt;
typedef struct AST_deep_restrict_stmt *deep_restrict_stmt;
typedef struct AST_unary *unary;
typedef struct AST_binary *binary;
typedef struct AST_comma *comma;
typedef struct AST_sizeof_type *sizeof_type;
typedef struct AST_alignof_type *alignof_type;
typedef struct AST_label_address *label_address;
typedef struct AST_cast *cast;
typedef struct AST_cast_list *cast_list;
typedef struct AST_conditional *conditional;
typedef struct AST_identifier *identifier;
typedef struct AST_compound_expr *compound_expr;
typedef struct AST_function_call *function_call;
typedef struct AST_array_ref *array_ref;
typedef struct AST_field_ref *field_ref;
typedef struct AST_init_list *init_list;
typedef struct AST_init_index *init_index;
typedef struct AST_init_field *init_field;
typedef struct AST_lexical_cst *lexical_cst;
typedef struct AST_string_cst *string_cst;
typedef struct AST_string *string;
typedef struct AST_id_label *id_label;
typedef struct AST_case_label *case_label;
typedef struct AST_default_label *default_label;
typedef struct AST_word *word;
typedef struct AST_asm_operand *asm_operand;
typedef struct AST_declaration *error_decl;
typedef struct AST_tag_ref *struct_ref;
typedef struct AST_tag_ref *union_ref;
typedef struct AST_tag_ref *enum_ref;
typedef struct AST_statement *error_stmt;
typedef struct AST_conditional_stmt *while_stmt;
typedef struct AST_conditional_stmt *dowhile_stmt;
typedef struct AST_expression *error_expr;
typedef struct AST_unary *dereference;
typedef struct AST_unary *extension_expr;
typedef struct AST_unary *sizeof_expr;
typedef struct AST_unary *alignof_expr;
typedef struct AST_unary *realpart;
typedef struct AST_unary *imagpart;
typedef struct AST_unary *address_of;
typedef struct AST_unary *unary_minus;
typedef struct AST_unary *unary_plus;
typedef struct AST_unary *conjugate;
typedef struct AST_unary *preincrement;
typedef struct AST_unary *predecrement;
typedef struct AST_unary *postincrement;
typedef struct AST_unary *postdecrement;
typedef struct AST_unary *bitnot;
typedef struct AST_unary *not;
typedef struct AST_binary *plus;
typedef struct AST_binary *minus;
typedef struct AST_binary *times;
typedef struct AST_binary *divide;
typedef struct AST_binary *modulo;
typedef struct AST_binary *lshift;
typedef struct AST_binary *rshift;
typedef struct AST_binary *leq;
typedef struct AST_binary *geq;
typedef struct AST_binary *lt;
typedef struct AST_binary *gt;
typedef struct AST_binary *eq;
typedef struct AST_binary *ne;
typedef struct AST_binary *bitand;
typedef struct AST_binary *bitor;
typedef struct AST_binary *bitxor;
typedef struct AST_binary *andand;
typedef struct AST_binary *oror;
typedef struct AST_binary *assign;
typedef struct AST_binary *plus_assign;
typedef struct AST_binary *minus_assign;
typedef struct AST_binary *times_assign;
typedef struct AST_binary *divide_assign;
typedef struct AST_binary *modulo_assign;
typedef struct AST_binary *lshift_assign;
typedef struct AST_binary *rshift_assign;
typedef struct AST_binary *bitand_assign;
typedef struct AST_binary *bitor_assign;
typedef struct AST_binary *bitxor_assign;
typedef enum {
  kind_node = 42,
  postkind_node = 168,
  kind_declaration = 43,
  postkind_declaration = 54,
  kind_statement = 55,
  postkind_statement = 76,
  kind_expression = 77,
  postkind_expression = 143,
  kind_type_element = 144,
  postkind_type_element = 156,
  kind_declarator = 157,
  postkind_declarator = 161,
  kind_label = 162,
  postkind_label = 165,
  kind_asm_decl = 44,
  postkind_asm_decl = 44,
  kind_data_decl = 45,
  postkind_data_decl = 45,
  kind_extension_decl = 46,
  postkind_extension_decl = 46,
  kind_ellipsis_decl = 47,
  postkind_ellipsis_decl = 47,
  kind_enumerator = 48,
  postkind_enumerator = 48,
  kind_oldidentifier_decl = 49,
  postkind_oldidentifier_decl = 49,
  kind_function_decl = 50,
  postkind_function_decl = 50,
  kind_implicit_decl = 51,
  postkind_implicit_decl = 51,
  kind_variable_decl = 52,
  postkind_variable_decl = 52,
  kind_field_decl = 53,
  postkind_field_decl = 53,
  kind_asttype = 166,
  postkind_asttype = 166,
  kind_typename = 145,
  postkind_typename = 145,
  kind_type_variable = 146,
  postkind_type_variable = 146,
  kind_typeof_expr = 147,
  postkind_typeof_expr = 147,
  kind_typeof_type = 148,
  postkind_typeof_type = 148,
  kind_attribute = 149,
  postkind_attribute = 149,
  kind_rid = 150,
  postkind_rid = 150,
  kind_user_qual = 151,
  postkind_user_qual = 151,
  kind_qualifier = 152,
  postkind_qualifier = 152,
  kind_tag_ref = 153,
  postkind_tag_ref = 156,
  kind_function_declarator = 158,
  postkind_function_declarator = 158,
  kind_pointer_declarator = 159,
  postkind_pointer_declarator = 159,
  kind_array_declarator = 160,
  postkind_array_declarator = 160,
  kind_identifier_declarator = 161,
  postkind_identifier_declarator = 161,
  kind_asm_stmt = 56,
  postkind_asm_stmt = 56,
  kind_compound_stmt = 57,
  postkind_compound_stmt = 57,
  kind_if_stmt = 58,
  postkind_if_stmt = 58,
  kind_labeled_stmt = 59,
  postkind_labeled_stmt = 59,
  kind_expression_stmt = 60,
  postkind_expression_stmt = 60,
  kind_breakable_stmt = 61,
  postkind_breakable_stmt = 66,
  kind_conditional_stmt = 62,
  postkind_conditional_stmt = 65,
  kind_switch_stmt = 63,
  postkind_switch_stmt = 63,
  kind_for_stmt = 66,
  postkind_for_stmt = 66,
  kind_break_stmt = 67,
  postkind_break_stmt = 67,
  kind_continue_stmt = 68,
  postkind_continue_stmt = 68,
  kind_return_stmt = 69,
  postkind_return_stmt = 69,
  kind_goto_stmt = 70,
  postkind_goto_stmt = 70,
  kind_computed_goto_stmt = 71,
  postkind_computed_goto_stmt = 71,
  kind_empty_stmt = 72,
  postkind_empty_stmt = 72,
  kind_assert_type_stmt = 73,
  postkind_assert_type_stmt = 73,
  kind_change_type_stmt = 74,
  postkind_change_type_stmt = 74,
  kind_deep_restrict_stmt = 75,
  postkind_deep_restrict_stmt = 75,
  kind_unary = 78,
  postkind_unary = 96,
  kind_binary = 97,
  postkind_binary = 127,
  kind_comma = 128,
  postkind_comma = 128,
  kind_sizeof_type = 129,
  postkind_sizeof_type = 129,
  kind_alignof_type = 130,
  postkind_alignof_type = 130,
  kind_label_address = 131,
  postkind_label_address = 131,
  kind_cast = 79,
  postkind_cast = 79,
  kind_cast_list = 132,
  postkind_cast_list = 132,
  kind_conditional = 133,
  postkind_conditional = 133,
  kind_identifier = 134,
  postkind_identifier = 134,
  kind_compound_expr = 135,
  postkind_compound_expr = 135,
  kind_function_call = 136,
  postkind_function_call = 136,
  kind_array_ref = 98,
  postkind_array_ref = 98,
  kind_field_ref = 80,
  postkind_field_ref = 80,
  kind_init_list = 137,
  postkind_init_list = 137,
  kind_init_index = 138,
  postkind_init_index = 138,
  kind_init_field = 139,
  postkind_init_field = 139,
  kind_lexical_cst = 140,
  postkind_lexical_cst = 141,
  kind_string_cst = 141,
  postkind_string_cst = 141,
  kind_string = 142,
  postkind_string = 142,
  kind_id_label = 163,
  postkind_id_label = 163,
  kind_case_label = 164,
  postkind_case_label = 164,
  kind_default_label = 165,
  postkind_default_label = 165,
  kind_word = 167,
  postkind_word = 167,
  kind_asm_operand = 168,
  postkind_asm_operand = 168,
  kind_error_decl = 54,
  postkind_error_decl = 54,
  kind_struct_ref = 154,
  postkind_struct_ref = 154,
  kind_union_ref = 155,
  postkind_union_ref = 155,
  kind_enum_ref = 156,
  postkind_enum_ref = 156,
  kind_error_stmt = 76,
  postkind_error_stmt = 76,
  kind_while_stmt = 64,
  postkind_while_stmt = 64,
  kind_dowhile_stmt = 65,
  postkind_dowhile_stmt = 65,
  kind_error_expr = 143,
  postkind_error_expr = 143,
  kind_dereference = 81,
  postkind_dereference = 81,
  kind_extension_expr = 82,
  postkind_extension_expr = 82,
  kind_sizeof_expr = 83,
  postkind_sizeof_expr = 83,
  kind_alignof_expr = 84,
  postkind_alignof_expr = 84,
  kind_realpart = 85,
  postkind_realpart = 85,
  kind_imagpart = 86,
  postkind_imagpart = 86,
  kind_address_of = 87,
  postkind_address_of = 87,
  kind_unary_minus = 88,
  postkind_unary_minus = 88,
  kind_unary_plus = 89,
  postkind_unary_plus = 89,
  kind_conjugate = 90,
  postkind_conjugate = 90,
  kind_preincrement = 91,
  postkind_preincrement = 91,
  kind_predecrement = 92,
  postkind_predecrement = 92,
  kind_postincrement = 93,
  postkind_postincrement = 93,
  kind_postdecrement = 94,
  postkind_postdecrement = 94,
  kind_bitnot = 95,
  postkind_bitnot = 95,
  kind_not = 96,
  postkind_not = 96,
  kind_plus = 99,
  postkind_plus = 99,
  kind_minus = 100,
  postkind_minus = 100,
  kind_times = 101,
  postkind_times = 101,
  kind_divide = 102,
  postkind_divide = 102,
  kind_modulo = 103,
  postkind_modulo = 103,
  kind_lshift = 104,
  postkind_lshift = 104,
  kind_rshift = 105,
  postkind_rshift = 105,
  kind_leq = 106,
  postkind_leq = 106,
  kind_geq = 107,
  postkind_geq = 107,
  kind_lt = 108,
  postkind_lt = 108,
  kind_gt = 109,
  postkind_gt = 109,
  kind_eq = 110,
  postkind_eq = 110,
  kind_ne = 111,
  postkind_ne = 111,
  kind_bitand = 112,
  postkind_bitand = 112,
  kind_bitor = 113,
  postkind_bitor = 113,
  kind_bitxor = 114,
  postkind_bitxor = 114,
  kind_andand = 115,
  postkind_andand = 115,
  kind_oror = 116,
  postkind_oror = 116,
  kind_assign = 117,
  postkind_assign = 117,
  kind_plus_assign = 118,
  postkind_plus_assign = 118,
  kind_minus_assign = 119,
  postkind_minus_assign = 119,
  kind_times_assign = 120,
  postkind_times_assign = 120,
  kind_divide_assign = 121,
  postkind_divide_assign = 121,
  kind_modulo_assign = 122,
  postkind_modulo_assign = 122,
  kind_lshift_assign = 123,
  postkind_lshift_assign = 123,
  kind_rshift_assign = 124,
  postkind_rshift_assign = 124,
  kind_bitand_assign = 125,
  postkind_bitand_assign = 125,
  kind_bitor_assign = 126,
  postkind_bitor_assign = 126,
  kind_bitxor_assign = 127,
  postkind_bitxor_assign = 127
} ast_kind;
extern location last_location, dummy_location;
enum rid
{
  RID_UNUSED,
  RID_INT,
  RID_CHAR,
  RID_FLOAT,
  RID_DOUBLE,
  RID_VOID,
  RID_UNSIGNED,
  RID_SHORT,
  RID_LONG,
  RID_SIGNED,
  RID_INLINE,
  RID_COMPLEX,
  RID_AUTO,
  RID_STATIC,
  RID_EXTERN,
  RID_REGISTER,
  RID_TYPEDEF,
  RID_MAX
};
extern char * token_buffer;
extern bool in_system_header;
extern bool in_prelude;
int yylex(void);
void init_lex(void);
typedef void *hash_key;
typedef void *hash_data;
typedef unsigned long (*hash_fn)(hash_key k);
typedef bool (*keyeq_fn)(hash_key k1, hash_key k2);
typedef void (*hash_apply_fn)(hash_key k, hash_data d, void *arg);
typedef hash_data (*hash_map_fn)(hash_key k, hash_data d, void *arg);
typedef struct Hash_table *hash_table;
hash_table make_hash_table(region rhash, unsigned long size, hash_fn hash,
                           keyeq_fn cmp);
hash_table make_string_hash_table(region rhash, unsigned long size);
void hash_table_reset(hash_table ht);
unsigned long hash_table_size(hash_table ht);
bool hash_table_hash_search(hash_table ht, keyeq_fn cmp,
                            hash_key k, hash_data *d);
bool hash_table_lookup(hash_table ht, hash_key k, hash_data *d);
bool hash_table_insert(hash_table ht, hash_key k, hash_data d);
bool hash_table_remove(hash_table ht, hash_key k);
hash_table hash_table_copy(region rhash, hash_table ht);
hash_table hash_table_map(region r, hash_table ht, hash_map_fn f, void *arg);
typedef struct bucket *bucket;
typedef struct
{
  hash_table ht;
  unsigned long i;
  bucket cur;
} hash_table_scanner;
void hash_table_scan(hash_table ht, hash_table_scanner *);
bool hash_table_next(hash_table_scanner *, hash_key *k, hash_data *d);
typedef int (*keycmp_fn)(hash_key k1, hash_key k2);
struct sorted_entry
{
  hash_key k;
  hash_data d;
};
typedef struct
{
  region r;
  unsigned long i;
  unsigned long size;
  struct sorted_entry *entries;
} hash_table_scanner_sorted;
void hash_table_scan_sorted(hash_table ht, keycmp_fn f,
                            hash_table_scanner_sorted *htss);
bool hash_table_next_sorted(hash_table_scanner_sorted *htss, hash_key *k,
                            hash_data *d);
typedef struct env *env;
env new_env(region r, env parent);
env env_parent(env e);
region env_region(env e);
void *env_lookup(env e, const char *s, bool this_level_only);
void env_add(env e, const char *s, void *value);
typedef hash_table_scanner env_scanner;
void env_scan(env e, env_scanner *scanner);
bool env_next(env_scanner *scanner, const char **name, void **value);
typedef struct field_declaration {
  struct field_declaration *next;
  const char *name;
  type type;
  field_decl ast;
  int bitwidth;
  size_t offset;
  bool offset_cc:1;
  bool packed:1;
} *field_declaration;
typedef struct tag_declaration {
  int kind;
  const char *name;
  type reptype;
  env fields;
  field_declaration fieldlist;
  tag_ref ast;
  struct tag_declaration *shadowed;
  bool defined:1;
  bool being_defined:1;
  bool fields_const:1;
  bool fields_volatile:1;
  bool transparent_union:1;
  bool size_cc:1;
  bool packed:1;
  size_t size, alignment;
  qtype qtype;
} *tag_declaration;
typedef enum { decl_variable, decl_constant, decl_function,
               decl_typedef, decl_error, decl_magic_string } data_kind;
typedef struct data_declaration {
  data_kind kind;
  const char *name;
  type type;
  struct data_declaration *shadowed;
  struct data_declaration *global;
  declaration definition;
  declaration ast;
  expression initialiser;
  bool islimbo:1;
  bool isexternalscope:1;
  bool isfilescoperef:1;
  bool needsmemory:1;
  bool isused:1;
  bool in_system_header:1;
  bool in_prelude:1;
  bool defined:1;
  bool isallocated:1;
  bool addresstaken:1;
  bool __init:1;
  enum { function_implicit, function_normal, function_static, function_nested }
    ftype:2;
  bool isinline:1;
  bool isexterninline:1;
  bool noreturn:1;
  typelist oldstyle_args;
  char * alias;
  enum { variable_register, variable_static, variable_normal } vtype:2;
  bool islocal:1;
  bool isparameter:1;
  known_cst value;
  const wchar_t *chars;
  size_t chars_length;
  qtype qtype;
  qtype fs_qtype;
  alocreftype alref;
  compound_stmt cs;
} *data_declaration;
typedef struct label_declaration {
  const char *name;
  bool explicitly_declared:1;
  bool used:1;
  id_label firstuse;
  id_label definition;
  function_decl containing_function;
} *label_declaration;
typedef struct environment
{
  struct environment * parent;
  function_decl fdecl;
  bool parm_level;
  env id_env;
  env tag_env;
} *environment;
extern data_declaration bad_decl;
typedef struct {
  enum { cval_variable, cval_unk, cval_address,
         cval_float, cval_float_complex,
         cval_uint, cval_uint_complex,
         cval_sint, cval_sint_complex } kind;
  union {
    struct {
      long double d, d_i;
    } fl;
    struct {
      size_t isize;
      union {
        largest_int si;
        largest_uint ui;
      } real;
      union {
        largest_int si_i;
        largest_uint ui_i;
        struct {
          struct data_declaration *ddecl;
          struct label_declaration *ldecl;
        } addr;
      } imag;
    } i;
  } u;
} cval;
extern cval cval_top;
extern cval cval_unknown;
extern cval cval_zero;
void cval_init(void);
cval make_cval_signed(largest_int i, type t);
cval make_cval_unsigned(largest_uint i, type t);
cval make_cval_float(long double d);
cval make_cval_complex(cval r, cval i);
cval make_cval_address(data_declaration ddecl, label_declaration ldecl,
                       largest_int offset);
bool cval_isinteger(cval c);
bool cval_iscomplex(cval c);
bool cval_knownbool(cval c);
bool cval_boolvalue(cval c);
bool cval_knownvalue(cval c);
largest_uint cval_uint_value(cval c);
largest_int cval_sint_value(cval c);
long double cval_float_value(cval c);
bool cval_isone(cval c);
cval cval_cast(cval c, type to);
cval cval_not(cval c);
cval cval_negate(cval c);
cval cval_bitnot(cval c);
cval cval_conjugate(cval c);
cval cval_realpart(cval c);
cval cval_imagpart(cval c);
cval cval_add(cval c1, cval c2);
cval cval_sub(cval c1, cval c2);
cval cval_times(cval c1, cval c2);
cval cval_divide(cval c1, cval c2);
cval cval_modulo(cval c1, cval c2);
cval cval_lshift(cval c1, cval c2);
cval cval_rshift(cval c1, cval c2);
cval cval_bitand(cval c1, cval c2);
cval cval_bitor(cval c1, cval c2);
cval cval_bitxor(cval c1, cval c2);
cval cval_eq(cval c1, cval c2);
cval cval_leq(cval c1, cval c2);
bool uint_inrange(largest_uint x, type t);
bool sint_inrange(largest_int x, type t);
bool cval_inrange(cval c, type t);
largest_int cval_intcompare(cval c1, cval c2);
extern region parse_region;
typedef enum
{
  no_qualifiers = 0,
  transparent_qualifier = 1,
const_qualifier = 2,
volatile_qualifier = 4,
restrict_qualifier = 8,
  last_qualifier
} type_quals;
typedef struct user_qual_list
{
  user_qual qual;
  struct user_qual_list *next;
} *user_qual_list;
user_qual_list new_user_qual_list(user_qual, user_qual_list);
user_qual_list copy_user_qual_list(user_qual_list l);
bool member_user_qual_list(user_qual_list l, char *q);
user_qual_list union_user_qual_list(user_qual_list left,
                                    user_qual_list right);
bool empty_user_qual_list(user_qual_list);
user_qual_list type_user_quals(type t);
extern type float_type, double_type, long_double_type,
  int_type, unsigned_int_type, long_type, unsigned_long_type,
  long_long_type, unsigned_long_long_type, short_type, unsigned_short_type,
  char_type, char_array_type, wchar_type, wchar_array_type,
  unsigned_char_type, signed_char_type, void_type, ptr_void_type,
  size_t_type, ptrdiff_t_type, intptr_type;
extern type error_type;
void init_types(void);
type make_complex_type(type t);
type make_base_type(type t);
type make_qualified_type(type t, type_quals qualifiers,
                         user_qual_list user_quals);
type qualify_type1(type t, type t1);
type qualify_type2(type t, type t1, type t2);
type align_type(type t, int new_alignment);
type make_pointer_type(type t);
type make_array_type(type t, expression size);
type make_function_type(type t, typelist argtypes, bool varargs, type_quals varargs_quals, user_qual_list varargs_user_quals, bool oldstyle);
type make_tagged_type(tag_declaration d);
typelist new_typelist(region r);
void typelist_append(typelist l, type t);
bool empty_typelist(typelist l);
typedef struct typelist_element *typelist_scanner;
void typelist_scan(typelist tl, typelist_scanner *scanner);
type typelist_next(typelist_scanner *scanner);
size_t type_size(type t);
size_t type_alignment(type t);
bool type_has_size(type t);
bool type_size_cc(type t);
type common_type(type t1, type t2);
bool type_equal(type t1, type t2);
bool type_equal_unqualified(type t1, type t2);
bool type_compatible(type t1, type t2);
bool type_compatible_unqualified(type t1, type t2);
bool type_self_promoting(type t);
bool type_incomplete(type t);
char *qualifier_name(type_quals q);
type_quals type_qualifiers(type t);
bool qual_const(type_quals q);
bool qual_volatile(type_quals q);
bool qual_restrict(type_quals q);
bool force_qual_const(type_quals q);
bool force_qual_volatile(type_quals q);
bool force_qual_restrict(type_quals q);
bool type_const(type t);
bool type_volatile(type t);
bool type_restrict(type t);
bool type_transparent(type t);
bool type_readonly(type t);
bool type_atomic(type t);
bool type_plain_char(type t);
bool type_signed_char(type t);
bool type_unsigned_char(type t);
bool type_short(type t);
bool type_unsigned_short(type t);
bool type_int(type t);
bool type_unsigned_int(type t);
bool type_long(type t);
bool type_unsigned_long(type t);
bool type_long_long(type t);
bool type_unsigned_long_long(type t);
bool type_long_double(type t);
bool type_tagged(type t);
bool type_integral(type t);
bool type_floating(type t);
bool type_complex(type t);
bool type_float(type t);
bool type_double(type t);
bool type_void(type t);
bool type_char(type t);
bool type_function(type t);
bool type_array(type t);
bool type_pointer(type t);
bool type_enum(type t);
bool type_struct(type t);
bool type_union(type t);
bool type_integer(type t);
bool type_unsigned(type t);
bool type_smallerthanint(type t);
bool type_real(type t);
bool type_arithmetic(type t);
bool type_scalar(type t);
bool type_aggregate(type t);
type make_unsigned_type(type t);
type type_function_return_type(type t);
typelist type_function_arguments(type t);
bool type_function_varargs(type t);
type_quals type_function_varargs_quals(type t);
user_qual_list type_function_varargs_user_quals(type t);
bool type_function_oldstyle(type t);
bool self_promoting_args(type fntype);
type type_points_to(type t);
type type_array_of(type t);
type type_array_of_base(type t);
expression type_array_size(type t);
tag_declaration type_tag(type t);
type type_base(type t);
qtype tag_decl_qtype(tag_declaration td);
tag_ref tag_decl_to_tag_ref(tag_declaration td);
void tag_decl_set_qtype(tag_declaration td, qtype qt);
bool tag_decl_is_defined(tag_declaration td);
const char * tag_decl_name(tag_declaration td);
const char * field_decl_name(field_declaration fd);
typedef field_declaration tag_decl_scanner;
void tag_decl_scan(tag_declaration, tag_decl_scanner *);
field_declaration tag_decl_next(tag_decl_scanner *);
void type2ast(region r, location loc, type t, declarator inside,
              declarator *d, type_element *modifiers);
bool type_contains_pointers(type t);
bool type_contains_union_with_pointers(type t);
bool type_contains_quals(type t);
bool type_contains_user_quals(type t);
type type_default_conversion(type from);
type function_call_type(function_call fcall);
void name_tag(tag_declaration tag);
type type_for_size(int size, bool isunsigned);
type type_for_cval(cval c, bool isunsigned);
type make_type_var(cstring cs);
bool type_var(type t);
const char *type_name(type t);
         enum { struct_type, union_type, enum_type };
node new_node(region r, location loc);
declaration new_declaration(region r, location loc);
statement new_statement(region r, location loc);
expression new_expression(region r, location loc);
type_element new_type_element(region r, location loc);
declarator new_declarator(region r, location loc);
label new_label(region r, location loc);
asm_decl new_asm_decl(region r, location loc, asm_stmt asm_stmt);
data_decl new_data_decl(region r, location loc, type_element modifiers, attribute attributes, declaration decls);
extension_decl new_extension_decl(region r, location loc, declaration decl);
ellipsis_decl new_ellipsis_decl(region r, location loc, type_element qualifiers);
enumerator new_enumerator(region r, location loc, cstring cstring, expression arg1, data_declaration ddecl);
oldidentifier_decl new_oldidentifier_decl(region r, location loc, cstring cstring, data_declaration ddecl);
function_decl new_function_decl(region r, location loc, declarator declarator, type_element qualifiers, attribute attributes, declaration old_parms, statement stmt, function_decl parent_function, data_declaration ddecl);
implicit_decl new_implicit_decl(region r, location loc, identifier ident);
variable_decl new_variable_decl(region r, location loc, declarator declarator, attribute attributes, expression arg1, asm_stmt asm_stmt, data_declaration ddecl);
field_decl new_field_decl(region r, location loc, declarator declarator, attribute attributes, expression arg1);
asttype new_asttype(region r, location loc, declarator declarator, type_element qualifiers);
typename new_typename(region r, location loc, data_declaration ddecl);
type_variable new_type_variable(region r, location loc, cstring cstring);
typeof_expr new_typeof_expr(region r, location loc, expression arg1);
typeof_type new_typeof_type(region r, location loc, asttype asttype);
attribute new_attribute(region r, location loc, word word1, word word2, expression args);
rid new_rid(region r, location loc, enum rid id);
user_qual new_user_qual(region r, location loc, cstring cstring);
qualifier new_qualifier(region r, location loc, enum rid id);
tag_ref new_tag_ref(region r, location loc, word word1, attribute attributes, declaration fields, bool defined);
function_declarator new_function_declarator(region r, location loc, declarator declarator, declaration parms, type_element qualifiers, environment env);
pointer_declarator new_pointer_declarator(region r, location loc, declarator declarator, type_element qualifiers);
array_declarator new_array_declarator(region r, location loc, declarator declarator, expression arg1);
identifier_declarator new_identifier_declarator(region r, location loc, cstring cstring);
asm_stmt new_asm_stmt(region r, location loc, expression arg1, asm_operand asm_operands1, asm_operand asm_operands2, string asm_clobbers, type_element qualifiers);
compound_stmt new_compound_stmt(region r, location loc, id_label id_labels, declaration decls, statement stmts, environment env);
if_stmt new_if_stmt(region r, location loc, expression condition, statement stmt1, statement stmt2);
labeled_stmt new_labeled_stmt(region r, location loc, label label, statement stmt);
expression_stmt new_expression_stmt(region r, location loc, expression arg1);
breakable_stmt new_breakable_stmt(region r, location loc);
conditional_stmt new_conditional_stmt(region r, location loc, expression condition, statement stmt);
switch_stmt new_switch_stmt(region r, location loc, expression condition, statement stmt);
for_stmt new_for_stmt(region r, location loc, expression arg1, expression arg2, expression arg3, statement stmt);
break_stmt new_break_stmt(region r, location loc);
continue_stmt new_continue_stmt(region r, location loc);
return_stmt new_return_stmt(region r, location loc, expression arg1);
goto_stmt new_goto_stmt(region r, location loc, id_label id_label);
computed_goto_stmt new_computed_goto_stmt(region r, location loc, expression arg1);
empty_stmt new_empty_stmt(region r, location loc);
assert_type_stmt new_assert_type_stmt(region r, location loc, expression arg1, asttype asttype);
change_type_stmt new_change_type_stmt(region r, location loc, expression arg1, asttype asttype);
deep_restrict_stmt new_deep_restrict_stmt(region r, location loc, expression arg1, statement stmt);
unary new_unary(region r, location loc, expression arg1);
binary new_binary(region r, location loc, expression arg1, expression arg2);
comma new_comma(region r, location loc, expression arg1);
sizeof_type new_sizeof_type(region r, location loc, asttype asttype);
alignof_type new_alignof_type(region r, location loc, asttype asttype);
label_address new_label_address(region r, location loc, id_label id_label);
cast new_cast(region r, location loc, expression arg1, asttype asttype);
cast_list new_cast_list(region r, location loc, asttype asttype, expression init_expr);
conditional new_conditional(region r, location loc, expression condition, expression arg1, expression arg2);
identifier new_identifier(region r, location loc, cstring cstring, data_declaration ddecl);
compound_expr new_compound_expr(region r, location loc, statement stmt);
function_call new_function_call(region r, location loc, expression arg1, expression args, asttype va_arg_call);
array_ref new_array_ref(region r, location loc, expression arg1, expression arg2);
field_ref new_field_ref(region r, location loc, expression arg1, cstring cstring, location cstring_loc);
init_list new_init_list(region r, location loc, expression args);
init_index new_init_index(region r, location loc, expression arg1, expression arg2, expression init_expr);
init_field new_init_field(region r, location loc, word word1, expression init_expr);
lexical_cst new_lexical_cst(region r, location loc, cstring cstring);
string_cst new_string_cst(region r, location loc, cstring cstring, wchar_t * chars, size_t length);
string new_string(region r, location loc, expression strings, data_declaration ddecl);
id_label new_id_label(region r, location loc, cstring cstring);
case_label new_case_label(region r, location loc, expression arg1, expression arg2);
default_label new_default_label(region r, location loc);
word new_word(region r, location loc, cstring cstring);
asm_operand new_asm_operand(region r, location loc, string string, expression arg1);
error_decl new_error_decl(region r, location loc);
struct_ref new_struct_ref(region r, location loc, word word1, attribute attributes, declaration fields, bool defined);
union_ref new_union_ref(region r, location loc, word word1, attribute attributes, declaration fields, bool defined);
enum_ref new_enum_ref(region r, location loc, word word1, attribute attributes, declaration fields, bool defined);
error_stmt new_error_stmt(region r, location loc);
while_stmt new_while_stmt(region r, location loc, expression condition, statement stmt);
dowhile_stmt new_dowhile_stmt(region r, location loc, expression condition, statement stmt);
error_expr new_error_expr(region r, location loc);
dereference new_dereference(region r, location loc, expression arg1);
extension_expr new_extension_expr(region r, location loc, expression arg1);
sizeof_expr new_sizeof_expr(region r, location loc, expression arg1);
alignof_expr new_alignof_expr(region r, location loc, expression arg1);
realpart new_realpart(region r, location loc, expression arg1);
imagpart new_imagpart(region r, location loc, expression arg1);
address_of new_address_of(region r, location loc, expression arg1);
unary_minus new_unary_minus(region r, location loc, expression arg1);
unary_plus new_unary_plus(region r, location loc, expression arg1);
conjugate new_conjugate(region r, location loc, expression arg1);
preincrement new_preincrement(region r, location loc, expression arg1);
predecrement new_predecrement(region r, location loc, expression arg1);
postincrement new_postincrement(region r, location loc, expression arg1);
postdecrement new_postdecrement(region r, location loc, expression arg1);
bitnot new_bitnot(region r, location loc, expression arg1);
not new_not(region r, location loc, expression arg1);
plus new_plus(region r, location loc, expression arg1, expression arg2);
minus new_minus(region r, location loc, expression arg1, expression arg2);
times new_times(region r, location loc, expression arg1, expression arg2);
divide new_divide(region r, location loc, expression arg1, expression arg2);
modulo new_modulo(region r, location loc, expression arg1, expression arg2);
lshift new_lshift(region r, location loc, expression arg1, expression arg2);
rshift new_rshift(region r, location loc, expression arg1, expression arg2);
leq new_leq(region r, location loc, expression arg1, expression arg2);
geq new_geq(region r, location loc, expression arg1, expression arg2);
lt new_lt(region r, location loc, expression arg1, expression arg2);
gt new_gt(region r, location loc, expression arg1, expression arg2);
eq new_eq(region r, location loc, expression arg1, expression arg2);
ne new_ne(region r, location loc, expression arg1, expression arg2);
bitand new_bitand(region r, location loc, expression arg1, expression arg2);
bitor new_bitor(region r, location loc, expression arg1, expression arg2);
bitxor new_bitxor(region r, location loc, expression arg1, expression arg2);
andand new_andand(region r, location loc, expression arg1, expression arg2);
oror new_oror(region r, location loc, expression arg1, expression arg2);
assign new_assign(region r, location loc, expression arg1, expression arg2);
plus_assign new_plus_assign(region r, location loc, expression arg1, expression arg2);
minus_assign new_minus_assign(region r, location loc, expression arg1, expression arg2);
times_assign new_times_assign(region r, location loc, expression arg1, expression arg2);
divide_assign new_divide_assign(region r, location loc, expression arg1, expression arg2);
modulo_assign new_modulo_assign(region r, location loc, expression arg1, expression arg2);
lshift_assign new_lshift_assign(region r, location loc, expression arg1, expression arg2);
rshift_assign new_rshift_assign(region r, location loc, expression arg1, expression arg2);
bitand_assign new_bitand_assign(region r, location loc, expression arg1, expression arg2);
bitor_assign new_bitor_assign(region r, location loc, expression arg1, expression arg2);
bitxor_assign new_bitxor_assign(region r, location loc, expression arg1, expression arg2);
struct AST_node {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_declaration {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_statement {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_expression {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
};
struct AST_type_element {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_declarator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_label {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  label next_label;
  compound_stmt enclosing_cs;
  store store_in;
};
struct AST_asm_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  asm_stmt asm_stmt;
};
struct AST_data_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type_element modifiers;
  attribute attributes;
  declaration decls;
};
struct AST_extension_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declaration decl;
};
struct AST_ellipsis_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type_element qualifiers;
};
struct AST_enumerator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
  expression arg1;
  data_declaration ddecl;
};
struct AST_oldidentifier_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
  data_declaration ddecl;
};
struct AST_function_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  type_element qualifiers;
  attribute attributes;
  declaration old_parms;
  statement stmt;
  function_decl parent_function;
  data_declaration ddecl;
  function_declarator fdeclarator;
  type declared_type;
  env undeclared_variables;
  env base_labels;
  env scoped_labels;
  breakable_stmt current_loop;
  int nlocals;
  effect scope_env;
};
struct AST_implicit_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  identifier ident;
};
struct AST_variable_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  attribute attributes;
  expression arg1;
  asm_stmt asm_stmt;
  data_declaration ddecl;
  type declared_type;
  bool forward;
  effect arg1_eff;
  rinfo rinfo;
};
struct AST_field_decl {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  attribute attributes;
  expression arg1;
};
struct AST_asttype {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  type_element qualifiers;
  type type;
};
struct AST_typename {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  data_declaration ddecl;
};
struct AST_type_variable {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
};
struct AST_typeof_expr {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
};
struct AST_typeof_type {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  asttype asttype;
};
struct AST_attribute {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  word word1;
  word word2;
  expression args;
};
struct AST_rid {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  enum rid id;
};
struct AST_user_qual {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
};
struct AST_qualifier {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  enum rid id;
};
struct AST_tag_ref {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  word word1;
  attribute attributes;
  declaration fields;
  bool defined;
  tag_declaration tdecl;
};
struct AST_function_declarator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  declaration parms;
  type_element qualifiers;
  environment env;
};
struct AST_pointer_declarator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  type_element qualifiers;
};
struct AST_array_declarator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  declarator declarator;
  expression arg1;
};
struct AST_identifier_declarator {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
};
struct AST_asm_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
  asm_operand asm_operands1;
  asm_operand asm_operands2;
  string asm_clobbers;
  type_element qualifiers;
};
struct AST_compound_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  id_label id_labels;
  declaration decls;
  statement stmts;
  compound_stmt enclosing_cs;
  int visited;
  environment env;
  effect scope_env;
  effect filter_env;
  dd_list confine_expressions;
  dd_list drinfolist;
};
struct AST_if_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression condition;
  statement stmt1;
  statement stmt2;
};
struct AST_labeled_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  label label;
  statement stmt;
};
struct AST_expression_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
};
struct AST_breakable_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  compound_stmt enclosing_cs;
  breakable_stmt parent_loop;
  store break_dest;
  store continue_dest;
};
struct AST_conditional_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  compound_stmt enclosing_cs;
  breakable_stmt parent_loop;
  store break_dest;
  store continue_dest;
  expression condition;
  statement stmt;
};
struct AST_switch_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  compound_stmt enclosing_cs;
  breakable_stmt parent_loop;
  store break_dest;
  store continue_dest;
  expression condition;
  statement stmt;
  label next_label;
};
struct AST_for_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  compound_stmt enclosing_cs;
  breakable_stmt parent_loop;
  store break_dest;
  store continue_dest;
  expression arg1;
  expression arg2;
  expression arg3;
  statement stmt;
};
struct AST_break_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  breakable_stmt parent_loop;
  compound_stmt enclosing_cs;
};
struct AST_continue_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  breakable_stmt parent_loop;
  compound_stmt enclosing_cs;
};
struct AST_return_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
  compound_stmt enclosing_cs;
};
struct AST_goto_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  id_label id_label;
  compound_stmt enclosing_cs;
};
struct AST_computed_goto_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
};
struct AST_empty_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
};
struct AST_assert_type_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
  asttype asttype;
  qtype qtype;
};
struct AST_change_type_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
  asttype asttype;
  qtype qtype;
};
struct AST_deep_restrict_stmt {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  expression arg1;
  statement stmt;
  drinfo drinfo;
};
struct AST_unary {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
};
struct AST_binary {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  expression arg2;
};
struct AST_comma {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
};
struct AST_sizeof_type {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  asttype asttype;
};
struct AST_alignof_type {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  asttype asttype;
};
struct AST_label_address {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  id_label id_label;
};
struct AST_cast {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  asttype asttype;
};
struct AST_cast_list {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  asttype asttype;
  qtype astqtype;
  expression init_expr;
};
struct AST_conditional {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression condition;
  expression arg1;
  expression arg2;
};
struct AST_identifier {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  cstring cstring;
  data_declaration ddecl;
  aloc aloc;
};
struct AST_compound_expr {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  statement stmt;
  effect filter_env;
};
struct AST_function_call {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  expression args;
  asttype va_arg_call;
};
struct AST_array_ref {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  expression arg2;
  expression alt;
};
struct AST_field_ref {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  cstring cstring;
  location cstring_loc;
};
struct AST_init_list {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression args;
};
struct AST_init_index {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression arg1;
  expression arg2;
  expression init_expr;
};
struct AST_init_field {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  word word1;
  expression init_expr;
};
struct AST_lexical_cst {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  cstring cstring;
};
struct AST_string_cst {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  cstring cstring;
  wchar_t * chars;
  size_t length;
};
struct AST_string {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  type type;
  bool lvalue;
  bool side_effects;
  known_cst cst;
  bool bitfield;
  bool isregister;
  known_cst static_address;
  qtype qtype;
  expression strings;
  data_declaration ddecl;
};
struct AST_id_label {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  label next_label;
  compound_stmt enclosing_cs;
  store store_in;
  cstring cstring;
  label_declaration ldecl;
};
struct AST_case_label {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  label next_label;
  compound_stmt enclosing_cs;
  store store_in;
  expression arg1;
  expression arg2;
};
struct AST_default_label {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  label next_label;
  compound_stmt enclosing_cs;
  store store_in;
};
struct AST_word {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  cstring cstring;
};
struct AST_asm_operand {
  ast_kind kind;
  location loc;
  node next;
  node parent;
  string string;
  expression arg1;
};
node node_chain(node l1, node l2);
declaration declaration_chain(declaration l1, declaration l2);
statement statement_chain(statement l1, statement l2);
expression expression_chain(expression l1, expression l2);
type_element type_element_chain(type_element l1, type_element l2);
declarator declarator_chain(declarator l1, declarator l2);
label label_chain(label l1, label l2);
asm_decl asm_decl_chain(asm_decl l1, asm_decl l2);
data_decl data_decl_chain(data_decl l1, data_decl l2);
extension_decl extension_decl_chain(extension_decl l1, extension_decl l2);
ellipsis_decl ellipsis_decl_chain(ellipsis_decl l1, ellipsis_decl l2);
enumerator enumerator_chain(enumerator l1, enumerator l2);
oldidentifier_decl oldidentifier_decl_chain(oldidentifier_decl l1, oldidentifier_decl l2);
function_decl function_decl_chain(function_decl l1, function_decl l2);
implicit_decl implicit_decl_chain(implicit_decl l1, implicit_decl l2);
variable_decl variable_decl_chain(variable_decl l1, variable_decl l2);
field_decl field_decl_chain(field_decl l1, field_decl l2);
asttype asttype_chain(asttype l1, asttype l2);
typename typename_chain(typename l1, typename l2);
type_variable type_variable_chain(type_variable l1, type_variable l2);
typeof_expr typeof_expr_chain(typeof_expr l1, typeof_expr l2);
typeof_type typeof_type_chain(typeof_type l1, typeof_type l2);
attribute attribute_chain(attribute l1, attribute l2);
rid rid_chain(rid l1, rid l2);
user_qual user_qual_chain(user_qual l1, user_qual l2);
qualifier qualifier_chain(qualifier l1, qualifier l2);
tag_ref tag_ref_chain(tag_ref l1, tag_ref l2);
function_declarator function_declarator_chain(function_declarator l1, function_declarator l2);
pointer_declarator pointer_declarator_chain(pointer_declarator l1, pointer_declarator l2);
array_declarator array_declarator_chain(array_declarator l1, array_declarator l2);
identifier_declarator identifier_declarator_chain(identifier_declarator l1, identifier_declarator l2);
asm_stmt asm_stmt_chain(asm_stmt l1, asm_stmt l2);
compound_stmt compound_stmt_chain(compound_stmt l1, compound_stmt l2);
if_stmt if_stmt_chain(if_stmt l1, if_stmt l2);
labeled_stmt labeled_stmt_chain(labeled_stmt l1, labeled_stmt l2);
expression_stmt expression_stmt_chain(expression_stmt l1, expression_stmt l2);
breakable_stmt breakable_stmt_chain(breakable_stmt l1, breakable_stmt l2);
conditional_stmt conditional_stmt_chain(conditional_stmt l1, conditional_stmt l2);
switch_stmt switch_stmt_chain(switch_stmt l1, switch_stmt l2);
for_stmt for_stmt_chain(for_stmt l1, for_stmt l2);
break_stmt break_stmt_chain(break_stmt l1, break_stmt l2);
continue_stmt continue_stmt_chain(continue_stmt l1, continue_stmt l2);
return_stmt return_stmt_chain(return_stmt l1, return_stmt l2);
goto_stmt goto_stmt_chain(goto_stmt l1, goto_stmt l2);
computed_goto_stmt computed_goto_stmt_chain(computed_goto_stmt l1, computed_goto_stmt l2);
empty_stmt empty_stmt_chain(empty_stmt l1, empty_stmt l2);
assert_type_stmt assert_type_stmt_chain(assert_type_stmt l1, assert_type_stmt l2);
change_type_stmt change_type_stmt_chain(change_type_stmt l1, change_type_stmt l2);
deep_restrict_stmt deep_restrict_stmt_chain(deep_restrict_stmt l1, deep_restrict_stmt l2);
unary unary_chain(unary l1, unary l2);
binary binary_chain(binary l1, binary l2);
comma comma_chain(comma l1, comma l2);
sizeof_type sizeof_type_chain(sizeof_type l1, sizeof_type l2);
alignof_type alignof_type_chain(alignof_type l1, alignof_type l2);
label_address label_address_chain(label_address l1, label_address l2);
cast cast_chain(cast l1, cast l2);
cast_list cast_list_chain(cast_list l1, cast_list l2);
conditional conditional_chain(conditional l1, conditional l2);
identifier identifier_chain(identifier l1, identifier l2);
compound_expr compound_expr_chain(compound_expr l1, compound_expr l2);
function_call function_call_chain(function_call l1, function_call l2);
array_ref array_ref_chain(array_ref l1, array_ref l2);
field_ref field_ref_chain(field_ref l1, field_ref l2);
init_list init_list_chain(init_list l1, init_list l2);
init_index init_index_chain(init_index l1, init_index l2);
init_field init_field_chain(init_field l1, init_field l2);
lexical_cst lexical_cst_chain(lexical_cst l1, lexical_cst l2);
string_cst string_cst_chain(string_cst l1, string_cst l2);
string string_chain(string l1, string l2);
id_label id_label_chain(id_label l1, id_label l2);
case_label case_label_chain(case_label l1, case_label l2);
default_label default_label_chain(default_label l1, default_label l2);
word word_chain(word l1, word l2);
asm_operand asm_operand_chain(asm_operand l1, asm_operand l2);
error_decl error_decl_chain(error_decl l1, error_decl l2);
struct_ref struct_ref_chain(struct_ref l1, struct_ref l2);
union_ref union_ref_chain(union_ref l1, union_ref l2);
enum_ref enum_ref_chain(enum_ref l1, enum_ref l2);
error_stmt error_stmt_chain(error_stmt l1, error_stmt l2);
while_stmt while_stmt_chain(while_stmt l1, while_stmt l2);
dowhile_stmt dowhile_stmt_chain(dowhile_stmt l1, dowhile_stmt l2);
error_expr error_expr_chain(error_expr l1, error_expr l2);
dereference dereference_chain(dereference l1, dereference l2);
extension_expr extension_expr_chain(extension_expr l1, extension_expr l2);
sizeof_expr sizeof_expr_chain(sizeof_expr l1, sizeof_expr l2);
alignof_expr alignof_expr_chain(alignof_expr l1, alignof_expr l2);
realpart realpart_chain(realpart l1, realpart l2);
imagpart imagpart_chain(imagpart l1, imagpart l2);
address_of address_of_chain(address_of l1, address_of l2);
unary_minus unary_minus_chain(unary_minus l1, unary_minus l2);
unary_plus unary_plus_chain(unary_plus l1, unary_plus l2);
conjugate conjugate_chain(conjugate l1, conjugate l2);
preincrement preincrement_chain(preincrement l1, preincrement l2);
predecrement predecrement_chain(predecrement l1, predecrement l2);
postincrement postincrement_chain(postincrement l1, postincrement l2);
postdecrement postdecrement_chain(postdecrement l1, postdecrement l2);
bitnot bitnot_chain(bitnot l1, bitnot l2);
not not_chain(not l1, not l2);
plus plus_chain(plus l1, plus l2);
minus minus_chain(minus l1, minus l2);
times times_chain(times l1, times l2);
divide divide_chain(divide l1, divide l2);
modulo modulo_chain(modulo l1, modulo l2);
lshift lshift_chain(lshift l1, lshift l2);
rshift rshift_chain(rshift l1, rshift l2);
leq leq_chain(leq l1, leq l2);
geq geq_chain(geq l1, geq l2);
lt lt_chain(lt l1, lt l2);
gt gt_chain(gt l1, gt l2);
eq eq_chain(eq l1, eq l2);
ne ne_chain(ne l1, ne l2);
bitand bitand_chain(bitand l1, bitand l2);
bitor bitor_chain(bitor l1, bitor l2);
bitxor bitxor_chain(bitxor l1, bitxor l2);
andand andand_chain(andand l1, andand l2);
oror oror_chain(oror l1, oror l2);
assign assign_chain(assign l1, assign l2);
plus_assign plus_assign_chain(plus_assign l1, plus_assign l2);
minus_assign minus_assign_chain(minus_assign l1, minus_assign l2);
times_assign times_assign_chain(times_assign l1, times_assign l2);
divide_assign divide_assign_chain(divide_assign l1, divide_assign l2);
modulo_assign modulo_assign_chain(modulo_assign l1, modulo_assign l2);
lshift_assign lshift_assign_chain(lshift_assign l1, lshift_assign l2);
rshift_assign rshift_assign_chain(rshift_assign l1, rshift_assign l2);
bitand_assign bitand_assign_chain(bitand_assign l1, bitand_assign l2);
bitor_assign bitor_assign_chain(bitor_assign l1, bitor_assign l2);
bitxor_assign bitxor_assign_chain(bitxor_assign l1, bitxor_assign l2);
typedef struct AST_ast_generic
{
  ast_kind kind;
} *ast_generic;
extern declaration the_program;
unary newkind_unary(region r, ast_kind kind, location location, expression arg1);
binary newkind_binary(region r, ast_kind kind, location location,
                      expression arg1, expression arg2);
tag_ref newkind_tag_ref(region r, ast_kind kind, location location, word word1, attribute attributes, declaration fields, bool defined);
node last_node(node n);
int chain_length(node n);
node ast_chain(node l1, node l2);
void insert_before(node *list, node before, node n);
node ast_reverse(node l);
void AST_set_parents(node n);
void AST_print(node n);
typedef dd_list identifier_set; typedef dd_list_pos identifier_set_scanner; static inline identifier_set empty_identifier_set(region r) { return dd_new_list(r); } static inline identifier_set identifier_set_copy(region r, identifier_set s) { if (s == ((void *)0)) return ((void *)0); return dd_copy(r, s); } static inline bool identifier_set_empty(identifier_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool identifier_set_member(int (*cmp)(identifier, identifier), identifier_set s, identifier elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) cmp, (void *) elt) != ((void *)0); } static inline int identifier_set_size(identifier_set s) { if (s == ((void *)0)) return 0; return dd_length(s); } static inline bool identifier_set_insert(region r, identifier_set *s, identifier elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool identifier_set_insert_last(region r, identifier_set *s, identifier elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline identifier_set identifier_set_union(identifier_set s1, identifier_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool identifier_set_single(identifier_set s) { return identifier_set_size(s) == 1; } static inline void identifier_set_sort(int (*cmp)(identifier, identifier), identifier_set s) { if (s == ((void *)0)) return; dd_sort(s, (set_cmp_fn) cmp); } static inline void identifier_set_remove_dups(int (*cmp)(identifier, identifier), identifier_set s) { if (s == ((void *)0)) return; dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void identifier_set_scan(identifier_set s, identifier_set_scanner *ss) { if (s == ((void *)0)) *ss = ((void *)0); else *ss = dd_first(s); } static inline identifier identifier_set_next(identifier_set_scanner *ss) { identifier result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((identifier)((*ss)->data)); *ss = ((*ss)->next); return result; };
extern int flag_volatile;
extern int flag_volatile_global;
extern int flag_syntax_only;
extern int flag_pedantic_errors;
extern int flag_pack_struct;
extern int inhibit_warnings;
extern int extra_warnings;
extern int warnings_are_errors;
extern int warn_unused;
extern int warn_uninitialized;
extern int warn_shadow;
extern int error_shadow;
extern int warn_switch;
extern int warn_return_type;
extern int warn_cast_align;
extern int warn_id_clash;
extern unsigned id_clash_len;
extern int warn_larger_than;
extern unsigned larger_than_size;
extern int warn_inline;
extern int warn_aggregate_return;
extern int dollars_in_ident;
extern int flag_cond_mismatch;
extern int flag_no_asm;
extern int flag_hosted;
extern int warn_implicit;
extern int warn_write_strings;
extern int warn_pointer_arith;
extern int warn_strict_prototypes;
extern int warn_redundant_decls;
extern int warn_nested_externs;
extern int warn_cast_qual;
extern int warn_bad_function_cast;
extern int warn_traditional;
extern int warn_format;
extern int warn_char_subscripts;
extern int warn_conversion;
extern int warn_main;
extern int warn_multichar;
extern int flag_traditional;
extern int flag_allow_single_precision;
extern int warn_parentheses;
extern int warn_missing_braces;
extern int warn_sign_compare;
extern int mesg_implicit_function_declaration;
extern bool pedantic;
extern int warn_implicit_int;
extern int warn_missing_prototypes;
extern int warn_missing_declarations;
extern int quiet_flag;
extern int flag_signed_char;
extern int flag_short_enums;
extern int flag_signed_bitfields;
extern int explicit_flag_signed_bitfields;
extern int flag_parse_only;
extern int flag_pam_mode;
extern int flag_pam_html;
extern int flag_print_quals_graph;
extern int flag_strict_const;
extern int flag_print_results;
extern int flag_casts_preserve;
extern int flag_const_subtyping;
extern int flag_flow_sensitive;
extern int flag_poly;
extern int flag_aloc_subtyping;
extern int flag_force_flow_sensitive;
extern int flag_print_lin;
extern int flag_driver;
extern int flag_casts_warn;
extern int flag_ugly;
extern int flag_statistics;
extern int flag_print_stores;
extern int flag_confine_inf;
extern int flag_discover_scopes;
extern int flag_confine_inf_aggressive;
extern int flag_restrict_inf;
extern int flag_print_assert_type_failures;
int flag_strong_updates;
extern int flag_errors_only;
extern int flag_context_summary;
extern int flag_warn_dangerous_globals;
extern int flag_measure_consts;
extern int errorcount;
extern int warningcount;
void verror_with_file_and_line(const char *filename, int lineno,
                               const char *format, va_list args);
void verror_with_location(location l, const char *format, va_list args);
void verror_with_decl(declaration d, const char *format, va_list args);
void verror(const char *format, va_list args);
void error(const char *format, ...);
void error_with_decl(declaration d, const char *format, ...);
void error_with_location(location l, const char *format, ...);
void vfatal(const char *format, va_list args);
void fatal(const char *format, ...);
void vwarning_with_file_and_line(const char *filename, int lineno,
                                 const char *format, va_list args);
void vwarning_with_location(location l, const char *format, va_list args);
void vwarning_with_decl(declaration d, const char *format, va_list args);
void vwarning(const char *format, va_list args);
void warning(const char *format, ...);
void warning_with_file_and_line(const char *filename, int lineno,
                                const char *format, ...);
void warning_with_decl(declaration d, const char *format, ...);
void warning_with_location(location l, const char *format, ...);
void warning_or_error(bool iswarning, const char *format, ...);
void warning_or_error_with_file_and_line(bool iswarning,
                                         const char *filename, int lineno,
                                         const char *format, ...);
void warning_or_error_with_decl(bool iswarning, declaration d,
                                const char *format, ...);
void warning_or_error_with_location(bool iswarning, location l,
                                    const char *format, ...);
void pedwarn(const char *format, ...);
void pedwarn_with_decl(declaration d, const char *format, ...);
void pedwarn_with_location(location l, const char *format, ...);
extern char *progname;
extern int copy_argc;
extern char **copy_argv;
typedef struct Qtypelist *qtypelist;
typedef qtypelist qtypelist_scanner;
typedef void *field_scanner;
void init_qtype(void);
void qtypelist_scan(qtypelist l, qtypelist_scanner *);
qtype qtypelist_next(qtypelist_scanner *);
bool qtype_error(qtype qt);
bool qtype_primitive(qtype qt);
bool qtype_char(qtype qt);
bool qtype_signed_char(qtype qt);
bool qtype_unsigned_char(qtype qt);
bool qtype_int(qtype qt);
bool qtype_unsigned_int(qtype qt);
bool qtype_float(qtype qt);
bool qtype_complex(qtype qt);
bool qtype_aggregate(qtype qt);
bool qtype_struct(qtype qt);
bool qtype_union(qtype qt);
bool qtype_transparent(qtype qt);
bool qtype_void(qtype qt);
bool qtype_pointer(qtype qt);
bool qtype_pointer_loc(qtype qt);
bool qtype_pointer_or_loc(qtype qt);
bool qtype_function(qtype qt);
bool qtype_varargs(qtype qt);
bool qtype_oldstyle(qtype qt);
bool qtype_array(qtype qt);
bool qtype_unsigned(qtype qt);
bool qtype_smallerthanint(qtype qt);
bool qtype_integral(qtype qt);
bool qtype_string(qtype qt);
bool qtype_var(qtype qt);
bool qtype_scheme(qtype qt);
bool qtype_void_pointer(qtype qt);
bool qtype_char_pointer(qtype qt);
extern qtype error_qtype;
qtype mkqtype_void(qual);
qtype mkqtype_bool(qual);
qtype mkqtype_char(qual);
qtype mkqtype_int(qual);
qtype mkqtype_unsigned_int(qual);
qtype mkqtype_long(qual);
qtype mkqtype_unsigned_long(qual);
qtype mkqtype_long_long(qual);
qtype mkqtype_unsigned_long_long(qual);
qtype mkqtype_float(qual);
qtype mkqtype_double(qual);
qtype mkqtype_size_t(qual);
qtype mkqtype_ptrdiff_t(qual);
qtype mkqtype_pointer(location loc, qual, qtype, aloc);
qtype mkqtype_pointer_loc(location loc, qual, aloc);
qtype mkqtype_function(qual q, bool varargs, qual vquals, bool oldstyle,
                       qtype ret, qtypelist args, store store_in,
                       store store_out, aloc al, effect eff);
qtype mkqtype_var(qual q, const char *name);
qtype mkqtype_fresh(void);
qtype complex_base_qtype(qtype qt);
qtype points_to_qtype(qtype qt);
aloc aloc_qtype(qtype qt);
qtype array_of_qtype(qtype qt);
expression array_size_qtype(qtype qt);
aloc array_aloc_qtype(qtype qt);
qtype return_qtype(qtype qt);
qtypelist arg_qtypes(qtype qt);
effect effect_qtype(qtype qt);
qual vqual_qtype(qtype qt);
store store_in_qtype(qtype qt);
store store_out_qtype(qtype qt);
const char *tag_name_qtype(qtype qt);
qtype field_qtype(qtype qt, cstring field);
void field_scan(qtype qt, field_scanner *);
void field_scan_at(qtype qt, cstring field, field_scanner *);
qtype field_next(field_scanner *);
qual qual_qtype(qtype qt);
bool has_qual_qtype(qtype qt, qual bound);
effect alocs_qtype(qtype qt);
effect rwr_alocs_qtype(qtype qt);
effect defn_effect_qtype(qtype qt);
effect constr_effect_qtype(qtype qt, eff_kind k);
qtype decl_to_qtype(type lifted_t, location loc, const char *name,
                    bool preferred, bool isglobal, bool generalize,
                    bool noreturn, bool __init);
qtype get_fdecl_qtype(const char *tag_name, field_declaration fdecl, type ft, location loc);
qtype type_to_qtype(type, const char *, location loc);
qtype copy_qtype(qtype qt, location loc);
qtype copy_qtype_poly(qtype qt, location loc);
qtype generalize_qtype(qtype qt);
qtype instantiate_qtype(qtype qt, location loc);
bool match_qtype(qtype qt1, qtype qt2,
                 void (*match_quals)(qual q1, qual q2, void *arg), void *arg);
bool match_type(type t1, type t2);
bool mkleq_qtype(location loc, qual_gate *qgate, qtype qt1, qtype qt2);
bool mkeq_qtype(location loc, qual_gate *qgate, qtype qt1, qtype qt2);
bool mkinst_qtype(location loc, qual_gate *qgate, qtype qt1, qtype qt2, polarity p);
bool unify_qtype(location loc, qtype qt1, qtype qt2);
bool mkeq_qtype_cast(location loc, qual_gate *qgate, qtype qt1, qtype qt2);
bool lub_qtype(location loc, qual_gate *qgate, qtype qt1, qtype qt2, qtype *lub);
bool eq_qtype(qtype qt1, qtype qt2);
bool varargs_constrain_quals_qtype(location loc, qtype qt, qual q);
void mknin_effect_qtype(location loc, effect e, qtype qt);
qtype default_conversion_qtype(location loc, qtype qt);
bool mkleq_assign_convert(location loc, qual_gate *qgate, qtype rhs_qtype,
                          qtype lhs_qtype);
int print_qtype(printf_func, qtype, store);
int print_qtype_raw(printf_func, qtype, store);
int print_qtype_qf(printf_func, pr_qual_fn, qtype, store, bool);
const char *color_qtype(qtype, store);
bool nontriv_soln_qtype(qtype qt, store s);
store ref_qtype_store(location loc, qtype qt, store in);
qtype qtype_to_fs_qtype_with_quals(location loc, qtype qt, store s);
qtype qtype_to_fs_qtype(location loc, qtype qt);
bool store_aloc_qtype(qtype qt, store s, aloc al);
bool toplvl_qual_fs_qtype(qtype qt);
void mk_no_qual_qtype_fs(location loc, qtype qt, store s);
qtype points_to_fs_qtype(qtype qt, store s);
qtype default_conversion_fs_qtype(location loc, qtype fs_qt);
bool mkleq_fs_qtype(location loc, qtype qt1, qtype qt2, store s);
bool mkleq_fs_assign_convert(location loc, qtype rhs_qtype, qtype lhs_qtype,
                             store s);
store assign_flow_sensitive(location loc, const char *err_msg,
                            qtype rhs_qtype, qtype lhs_qtype,
                            store s, bool strong);
void init_effects(void);
void init_store(void);
aloc make_aloc(const char *name, qtype points_to, bool global);
void set_points_to_aloc(aloc al, qtype points_to);
const char *name_aloc(aloc al);
unsigned long hash_aloc(aloc al);
bool global_aloc(aloc al);
unsigned long hash_aloc(aloc al);
qtype points_to_aloc(aloc al);
bool eq_aloc(aloc al1, aloc al2);
void unify_aloc(location loc, aloc al1, aloc al2);
void inst_aloc(location loc,aloc al1, aloc al2, polarity p);
int print_aloc(printf_func pf, aloc al);
extern effect effect_empty;
aloc aloc_effect(effect e);
void mark_aloc_interesting(aloc al);
effect effect_single(aloc al);
effect effect_alloc(aloc al);
effect effect_rwr(aloc al);
effect effect_r(aloc al);
effect effect_wr(aloc al);
effect effect_constr(aloc al, eff_kind k);
effect effect_var(const char *name);
effect effect_fresh(void);
effect effect_union(effect e1, effect e2);
effect effect_inter(effect e1, effect e2);
void mark_effect_interesting(effect e);
bool eq_effect(effect e1, effect e2);
void mkleq_effect(effect e1, effect e2);
void mkinst_effect(location l,effect e1, effect e2, polarity p);
void mkeq_effect(effect e1, effect e2);
void unify_effect(effect e1, effect e2);
void mknin_aloc_effect(location loc, aloc al, eff_kind kind, effect e);
void mknin_effect(location loc, effect e1, effect e2);
rinf_const mkrinf_constraint(location loc,
                             aloc r_al, aloc old_al,
                             aloc top_al,
                             effect r_type, effect pt_type,
                             effect r_body, effect env);
void set_bodyeffect(rinf_const rc, effect body);
void forall_aloc_mknin_effect(location loc, eff_kind kind, effect e);
void forall_aloc_in_effect_mknin_effect(location loc,
                                        eff_kind kind1, effect e1,
                                        eff_kind kind2, effect e2);
bool check_nin_aloc_effect(aloc al, effect e);
void check_nin(void);
void check_rinf_constraints(void);
int print_effect(printf_func pf, effect e);
bool eq_path_sig(int *p1, int *p2);
bool eq_store(store s1, store s2);
unsigned long hash_store(store s);
store make_store_var(location loc, const char *name);
const char *name_store(store s);
store make_store_filter(location loc, store s, effect e);
store make_store_ref(location loc, store s, aloc al);
store make_store_ref_effect(location loc, store s, effect e);
store make_store_ow(location loc, store s1, store s2, effect e);
store make_store_assign(location loc, const char *err_msg, store s, aloc al,
                        qtype qt, bool strong);
void mkleq_store(location loc, const char *err_msg, store left, store right);
void mkinst_store(location loc, const char *err_msg, store left, store right,
                  polarity p);
void mkeq_store(location loc, const char *err_msg, store s1, store s2);
store lub_store(location loc, const char *err_msg, store s1, store s2);
void unify_store(location loc, const char *err_msg, store s1, store s2);
void propagate_store_cell_forward(store s, aloc al);
void propagate_store_cell_backward(store s, aloc al);
void compute_lins(void);
int lin_from_store(store s, aloc al);
qtype qtype_from_store(store s, aloc al);
int print_store(printf_func pf, store s);
int print_store_cells(printf_func pf, pr_qual_fn pr_qual, store s,
                      bool print_alocs);
int print_store_raw(printf_func pf, store s);
void mk_confine_inf_const(drinfo dri);
void check_effect_constraints(void);
typedef enum {sev_err, sev_warn, sev_info} severity;
void init_qerror(void);
extern bool qerrors;
extern growbuf qual_error_detail;
extern growbuf qtype_error_detail;
int qual_ed_printf(const char *fmt, ...);
int qtype_ed_printf(const char *fmt, ...);
struct Rinfo {
  bool isglobal;
  effect flocs;
  effect flocs_alloc;
  qtype fqtype;
  qtype fs_fqtype;
  effect rlocs;
  effect rlocs_alloc;
  qtype rqtype;
  qtype fs_rqtype;
  effect arg1_eff;
  qual lin;
};
struct Drinfo {
  effect effect;
  expression expression;
  qtype qtype;
  qtype rqtype;
  qtype fs_qtype;
  qtype fs_rqtype;
  location location;
  effect env;
  compound_stmt cs;
  effect body_eff;
  bool failed;
  bool inference;
};
struct ExprDrinfoPair {
  expression e;
  drinfo drinfo;
};
void analyze(declaration program);
qtype analyze_field_declaration(const char *name, field_declaration fd);
void analyze_tag_ref(tag_ref);
void init_analyze(void);
void finish_analyze(void);
extern function_decl current_function_decl;
void report_qerror(location loc, severity sev, const char *format, ...);
location location_declaration(declaration);
data_declaration root_ddecl(data_declaration ddecl);
bool static_ddecl(data_declaration ddecl);
identifier_declarator get_id_declarator(declarator d);
bool is_undefined_global(data_declaration ddecl, store s);
bool is_unannotated_global(data_declaration ddecl, store s);
void warn_if_dangerous_global(data_declaration ddecl, store s);
typedef void (*traverse_global_fn)(data_declaration, void *);
void traverse_globals(traverse_global_fn f, void *arg);
void traverse_globals_sorted(traverse_global_fn f, void *arg);
effect get_global_effect(void);
data_declaration get_main_ddecl(void);
void measure_consts(void);
extern bool have_stream_quals;
extern qual open_unchecked_qual;
extern qual read_unchecked_qual;
extern qual write_unchecked_qual;
extern qual readwrite_unchecked_qual;
extern qual open_qual;
extern qual read_qual;
extern qual write_qual;
extern qual readwrite_qual;
extern qual closed_qual;
extern bool have_sleepy_quals;
extern qual enabled_qual;
extern qual disabled_qual;
extern qtype interrupt_status_qtype;
bool file_pointer_qtype(qtype qt);
static void unify_constrs(effect *c1, effect *c2);
int cmp_store(store s1, store s2);
typedef struct Nin_constraint
{
  aloc aloc;
  eff_kind kind;
  effect eff;
  location loc;
  bool failed;
  bool inference;
} *nin_constraint;
typedef struct Forall_nin_constraint
{
  eff_kind kind;
  location loc;
  effect eff;
  bool failed;
  bool inference;
} *forall_nin_constraint;
typedef struct Forall_cond_nin_constraint
{
  location loc;
  eff_kind kind1;
  effect eff1;
  eff_kind kind2;
  effect eff2;
  bool failed;
  bool inference;
} *forall_cond_nin_constraint;
effect effect_empty = ((void *)0);
dd_list constraints = ((void *)0);
dd_list forall_constraints = ((void *)0);
dd_list forall_cond_constraints = ((void *)0);
region effect_region = ((void *)0);
int next_effect = 0;
unsigned int effect_cache_time = 0;
static enum { state_init, state_reach, state_lin } hax_state, state = state_init;
dd_list aloc_list;
hash_table global_ht;
dd_list cinf_constraints;
void init_effects(void)
{
  ((void) ((effect_region == ((void *)0)) ? 0 : (__assert_fail ("effect_region == ((void *)0)", "effect.c", 102, __PRETTY_FUNCTION__), 0)));
  effect_region = newregion();
  next_effect = 0;
  constraints = dd_new_list(effect_region);
  cinf_constraints = dd_new_list(effect_region);
  forall_constraints = dd_new_list(effect_region);
  forall_cond_constraints = dd_new_list(effect_region);
  aloc_list = dd_new_list(effect_region);
  state = state_init;
  effect_cache_time = 0;
}
struct Abstract_loc
{
  enum { aloc_var, aloc_link } kind;
  union
  {
    struct {
      const char *name;
      int num_equiv;
      effect *effects;
      qtype points_to;
      qtype fs_points_to;
      bool interesting:1;
      bool reach_computed:1;
    } var;
    aloc link;
  } u;
};
unsigned long hash_aloc(aloc al);
typedef struct { hash_table ht; } *aloc_int_map; typedef struct { hash_table_scanner hts; } aloc_int_map_scanner; typedef struct { hash_table_scanner_sorted htss; } aloc_int_map_scanner_sorted; static inline aloc_int_map make_aloc_int_map(region r, unsigned long size) { return (aloc_int_map) make_hash_table(r, size, (hash_fn) hash_aloc, (keyeq_fn) eq_aloc); } static inline void aloc_int_map_reset(aloc_int_map m) { hash_table_reset((hash_table) m); } static inline unsigned long aloc_int_map_size(aloc_int_map m) { return hash_table_size((hash_table) m); } static inline bool aloc_int_map_lookup(aloc_int_map m, aloc k, int *d) { return hash_table_lookup((hash_table) m, (hash_key) k, (hash_data *) d); } static inline bool aloc_int_map_insert(aloc_int_map m, aloc k, int d) { return hash_table_insert((hash_table) m, (hash_key) k, (hash_data) (intptr_t) d); } static inline bool aloc_int_map_remove(aloc_int_map m, aloc k) { return hash_table_remove((hash_table) m, (hash_key) k); } static inline aloc_int_map aloc_int_map_copy(region r, aloc_int_map m) { return (aloc_int_map) hash_table_copy(r, (hash_table) m); } static inline aloc_int_map aloc_int_map_map(region r, aloc_int_map m, int (*f)(aloc, int, void *), void *arg) { return (aloc_int_map) hash_table_map(r, (hash_table) m, (hash_map_fn) f, arg); } static inline void aloc_int_map_scan(aloc_int_map m, aloc_int_map_scanner *ms) { hash_table_scan((hash_table) m, &ms->hts); } static inline bool aloc_int_map_next(aloc_int_map_scanner *ms, aloc *k, int *d) { return hash_table_next(&ms->hts, (hash_key *) k, (hash_data *) d); } static inline void aloc_int_map_scan_sorted(aloc_int_map m, int (*f)(aloc, aloc), aloc_int_map_scanner_sorted *mss) { hash_table_scan_sorted((hash_table) m, (keycmp_fn) f, &mss->htss); } static inline bool aloc_int_map_next_sorted(aloc_int_map_scanner_sorted *ms, aloc *k, int *d) { return hash_table_next_sorted(&ms->htss, (hash_key *) k, (hash_data *) d); };
aloc make_aloc(const char *name, qtype points_to, bool global)
{
  aloc al;
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  al = (sizeof(struct Abstract_loc) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Abstract_loc)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Abstract_loc), 0));
  al->kind = aloc_var;
  al->u.var.name = name;
  al->u.var.num_equiv = 1;
  al->u.var.effects = ((void *)0);
  al->u.var.points_to = points_to;
  al->u.var.fs_points_to = ((void *)0);
  al->u.var.interesting = 0;
  al->u.var.reach_computed = 0;
  dd_add_last(effect_region, aloc_list, al);
  return al;
}
static aloc ecr_aloc(aloc al)
{
  if (al->kind == aloc_link)
    {
      aloc ecr = al, cur, temp;
      while (ecr->kind == aloc_link)
        ecr = ecr->u.link;
      cur = al;
      while (cur->u.link != ecr)
        {
          temp = cur->u.link;
          cur->u.link = ecr;
          cur = temp;
        }
      return ecr;
    }
  else
    return al;
}
void set_points_to_aloc(aloc al, qtype points_to) {
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 211, __PRETTY_FUNCTION__), 0)));
  al->u.var.points_to = points_to;
}
const char *name_aloc(aloc al)
{
  if (!al)
    return "";
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 222, __PRETTY_FUNCTION__), 0)));
  return al->u.var.name;
}
bool global_aloc(aloc al)
{
  return 1;
}
unsigned long hash_aloc(aloc al)
{
  al = ecr_aloc(al);
  return ptr_hash(al);
}
qtype points_to_aloc(aloc al)
{
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 242, __PRETTY_FUNCTION__), 0)));
  return al->u.var.points_to;
}
void unify_aloc(location loc, aloc al1, aloc al2)
{
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  al1 = ecr_aloc(al1);
  al2 = ecr_aloc(al2);
  ((void) ((al1->kind == aloc_var && al2->kind == aloc_var) ? 0 : (__assert_fail ("al1->kind == aloc_var && al2->kind == aloc_var", "effect.c", 262, __PRETTY_FUNCTION__), 0)));
  if (al1 != al2)
    {
      aloc new_ecr, new_link;
      if (qtype_var(al1->u.var.points_to))
        {
          new_ecr = al2;
          new_link = al1;
        }
      else if (qtype_var(al2->u.var.points_to))
        {
          new_ecr = al1;
          new_link = al2;
        }
      else if (al1->u.var.num_equiv <= al2->u.var.num_equiv)
        {
          new_ecr = al2;
          new_link = al1;
        }
      else
        {
          new_ecr = al1;
          new_link = al2;
        }
      if (new_ecr->u.var.effects && new_link->u.var.effects)
        unify_constrs(new_ecr->u.var.effects, new_link->u.var.effects);
      else if (new_link->u.var.effects)
        new_ecr->u.var.effects = new_link->u.var.effects;
      if (flag_flow_sensitive &&
          !match_qtype(new_ecr->u.var.points_to, new_link->u.var.points_to,
                       ((void *)0), ((void *)0)))
        {
          if (!qtype_var(new_ecr->u.var.points_to) &&
              !qtype_var(new_link->u.var.points_to)) {
            report_qerror(loc, sev_err,
                          "inconsistent location type for flow-sensitive pass");
            new_ecr->u.var.points_to = error_qtype;
          }
        }
      new_ecr->u.var.num_equiv += new_link->u.var.num_equiv;
      new_ecr->u.var.interesting =
        new_ecr->u.var.interesting || new_link->u.var.interesting;
      new_link->kind = aloc_link;
      new_link->u.link = new_ecr;
      new_ecr->u.var.reach_computed = 0;
   }
}
void inst_aloc(location loc, aloc al1, aloc al2, polarity p)
{
  unify_aloc(loc,al1,al2);
}
int cmp_aloc(aloc left, aloc right)
{
  left = ecr_aloc(left);
  right = ecr_aloc(right);
  if ((intptr_t) left > (intptr_t) right)
    return 1;
  else if ((intptr_t) left < (intptr_t) right)
    return -1;
  return 0;
}
bool eq_aloc(aloc al1, aloc al2)
{
  return (ecr_aloc(al1) == ecr_aloc(al2));
}
int print_aloc(printf_func pf, aloc al)
{
  int result;
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 358, __PRETTY_FUNCTION__), 0)));
  result = 0;
  result += pf("%s", al->u.var.name);
  return result;
}
static int print_unique_aloc(printf_func pf, aloc al)
{
  int result;
  al = ecr_aloc(al);
  result = print_aloc(pf, al);
  if (flag_ugly)
    result += pf(" %s", ptr_to_ascii(al));
  return result;
}
typedef dd_list effect_set; typedef dd_list_pos effect_set_scanner; static inline effect_set empty_effect_set(region r) { return dd_new_list(r); } static inline effect_set effect_set_copy(region r, effect_set s) { if (s == ((void *)0)) return ((void *)0); return dd_copy(r, s); } static inline bool effect_set_empty(effect_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool effect_set_member(int (*cmp)(effect, effect), effect_set s, effect elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) cmp, (void *) elt) != ((void *)0); } static inline int effect_set_size(effect_set s) { if (s == ((void *)0)) return 0; return dd_length(s); } static inline bool effect_set_insert(region r, effect_set *s, effect elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool effect_set_insert_last(region r, effect_set *s, effect elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline effect_set effect_set_union(effect_set s1, effect_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool effect_set_single(effect_set s) { return effect_set_size(s) == 1; } static inline void effect_set_sort(int (*cmp)(effect, effect), effect_set s) { if (s == ((void *)0)) return; dd_sort(s, (set_cmp_fn) cmp); } static inline void effect_set_remove_dups(int (*cmp)(effect, effect), effect_set s) { if (s == ((void *)0)) return; dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void effect_set_scan(effect_set s, effect_set_scanner *ss) { if (s == ((void *)0)) *ss = ((void *)0); else *ss = dd_first(s); } static inline effect effect_set_next(effect_set_scanner *ss) { effect result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((effect)((*ss)->data)); *ss = ((*ss)->next); return result; };
struct Effect
{
  enum { eff_constr, eff_union, eff_inter, eff_var, eff_link } kind;
  int visited;
  union
  {
    struct {
      eff_kind kind;
      aloc base;
      effect_set ub;
    } constr;
    struct {
      effect e1;
      effect e2;
    } u;
    struct {
      effect e1;
      effect e2;
      effect_set ub;
      int visited1, visited2;
    } inter;
    struct {
      const char *name;
      effect_set lb, ub;
      int num_equiv;
      bool interesting;
      int cache_time_stamp;
      aloc_int_map reach_cache;
      bool visited;
    } var;
    effect link;
  } u;
};
static effect ecr_effect(effect e)
{
  if (e == ((void *)0))
    return e;
  if (e->kind == eff_link)
    {
      effect ecr = e, cur, temp;
      while (ecr && ecr->kind == eff_link)
        ecr = ecr->u.link;
      cur = e;
      while (cur->u.link != ecr)
        {
          temp = cur->u.link;
          cur->u.link = ecr;
          cur = temp;
        }
      return ecr;
    }
  else
    return e;
}
void mark_aloc_interesting(aloc al)
{
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 473, __PRETTY_FUNCTION__), 0)));
  al->u.var.interesting = 1;
}
static bool aloc_interesting(aloc al)
{
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 480, __PRETTY_FUNCTION__), 0)));
  return al->u.var.interesting;
}
aloc aloc_effect(effect e)
{
  e = ecr_effect(e);
  ((void) ((e && e->kind == eff_constr) ? 0 : (__assert_fail ("e && e->kind == eff_constr", "effect.c", 487, __PRETTY_FUNCTION__), 0)));
  return e->u.constr.base;
}
effect effect_constr(aloc al, eff_kind k)
{
  int i;
  effect *effects;
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 497, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 502, __PRETTY_FUNCTION__), 0)));
  ((void) ((0 <= k && k <= eff_last) ? 0 : (__assert_fail ("0 <= k && k <= eff_last", "effect.c", 503, __PRETTY_FUNCTION__), 0)));
  if (!al->u.var.effects)
    {
      al->u.var.effects = (((void)0), __rc_typed_rarrayalloc)((effect_region), (eff_last + 1), sizeof(effect), 0);
      for (i = 0; i <= eff_last; i++)
        al->u.var.effects[i] = ((void *)0);
    }
  effects = al->u.var.effects;
  if (!effects[k])
    {
      effect e;
      e = (sizeof(struct Effect) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Effect)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Effect), 0));
      e->kind = eff_constr;
      e->visited = 0;
      e->u.constr.kind = k;
      e->u.constr.base = al;
      e->u.constr.ub = empty_effect_set(effect_region);
      effects[k] = e;
    }
  return effects[k];
}
effect effect_single(aloc al)
{
  return effect_constr(al, eff_any);
}
effect effect_alloc(aloc al)
{
  return effect_constr(al, eff_alloc);
}
effect effect_r(aloc al)
{
  return effect_constr(al, eff_r);
}
effect effect_wr(aloc al)
{
  return effect_constr(al, eff_wr);
}
effect effect_rwr(aloc al)
{
  return effect_constr(al, eff_rwr);
}
effect effect_var(const char *name)
{
  effect e;
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 568, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  e = (sizeof(struct Effect) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Effect)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Effect), 0));
  e->kind = eff_var;
  e->u.var.name = name;
  e->u.var.lb = empty_effect_set(effect_region);
  e->u.var.ub = empty_effect_set(effect_region);
  e->u.var.interesting = 0;
  e->u.var.reach_cache = ((void *)0);
  e->u.var.cache_time_stamp = 0;
  e->u.var.visited = 0;
  e->visited = 0;
  return e;
}
void mark_effect_interesting(effect e)
{
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 593, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  e = ecr_effect(e);
  switch (e->kind)
    {
    case eff_constr:
      break;
    case eff_var:
      e->u.var.interesting = 1;
      break;
    case eff_union:
      mark_effect_interesting(e->u.u.e1);
      mark_effect_interesting(e->u.u.e2);
      break;
    case eff_inter:
      mark_effect_interesting(e->u.inter.e1);
      mark_effect_interesting(e->u.inter.e2);
      break;
    default:
      __fail("effect.c", 614, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
effect effect_fresh(void)
{
  const char *name;
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  name = rstrcat(effect_region, "ef",
                 inttostr(effect_region, next_effect++));
  return effect_var(name);
}
effect effect_union(effect e1, effect e2)
{
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  if (e1 == e2)
    return e1;
  if (e1 == ((void *)0))
    return e2;
  else if (e2 == ((void *)0))
    return e1;
  else
    {
      effect e;
      e = (sizeof(struct Effect) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Effect)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Effect), 0));
      e->kind = eff_union;
      e->u.u.e1 = e1;
      e->u.u.e2 = e2;
      e->visited = 0;
      return e;
    }
}
static void add_to_effect_ub(effect e, effect to_add)
{
  e = ecr_effect(e);
  switch (e->kind)
    {
    case eff_constr:
      effect_set_insert(effect_region, &e->u.constr.ub, to_add);
      break;
    case eff_inter:
      effect_set_insert(effect_region, &e->u.inter.ub, to_add);
      break;
    case eff_var:
      effect_set_insert(effect_region, &e->u.var.ub, to_add);
      break;
    default:
      __fail("effect.c", 679, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
effect effect_inter(effect e1, effect e2)
{
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 686, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return ((void *)0);
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  ((void) ((e2 != ((void *)0)) ? 0 : (__assert_fail ("e2 != ((void *)0)", "effect.c", 693, __PRETTY_FUNCTION__), 0)));
  if (e1 == ((void *)0))
    return effect_empty;
  else
    {
      effect e;
      ((void) ((e2 != ((void *)0)) ? 0 : (__assert_fail ("e2 != ((void *)0)", "effect.c", 701, __PRETTY_FUNCTION__), 0)));
      if (e1->kind == eff_union)
        {
          effect temp;
          temp = effect_fresh();
          mkleq_effect(e1, temp);
          e1 = temp;
        }
      if (e2->kind == eff_union)
        {
          effect temp;
          temp = effect_fresh();
          mkleq_effect(e2, temp);
          e2 = temp;
        }
      e = (sizeof(struct Effect) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Effect)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Effect), 0));
      e->kind = eff_inter;
      e->u.inter.e1 = e1;
      e->u.inter.e2 = e2;
      e->u.inter.ub = empty_effect_set(effect_region);
      e->u.inter.visited1 = 0;
      e->u.inter.visited2 = 0;
      e->visited = 0;
      add_to_effect_ub(e1, e);
      add_to_effect_ub(e2, e);
      return e;
    }
}
bool eq_effect(effect e1, effect e2)
{
  return (ecr_effect(e1) == ecr_effect(e2));
}
int cmp_effect(effect e1, effect e2)
{
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  if ((intptr_t) e1 > (intptr_t) e2)
    return 1;
  else if ((intptr_t) e1 < (intptr_t) e2)
    return -1;
  return 0;
}
void internal_mkleq_effect(effect e1, effect e2)
{
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  ((void) ((e2->kind == eff_var) ? 0 : (__assert_fail ("e2->kind == eff_var", "effect.c", 766, __PRETTY_FUNCTION__), 0)));
  if (e1 == ((void *)0))
    return;
  else
    switch (e1->kind)
      {
      case eff_union:
        internal_mkleq_effect(e1->u.u.e1, e2);
        internal_mkleq_effect(e1->u.u.e2, e2);
        break;
      case eff_constr:
        effect_set_insert(effect_region, &e1->u.constr.ub, e2);
        effect_set_insert(effect_region, &e2->u.var.lb, e1);
        break;
      case eff_var:
        effect_set_insert(effect_region, &e1->u.var.ub, e2);
        effect_set_insert(effect_region, &e2->u.var.lb, e1);
        break;
      case eff_inter:
        effect_set_insert(effect_region, &e1->u.inter.ub, e2);
        effect_set_insert(effect_region, &e2->u.var.lb, e1);
        break;
      default:
        __fail("effect.c", 790, __FUNCTION__, "Unexpected effect kind %x\n", e1->kind);
      }
}
void mkleq_effect(effect e1, effect e2)
{
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 797, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  internal_mkleq_effect(e1, e2);
}
void mkinst_effect(location l,effect e1, effect e2, polarity p)
{
  mkleq_effect(e1,e2);
}
void mkeq_effect(effect e1, effect e2)
{
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 821, __PRETTY_FUNCTION__), 0)));
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  internal_mkleq_effect(e1, e2);
  internal_mkleq_effect(e2, e1);
}
void unify_effect(effect e1, effect e2)
{
  effect new_ecr, new_link;
  if (!restrict_qual && !flag_flow_sensitive)
    return;
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  if (e1 == e2)
    return;
  ((void) ((e1->kind == eff_var && e2->kind == eff_var) ? 0 : (__assert_fail ("e1->kind == eff_var && e2->kind == eff_var", "effect.c", 857, __PRETTY_FUNCTION__), 0)));
  ((void) ((state == state_init) ? 0 : (__assert_fail ("state == state_init", "effect.c", 858, __PRETTY_FUNCTION__), 0)));
  ((void) ((!e1->u.var.reach_cache && !e2->u.var.reach_cache) ? 0 : (__assert_fail ("!e1->u.var.reach_cache && !e2->u.var.reach_cache", "effect.c", 859, __PRETTY_FUNCTION__), 0)));
  mkeq_effect(e1, e2);
  if (e1->u.var.num_equiv <= e2->u.var.num_equiv)
    {
      new_ecr = e2;
      new_link = e1;
    }
  else
    {
      new_ecr = e1;
      new_link = e2;
    }
  new_ecr->u.var.num_equiv += new_link->u.var.num_equiv;
  new_ecr->u.var.lb = effect_set_union(new_ecr->u.var.lb,
                                       new_link->u.var.lb);
  new_ecr->u.var.ub = effect_set_union(new_ecr->u.var.ub,
                                       new_link->u.var.ub);
  new_ecr->u.var.interesting =
    new_ecr->u.var.interesting || new_link->u.var.interesting;
  new_link->kind = eff_link;
  new_link->u.link = new_ecr;
}
static void unify_constrs(effect *c1, effect *c2)
{
  int i;
  for (i = 0; i <= eff_last; i++)
    {
      effect e1, e2;
      e1 = ecr_effect(c1[i]);
      e2 = ecr_effect(c2[i]);
      if (!e1)
        c1[i] = e2;
      else if (!e2)
        c2[i] = e1;
      else
        {
          ((void) ((e1->kind == eff_constr && e2->kind == eff_constr && e1->u.constr.kind == e2->u.constr.kind) ? 0 : (__assert_fail ("e1->kind == eff_constr && e2->kind == eff_constr && e1->u.constr.kind == e2->u.constr.kind", "effect.c", 902, __PRETTY_FUNCTION__), 0)));
          if (e1 != e2)
            {
              e1->u.constr.ub = effect_set_union(e1->u.constr.ub,
                                                 e2->u.constr.ub);
              e2->kind = eff_link;
              e2->u.link = e1;
            }
        }
    }
}
static nin_constraint internal_mknin_aloc_effect(location loc, aloc al, eff_kind kind, effect e)
{
  nin_constraint c;
  e = ecr_effect(e);
  if (e == ((void *)0))
    return ((void *)0);
  else if (e->kind != eff_var)
    {
      effect temp;
      temp = effect_fresh();
      mkleq_effect(e, temp);
      e = temp;
    }
  mark_effect_interesting(e);
  c = (sizeof(struct Nin_constraint) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Nin_constraint)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Nin_constraint), 0));
  c->aloc = al;
  c->eff = e;
  c->kind = kind;
  c->loc = loc;
  c->failed = 0;
  c->inference = 0;
  dd_add_last(effect_region, constraints, c);
  return c;
}
void mknin_aloc_effect(location loc, aloc al, eff_kind kind, effect e)
{
  internal_mknin_aloc_effect(loc, al, kind, e);
  return;
}
static forall_nin_constraint internal_forall_aloc_mknin_effect(location loc, eff_kind kind, effect e)
{
  forall_nin_constraint c;
  e = ecr_effect(e);
  if (e == ((void *)0))
    return ((void *)0);
  else if (e->kind != eff_var)
    {
      effect temp;
      temp = effect_fresh();
      mkleq_effect(e, temp);
      e = temp;
    }
  mark_effect_interesting(e);
  c = (sizeof(struct Forall_nin_constraint) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Forall_nin_constraint)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Forall_nin_constraint), 0));
  c->kind = kind;
  c->loc = loc;
  c->eff = e;
  c->failed = 0;
  c->inference = 0;
  dd_add_last(effect_region, forall_constraints, c);
  return c;
}
void forall_aloc_mknin_effect(location loc, eff_kind kind, effect e)
{
  internal_forall_aloc_mknin_effect(loc, kind, e);
  return;
}
static forall_cond_nin_constraint internal_forall_aloc_in_effect_mknin_effect(location loc, eff_kind kind1, effect e1, eff_kind kind2, effect e2)
{
  forall_cond_nin_constraint c;
  e1 = ecr_effect(e1);
  if (e1 == ((void *)0))
    return ((void *)0);
  else if (e1->kind != eff_var)
    {
      effect temp;
      temp = effect_fresh();
      mkleq_effect(e1, temp);
      e1 = temp;
    }
  mark_effect_interesting(e1);
  if (e2 == ((void *)0))
    return ((void *)0);
  else if (e2->kind != eff_var)
    {
      effect temp;
      temp = effect_fresh();
      mkleq_effect(e2, temp);
      e2 = temp;
    }
  mark_effect_interesting(e2);
  c = (sizeof(struct Forall_cond_nin_constraint) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Forall_cond_nin_constraint)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Forall_cond_nin_constraint), 0));
  c->kind1 = kind1;
  c->kind2 = kind2;
  c->loc = loc;
  c->eff1 = e1;
  c->eff2 = e2;
  c->failed = 0;
  c->inference = 0;
  dd_add_last(effect_region, forall_cond_constraints, c);
  return c;
}
void forall_aloc_in_effect_mknin_effect(location loc, eff_kind kind1,
                                        effect e1, eff_kind kind2, effect e2)
{
  internal_forall_aloc_in_effect_mknin_effect(loc, kind1, e1, kind2, e2);
  return;
}
void mknin_effect(location loc, effect e1, effect e2)
{
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  if (e1 == ((void *)0) || e2 == ((void *)0))
    return;
  if (e1->kind == eff_constr)
    mknin_aloc_effect(loc, e1->u.constr.base, e1->u.constr.kind, e2);
  else if (e1->kind == eff_union)
    {
      mknin_effect(loc, e1->u.u.e1, e2);
      mknin_effect(loc, e1->u.u.e2, e2);
    }
  else
    __fail("effect.c", 1064, __FUNCTION__, "Unexpected effect kind %x\n", e1->kind);
}
static bool reachable_forward(effect e1, effect e2) {
  effect_set_scanner ss;
  effect b;
  e1 = ecr_effect(e1);
  e2 = ecr_effect(e2);
  ((void) ((e2->kind == eff_var) ? 0 : (__assert_fail ("e2->kind == eff_var", "effect.c", 1076, __PRETTY_FUNCTION__), 0)));
  if (e1 == e2)
    return 1;
  else
    switch (e1->kind)
      {
      case eff_constr:
        {
          ((void) ((e1->visited == 0) ? 0 : (__assert_fail ("e1->visited == 0", "effect.c", 1089, __PRETTY_FUNCTION__), 0)));
          e1->visited = 1;
          for (effect_set_scan(e1->u.constr.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
            if (reachable_forward(b, e2))
              return 1;
        }
        break;
      case eff_inter:
        {
          if (e1->visited == 0)
            e1->visited = 1;
          else if (e1->visited == 1)
            {
              e1->visited = 2;
              for (effect_set_scan(e1->u.inter.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
                if (reachable_forward(b, e2))
                  return 1;
            }
        }
        break;
      case eff_var:
        {
          if (!e1->visited)
            {
              e1->visited = 1;
              for (effect_set_scan(e1->u.var.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
                if (reachable_forward(b, e2))
                  return 1;
            }
        }
        break;
      default:
        __fail("effect.c", 1121, __FUNCTION__, "Unexpected effect kind %x\n", e1->kind);
      }
  return 0;
}
static void clean_reachable_forward(aloc al, effect e) {
  effect_set_scanner ss;
  effect b;
  e = ecr_effect(e);
  if (e->visited == 0 &&
      (e->kind != eff_inter || (e->u.inter.visited1 == 0 &&
                                e->u.inter.visited2 == 0)))
    return;
  switch (e->kind)
    {
    case eff_constr:
      e->visited = 0;
      for (effect_set_scan(e->u.constr.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
        clean_reachable_forward(al, b);
      break;
    case eff_inter:
      e->visited = 0;
      e->u.inter.visited1 = 0;
      e->u.inter.visited2 = 0;
      for (effect_set_scan(e->u.inter.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
        clean_reachable_forward(al, b);
      break;
    case eff_var:
      if (e->u.var.interesting)
        {
          int mark = 0;
          if (!e->u.var.reach_cache)
            {
              e->u.var.reach_cache = make_aloc_int_map(effect_region, 17);
              e->u.var.cache_time_stamp = effect_cache_time;
            }
          else if (e->u.var.cache_time_stamp < effect_cache_time)
            {
              aloc_int_map_scanner ms;
              aloc old_al;
              int old_mark = 0;
              aloc_int_map old_cache = e->u.var.reach_cache;
              e->u.var.reach_cache = make_aloc_int_map(effect_region, 17);
              e->u.var.cache_time_stamp = effect_cache_time;
              aloc_int_map_scan(old_cache, &ms);
              while (aloc_int_map_next(&ms, &old_al, &old_mark))
                {
                  int older_mark ;
                  older_mark = 0;
                  aloc_int_map_lookup(e->u.var.reach_cache, old_al,
                                      &older_mark);
                  aloc_int_map_insert(e->u.var.reach_cache, old_al,
                                      old_mark | older_mark);
                }
            }
          aloc_int_map_lookup(e->u.var.reach_cache, al, &mark);
          aloc_int_map_insert(e->u.var.reach_cache, al, mark | e->visited);
        }
      e->visited = 0;
      for (effect_set_scan(e->u.var.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
        clean_reachable_forward(al, b);
      break;
    default:
      __fail("effect.c", 1193, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
static void mark_reachable_forward(effect lb, effect ub, int mark)
{
  effect_set_scanner ss;
  effect b;
  ub = ecr_effect(ub);
  switch(ub->kind)
    {
    case eff_constr:
      {
        ((void) ((!ub->visited) ? 0 : (__assert_fail ("!ub->visited", "effect.c", 1214, __PRETTY_FUNCTION__), 0)));
        ub->visited = mark;
        for (effect_set_scan(ub->u.constr.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
          mark_reachable_forward(ub, b, mark);
      }
      break;
    case eff_inter:
      {
        int old_mark = ub->visited;
        if (eq_effect(lb, ub->u.inter.e1))
          ub->u.inter.visited1 |= mark;
        if (eq_effect(lb, ub->u.inter.e2))
          ub->u.inter.visited2 |= mark;
        ub->visited = ub->u.inter.visited1 & ub->u.inter.visited2;
        if (ub->visited != old_mark)
          for (effect_set_scan(ub->u.var.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
            mark_reachable_forward(ub, b, ub->visited);
      }
      break;
    case eff_var:
      {
        int old_mark = ub->visited;
        ub->visited |= mark;
        if (ub->visited != old_mark)
          for (effect_set_scan(ub->u.var.ub, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
            mark_reachable_forward(ub, b, mark);
      }
      break;
    default:
      __fail("effect.c", 1245, __FUNCTION__, "Unexpected effect kind %x\n", ub->kind);
    }
}
static void compute_reachable_forward(aloc al)
{
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 1259, __PRETTY_FUNCTION__), 0)));
  ((void) ((!al->u.var.reach_computed) ? 0 : (__assert_fail ("!al->u.var.reach_computed", "effect.c", 1260, __PRETTY_FUNCTION__), 0)));
  if (al->u.var.effects)
    {
      effect any, rwr, r, wr, alloc;
      any = al->u.var.effects[eff_any];
      rwr = al->u.var.effects[eff_rwr];
      r = al->u.var.effects[eff_r];
      wr = al->u.var.effects[eff_wr];
      alloc = al->u.var.effects[eff_alloc];
      if (any)
        mark_reachable_forward(effect_empty, any, (((1 << (eff_r - 1)) | (1 << (eff_wr - 1))) | (1 << (eff_alloc - 1))));
      if (rwr)
        mark_reachable_forward(effect_empty, rwr, ((1 << (eff_r - 1)) | (1 << (eff_wr - 1))));
      if (r)
        mark_reachable_forward(effect_empty, r, (1 << (eff_r - 1)));
      if (wr)
        mark_reachable_forward(effect_empty, wr, (1 << (eff_wr - 1)));
      if (alloc)
        mark_reachable_forward(effect_empty, alloc, (1 << (eff_alloc - 1)));
      if (alloc)
        clean_reachable_forward(al, alloc);
      if (rwr)
        clean_reachable_forward(al, rwr);
      if (r)
        clean_reachable_forward(al, r);
      if (wr)
        clean_reachable_forward(al, wr);
      if (any)
        clean_reachable_forward(al, any);
    }
  al->u.var.reach_computed = 1;
}
static unsigned int get_mask(eff_kind kind)
{
  unsigned int mask;
  switch (kind)
    {
    case eff_any:
      mask = (((1 << (eff_r - 1)) | (1 << (eff_wr - 1))) | (1 << (eff_alloc - 1)));
      break;
    case eff_rwr:
      mask = ((1 << (eff_r - 1)) | (1 << (eff_wr - 1)));
      break;
    case eff_r:
      mask = (1 << (eff_r - 1));
      break;
    case eff_wr:
      mask = (1 << (eff_wr - 1));
      break;
    case eff_alloc:
      mask = (1 << (eff_alloc - 1));
      break;
    default:
      __fail("effect.c", 1317, __FUNCTION__, "Unexpected constraint kind %x\n", kind);
    }
  return mask;
}
static void clean_reachable_backward(effect e)
{
  e = ecr_effect(e);
  switch (e->kind)
    {
    case eff_constr:
      if (!e->visited)
        return;
      e->visited = 0;
      return;
    case eff_union:
      if (!e->visited)
        return;
      e->visited = 0;
      clean_reachable_backward(e->u.u.e1);
      clean_reachable_backward(e->u.u.e1);
      return;
    case eff_inter:
      if (!e->visited)
        return;
      e->visited = 0;
      clean_reachable_backward(e->u.inter.e1);
      clean_reachable_backward(e->u.inter.e2);
      return;
    case eff_var:
      {
        effect b;
        effect_set_scanner ss;
        if (!e->visited && !e->u.var.visited)
          {
            return;
          }
        e->visited = 0;
        e->u.var.visited = 0;
        for (effect_set_scan(e->u.var.lb, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
          clean_reachable_backward(b);
      }
      return;
    default:
      __fail("effect.c", 1369, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
static int compute_reachable_backward(aloc al, effect e)
{
  e = ecr_effect(e);
  if (e->visited) return e->visited;
  switch (e->kind)
    {
    case eff_constr:
      {
        if (eq_aloc(al, e->u.constr.base))
          e->visited = get_mask(e->u.constr.kind);
        return e->visited;
      }
    case eff_union:
      {
        e->visited = compute_reachable_backward(al, e->u.u.e1) |
          compute_reachable_backward(al, e->u.u.e2);
        return e->visited;
      }
    case eff_inter:
      return (((1 << (eff_r - 1)) | (1 << (eff_wr - 1))) | (1 << (eff_alloc - 1)));
    case eff_var:
      {
        effect b;
        effect_set_scanner ss;
        if (e->u.var.visited)
          return e->visited;
        e->u.var.visited = 1;
        for (effect_set_scan(e->u.var.lb, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
          {
            e->visited = e->visited | compute_reachable_backward(al, b);
          }
        return e->visited;
      }
    default:
      __fail("effect.c", 1418, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
static int internal_compute_reach_cache(effect e, aloc_int_map cache)
{
  e = ecr_effect(e);
  if (e->visited) return e->visited;
  e->visited = 1;
  switch (e->kind)
    {
    case eff_constr:
      {
        int mark = 0;
        ((void) ((cache) ? 0 : (__assert_fail ("cache", "effect.c", 1438, __PRETTY_FUNCTION__), 0)));
        aloc_int_map_lookup(cache, e->u.constr.base, &mark);
        aloc_int_map_insert(cache, e->u.constr.base, mark | get_mask(e->u.constr.kind));
        return e->visited;
      }
    case eff_union:
      {
        if (internal_compute_reach_cache(e->u.u.e1, cache) == 1 &&
            internal_compute_reach_cache(e->u.u.e2, cache) == 1)
          {
            return e->visited;
          }
        else
          return e->visited = 2;
      }
    case eff_inter:
      return e->visited = 2;
    case eff_var:
      {
        effect b;
        effect_set_scanner ss;
        for (effect_set_scan(e->u.var.lb, &ss), b = effect_set_next(&ss); b; b = effect_set_next(&ss))
          {
            if (internal_compute_reach_cache(b, cache) != 1)
              return e->visited = 2;
          }
        return e->visited;
      }
    default:
      __fail("effect.c", 1472, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
int compute_reach_cache(effect e, aloc_int_map * cache)
{
  int result;
  *cache = make_aloc_int_map(effect_region, 17);
  result = internal_compute_reach_cache(e, *cache);
  clean_reachable_backward(e);
  return result;
}
static int aloc_in_effect(aloc al, effect e, bool compute_cache)
{
  int mark;
  state = state_reach;
  if (!restrict_qual && !flag_flow_sensitive)
    return 0;
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 1498, __PRETTY_FUNCTION__), 0)));
  e = ecr_effect(e);
  mark = 0;
  switch (e->kind)
    {
    case eff_constr:
      if (! eq_aloc(al,e->u.constr.base))
        mark = 0;
      else
        mark = get_mask(e->u.constr.kind);
      break;
    case eff_union:
      mark = aloc_in_effect(al, e->u.u.e1, compute_cache) |
        aloc_in_effect(al, e->u.u.e2, compute_cache);
      break;
    case eff_inter:
      mark = (aloc_in_effect(al, e->u.inter.e1, compute_cache) &
              aloc_in_effect(al, e->u.inter.e2, compute_cache));
      break;
    case eff_var:
      ((void) ((e->u.var.interesting) ? 0 : (__assert_fail ("e->u.var.interesting", "effect.c", 1518, __PRETTY_FUNCTION__), 0)));
      if (!al->u.var.reach_computed)
        compute_reachable_forward(al);
      if (e->u.var.reach_cache)
        aloc_int_map_lookup(e->u.var.reach_cache, al, &mark);
      break;
    default:
      __fail("effect.c", 1564, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
  return mark;
}
bool no_eff_in_cache(eff_kind kind, aloc_int_map cache)
{
  aloc_int_map_scanner ms;
  aloc al;
  int mark, oldmark;
  mark = 0;
  oldmark = get_mask(kind);
  aloc_int_map_scan(cache, &ms);
  while (aloc_int_map_next(&ms, &al, &mark))
    {
      if (oldmark & mark)
        return 0;
    }
  return 1;
}
bool no_eff_from_cache_in_eff(eff_kind kind1, aloc_int_map cache,
                                     eff_kind kind2, effect eff)
{
  aloc_int_map_scanner ms;
  aloc al;
  int mark, mark1, mark2;
  mark = 0;
  mark1 = get_mask(kind1);
  mark2 = get_mask(kind2);
  aloc_int_map_scan(cache, &ms);
  while (aloc_int_map_next(&ms, &al, &mark))
    {
      if ((mark & mark1) &&
          (aloc_in_effect(al, eff, 0) & mark2))
        return 0;
    }
  return 1;
}
void check_nin(void)
{
  dd_list_pos cur;
  effect_cache_time++;
  {
    dd_list_pos curconst;
    for (curconst = dd_first((forall_constraints)); !(!(curconst)->next); curconst = ((curconst)->next))
      {
        aloc_int_map temp_cache = ((void *)0);
        forall_nin_constraint c = ((forall_nin_constraint)((curconst)->data));
        ((void) ((c->eff->kind == eff_var) ? 0 : (__assert_fail ("c->eff->kind == eff_var", "effect.c", 1624, __PRETTY_FUNCTION__), 0)));
        if (!c->failed &&
            ((compute_reach_cache(c->eff, &temp_cache) == 2) ||
             !no_eff_in_cache(c->kind, temp_cache)))
          {
            c->failed = 1;
            if (!c->inference)
              report_qerror(c->loc, sev_err,
                            "Invalid use of restrict - side effect");
          }
      }
    for (curconst = dd_first((forall_cond_constraints)); !(!(curconst)->next); curconst = ((curconst)->next))
      {
        aloc_int_map temp_cache = ((void *)0);
        forall_cond_nin_constraint c = ((forall_cond_nin_constraint)((curconst)->data));
        if (!c->failed &&
            ((compute_reach_cache(c->eff1, &temp_cache) == 2) ||
             (!no_eff_from_cache_in_eff(c->kind1, temp_cache,
                                        c->kind2, c->eff2))))
          {
            c->failed = 1;
            if (!c->inference)
              report_qerror(c->loc, sev_err,
                            "Invalid use of restrict - readonly written");
          }
      }
  }
  for (cur = dd_first((constraints)); !(!(cur)->next); cur = ((cur)->next))
    {
      nin_constraint c = ((nin_constraint)((cur)->data));
      if (!c->failed &&
          (aloc_in_effect(c->aloc, c->eff, 0) & get_mask(c->kind)))
        {
          c->failed = 1;
          if (!c->inference)
            report_qerror(c->loc, sev_err,
                          "Invalid use of restrict");
        }
    }
}
int print_effect(printf_func pf, effect e)
{
  int result;
  result = 0;
  e = ecr_effect(e);
  if (e == ((void *)0))
    return pf("0");
  switch(e->kind)
    {
    case eff_constr:
      switch (e->u.constr.kind)
        {
        case eff_any:
          break;
        case eff_alloc:
          result += pf("alloc(");
          break;
        case eff_rwr:
          result += pf("rwr(");
          break;
        case eff_r:
          result += pf("r(");
          break;
        case eff_wr:
          result += pf("wr(");
          break;
        default:
          __fail("effect.c", 1756, __FUNCTION__, "Unexpected constructor kind %x\n", e->u.constr.kind);
        }
      result += print_unique_aloc(pf, e->u.constr.base);
      switch (e->u.constr.kind)
        {
        case eff_alloc:
        case eff_rwr:
          result += pf(")");
          break;
        case eff_r:
          result += pf(")");
          break;
        case eff_wr:
          result += pf(")");
          break;
        case eff_any:
          break;
        default:
          __fail("effect.c", 1774, __FUNCTION__, "Unexpected constructor kind %x\n", e->u.constr.kind);
        }
      break;
    case eff_union:
      result += print_effect(pf, e->u.u.e1);
      result += pf(" + ");
      result += print_effect(pf, e->u.u.e2);
      break;
    case eff_inter:
      result += pf("[(");
      result += print_effect(pf, e->u.inter.e1);
      result += pf(") & (");
      result += print_effect(pf, e->u.inter.e2);
      result += pf(")]");
      break;
    case eff_var:
      if (flag_ugly)
        result += pf("%s %p", e->u.var.name, e);
      else
        result += pf("%s", e->u.var.name);
      break;
    default:
      __fail("effect.c", 1796, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
  return result;
}
struct Store_edge {
  location loc;
  const char *err_msg;
  store store;
  polarity p;
};
typedef struct Store_edge *store_edge;
typedef dd_list store_edge_set; typedef dd_list_pos store_edge_set_scanner; static inline store_edge_set empty_store_edge_set(region r) { return dd_new_list(r); } static inline store_edge_set store_edge_set_copy(region r, store_edge_set s) { if (s == ((void *)0)) return ((void *)0); return dd_copy(r, s); } static inline bool store_edge_set_empty(store_edge_set s) { return s == ((void *)0) || ((!(dd_first((s)))->next)); } static inline bool store_edge_set_member(int (*cmp)(store_edge, store_edge), store_edge_set s, store_edge elt) { return s != ((void *)0) && dd_search(s, (dd_cmp_fn) cmp, (void *) elt) != ((void *)0); } static inline int store_edge_set_size(store_edge_set s) { if (s == ((void *)0)) return 0; return dd_length(s); } static inline bool store_edge_set_insert(region r, store_edge_set *s, store_edge elt) { *s = dd_fix_null(r, *s); dd_add_first(r, *s, (void *) elt); return 1; } static inline bool store_edge_set_insert_last(region r, store_edge_set *s, store_edge elt) { *s = dd_fix_null(r, *s); dd_add_last(r, *s, (void *) elt); return 1; } static inline store_edge_set store_edge_set_union(store_edge_set s1, store_edge_set s2) { if (s1 == ((void *)0)) return s2; else if (s2 == ((void *)0)) return s1; dd_append(s1, s2); return s1; } static inline bool store_edge_set_single(store_edge_set s) { return store_edge_set_size(s) == 1; } static inline void store_edge_set_sort(int (*cmp)(store_edge, store_edge), store_edge_set s) { if (s == ((void *)0)) return; dd_sort(s, (set_cmp_fn) cmp); } static inline void store_edge_set_remove_dups(int (*cmp)(store_edge, store_edge), store_edge_set s) { if (s == ((void *)0)) return; dd_remove_dups(s, (dd_cmp_fn)cmp); } static inline void store_edge_set_scan(store_edge_set s, store_edge_set_scanner *ss) { if (s == ((void *)0)) *ss = ((void *)0); else *ss = dd_first(s); } static inline store_edge store_edge_set_next(store_edge_set_scanner *ss) { store_edge result; if (*ss == ((void *)0) || (!(*ss)->next)) return ((void *)0); result = ((store_edge)((*ss)->data)); *ss = ((*ss)->next); return result; };
struct Cell {
  qtype qtype;
  bool propagated_forward;
  bool propagated_backward;
};
typedef struct Cell *cell;
typedef struct { hash_table ht; } *cell_map; typedef struct { hash_table_scanner hts; } cell_map_scanner; typedef struct { hash_table_scanner_sorted htss; } cell_map_scanner_sorted; static inline cell_map make_cell_map(region r, unsigned long size) { return (cell_map) make_hash_table(r, size, (hash_fn) hash_aloc, (keyeq_fn) eq_aloc); } static inline void cell_map_reset(cell_map m) { hash_table_reset((hash_table) m); } static inline unsigned long cell_map_size(cell_map m) { return hash_table_size((hash_table) m); } static inline bool cell_map_lookup(cell_map m, aloc k, cell *d) { return hash_table_lookup((hash_table) m, (hash_key) k, (hash_data *) d); } static inline bool cell_map_insert(cell_map m, aloc k, cell d) { return hash_table_insert((hash_table) m, (hash_key) k, (hash_data) (intptr_t) d); } static inline bool cell_map_remove(cell_map m, aloc k) { return hash_table_remove((hash_table) m, (hash_key) k); } static inline cell_map cell_map_copy(region r, cell_map m) { return (cell_map) hash_table_copy(r, (hash_table) m); } static inline cell_map cell_map_map(region r, cell_map m, cell (*f)(aloc, cell, void *), void *arg) { return (cell_map) hash_table_map(r, (hash_table) m, (hash_map_fn) f, arg); } static inline void cell_map_scan(cell_map m, cell_map_scanner *ms) { hash_table_scan((hash_table) m, &ms->hts); } static inline bool cell_map_next(cell_map_scanner *ms, aloc *k, cell *d) { return hash_table_next(&ms->hts, (hash_key *) k, (hash_data *) d); } static inline void cell_map_scan_sorted(cell_map m, int (*f)(aloc, aloc), cell_map_scanner_sorted *mss) { hash_table_scan_sorted((hash_table) m, (keycmp_fn) f, &mss->htss); } static inline bool cell_map_next_sorted(cell_map_scanner_sorted *ms, aloc *k, cell *d) { return hash_table_next_sorted(&ms->htss, (hash_key *) k, (hash_data *) d); };
struct Store {
  enum { store_var, store_filter, store_ref, store_ow, store_assign,
         store_link } kind;
  location loc;
  int lin;
  int lowlink;
  int visited;
  store_edge_set ub;
  union {
    struct {
      const char *name;
      int num_equiv;
      store_edge_set lb;
      cell_map cells;
    } var;
    struct {
      store s;
      effect e;
    } filter;
    struct {
      store s;
      aloc al;
    } ref;
    struct {
      store s1;
      store s2;
      effect e;
    } ow;
    struct {
      store s;
      aloc al;
      bool omega_warned:1;
      int lin;
      bool strong:1;
      cell cell;
      const char *err_msg;
    } assign;
    store link;
  } u;
};
static dd_list assign_stores = ((void *)0);
static region store_region = ((void *)0);
static int store_count = 0;
static bool propagate_cell_backward(location loc, store lb, aloc al, qtype qt,
                                    polarity p);
static bool propagate_cell_forward(location loc, store lb, store ub, aloc al, qtype qt,
                                   polarity p);
static cell_map store_to_cells(region r, store s);
static cell cell_from_store(store s, aloc al, store *store_with_cell, bool ignore_filter);
static int print_cells(printf_func pf, pr_qual_fn pr_qual, cell_map cells,
                       bool print_alocs);
void init_store(void)
{
  ((void) ((store_region == ((void *)0)) ? 0 : (__assert_fail ("store_region == ((void *)0)", "effect.c", 1893, __PRETTY_FUNCTION__), 0)));
  store_region = newregion();
  assign_stores = dd_new_list(store_region);
}
static inline store_edge mkstore_edge(region r, location loc,
                                      const char *err_msg, store s, polarity p)
{
  store_edge result = (sizeof(struct Store_edge) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((r), sizeof(struct Store_edge)) : (((void)0), __rc_typed_ralloc)((r), sizeof(struct Store_edge), 0));
  result->loc = loc;
  result->err_msg = err_msg;
  result->store = s;
  result->p = p;
  return result;
}
static inline cell mkcell(region r, qtype qt, bool propagated_forward,
                          bool propagated_backward)
{
  cell result = (sizeof(struct Cell) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((r), sizeof(struct Cell)) : (((void)0), __rc_typed_ralloc)((r), sizeof(struct Cell), 0));
  result->qtype = qt;
  result->propagated_forward = propagated_forward;
  result->propagated_backward = propagated_backward;
  return result;
}
static store ecr_store(store s)
{
  if (!s) return s;
  if (s->kind == store_link)
    {
      store ecr = s, cur, temp;
      while (ecr->kind == store_link)
        ecr = ecr->u.link;
      cur = s;
      while (cur->u.link != ecr)
        {
          temp = cur->u.link;
          cur->u.link = ecr;
          cur = temp;
        }
      return ecr;
    }
  else
    return s;
}
int cmp_store(store s1, store s2)
{
  s1 = ecr_store(s1);
  s2 = ecr_store(s2);
  return s1 - s2;
}
bool eq_store(store s1, store s2)
{
  return ecr_store(s1) == ecr_store(s2);
}
unsigned long hash_store(store s)
{
  s = ecr_store(s);
  return ptr_hash(s);
}
store make_store_var(location loc, const char *name)
{
  store result;
  result = (sizeof(struct Store) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((store_region), sizeof(struct Store)) : (((void)0), __rc_typed_ralloc)((store_region), sizeof(struct Store), 0));
  result->kind = store_var;
  result->loc = loc;
  result->u.var.name = name;
  result->u.var.num_equiv = 1;
  result->u.var.lb = empty_store_edge_set(store_region);
  result->u.var.cells = make_cell_map(store_region, 7);
  result->ub = empty_store_edge_set(store_region);
  result->lin = -1;
  result->lowlink = 0;
  result->visited = 0;
  store_count++;
  return result;
}
const char *name_store(store s) {
  s = ecr_store(s);
  ((void) ((s->kind == store_var) ? 0 : (__assert_fail ("s->kind == store_var", "effect.c", 1994, __PRETTY_FUNCTION__), 0)));
  return s->u.var.name;
}
store make_store_filter(location loc, store s, effect e)
{
  store result;
  s = ecr_store(s);
  result = (sizeof(struct Store) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((store_region), sizeof(struct Store)) : (((void)0), __rc_typed_ralloc)((store_region), sizeof(struct Store), 0));
  result->kind = store_filter;
  result->loc = loc;
  result->u.filter.s = s;
  result->u.filter.e = e;
  result->lin = -1;
  result->lowlink = 0;
  result->visited = 0;
  result->ub = empty_store_edge_set(store_region);
  store_edge_set_insert(store_region, &s->ub,
                        mkstore_edge(store_region, loc, ((void *)0), result, p_sub));
  store_count++;
  return result;
}
store make_store_ref(location loc, store s, aloc al)
{
  store result;
  al = ecr_aloc(al);
  s = ecr_store(s);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 2040, __PRETTY_FUNCTION__), 0)));
  result = (sizeof(struct Store) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((store_region), sizeof(struct Store)) : (((void)0), __rc_typed_ralloc)((store_region), sizeof(struct Store), 0));
  result->kind = store_ref;
  result->loc = loc;
  result->u.ref.s = s;
  result->u.ref.al = al;
  result->lin = -1;
  result->lowlink = 0;
  result->visited = 0;
  result->ub = empty_store_edge_set(store_region);
  store_edge_set_insert(store_region, &s->ub,
                        mkstore_edge(store_region, loc, ((void *)0), result, p_sub));
  store_count++;
  return result;
}
store make_store_ref_effect(location loc, store s, effect e)
{
  e = ecr_effect(e);
  switch (e->kind)
    {
    case eff_constr:
      return make_store_ref(loc, s, e->u.constr.base);
    case eff_union:
      return make_store_ref_effect(loc,
                                   make_store_ref_effect(loc, s, e->u.u.e2),
                                   e->u.u.e1);
    default:
      __fail("effect.c", 2074, __FUNCTION__, "Unexpected effect kind %x\n", e->kind);
    }
}
store make_store_ow(location loc, store s1, store s2, effect e)
{
  store result;
  s1 = ecr_store(s1);
  s2 = ecr_store(s2);
  e = ecr_effect(e);
  result = (sizeof(struct Store) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((store_region), sizeof(struct Store)) : (((void)0), __rc_typed_ralloc)((store_region), sizeof(struct Store), 0));
  result->kind = store_ow;
  result->loc = loc;
  result->u.ow.s1 = s1;
  result->u.ow.s2 = s2;
  if (e->kind != eff_var)
    {
      effect temp;
      temp = effect_var("s_ow");
      mkleq_effect(e, temp);
      e = temp;
    }
  result->u.ow.e = e;
  result->lin = -1;
  result->lowlink = 0;
  result->visited = 0;
  result->ub = empty_store_edge_set(store_region);
  store_edge_set_insert(store_region, &s1->ub,
                        mkstore_edge(store_region, loc, ((void *)0), result, p_sub));
  store_edge_set_insert(store_region, &s2->ub,
                        mkstore_edge(store_region, loc, ((void *)0), result, p_sub));
  store_count++;
  return result;
}
store make_store_assign(location loc, const char *err_msg, store s, aloc al,
                        qtype qt, bool strong)
{
  store result;
  al = ecr_aloc(al);
  ((void) ((al->u.var.interesting) ? 0 : (__assert_fail ("al->u.var.interesting", "effect.c", 2125, __PRETTY_FUNCTION__), 0)));
  s = ecr_store(s);
  result = (sizeof(struct Store) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((store_region), sizeof(struct Store)) : (((void)0), __rc_typed_ralloc)((store_region), sizeof(struct Store), 0));
  result->kind = store_assign;
  result->loc = loc;
  result->u.assign.s = s;
  result->u.assign.al = al;
  result->u.assign.omega_warned = 0;
  result->u.assign.lin = -1;
  result->u.assign.strong = strong;
  result->u.assign.cell = mkcell(store_region,
                                 qt,
                                 0, 0);
  result->u.assign.err_msg = err_msg;
  result->lin = -1;
  result->lowlink = 0;
  result->visited = 0;
  result->ub = empty_store_edge_set(store_region);
  store_edge_set_insert(store_region, &s->ub,
                        mkstore_edge(store_region, loc, ((void *)0), result, p_sub));
  store_count++;
  dd_add_last(store_region, assign_stores, result);
  store_aloc_qtype(result->u.assign.cell->qtype, result,
                   result->u.assign.al);
  return result;
}
void _mkleq_store(location loc, const char *err_msg, store left, store right,
                  polarity p)
{
  bool err;
  cell_map_scanner cms;
  cell right_cell, left_cell;
  aloc al;
  region scratch_region;
  ((void) ((state == state_init || state == state_reach) ? 0 : (__assert_fail ("state == state_init || state == state_reach", "effect.c", 2167, __PRETTY_FUNCTION__), 0)));
  right = ecr_store(right);
  left = ecr_store(left);
  ((void) ((right->kind == store_var) ? 0 : (__assert_fail ("right->kind == store_var", "effect.c", 2181, __PRETTY_FUNCTION__), 0)));
  if (right == left)
    return;
  err = 0;
  cell_map_scan(right->u.var.cells, &cms);
  while (cell_map_next(&cms, &al, &right_cell))
    if (right_cell->propagated_backward)
      {
        store left_store;
        left_cell = cell_from_store(left, al, &left_store, 0);
        if (left_cell)
          {
            if (p == p_sub)
              err = mkleq_qtype(loc, &fs_qgate, left_cell->qtype,
                                right_cell->qtype) || err;
            else
              err = mkinst_qtype(loc, &open_qgate, left_cell->qtype,
                                 right_cell->qtype, p) || err;
          }
      }
  scratch_region = newregion();
  cell_map_scan(store_to_cells(scratch_region, left), &cms);
  while (cell_map_next(&cms, &al, &left_cell))
    if (left_cell->propagated_forward)
      {
        if (!cell_map_lookup(right->u.var.cells, al, &right_cell))
          {
            qtype qt;
            qt = qtype_to_fs_qtype(right->loc, al->u.var.points_to);
            ((void) ((!(store_aloc_qtype(qt, right, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt, right, al))", "effect.c", 2227, __PRETTY_FUNCTION__), 0)));
            right_cell = mkcell(store_region, qt, 0, 0);
            ((void) ((cell_map_insert(right->u.var.cells, al, right_cell)) ? 0 : (__assert_fail ("cell_map_insert(right->u.var.cells, al, right_cell)", "effect.c", 2229, __PRETTY_FUNCTION__), 0)));
          }
        if (p == p_sub)
          err = mkleq_qtype(loc, &fs_qgate, left_cell->qtype,
                            right_cell->qtype) || err;
        else
          err = mkinst_qtype(loc, &open_qgate, left_cell->qtype,
                             right_cell->qtype, p) || err;
      }
  deleteregion(scratch_region);
  if (err)
    report_qerror(loc, sev_err, "%s", err_msg);
  store_edge_set_insert(store_region, &right->u.var.lb,
                        mkstore_edge(store_region, loc, err_msg, left, p));
  store_edge_set_insert(store_region, &left->ub,
                        mkstore_edge(store_region, loc, err_msg, right, p));
}
void mkleq_store(location loc, const char *err_msg, store left, store right) {
  _mkleq_store(loc, err_msg, left, right, p_sub);
}
void mkinst_store(location loc, const char *err_msg, store left, store right,
                  polarity p) {
  _mkleq_store(loc, err_msg, left, right, p);
}
void mkeq_store(location loc, const char *err_msg, store s1, store s2)
{
  mkleq_store(loc, err_msg, s1, s2);
  mkleq_store(loc, err_msg, s2, s2);
}
store lub_store(location loc, const char *err_msg, store s1, store s2)
{
  store result;
  if (eq_store(s1, s2))
    return s1;
  result = make_store_var(loc, "lub");
  mkleq_store(loc, err_msg, s1, result);
  mkleq_store(loc, err_msg, s2, result);
  return result;
}
void unify_store(location loc, const char *err_msg, store s1, store s2)
{
  store new_ecr, new_link;
  cell_map_scanner cms;
  aloc al;
  cell link_cell, ecr_cell;
  s1 = ecr_store(s1);
  s2 = ecr_store(s2);
  ((void) ((s1->kind == store_var && s2->kind == store_var) ? 0 : (__assert_fail ("s1->kind == store_var && s2->kind == store_var", "effect.c", 2300, __PRETTY_FUNCTION__), 0)));
  if (s1 == s2)
    return;
  if (s1->u.var.num_equiv <= s2->u.var.num_equiv)
    {
      new_ecr = s2;
      new_link = s1;
    }
  else
    {
      new_ecr = s1;
      new_link = s2;
    }
  mkleq_store(loc, err_msg, new_link, new_ecr);
  mkleq_store(loc, err_msg, new_ecr, new_link);
  cell_map_scan(new_link->u.var.cells, &cms);
  while (cell_map_next(&cms, &al, &link_cell))
    if (!link_cell->propagated_forward && !link_cell->propagated_backward)
      {
        if (cell_map_lookup(new_ecr->u.var.cells, al, &ecr_cell))
          {
            ((void) ((ecr_cell->propagated_forward == link_cell->propagated_forward) ? 0 : (__assert_fail ("ecr_cell->propagated_forward == link_cell->propagated_forward", "effect.c", 2330, __PRETTY_FUNCTION__), 0)));
            ((void) ((ecr_cell->propagated_backward == link_cell->propagated_backward) ? 0 : (__assert_fail ("ecr_cell->propagated_backward == link_cell->propagated_backward", "effect.c", 2331, __PRETTY_FUNCTION__), 0)));
            ((void) ((!(unify_qtype(loc, link_cell->qtype, ecr_cell->qtype))) ? 0 : (__assert_fail ("!(unify_qtype(loc, link_cell->qtype, ecr_cell->qtype))", "effect.c", 2332, __PRETTY_FUNCTION__), 0)));
          }
        else
          ((void) ((cell_map_insert(new_ecr->u.var.cells, al, link_cell)) ? 0 : (__assert_fail ("cell_map_insert(new_ecr->u.var.cells, al, link_cell)", "effect.c", 2335, __PRETTY_FUNCTION__), 0)));
      }
  new_ecr->u.var.lb = store_edge_set_union(new_ecr->u.var.lb,
                                           new_link->u.var.lb);
  new_ecr->u.var.num_equiv += new_link->u.var.num_equiv;
  new_ecr->ub = store_edge_set_union(new_ecr->ub, new_link->ub);
  new_link->kind = store_link;
  new_link->u.link = new_ecr;;
}
static int scc_store_number = 0;
static store *scc_store_stack = ((void *)0);
static int scc_store_size = 0;
static int scc_store_top = 0;
static region scc_store_region = ((void *)0);
static void scc_store_stack_push(store s)
{
  ((void) ((scc_store_size > scc_store_top) ? 0 : (__assert_fail ("scc_store_size > scc_store_top", "effect.c", 2363, __PRETTY_FUNCTION__), 0)));
  scc_store_stack[scc_store_top] = s;
  scc_store_top++;
}
static store scc_store_stack_pop(void)
{
  ((void) ((scc_store_top > 0) ? 0 : (__assert_fail ("scc_store_top > 0", "effect.c", 2370, __PRETTY_FUNCTION__), 0)));
  scc_store_top--;
  return scc_store_stack[scc_store_top];
}
static void scc_store_stack_clear(void)
{
  scc_store_top = 0;
}
static void scc_store_stack_init(int size)
{
  ((void) ((scc_store_region == ((void *)0)) ? 0 : (__assert_fail ("scc_store_region == ((void *)0)", "effect.c", 2382, __PRETTY_FUNCTION__), 0)));
  scc_store_region = newregion();
  scc_store_stack = (((void)0), __rc_typed_rarrayalloc)((scc_store_region), (size), sizeof(store), 0);
  scc_store_size = size;
  scc_store_top = 0;
}
static void scc_store_stack_delete(void)
{
  deleteregion(scc_store_region);
  scc_store_region = ((void *)0);
  scc_store_size = 0;
  scc_store_top = 0;
  scc_store_stack = ((void *)0);
}
static bool in_scc_store_stack(store s)
{
  int i;
  for (i=0; i < scc_store_top; i++)
    {
      if (s == scc_store_stack[i])
        return 1;
    }
  return 0;
}
static int compute_lin_scc_sub(aloc al, store parent, store child);
static int compute_lin_scc(aloc al, store s)
{
  s->lowlink = s->visited = ++scc_store_number;
  scc_store_stack_push(s);
  switch (s->kind)
    {
    case store_var:
      {
        store_edge_set_scanner sess;
        store_edge b;
        int lin, max;
        max = 0;
        for (store_edge_set_scan(s->u.var.lb, &sess), b = store_edge_set_next(&sess); b; b = store_edge_set_next(&sess))
          {
            b->store = ecr_store(b->store);
            lin = compute_lin_scc_sub(al, s, b->store);
            max = ((max) > (lin) ? (max) : (lin));
          }
        s->lin = max;
      }
      break;
    case store_filter:
      {
        effect e;
        e = ecr_effect(s->u.filter.e);
        if (e && aloc_in_effect(al, e, 0))
          s->lin = compute_lin_scc_sub(al, s, ecr_store(s->u.filter.s));
        else
          s->lin = 0;
      }
      break;
    case store_ref:
      {
        s->lin = compute_lin_scc_sub(al, s, ecr_store(s->u.ref.s));
        if (eq_aloc(s->u.ref.al, al))
          s->lin = ((s->lin + 1) < (2) ? (s->lin + 1) : (2));
      }
      break;
    case store_ow:
      {
        effect e;
        e = ecr_effect(s->u.ow.e);
        if (e && (aloc_in_effect(al, e, 0) & (1 << (eff_alloc - 1))))
          s->lin = compute_lin_scc_sub(al, s, ecr_store(s->u.ow.s1));
        else
          s->lin = compute_lin_scc_sub(al, s, ecr_store(s->u.ow.s2));
      }
      break;
    case store_assign:
      {
        s->lin = compute_lin_scc_sub(al, s, ecr_store(s->u.assign.s));
      }
      break;
    default:
      __fail("effect.c", 2482, __FUNCTION__, "Unexpected store kind %d\n", s->kind);
    }
  if (s->visited == s->lowlink)
    {
      bool ref = 0;
      store c;
      int i, max, count = 0;
      max = 0;
      for (i = scc_store_top - 1; i >= 0; i--)
        {
          c = scc_store_stack[i];
          if (c->kind == store_ref && eq_aloc(c->u.ref.al, al))
            ref = 1;
          max = ((max) > (c->lin) ? (max) : (c->lin));
          count++;
          if (c->visited <= s->visited)
            break;
        }
      if (ref && count > 1)
        max = 2;
      do {
        c = scc_store_stack_pop();
        c->lin = max;
      } while (c->visited > s->visited);
    }
  return s->lin;
}
static int compute_lin_scc_sub(aloc al, store parent, store child)
{
  int lin = 0;
  if (!child->visited)
    {
      if (child->lin != -1)
        lin = child->lin;
      else
        {
          lin = compute_lin_scc(al, child);
          parent->lowlink = ((parent->lowlink) < (child->lowlink) ? (parent->lowlink) : (child->lowlink));
        }
    }
  else
    {
      lin = child->lin;
      if (child->visited < parent->visited)
        {
          if (in_scc_store_stack(child))
            parent->lowlink = ((parent->lowlink) < (child->visited) ? (parent->lowlink) : (child->visited));
        }
    }
  return lin;
}
static void clean_store(store s, bool cleanlin)
{
  s = ecr_store(s);
  if (cleanlin)
    {
      if (!s->visited && s->lin == -1)
        return;
      else
        {
          s->visited = 0;
          s->lowlink = 0;
          s->lin = -1;
        }
    }
  else
    {
      if (!s->visited)
        return;
      else
        {
          s->visited = 0;
          s->lowlink = 0;
        }
    }
  switch (s->kind)
    {
    case store_var:
      {
        store_edge_set_scanner sess;
        store_edge b;
        for (store_edge_set_scan(s->u.var.lb, &sess), b = store_edge_set_next(&sess); b; b = store_edge_set_next(&sess))
          clean_store(b->store, cleanlin);
        return;
      }
      break;
    case store_filter:
      clean_store(s->u.filter.s, cleanlin);
      break;
    case store_ref:
      clean_store(s->u.ref.s, cleanlin);
      break;
    case store_ow:
      clean_store(s->u.ow.s1, cleanlin);
      clean_store(s->u.ow.s2, cleanlin);
      break;
    case store_assign:
      clean_store(s->u.assign.s, cleanlin);
      break;
    default:
      __fail("effect.c", 2612, __FUNCTION__, "Unexpected store kind %d\n", s->kind);
    }
}
static int cmp_assign_store_by_al(const void *s1, const void *s2)
{
  store *sl1 = (store *) s1;
  store *sl2 = (store *) s2;
  *sl1 = ecr_store(*sl1);
  *sl2 = ecr_store(*sl2);
  ((void) (((*sl1)->kind == store_assign && (*sl2)->kind == store_assign) ? 0 : (__assert_fail ("(*sl1)->kind == store_assign && (*sl2)->kind == store_assign", "effect.c", 2622, __PRETTY_FUNCTION__), 0)));
  return cmp_aloc((*sl1)->u.assign.al, (*sl2)->u.assign.al);
}
void compute_lins(void)
{
  dd_list_pos cur;
  int sched_size, last, processed, i = 0;
  store *sched_array;
  aloc cur_al;
  state = state_lin;
  scc_store_stack_init(store_count);
  sched_size = dd_length(assign_stores);
  sched_array = (((void)0), __rc_typed_rarrayalloc)((store_region), (sched_size), sizeof(store), 0);
  for (cur = dd_first((assign_stores)); !(!(cur)->next); cur = ((cur)->next))
    {
      sched_array[i] = ((store)((cur)->data));
      i++;
    }
  ((void) ((i == sched_size) ? 0 : (__assert_fail ("i == sched_size", "effect.c", 2653, __PRETTY_FUNCTION__), 0)));
  qsort(sched_array, i, sizeof(store), cmp_assign_store_by_al);
  do {
    cur_al = ((void *)0);
    last = 0;
    processed = 0;
    for (i = 0; i < sched_size; i++)
      {
        int lin;
        qtype lhs_qtype;
        sched_array[i] = ecr_store(sched_array[i]);
        ((void) ((sched_array[i]->kind == store_assign) ? 0 : (__assert_fail ("sched_array[i]->kind == store_assign", "effect.c", 2666, __PRETTY_FUNCTION__), 0)));
        lhs_qtype = qtype_from_store(sched_array[i]->u.assign.s,
                                     sched_array[i]->u.assign.al);
        if ((!toplvl_qual_fs_qtype(sched_array[i]->u.assign.cell->qtype) &&
             !toplvl_qual_fs_qtype(lhs_qtype)) ||
            sched_array[i]->u.assign.lin > -1)
          continue;
        if (processed == 0)
          {
            cur_al = sched_array[i]->u.assign.al;
            scc_store_number = 0;
            scc_store_stack_clear();
            lin = compute_lin_scc(cur_al,
                                  ecr_store(sched_array[i]->u.assign.s));
          }
        else if (eq_aloc(cur_al, sched_array[i]->u.assign.al))
          {
            store cur;
            clean_store(sched_array[i-1]->u.assign.s, 0);
            scc_store_number = 0;
            scc_store_stack_clear();
            cur = ecr_store(sched_array[i]->u.assign.s);
            if (cur->lin != -1)
              lin = cur->lin;
            else
              lin = compute_lin_scc(cur_al, cur);
          }
        else
          {
            int j;
            for (j = last; j < i; j++)
              clean_store(sched_array[j]->u.assign.s, 1);
            cur_al = sched_array[i]->u.assign.al;
            scc_store_number = 0;
            scc_store_stack_clear();
            lin = compute_lin_scc(cur_al,
                                  ecr_store(sched_array[i]->u.assign.s));
            last = i;
          }
        ((void) ((lin > -1) ? 0 : (__assert_fail ("lin > -1", "effect.c", 2716, __PRETTY_FUNCTION__), 0)));
        sched_array[i]->u.assign.lin = lin;
        processed++;
      }
    if (cur_al)
      {
        int j;
        for (j = last; j < i; j++)
          clean_store(sched_array[j]->u.assign.s, 1);
      }
    for (i = 0; i < sched_size; i++)
      {
        store s;
        s = sched_array[i];
        ((void) ((s->kind == store_assign) ? 0 : (__assert_fail ("s->kind == store_assign", "effect.c", 2746, __PRETTY_FUNCTION__), 0)));
        if (s->u.assign.lin == 2 && !s->u.assign.omega_warned)
        {
          s->u.assign.omega_warned = 1;
          if (s->u.assign.strong)
            report_qerror(sched_array[i]->loc, sev_warn,
                          "change_type applied to non-unique location");
          if (mkeq_qtype(s->loc, &fs_qgate,
                         s->u.assign.cell->qtype,
                         qtype_from_store(s->u.assign.s,
                                          s->u.assign.al)))
            report_qerror(s->loc, sev_err, "%s", s->u.assign.err_msg);
        }
      }
  } while (processed != 0);
  scc_store_stack_delete();
}
void propagate_store_cell_backward(store s, aloc al)
{
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 2796, __PRETTY_FUNCTION__), 0)));
  if (aloc_interesting(al))
    {
      s = ecr_store(s);
      switch (s->kind)
        {
        case store_var:
          {
            cell c;
            if (!cell_map_lookup(s->u.var.cells, al, &c))
              {
                qtype qt;
                qt = qtype_to_fs_qtype(s->loc, al->u.var.points_to);
                ((void) ((!(store_aloc_qtype(qt, s, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt, s, al))", "effect.c", 2811, __PRETTY_FUNCTION__), 0)));
                c = mkcell(store_region, qt, 0, 0);
                ((void) ((cell_map_insert(s->u.var.cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(s->u.var.cells, al, c)", "effect.c", 2813, __PRETTY_FUNCTION__), 0)));
              }
            if (!c->propagated_backward)
              {
                store_edge_set_scanner sess;
                store_edge lb;
                c->propagated_backward = 1;
                for (store_edge_set_scan(s->u.var.lb, &sess), lb = store_edge_set_next(&sess); lb; lb = store_edge_set_next(&sess))
                  if (propagate_cell_backward(lb->loc, lb->store, al,
                                              c->qtype, lb->p))
                    report_qerror(lb->loc, sev_err, "%s", lb->err_msg);
              }
          }
          break;
        case store_assign:
          break;
        default:
          __fail("effect.c", 2832, __FUNCTION__, "Unexpected store kind %x\n", s->kind);
        }
    }
}
void propagate_store_cell_forward(store s, aloc al)
{
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 2840, __PRETTY_FUNCTION__), 0)));
  if (aloc_interesting(al))
    {
      s = ecr_store(s);
      switch (s->kind)
        {
        case store_var:
          {
            cell c;
            if (!cell_map_lookup(s->u.var.cells, al, &c))
              {
                qtype qt;
                qt = qtype_to_fs_qtype(s->loc, al->u.var.points_to);
                ((void) ((!(store_aloc_qtype(qt, s, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt, s, al))", "effect.c", 2855, __PRETTY_FUNCTION__), 0)));
                c = mkcell(store_region, qt, 0, 0);
                ((void) ((cell_map_insert(s->u.var.cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(s->u.var.cells, al, c)", "effect.c", 2857, __PRETTY_FUNCTION__), 0)));
              }
            if (!c->propagated_forward)
              {
                store_edge_set_scanner sess;
                store_edge ub;
                c->propagated_forward = 1;
                for (store_edge_set_scan(s->ub, &sess), ub = store_edge_set_next(&sess); ub; ub = store_edge_set_next(&sess))
                  if (propagate_cell_forward(ub->loc, s, ub->store, al,
                                             c->qtype, ub->p))
                    report_qerror(ub->loc, sev_err, "%s", ub->err_msg);
              }
          }
          break;
        case store_assign:
          ((void) ((eq_aloc(al, s->u.assign.al)) ? 0 : (__assert_fail ("eq_aloc(al, s->u.assign.al)", "effect.c", 2873, __PRETTY_FUNCTION__), 0)));
          if (!s->u.assign.cell->propagated_forward)
            {
              store_edge_set_scanner sess;
              store_edge ub;
              s->u.assign.cell->propagated_forward = 1;
              for (store_edge_set_scan(s->ub, &sess), ub = store_edge_set_next(&sess); ub; ub = store_edge_set_next(&sess))
                  if (propagate_cell_forward(ub->loc, s, ub->store, al,
                                           s->u.assign.cell->qtype, ub->p))
                  report_qerror(ub->loc, sev_err, "%s", ub->err_msg);
            }
          break;
        default:
          __fail("effect.c", 2887, __FUNCTION__, "Unexpected store kind %x\n", s->kind);
        }
    }
}
static bool propagate_cell_backward(location loc, store lb, aloc al, qtype qt,
                                    polarity p)
{
  bool result;
  lb = ecr_store(lb);
  result = 0;
  switch (lb->kind)
    {
    case store_var:
      {
        cell c;
        if (!cell_map_lookup(lb->u.var.cells, al, &c))
          {
            qtype qt_fresh;
            qt_fresh = qtype_to_fs_qtype(lb->loc, al->u.var.points_to);
            ((void) ((!(store_aloc_qtype(qt_fresh, lb, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt_fresh, lb, al))", "effect.c", 2920, __PRETTY_FUNCTION__), 0)));
            c = mkcell(store_region, qt_fresh, 0, 0);
            ((void) ((cell_map_insert(lb->u.var.cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(lb->u.var.cells, al, c)", "effect.c", 2922, __PRETTY_FUNCTION__), 0)));
          }
        if (p == p_sub)
          result = mkleq_qtype(loc, &fs_qgate, c->qtype, qt) || result;
        else
          result = mkinst_qtype(loc, &open_qgate, c->qtype, qt, p) || result;
      }
      break;
    case store_filter:
      if (aloc_in_effect(al, lb->u.filter.e, 0))
        result =
          propagate_cell_backward(loc, lb->u.filter.s, al, qt, p) ||
          result;
      break;
    case store_ref:
      result = propagate_cell_backward(loc, lb->u.ref.s, al, qt, p) || result;
      break;
    case store_ow:
      if (aloc_in_effect(al, lb->u.ow.e, 0))
        result = propagate_cell_backward(loc, lb->u.ow.s1, al, qt, p) ||
          result;
      else
        result = propagate_cell_backward(loc, lb->u.ow.s2, al, qt, p) ||
          result;
      break;
    case store_assign:
      if (eq_aloc(al, lb->u.assign.al))
        result = mkleq_qtype(loc, &fs_qgate,
                             lb->u.assign.cell->qtype, qt) || result;
      else
        result =
          propagate_cell_backward(loc, lb->u.assign.s, al, qt, p) ||
          result;
      break;
    default:
      __fail("effect.c", 2957, __FUNCTION__, "Unexpected store kind %x\n", lb->kind);
    }
  return result;
}
static bool propagate_cell_forward(location loc, store lb, store ub,
                                   aloc al, qtype qt, polarity p)
{
  bool result, propagate_to_ub;
  result = 0;
  propagate_to_ub = 0;
  ub = ecr_store(ub);
  switch (ub->kind)
    {
    case store_var:
      {
        cell c;
        if (!cell_map_lookup(ub->u.var.cells, al, &c))
          {
            qtype qt_fresh;
            qt_fresh = qtype_to_fs_qtype(ub->loc, al->u.var.points_to);
            ((void) ((!(store_aloc_qtype(qt_fresh, ub, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt_fresh, ub, al))", "effect.c", 2991, __PRETTY_FUNCTION__), 0)));
            c = mkcell(store_region, qt_fresh, 0, 0);
            ((void) ((cell_map_insert(ub->u.var.cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(ub->u.var.cells, al, c)", "effect.c", 2993, __PRETTY_FUNCTION__), 0)));
          }
        if (p == p_sub)
          result = mkleq_qtype(loc, &fs_qgate, qt, c->qtype) || result;
        else
          result = mkinst_qtype(loc, &open_qgate, qt, c->qtype, p) || result;
        propagate_to_ub = 0;
      }
      break;
    case store_filter:
      propagate_to_ub = aloc_in_effect(al, ub->u.filter.e, 0);
      break;
    case store_ref:
      propagate_to_ub = 1;
      break;
    case store_ow:
      if (aloc_in_effect(al, ub->u.ow.e, 0))
        propagate_to_ub = eq_store(lb, ub->u.ow.s1);
      else
        propagate_to_ub = eq_store(lb, ub->u.ow.s2);
      break;
    case store_assign:
      if (!eq_aloc(al, ub->u.assign.al))
        propagate_to_ub = 1;
      break;
    default:
      __fail("effect.c", 3022, __FUNCTION__, "Unexpected store kind %x\n", ub->kind);
    }
  if (propagate_to_ub)
    {
      store_edge_set_scanner sess;
      store_edge ub_ub;
      for (store_edge_set_scan(ub->ub, &sess), ub_ub = store_edge_set_next(&sess); ub_ub; ub_ub = store_edge_set_next(&sess))
        result = propagate_cell_forward(loc, ub, ub_ub->store, al, qt, ub_ub->p)
        || result;
    }
  return result;
}
cell_map store_to_cells(region r, store s)
{
  cell_map cells;
  cell_map_scanner cms;
  aloc al;
  cell c;
  s = ecr_store(s);
  switch (s->kind)
    {
    case store_var:
       cell_map_scan(s->u.var.cells, &cms);
       cells = make_cell_map(r, 7);
       while (cell_map_next(&cms, &al, &c))
       if (c->propagated_forward)
         ((void) ((cell_map_insert(cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(cells, al, c)", "effect.c", 3056, __PRETTY_FUNCTION__), 0)));
      break;
    case store_filter:
      cells = make_cell_map(r, 7);
      cell_map_scan(store_to_cells(r, s->u.filter.s), &cms);
      while (cell_map_next(&cms, &al, &c))
        if (aloc_in_effect(al, s->u.filter.e, 0))
          ((void) ((cell_map_insert(cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(cells, al, c)", "effect.c", 3063, __PRETTY_FUNCTION__), 0)));
      break;
    case store_ref:
      cells = store_to_cells(r, s->u.ref.s);
      break;
    case store_ow:
      cells = make_cell_map(r, 7);
      cell_map_scan(store_to_cells(r, s->u.ow.s1), &cms);
      while (cell_map_next(&cms, &al, &c))
        if (aloc_in_effect(al, s->u.ow.e, 0))
          ((void) ((cell_map_insert(cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(cells, al, c)", "effect.c", 3073, __PRETTY_FUNCTION__), 0)));
      cell_map_scan(store_to_cells(r, s->u.ow.s2), &cms);
      while (cell_map_next(&cms, &al, &c))
        if (!aloc_in_effect(al, s->u.ow.e, 0))
          ((void) ((cell_map_insert(cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(cells, al, c)", "effect.c", 3077, __PRETTY_FUNCTION__), 0)));
      break;
    case store_assign:
      cells = store_to_cells(r, s->u.assign.s);
      if (aloc_interesting(s->u.assign.al))
      {
        if (!s->u.assign.cell->propagated_forward)
          cell_map_remove(cells, s->u.assign.al);
        else
          cell_map_insert(cells, s->u.assign.al, s->u.assign.cell);
      }
      break;
    default:
      __fail("effect.c", 3092, __FUNCTION__, "Unexpected store kind %x\n", s->kind);
    }
  return cells;
}
static cell cell_from_store(store s, aloc al, store *store_with_cell, bool ignore_filter)
{
  cell c;
  ((void) ((aloc_interesting(al)) ? 0 : (__assert_fail ("aloc_interesting(al)", "effect.c", 3103, __PRETTY_FUNCTION__), 0)));
  s = ecr_store(s);
  switch (s->kind)
    {
    case store_var:
      if (!cell_map_lookup(s->u.var.cells, al, &c))
        {
          qtype qt;
          qt = qtype_to_fs_qtype(s->loc, al->u.var.points_to);
          ((void) ((!(store_aloc_qtype(qt, s, al))) ? 0 : (__assert_fail ("!(store_aloc_qtype(qt, s, al))", "effect.c", 3112, __PRETTY_FUNCTION__), 0)));
          c = mkcell(store_region, qt, 0, 0);
          ((void) ((cell_map_insert(s->u.var.cells, al, c)) ? 0 : (__assert_fail ("cell_map_insert(s->u.var.cells, al, c)", "effect.c", 3124, __PRETTY_FUNCTION__), 0)));
        }
      if (store_with_cell)
        *store_with_cell = s;
      break;
    case store_filter:
      if (ignore_filter || aloc_in_effect(al, s->u.filter.e, 0))
        c = cell_from_store(s->u.filter.s, al, store_with_cell, ignore_filter);
      else
        c = ((void *)0);
      break;
    case store_ref:
      c = cell_from_store(s->u.ref.s, al, store_with_cell, ignore_filter);
      break;
    case store_ow:
      if (aloc_in_effect(al, s->u.ow.e, 0))
        c = cell_from_store(s->u.ow.s1, al, store_with_cell, ignore_filter);
      else
        c = cell_from_store(s->u.ow.s2, al, store_with_cell, ignore_filter);
      break;
    case store_assign:
      if (eq_aloc(al, s->u.assign.al))
        {
          c = s->u.assign.cell;
          if (store_with_cell)
            *store_with_cell = s;
        }
      else
        c = cell_from_store(s->u.assign.s, al, store_with_cell, ignore_filter);
      break;
    default:
      __fail("effect.c", 3155, __FUNCTION__, "Unexpected store kind %x\n", s->kind);
    }
  return c;
}
qtype qtype_from_store(store s, aloc al)
{
  qtype qt;
  al = ecr_aloc(al);
  ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 3166, __PRETTY_FUNCTION__), 0)));
  if (al->u.var.interesting)
    {
      cell c;
      c = cell_from_store(s, al, ((void *)0), 1);
      if (c)
        return qt = c->qtype;
      else
        return qt = ((void *)0);
    }
  else
    {
      if (!al->u.var.fs_points_to)
        al->u.var.fs_points_to = qtype_to_fs_qtype(((void *)0), al->u.var.points_to);
      qt = al->u.var.fs_points_to;
    }
  return qt;
}
int lin_from_store(store s, aloc al)
{
  s = ecr_store(s);
  ((void) ((s->kind == store_assign) ? 0 : (__assert_fail ("s->kind == store_assign", "effect.c", 3210, __PRETTY_FUNCTION__), 0)));
  ((void) ((eq_aloc(al, s->u.assign.al)) ? 0 : (__assert_fail ("eq_aloc(al, s->u.assign.al)", "effect.c", 3211, __PRETTY_FUNCTION__), 0)));
  return s->u.assign.lin;
}
int print_store(printf_func pf, store s)
{
  int result = 0;
  s = ecr_store(s);
  switch (s->kind)
    {
    case store_var:
      return pf("%s", s->u.var.name);
      break;
    case store_filter:
      result = pf("filter(");
      result += print_store(pf, s->u.filter.s);
      result += pf(", ");
      result += print_effect(pf, s->u.filter.e);
      return result + pf(")");
      break;
    case store_ref:
      result = pf("ref(");
      result += print_aloc(pf, s->u.ref.al);
      result += printf(", ");
      result += print_store(pf, s->u.ref.s);
      return result + pf(")");
      break;
    case store_ow:
      result = pf("ow(");
      result += print_store(pf, s->u.ow.s1);
      result += pf(", ");
      result += print_store(pf, s->u.ow.s2);
      result += pf(", ");
      result += print_effect(pf, s->u.ow.e);
      return result + pf(")");
      break;
    case store_assign:
      result = pf("assign(");
      result += print_store(pf, s->u.assign.s);
      result += pf(", ");
      result += print_aloc(pf, s->u.assign.al);
      result += pf(", ");
      result += print_qtype(pf, s->u.assign.cell->qtype, s);
      return result + pf(")");
      break;
    default:
      __fail("effect.c", 3266, __FUNCTION__, "Unexpected store of kind %d\n", s->kind);
    }
}
static int print_cells(printf_func pf, pr_qual_fn pr_qual, cell_map cells,
                       bool print_alocs)
{
  int result;
  bool first;
  cell_map_scanner cms;
  aloc al;
  cell c;
  result = 0;
  first = 1;
  cell_map_scan(cells, &cms);
  result += pf("{");
  while (cell_map_next(&cms, &al, &c))
    {
      if (!first)
        result += pf(", ");
      else
        first = 0;
      result += print_aloc(pf, al);
      result += pf(": ");
      result += print_qtype_qf(pf, pr_qual, c->qtype, ((void *)0), print_alocs);
    }
  result += pf("}");
  return result;
}
int print_store_cells(printf_func pf, pr_qual_fn pr_qual, store s,
                      bool print_alocs)
{
  region scratch_region;
  cell_map cells;
  int result;
  scratch_region = newregion();
  cells = store_to_cells(scratch_region, s);
  result = print_cells(pf, pr_qual, cells, print_alocs);
  deleteregion(scratch_region);
  return result;
}
typedef struct Cinf_const {
  drinfo dri;
  nin_constraint nin1;
  nin_constraint nin2;
  nin_constraint nin3;
  forall_nin_constraint forall1;
  forall_nin_constraint forall2;
  forall_cond_nin_constraint forallcond;
  int num;
} *cinf_const;
int cinf_cnt = 0;
void mk_confine_inf_const(drinfo dri)
{
  cinf_const cc = (sizeof(struct Cinf_const) < (1 << (13 - 3)) ? (((void)0), __rc_ralloc_small0)((effect_region), sizeof(struct Cinf_const)) : (((void)0), __rc_typed_ralloc)((effect_region), sizeof(struct Cinf_const), 0));
  cinf_cnt++;
  cc->num = cinf_cnt;
  ((void) ((dri) ? 0 : (__assert_fail ("dri", "effect.c", 4074, __PRETTY_FUNCTION__), 0)));
  cc->dri = dri;
  cc->nin1 =
    internal_mknin_aloc_effect(dri->location,
                               aloc_qtype(dri->rqtype),
                               eff_rwr,
                               alocs_qtype(points_to_qtype(dri->qtype)));
  if (cc->nin1)
    cc->nin1->inference = 1;
  cc->nin2 =
    internal_mknin_aloc_effect(dri->location,
                               aloc_qtype(dri->rqtype),
                               eff_any,
                               dri->env);
  if (cc->nin2)
    cc->nin2->inference = 1;
  cc->nin3 =
    internal_mknin_aloc_effect(dri->location,
                               aloc_qtype(dri->qtype),
                               eff_rwr,
                               dri->body_eff);
  if (cc->nin3)
    cc->nin3->inference = 1;
  cc->forall1 =
    internal_forall_aloc_mknin_effect(dri->location,
                                      eff_wr,
                                      dri->effect);
  if (cc->forall1)
      cc->forall1->inference = 1;
  cc->forall2 =
    internal_forall_aloc_mknin_effect(dri->location,
                                      eff_alloc,
                                      dri->effect);
  if (cc->forall2)
    cc->forall2->inference = 1;
  cc->forallcond =
    internal_forall_aloc_in_effect_mknin_effect(dri->location,
                                                eff_r,
                                                dri->effect,
                                                eff_wr,
                                                dri->body_eff);
  if (cc->forallcond)
    cc->forallcond->inference = 1;
  dd_add_last(effect_region, cinf_constraints, cc);
}
void check_effect_constraints(void)
{
  dd_list_pos cur;
  bool unification_happened;
  int failed_count = 0;
  do {
    check_nin();
    unification_happened = 0;
    for (cur = dd_first((cinf_constraints)); !(!(cur)->next); cur = ((cur)->next))
      {
        cinf_const cc = ((cinf_const)((cur)->data));
        if (!cc->dri->failed &&
            ((cc->nin1 && cc->nin1->failed) ||
             (cc->nin2 && cc->nin2->failed) ||
             (cc->nin3 && cc->nin3->failed) ||
             (cc->forall1 && cc->forall1->failed) ||
             (cc->forall2 && cc->forall2->failed) ||
             (cc->forallcond && cc->forallcond->failed)))
          {
            ((void) ((cc->dri->inference) ? 0 : (__assert_fail ("cc->dri->inference", "effect.c", 4141, __PRETTY_FUNCTION__), 0)));
            cc->dri->failed = 1;
            unify_aloc(cc->dri->location,
                       aloc_qtype(cc->dri->rqtype),
                       aloc_qtype(cc->dri->qtype));
            unification_happened = 1;
            failed_count++;
          }
      }
  } while (unification_happened);
  for (cur = dd_first((cinf_constraints)); !(!(cur)->next); cur = ((cur)->next))
    {
      cinf_const cc = ((cinf_const)((cur)->data));
      if (!cc->dri->failed)
        {
          aloc al = ecr_aloc(aloc_qtype(cc->dri->rqtype));
          ((void) ((al->kind == aloc_var) ? 0 : (__assert_fail ("al->kind == aloc_var", "effect.c", 4181, __PRETTY_FUNCTION__), 0)));
          hax_state = state;
          state = state_init;
          mkleq_effect(effect_alloc(al), cc->dri->body_eff);
          al->u.var.reach_computed = 0;
          state = hax_state;
        }
    }
}
rinf_const mkrinf_constraint(location loc,
                             aloc r_al, aloc old_al,
                             aloc top_al,
                             effect r_type, effect pt_type,
                             effect r_body, effect env)
{
  return ((void *)0);
}
void set_bodyeffect(rinf_const rc, effect body)
{
}
void check_rinf_constraints(void)
{
}
const char CANON_IDENT_53a3e7a859c6a70ce8fed5e99c109ccd[] = "CANON_IDENT_/moa/sc1/jkodumal/work/banshee/experiments/cqual/src/effect.c";
