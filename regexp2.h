/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 *
 * *** NOTE: see regexp2.cpp for further credits and notes!
 *
 * $Id: regexp2.h,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#ifndef _REGEXP
#define _REGEXP 1

#define NSUBEXP  16
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

regexp *regcomp (const char *exp);
int regexec     (regexp *prog, const char *string);
void regsub     (regexp *prog, const char *source, char *dest);
void regerror   (char *msg);

char* regsubn   (regexp *re, int n, char *dest);

/* regmatch compiles and exec patter against given string */
int regmatch( const char* pattern, const char* string );

/* regmatchpos compiles and exec patter against given string */
/* return value is the position of found pattern or -1 if not found */
int regmatchpos( const char* pattern, const char* string );

#endif /* REGEXP */
