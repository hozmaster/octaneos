//
// Copyright (C) 2003, 2007 Berlin Brown
//
// $Id: system_calls.h,v 1.4 2005/05/26 00:06:45 bigbinc Exp $
//
// Externs system_calls 
//
#ifndef _SYSTEM_CALLS_H_
#define _SYSTEM_CALLS_H_

typedef int (*int_function_ptr)();

#ifdef __cplusplus
extern "C" {
#endif

extern int system_debug();

#ifdef __cplusplus
}
#endif

#endif // end if //
