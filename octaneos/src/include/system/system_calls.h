//
// Berlin Brown
//
// bigbinc@hotmail.com
//
// $Id: system_calls.h,v 1.4 2005/05/26 00:06:45 bigbinc Exp $
//
//  externs system_calls 
//
#ifndef _SYSTEM_CALLS_H_
#define _SYSTEM_CALLS_H_

typedef int (*int_function_ptr)();

#ifdef __cplusplus
extern "C" {
#endif

  // [ these sys calls are not unix standards ]
  extern int system_debug();    //  [ 0 ] 

#ifdef __cplusplus
}
#endif

#endif // end if //
