/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base character fsm event.
********************************************************/


#include "thCharacterFSM.h"
#include "thBaseMacro.h"


CTHBaseCharacterFSM::CTHBaseCharacterFSM()
{
}

CTHBaseCharacterFSM::~CTHBaseCharacterFSM()
{
}


CTHCharaterFSM::CTHCharaterFSM()
{
}

CTHCharaterFSM::~CTHCharaterFSM()
{
}

thBool CTHCharaterFSM::fsmInit(THCALLBACK_CFE fn, void* vpArgs) noexcept
{
	thBool bRet = THFALSE;
	bRet = fn(vpArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCharaterFSM::fsmUpdate(THCALLBACK_CFE fn, void* vpArgs) noexcept
{
	thBool bRet = THFALSE;
	bRet = fn(vpArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCharaterFSM::fsmRelease(THCALLBACK_CFE fn, void* vpArgs) noexcept
{
	thBool bRet = THFALSE;
	bRet = fn(vpArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}
