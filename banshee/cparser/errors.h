/* This file is part of the RC compiler.

This file is derived from the GNU C Compiler. It is thus
   Copyright (C) 1987, 88, 89, 92-7, 1998 Free Software Foundation, Inc.
and
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
Boston, MA 02111-1307, USA. */

#ifndef ERRORS_H
#define ERRORS_H

extern int errorcount;
extern int warningcount;

/* Report error msg at filename, lineno */
void verror_with_file_and_line(const char *filename, int lineno,
			       const char *format, va_list args);

/* Report error msg at l */
void verror_with_location(location l, const char *format, va_list args);

/* Report error msg at decl */
void verror_with_decl(declaration d, const char *format, va_list args);

/* Report error msg at current filename, lineno */
void verror(const char *format, va_list args);

/* Report error msg at current filename, lineno */
void error(const char *format, ...);

/* Report error msg at decl */
void error_with_decl(declaration d, const char *format, ...);

/* Report error msg at l */
void error_with_location(location l, const char *format, ...);

/* Report a fatal error at the current line number.  */
void vfatal(const char *format, va_list args);

void fatal(const char *format, ...);

/* Report warning msg at filename, lineno */
void vwarning_with_file_and_line(char *filename, int lineno,
				 const char *format, va_list args);

/* Report warning msg at l */
void vwarning_with_location(location l, const char *format, va_list args);

/* Report warning msg at decl */
void vwarning_with_decl(declaration d, const char *format, va_list args);

/* Report warning msg at current filename, lineno */
void vwarning(const char *format, va_list args);

/* Report warning msg at current filename, lineno */
void warning(const char *format, ...);


/* Report warning msg at filename, lineno */
void warning_with_file_and_line(char *filename, int lineno,
				const char *format, ...);

/* Report warning msg at decl */
void warning_with_decl(declaration d, const char *format, ...);

/* Report warning msg at l */
void warning_with_location(location l, const char *format, ...);

/* Report warning msg at current filename, lineno */
void warning_or_error(bool iswarning, const char *format, ...);


/* Report warning msg at filename, lineno */
void warning_or_error_with_file_and_line(bool iswarning,
					 char *filename, int lineno,
					 const char *format, ...);

/* Report warning msg at decl */
void warning_or_error_with_decl(bool iswarning, declaration d,
				const char *format, ...);

/* Report warning msg at l */
void warning_or_error_with_location(bool iswarning, location l,
				    const char *format, ...);

/* Report pedantic warning or error msg at current filename, lineno */
void pedwarn(const char *format, ...);

/* Report pedantic warning or error msg at d */
void pedwarn_with_decl(declaration d, const char *format, ...);

/* Report pedantic warning or error msg at l */
void pedwarn_with_location(location l, const char *format, ...);

#endif
