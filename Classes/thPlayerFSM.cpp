/********************************************************
* Filename		: thPlayerFSM.cpp
* Creator		: ac
* Date time		: 2024.04.16
* Description	: player fsm event.
********************************************************/


#include "thPlayerFSM.h"


thBool tfpUnknowInit(void* vpEnv)
{
	thBool bRet = THFALSE;
	CCLOG("INIT UN");
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool tfpUnknowUpdate(void* vpEnv)
{
	thBool bRet = THFALSE;
	CCLOG("UP UN");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool tfpUnknowRelease(void* vpEnv)
{
	thBool bRet = THFALSE;
	CCLOG("REL UN");
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool tfpStandbyInit(void* vpEnv)
{
	thBool bRet = THFALSE;
	PLAYER_DESC_PTR pPlayerDesc = NULL;
	CThPlayer* pEnv = static_cast<CThPlayer*>(vpEnv);
	TH_PROCESS_ERROR(pEnv);
	
	pEnv->getPlayerDesc(&pPlayerDesc);
	pEnv->doPlayAnimationStandby(pPlayerDesc->cszpPListName, 4, 1, 4);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool tfpStandbyUpdate(void* vpEnv)
{
	thBool bRet = THFALSE;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool tfpStandbyRelease(void* vpEnv)
{
	thBool bRet = THFALSE;
	CThPlayer* pEnv = static_cast<CThPlayer*>(vpEnv);
	TH_PROCESS_ERROR(pEnv);

	bRet = pEnv->setPlayerStopAllAction();
	TH_PROCESS_ERROR(bRet);
	bRet = THTRUE;
Exit0:
	return bRet;
}

