/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: target.h,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#ifndef _TARGET_H_
#define _TARGET_H_

#define _TARGET_UNKNOWN_

// ######################################### define target OS

#ifdef _TARGET_UNKNOWN_
  #if defined(DJGPP)
  #define _TARGET_GO32_
  #define _TARGET_DJGPP_
  #define _TARGET_DESCRIPTION_	"DOS/DJGPP"
  #undef _TARGET_UNKNOWN_
  #endif
#endif

#ifdef _TARGET_UNKNOWN_
  #if defined(__linux__) || defined(__Linux__)
  #define _TARGET_LINUX_
  #define _TARGET_DESCRIPTION_	"UNIX/LINUX"
  #undef _TARGET_UNKNOWN_
  #endif
#endif

#ifdef _TARGET_UNKNOWN_
  #if defined(__NetBSD__) 
  #define _TARGET_NETBSD_
  #define _TARGET_DESCRIPTION_	"UNIX/NETBSD"
  #undef _TARGET_UNKNOWN_
  #endif
#endif

#ifndef _TARGET_DESCRIPTION_
  #define _TARGET_DESCRIPTION_	"UNKNOWN/UNKNOWN"
#endif

// ######################################### define target DOS platforms

#if defined(_TARGET_DJGPP_)
  #define _TARGET_DOS_
  #undef _TARGET_UNKNOWN_
#endif

// ######################################### define target UNIX platforms

#if defined(_TARGET_LINUX_) || defined(_TARGET_NETBSD_)
  #define _TARGET_UNIX_
  #undef _TARGET_UNKNOWN_
#endif

#ifdef _TARGET_UNKNOWN_
  #error "Unknown target please define one manually!"
  #error "Supported are _TARGET_LINUX_, _TARGET_DJGPP_"
  #error "Read README or COMPILE file(s) for details"
#endif

#endif //_TARGET_H_
