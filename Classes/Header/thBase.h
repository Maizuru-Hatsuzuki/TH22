#ifndef __KBASE_H__
#define __KBASE_H__


#include <time.h>
#include <stdio.h>
#include <Windows.h>



#define THDLLEXPORT
#ifdef THDLLEXPORT
#define TH_DLLEXPORT __declspec(dllexport)
#else
#define TH_DLLEXPORT __declspec(dllimport)
#endif

#define THMAX_PLAYERFSMSTATUS 16
#define THMAX_SEQACTION 16

#define THMALLOC(t, s) (t*)malloc(s)
#define THFREE(p) { if (p) { free(p); (p) = NULL; }}


#define THMOVESPEED_LOW_PLAYER 5.0
#define THMOVESPEED_NORMAL_PLAYER 4.5
#define THMOVESPEED_HIGH_PLAYER 3.5
#define THMOVESPEED_HIGHEX_PLAYER 3.0

#define fn
#define var


#ifdef __cplusplus

typedef bool thBool;
#define THTRUE true
#define THFALSE false
#define THNEW_CLASS(p) new p()
#define THDELETE(p) { if (p) { delete (p); (p) = NULL; }}

#else

typedef int thBool;
#define THTRUE 1
#define THFALSE 0

#endif	



#endif
