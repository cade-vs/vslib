#ifndef REGEXP_H
#define REGEXP_H
/*
 * slightly modified to become .cpp and be used as part of vslib
 * modifications by (c) Vladi Belperchinov-Shabanski "Cade" 2002
 * <cade@biscom.net> [http://soul.datamax.bg/~cade]
 * $Id: regexp3.h,v 1.2 2002/11/27 21:41:52 cade Exp $
 */

/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#define NSUBEXP  16
typedef struct regexp {
	const char *startp[NSUBEXP];
	const char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

#define REGERROR_STR_SIZE 128
extern char regerror_str[REGERROR_STR_SIZE];

regexp * regcomp( const char *exp );
int regexec( regexp *prog, const char *string);

/* return n-th subpattern matched */
char* regsubn   (regexp *re, int n, char *dest);

/* regmatch compiles and exec patter against given string */
int regmatch( const char* pattern, const char* string );

/* regmatchpos compiles and exec patter against given string */
/* return value is the position of found pattern or -1 if not found */
int regmatchpos( const char* pattern, const char* string );

#endif //REGEXP_H
