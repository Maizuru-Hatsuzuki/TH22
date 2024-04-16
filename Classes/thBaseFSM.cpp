/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm.
********************************************************/


#include "thBaseFSM.h"
#include "thBaseMacro.h"


CThFSMCtrl::CThFSMCtrl()
{
}

CThFSMCtrl::~CThFSMCtrl()
{
}


thBool CThFSMCtrl::init()
{
	thBool bRet = THFALSE;
	
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCtrl::thFsmDriver() noexcept
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCtrl::thFsmSwitch(PLAYER_FSM_DESC_PTR pFsmDesc) noexcept
{
	thBool bRet = THFALSE;
	THCHARACTERFSM_DESC_PTR pCurFsmEvent = NULL;
	THCALLBACK_CFE fnRelease = NULL;
	THCALLBACK_CFE fnInit = NULL;

	for (int i = 0; i < THMAX_PLAYERFSMSTATUS; i++)
	{
		pCurFsmEvent = pFsmDesc->arrpFsmEvent[i];
		if (NULL != pFsmDesc->arrpFsmEvent[i] && pFsmDesc->emLastPlayerStatus == pCurFsmEvent->emStatus)
		{
			fnRelease = pCurFsmEvent->fnFsmRelease;
		}
		else if (NULL != pFsmDesc->arrpFsmEvent[i] && pFsmDesc->emCurPlayerStatus == pCurFsmEvent->emStatus)
		{
			fnInit = pCurFsmEvent->fnFsmInit;
		}
	}
	
	bRet = fnRelease(pFsmDesc->vpEnv);
	TH_PROCESS_ERROR(bRet);
	bRet = fnInit(pFsmDesc->vpEnv);
	TH_PROCESS_ERROR(bRet);

	pFsmDesc->emLastPlayerStatus = pFsmDesc->emCurPlayerStatus;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCtrl::thFsmPause() noexcept
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CThFSMCtrl::thFsmMain(PLAYER_FSM_DESC_PTR pFsmDesc)
{
	thBool bFnRet = THFALSE;
	THCHARACTERFSM_DESC_PTR pCurFsmEvent = NULL;

	if (pFsmDesc->emLastPlayerStatus != pFsmDesc->emCurPlayerStatus)
	{
		/* Switch status. */
		thFsmSwitch(pFsmDesc);
	}

	for (int i = 0; i < THMAX_PLAYERFSMSTATUS; i++)
	{
		pCurFsmEvent = pFsmDesc->arrpFsmEvent[i];
		if (NULL != pFsmDesc->arrpFsmEvent[i] && pFsmDesc->emCurPlayerStatus == pCurFsmEvent->emStatus)
		{
			bFnRet = pFsmDesc->arrpFsmEvent[i]->fnFsmUpdate(pFsmDesc->vpEnv);
			ASSERT(bFnRet);
			break;
		}
	}



Exit0:
	return;
}
