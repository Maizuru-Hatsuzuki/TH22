/********************************************************
* Filename		: thSceneDungeon.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: th22 engine container header.
********************************************************/

#ifndef __THEGCONTAINER_H__
#define __THEGCONTAINER_H__

#include "thBase.h"
#define THEG_TEST 1

typedef unsigned int thBitmap;
typedef thBitmap* thBitmap_ptr;
struct _tBitmap
{
	thBitmap* arrBitmap;
	unsigned int unArrCount;
	unsigned int unSize;
};
typedef struct _tBitmap THEC_BITMAP, * THEC_BITMAP_PTR;

#ifdef __cplusplus
extern "C" {
#endif	
	/* Bitmap */
	TH_DLLEXPORT thBool TecInitBitmap(unsigned short usMaxBit, THEC_BITMAP_PTR* ppRet);
	TH_DLLEXPORT void TecUninitBitmap(THEC_BITMAP_PTR pBitmap);
	TH_DLLEXPORT thBool TecGetBitmapVal(const unsigned int cunIdx, THEC_BITMAP_PTR pBitmap, thBool* pbRet);
	TH_DLLEXPORT thBool TecSetBitmapVal(const unsigned int cunIdx, thBool bVal, THEC_BITMAP_PTR pBitmap);

#ifdef __cplusplus
}
#endif
#endif // !__THEGCONTAINER_H__
