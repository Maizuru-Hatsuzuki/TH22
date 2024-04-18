#ifndef __KBASE_H__
#define __KBASE_H__

#define KLDLLEXPORT

#include <time.h>
#include <stdio.h>
#include "cocos2d.h"
#include <Windows.h>

USING_NS_CC;

#ifdef KLDLLEXPORT
#define KL_DLLEXPORT __declspec(dllexport)
#else
#define KL_DLLEXPORT __declspec(dllimport)
#endif

#define THTRUE true
#define THFALSE false

#define THMAX_PLAYERFSMSTATUS 16
#define THMAX_SEQACTION 16

#define THNEW_CLASS(p) new p()
#define THDELETE(p) { if (p) { delete (p); (p) = NULL; }}
#define THFREE(p) { if (p) { free(p); (p) = NULL; }}


#define THMOVESPEED_LOW_PLAYER 5.0
#define THMOVESPEED_NORMAL_PLAYER 4.5
#define THMOVESPEED_HIGH_PLAYER 3.5
#define THMOVESPEED_HIGHEX_PLAYER 3.0

#define fn
#define var

typedef bool thBool;

#endif
