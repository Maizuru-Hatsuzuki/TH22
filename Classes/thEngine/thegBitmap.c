/********************************************************
* Filename		: thSceneDungeon.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: th22 engine container.
********************************************************/

#include "pch.h"
#include "thegContainer.h"
#include "thBaseMacro.h"


TH_DLLEXPORT thBool TecInitBitmap(unsigned short usMaxBit, THEC_BITMAP_PTR* ppRet)
{
	thBool bRet = THFALSE;
	THEC_BITMAP_PTR ptBitMap = THMALLOC(THEC_BITMAP, sizeof(THEC_BITMAP));
	TH_PROCESS_ERROR(ptBitMap);
	ptBitMap->unArrCount = usMaxBit / 32 + 1;
	ptBitMap->unSize = ptBitMap->unArrCount * 32 - 1;
	ptBitMap->arrBitmap = THMALLOC(thBitmap, sizeof(thBitmap) * ptBitMap->unArrCount);
	TH_PROCESS_ERROR(ptBitMap->arrBitmap);

	memset(ptBitMap->arrBitmap, 0, sizeof(thBitmap) * ptBitMap->unArrCount);

	*ppRet = ptBitMap;
	bRet = THTRUE;
Exit0:
	return bRet;
}

TH_DLLEXPORT void TecUninitBitmap(THEC_BITMAP_PTR pBitmap)
{
	for (unsigned int i = 0; i < pBitmap->unArrCount; i++)
	{
		THFREE(pBitmap->arrBitmap[i]);
	}
	THFREE(pBitmap);
	return;
}

THDLLEXPORT thBool TecGetBitmapVal(const unsigned int cunIdx, THEC_BITMAP_PTR pBitmap, thBool* pbRet)
{
	thBool bRet = THFALSE;
	unsigned int unIdx = 0;
	unsigned int unArrPos = 0;
	unsigned int unBitmapPos = 0;

	TH_PROCESS_ERROR(cunIdx <= pBitmap->unSize);
	/* 1. Get idx pos of the array. */
	unArrPos = cunIdx / pBitmap->unSize;
	/* 2. Get idx pos of the bitmap. */
	unBitmapPos = cunIdx % 32;

	*pbRet = (pBitmap->arrBitmap[unArrPos] & (0x1 << unBitmapPos)) != 0 ? THTRUE : THFALSE;
Exit0:
	return bRet;
}

TH_DLLEXPORT thBool TecSetBitmapVal(const unsigned int cunIdx, thBool bVal, THEC_BITMAP_PTR pBitmap)
{
	thBool bRet = THFALSE;
	unsigned int unIdx = 0;
	unsigned int unArrPos = 0;
	unsigned int unBitmapPos = 0;

	TH_PROCESS_ERROR(cunIdx <= pBitmap->unSize);
	/* 1. Get idx pos of the array. */
	unArrPos = cunIdx / pBitmap->unSize;
	/* 2. Get idx pos of the bitmap. */
	unBitmapPos = cunIdx % 32;

	if (THTRUE == bVal)
	{
		pBitmap->arrBitmap[unArrPos] |= (0x1 << unBitmapPos);
	}
	else
	{
		pBitmap->arrBitmap[unArrPos] &= ~(0x1 << unBitmapPos);
	}

Exit0:
	return bRet;
}

