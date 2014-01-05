/*
 *
 *  Copyright (C) 1994 Arno Schaefer
 *
 *  AU: Prototypen und externe Variablen fuer getopt ()
 *
 *  PO: ANSI C
 *
 */

/* see getopt.h for changes */
#ifndef GETOPT_H
#define GETOPT_H

/* next line added by <cade@biscom.net> see getopt2.cpp for changes */
#define GETOPT(opts) while((optc = getopt2(argc, argv, opts)) != -1)


/* avoid C/C++ linkage declarations conflict with <getopt.h> */
extern "C" {

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
extern int optc;

/*
 * set this to `0' to avoid warning message from getopt when
 * reach unknown option
 */
extern int opterr_report;

} /* extern "C" */

/* name changed to getopt2 to avoid library function mismatch */

int getopt2(int argc, char *argv[], char *optstring);

#endif
