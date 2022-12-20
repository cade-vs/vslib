/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _FORM_IN_H_
#define _FORM_IN_H_

#include "unicon.h"
#include "vstring.h"
#include "wstring.h"

extern int EditStrBF; // bakground/foreground
extern int EditStrFH; // first hit color

// allows only FI_USERS chars
int FormInput( int x, int y, const char *prompt, const char *mask, VString *strres, void (*handlekey)( int key, VString &s, int &pos ) = NULL );
int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, VString *strres, void (*handlekey)( int key, VString &s, int &pos ) = NULL );
int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, char    *strres, void (*handlekey)( int key, VString &s, int &pos ) = NULL );

int TextInputWS( int x, int y, const char *prompt, int maxlen, int fieldlen, VString *strres, void (*handlekey)( int key, VString &s, int &pos ) = NULL );
int TextInputWS( int x, int y, const char *prompt, int maxlen, int fieldlen, char    *strres, void (*handlekey)( int key, VString &s, int &pos ) = NULL );

#endif //_FORM_IN_H_

