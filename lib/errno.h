/* This file is part of the Cyclone Library.
   Copyright (C) 2001 Greg Morrisett

   This library is free software; you can redistribute it and/or it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place, Suite
   330, Boston, MA 02111-1307 USA. */


/* This file is quite platform dependent.  It works for cygwin
   and some x86 linuxes only. */

#ifndef _ERRNO_H_
#define _ERRNO_H_

namespace Errno {
extern const char ?sys_err(int);
extern "C" {

/* __CYCLONE_SYS_NUM_ERR is the number of the highest error code
   plus 1.  It should agree with the _sys_nerr variable.  It's used
   as the bounds of an internal C array, so be sure it's correct. */
#if defined(__CYGWIN32__) || defined(__CYGWIN__)
/* cygwin */
#define __CYCLONE_SYS_NUM_ERR 137
int @ __errno(void);
#define errno (*Errno::__errno())
#else
/* x86 linux */
#define __CYCLONE_SYS_NUM_ERR 124
int @ __errno_location(void);
#define errno (*Errno::__errno_location())
#endif

#define	EPERM		 1	/* Operation not permitted */
#define	ENOENT		 2	/* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EINTR		 4	/* Interrupted system call */
#define	EIO		 5	/* I/O error */
#define	ENXIO		 6	/* No such device or address */
#define	E2BIG		 7	/* Arg list too long */
#define	ENOEXEC		 8	/* Exec format error */
#define	EBADF		 9	/* Bad file number */
#define	ECHILD		10	/* No child processes */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EACCES		13	/* Permission denied */
#define	EFAULT		14	/* Bad address */
#define	ENOTBLK		15	/* Block device required */
#define	EBUSY		16	/* Device or resource busy */
#define	EEXIST		17	/* File exists */
#define	EXDEV		18	/* Cross-device link */
#define	ENODEV		19	/* No such device */
#define	ENOTDIR		20	/* Not a directory */
#define	EISDIR		21	/* Is a directory */
#define	EINVAL		22	/* Invalid argument */
#define	ENFILE		23	/* File table overflow */
#define	EMFILE		24	/* Too many open files */
#define	ENOTTY		25	/* Not a typewriter */
#define	ETXTBSY		26	/* Text file busy */
#define	EFBIG		27	/* File too large */
#define	ENOSPC		28	/* No space left on device */
#define	ESPIPE		29	/* Illegal seek */
#define	EROFS		30	/* Read-only file system */
#define	EMLINK		31	/* Too many links */
#define	EPIPE		32	/* Broken pipe */
#define	EDOM		33	/* Math argument out of domain of func */
#define	ERANGE		34	/* Math result not representable */

#if defined(__CYGWIN32__) || defined(__CYGWIN__)
#define	ENOMSG 		35	/* No message of desired type */
#define	EIDRM 		36	/* Identifier removed */
#define	ECHRNG 		37	/* Channel number out of range */
#define	EL2NSYNC 	38	/* Level 2 not synchronized */
#define	EL3HLT 		39	/* Level 3 halted */
#define	EL3RST 		40	/* Level 3 reset */
#define	ELNRNG 		41	/* Link number out of range */
#define	EUNATCH 	42	/* Protocol driver not attached */
#define	ENOCSI 		43	/* No CSI structure available */
#define	EL2HLT 		44	/* Level 2 halted */
#define	EDEADLK 	45	/* Deadlock condition */
#define	ENOLCK 		46	/* No record locks available */
#define EBADE 		50	/* Invalid exchange */
#define EBADR 		51	/* Invalid request descriptor */
#define EXFULL 		52	/* Exchange full */
#define ENOANO 		53	/* No anode */
#define EBADRQC 	54	/* Invalid request code */
#define EBADSLT 	55	/* Invalid slot */
#define EDEADLOCK 	56	/* File locking deadlock error */
#define EBFONT		57	/* Bad font file fmt */
#else
#define	EDEADLK		35	/* Resource deadlock would occur */
#define	ENAMETOOLONG	36	/* File name too long */
#define	ENOLCK		37	/* No record locks available */
#define	ENOSYS		38	/* Function not implemented */
#define	ENOTEMPTY	39	/* Directory not empty */
#define	ELOOP		40	/* Too many symbolic links encountered */
#define	EWOULDBLOCK	EAGAIN	/* Operation would block */
#define	ENOMSG		42	/* No message of desired type */
#define	EIDRM		43	/* Identifier removed */
#define	ECHRNG		44	/* Channel number out of range */
#define	EL2NSYNC	45	/* Level 2 not synchronized */
#define	EL3HLT		46	/* Level 3 halted */
#define	EL3RST		47	/* Level 3 reset */
#define	ELNRNG		48	/* Link number out of range */
#define	EUNATCH		49	/* Protocol driver not attached */
#define	ENOCSI		50	/* No CSI structure available */
#define	EL2HLT		51	/* Level 2 halted */
#define	EBADE		52	/* Invalid exchange */
#define	EBADR		53	/* Invalid request descriptor */
#define	EXFULL		54	/* Exchange full */
#define	ENOANO		55	/* No anode */
#define	EBADRQC		56	/* Invalid request code */
#define	EBADSLT		57	/* Invalid slot */
#define	EDEADLOCK	EDEADLK
#define	EBFONT		59	/* Bad font file format */
#endif

#define	ENOSTR		60	/* Device not a stream */
#define	ENODATA		61	/* No data available */
#define	ETIME		62	/* Timer expired */
#define	ENOSR		63	/* Out of streams resources */
#define	ENONET		64	/* Machine is not on the network */
#define	ENOPKG		65	/* Package not installed */
#define	EREMOTE		66	/* Object is remote */
#define	ENOLINK		67	/* Link has been severed */
#define	EADV		68	/* Advertise error */
#define	ESRMNT		69	/* Srmount error */
#define	ECOMM		70	/* Communication error on send */
#define	EPROTO		71	/* Protocol error */

#if defined(__CYGWIN32__) || defined(__CYGWIN__)
#define	EMULTIHOP 	74	/* Multihop attempted */
#define	ELBIN 		75	/* Inode is remote (not really error) */
#define	EDOTDOT 	76	/* Cross mount point (not really error) */
#define EBADMSG 	77	/* Trying to read unreadable message */
#define ENOTUNIQ 	80	/* Given log. name not unique */
#define EBADFD 		81	/* f.d. invalid for this operation */
#define EREMCHG 	82	/* Remote address changed */
#define ELIBACC 	83	/* Can't access a needed shared lib */
#define ELIBBAD 	84	/* Accessing a corrupted shared lib */
#define ELIBSCN 	85	/* .lib section in a.out corrupted */
#define ELIBMAX 	86	/* Attempting to link in too many libs */
#define ELIBEXEC 	87	/* Attempting to exec a shared library */
#define ENOSYS 		88	/* Function not implemented */
#define ENMFILE 	89      /* No more files */
#define ENOTEMPTY 	90	/* Directory not empty */
#define ENAMETOOLONG 	91	/* File or path name too long */
#define ELOOP 		92	/* Too many symbolic links */
#define EOPNOTSUPP 	95	/* Operation not supported on transport endpoint */
#define EPFNOSUPPORT 	96 	/* Protocol family not supported */
#define ECONNRESET 	104  	/* Connection reset by peer */
#define ENOBUFS 	105	/* No buffer space available */
#define EAFNOSUPPORT 	106 	/* Address family not supported by protocol family */
#define EPROTOTYPE 	107	/* Protocol wrong type for socket */
#define ENOTSOCK 	108	/* Socket operation on non-socket */
#define ENOPROTOOPT 	109	/* Protocol not available */
#define ESHUTDOWN 	110	/* Can't send after socket shutdown */
#define ECONNREFUSED 	111	/* Connection refused */
#define EADDRINUSE 	112	/* Address already in use */
#define ECONNABORTED 	113	/* Connection aborted */
#define ENETUNREACH 	114	/* Network is unreachable */
#define ENETDOWN 	115	/* Network interface is not configured */
#define ETIMEDOUT 	116	/* Connection timed out */
#define EHOSTDOWN 	117	/* Host is down */
#define EHOSTUNREACH 	118	/* Host is unreachable */
#define EINPROGRESS 	119	/* Connection already in progress */
#define EALREADY 	120	/* Socket already connected */
#define EDESTADDRREQ 	121	/* Destination address required */
#define EMSGSIZE 	122	/* Message too long */
#define EPROTONOSUPPORT 123	/* Unknown protocol */
#define ESOCKTNOSUPPORT 124	/* Socket type not supported */
#define EADDRNOTAVAIL 	125	/* Address not available */
#define ENETRESET 	126
#define EISCONN 	127	/* Socket is already connected */
#define ENOTCONN 	128	/* Socket is not connected */
#define ETOOMANYREFS 	129
#define EPROCLIM 	130
#define EUSERS 		131
#define EDQUOT 		132
#define ESTALE 		133
#define ENOTSUP 	134	/* Not supported */
#define ENOMEDIUM 	135   	/* No medium (in tape drive) */
#define ENOSHARE 	136    	/* No such host or network path */
#define EWOULDBLOCK 	EAGAIN	/* Operation would block */
#else
#define	EMULTIHOP	72	/* Multihop attempted */
#define	EDOTDOT		73	/* RFS specific error */
#define	EBADMSG		74	/* Not a data message */
#define	EOVERFLOW	75	/* Value too large for defined data type */
#define	ENOTUNIQ	76	/* Name not unique on network */
#define	EBADFD		77	/* File descriptor in bad state */
#define	EREMCHG		78	/* Remote address changed */
#define	ELIBACC		79	/* Can not access a needed shared library */
#define	ELIBBAD		80	/* Accessing a corrupted shared library */
#define	ELIBSCN		81	/* .lib section in a.out corrupted */
#define	ELIBMAX		82	/* Attempting to link in too many shared libraries */
#define	ELIBEXEC	83	/* Cannot exec a shared library directly */
#define	EILSEQ		84	/* Illegal byte sequence */
#define	ERESTART	85	/* Interrupted system call should be restarted */
#define	ESTRPIPE	86	/* Streams pipe error */
#define	EUSERS		87	/* Too many users */
#define	ENOTSOCK	88	/* Socket operation on non-socket */
#define	EDESTADDRREQ	89	/* Destination address required */
#define	EMSGSIZE	90	/* Message too long */
#define	EPROTOTYPE	91	/* Protocol wrong type for socket */
#define	ENOPROTOOPT	92	/* Protocol not available */
#define	EPROTONOSUPPORT	93	/* Protocol not supported */
#define	ESOCKTNOSUPPORT	94	/* Socket type not supported */
#define	EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define	EPFNOSUPPORT	96	/* Protocol family not supported */
#define	EAFNOSUPPORT	97	/* Address family not supported by protocol */
#define	EADDRINUSE	98	/* Address already in use */
#define	EADDRNOTAVAIL	99	/* Cannot assign requested address */
#define	ENETDOWN	100	/* Network is down */
#define	ENETUNREACH	101	/* Network is unreachable */
#define	ENETRESET	102	/* Network dropped connection because of reset */
#define	ECONNABORTED	103	/* Software caused connection abort */
#define	ECONNRESET	104	/* Connection reset by peer */
#define	ENOBUFS		105	/* No buffer space available */
#define	EISCONN		106	/* Transport endpoint is already connected */
#define	ENOTCONN	107	/* Transport endpoint is not connected */
#define	ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define	ETOOMANYREFS	109	/* Too many references: cannot splice */
#define	ETIMEDOUT	110	/* Connection timed out */
#define	ECONNREFUSED	111	/* Connection refused */
#define	EHOSTDOWN	112	/* Host is down */
#define	EHOSTUNREACH	113	/* No route to host */
#define	EALREADY	114	/* Operation already in progress */
#define	EINPROGRESS	115	/* Operation now in progress */
#define	ESTALE		116	/* Stale NFS file handle */
#define	EUCLEAN		117	/* Structure needs cleaning */
#define	ENOTNAM		118	/* Not a XENIX named type file */
#define	ENAVAIL		119	/* No XENIX semaphores available */
#define	EISNAM		120	/* Is a named type file */
#define	EREMOTEIO	121	/* Remote I/O error */
#define	EDQUOT		122	/* Quota exceeded */
#define	ENOMEDIUM	123	/* No medium found */
#define	EMEDIUMTYPE	124	/* Wrong medium type */
#endif

}}
#endif /* _ERRNO_H */
