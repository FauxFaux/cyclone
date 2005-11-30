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


#include <stdarg.h>

#include "parser.h"
#include "errors.h"
#include "semantics.h"
#include "input.h"
#include "flags.h"

int errorcount;
int warningcount;

/* Count an error or warning.  Return 1 if the message should be printed.  */
int count_error(int warningp)
{
  if (/* warningp &&  */inhibit_warnings)
    return 0;

/*   if (inhibit_errors) */
/*     return 0; */

  if (warningp && !warnings_are_errors)
    warningcount++;
  else
    {
      static int warning_message = 0;

      if (warningp && !warning_message)
	{
	  fprintf (stderr, "%s: warnings being treated as errors\n", progname);
	  warning_message = 1;
	}
      errorcount++;
    }

  return 1;
}

/* Function of last error message;
   more generally, function such that if next error message is in it
   then we don't have to mention the function name.  */
static function_decl last_error_function = NULL;

/* Used to detect when input_file_stack has changed since last described.  */
static int last_error_tick;

/* The default function to print out name of current function that caused
   an error.  */

/* Called by report_error_function to print out function name. */
void print_error_function(char *file)
{
  if (last_error_function != current_function_decl)
    {
      if (file)
	fprintf (stderr, "%s: ", file);

      if (current_function_decl == NULL)
	fprintf (stderr, "At top level:\n");
      else
	{
	  const char *name =
	    declarator_name(current_function_decl->declarator);
	  fprintf (stderr, "In function `%s':\n", name);
	}

      last_error_function = current_function_decl;
    }
}

/* Prints out, if necessary, the name of the current function
  that caused an error.  Called from all error and warning functions.  */

void report_error_function(char *file)
{
  struct file_stack *p;

  print_error_function(file);

  if (input_file_stack && input_file_stack->next != 0
      && input_file_stack_tick != last_error_tick
      && file == input_filename)
    {
      fprintf (stderr, "In file included");
      for (p = input_file_stack->next; p; p = p->next)
	{
	  fprintf (stderr, " from %s:%d", p->name, p->line);
	  if (p->next)
	    fprintf (stderr, ",\n                ");
	}
      fprintf (stderr, ":\n");
      last_error_tick = input_file_stack_tick;
    }
}


void pfile_and_line(FILE *f, const char *filename, int lineno)
{
  fprintf(f, "%s:%d: ", filename, lineno);
}

/* Report error msg at filename, lineno */
void verror_with_file_and_line(const char *filename, int lineno,
			       const char *format, va_list args)
{
  count_error(FALSE);
  report_error_function(input_filename);
  pfile_and_line(stderr, filename, lineno);
  vfprintf(stderr, format, args);
  putc('\n', stderr);
}

/* Report error msg at l */
void verror_with_location(location l, const char *format, va_list args)
{
  verror_with_file_and_line(l.filename, l.lineno, format, args);
}

/* Report error msg at decl */
void verror_with_decl(declaration d, const char *format, va_list args)
{
  verror_with_location(d->location, format, args);
}

/* Report error msg at current filename, lineno */
void verror(const char *format, va_list args)
{
  verror_with_file_and_line(input_filename, lineno, format, args);
}

/* Report error msg at current filename, lineno */
void error(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  verror(format, args);
  va_end(args);
}

/* Report error msg at decl */
void error_with_decl(declaration d, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  verror_with_decl(d, format, args);
  va_end(args);
}

/* Report error msg at l */
void error_with_location(location l, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  verror_with_location(l, format, args);
  va_end(args);
}

/* Report a fatal error at the current line number.  */
void vfatal(const char *format, va_list args)
{
  verror(format, args);
  exit(FATAL_EXIT_CODE);
}

void fatal(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  vfatal(format, args);
  va_end(args);
}

/* Report warning msg at filename, lineno */
void vwarning_with_file_and_line(char *filename, int lineno,
				 const char *format, va_list args)
{
  if (count_error(TRUE))
    {
      report_error_function(filename);
      pfile_and_line(stderr, filename, lineno);
      fprintf(stderr, "warning: ");
      vfprintf(stderr, format, args);
      putc('\n', stderr);
    }
}

/* Report warning msg at l */
void vwarning_with_location(location l, const char *format, va_list args)
{
  vwarning_with_file_and_line(l.filename, l.lineno, format, args);
}

/* Report warning msg at decl */
void vwarning_with_decl(declaration d, const char *format, va_list args)
{
  vwarning_with_location(d->location, format, args);
}

/* Report warning msg at current filename, lineno */
void vwarning(const char *format, va_list args)
{
  vwarning_with_file_and_line(input_filename, lineno, format, args);
}

/* Report warning msg at current filename, lineno */
void warning(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  vwarning(format, args);
  va_end(args);
}


/* Report warning msg at filename, lineno */
void warning_with_file_and_line(char *filename, int lineno,
				const char *format, ...)
{
  va_list args;

  va_start(args, format);
  vwarning_with_file_and_line(filename, lineno, format, args);
  va_end(args);
}

/* Report warning msg at decl */
void warning_with_decl(declaration d, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  vwarning_with_decl(d, format, args);
  va_end(args);
}

/* Report warning msg at l */
void warning_with_location(location l, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  vwarning_with_location(l, format, args);
  va_end(args);
}

/* Report warning msg at current filename, lineno */
void warning_or_error(bool iswarning, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (iswarning)
    vwarning(format, args);
  else
    verror(format, args);
  va_end(args);
}


/* Report warning msg at filename, lineno */
void warning_or_error_with_file_and_line(bool iswarning,
					 char *filename, int lineno,
					 const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (iswarning)
    vwarning_with_file_and_line(filename, lineno, format, args);
  else
    verror_with_file_and_line(filename, lineno, format, args);
  va_end(args);
}

/* Report warning msg at decl */
void warning_or_error_with_decl(bool iswarning, declaration d,
				const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (iswarning)
    vwarning_with_decl(d, format, args);
  else
    verror_with_decl(d, format, args);
  va_end(args);
}

/* Report warning msg at l */
void warning_or_error_with_location(bool iswarning, location l,
				    const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (iswarning)
    vwarning_with_location(l, format, args);
  else
    verror_with_location(l, format, args);
  va_end(args);
}

/* Report pedantic warning or error msg at current filename, lineno */
void pedwarn(const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (flag_pedantic_errors)
    verror(format, args);
  else
    vwarning(format, args);
  va_end(args);
}

/* Report pedantic warning or error msg at d */
void pedwarn_with_decl(declaration d, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (flag_pedantic_errors)
    verror_with_decl(d, format, args);
  else
    vwarning_with_decl(d, format, args);
  va_end(args);
}

/* Report pedantic warning or error msg at l */
void pedwarn_with_location(location l, const char *format, ...)
{
  va_list args;

  va_start(args, format);
  if (flag_pedantic_errors)
    verror_with_location(l, format, args);
  else
    vwarning_with_location(l, format, args);
  va_end(args);
}

