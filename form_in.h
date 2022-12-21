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

int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, WString &str_io, void (*handlekey)( int key, WString &str_io, int &pos ) = NULL );

#endif //_FORM_IN_H_

