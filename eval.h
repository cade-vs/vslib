/****************************************************************************
 *
 * Copyright (c) 1996-2020 Vladi Belperchinov-Shabanski "Cade" 
 * http://cade.datamax.bg/  <cade@biscom.net> <cade@bis.bg> <cade@datamax.bg>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _EVAL_H_
#define _EVAL_H_

//
// this evaluates math expression with recursive parser etc.
// (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1996-2020
//

extern int EvalResult;
double Eval( const char* pExp );

#endif //_EVAL_H_
