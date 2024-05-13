/********************************************************
* Filename		: thBase.h
* Creator		: ac
* Date time		: 2024.04.29
* Description	: all the base.
********************************************************/


#pragma warning(disable: 26408)
#pragma warning(disable: 26409)
#pragma warning(disable: 26429)
#pragma warning(disable: 26430)
#pragma warning(disable: 26433)
#pragma warning(disable: 26440)
#pragma warning(disable: 26446)
#pragma warning(disable: 26481)
#pragma warning(disable: 26482)
#pragma warning(disable: 26496)
#pragma warning(disable: 26812)


#ifndef __KBASE_H__
#define __KBASE_H__


#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <CppCoreCheck/Warnings.h>
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)


#define THDLLEXPORT
#ifdef THDLLEXPORT
#define TH_DLLEXPORT __declspec(dllexport)
#else
#define TH_DLLEXPORT __declspec(dllimport)
#endif

/* CC use. */
#define THMAX_CHARACTER_FSMSTATUS			16
#define THMAX_SEQACTION						16
#define THMAX_ANI_COUNT						32
#define THMAX_ANI_TAG						16
#define THMAX_SP_COUNT						64
#define THMAX_CHAR_DESC						128
#define THMAX_DEFTOWER_TARLEVEL_WARRIORS	4
#define THMAX_DEFTOWER_SYNC_ANI				32

#define THMOVESPEED_LOW_PLAYER 5.0
#define THMOVESPEED_NORMAL_PLAYER 4.5
#define THMOVESPEED_HIGH_PLAYER 3.5
#define THMOVESPEED_HIGHEX_PLAYER 3.0

#define THSP_FLAG_DIE 0
#define THSP_FLAG_CLEAN -1

#define THINI_DEFAULT_STR "NA"

#define THMALLOC(t, s) (t*)malloc(s)
#define THFREE(p) { if (p) { free(p); (p) = NULL; }}

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

typedef thBool* pthBool;


#endif
