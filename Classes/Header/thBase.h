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

#define THNEW_CLASS(c) new c()
#define THDEL_CLASS(c) delete c

#define fn
#define var

typedef bool thBool;

#endif
