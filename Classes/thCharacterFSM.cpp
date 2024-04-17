/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base character fsm event.
********************************************************/


#include "thPlayerFSM.h"


namespace TFC
{
	void getCharacterFsmArrayVacantPos(THCHARACTERFSM_DESC_PTR* arrpArray, short* psRet)
	{
		*psRet = -1;
		for (int i = 0; i < THMAX_PLAYERFSMSTATUS; i++)
		{
			if (NULL == arrpArray[i])
			{
				*psRet = i;
				break;
			}
		}
		return;
	}

	thBool initPlayerFSMAllEvent(PLAYER_FSM_DESC_PTR* ppRet)
	{
		thBool bRet = THFALSE;
		thBool bFnRet = THFALSE;
		short sVacant = -1;
		PLAYER_FSM_DESC_PTR pRet = (PLAYER_FSM_DESC_PTR)malloc(sizeof(PLAYER_FSM_DESC));
		THCHARACTERFSM_DESC_PTR ptFsmEventUnknow = NULL;
		THCHARACTERFSM_DESC_PTR ptFsmEventStandby = NULL;
		THCHARACTERFSM_DESC_PTR ptFsmEventMoveLeft = NULL;

		for (int i = 0; i < THMAX_PLAYERFSMSTATUS; i++)
		{
			pRet->arrpFsmEvent[i] = NULL;
		}

		bFnRet = createPlayerFSMUnknow(&ptFsmEventUnknow);
		TH_PROCESS_ERROR(bFnRet);
		getCharacterFsmArrayVacantPos(pRet->arrpFsmEvent, &sVacant);
		pRet->arrpFsmEvent[sVacant] = ptFsmEventUnknow;

		bFnRet = createPlayerFSMStandby(&ptFsmEventStandby);
		TH_PROCESS_ERROR(bFnRet);
		getCharacterFsmArrayVacantPos(pRet->arrpFsmEvent, &sVacant);
		pRet->arrpFsmEvent[sVacant] = ptFsmEventStandby;

		bFnRet = createPlayerFSMLeftMove(&ptFsmEventMoveLeft);
		TH_PROCESS_ERROR(bFnRet);
		getCharacterFsmArrayVacantPos(pRet->arrpFsmEvent, &sVacant);
		pRet->arrpFsmEvent[sVacant] = ptFsmEventMoveLeft;

		*ppRet = pRet;
		bRet = THTRUE;
	Exit0:
		return bRet;
	}

	void uninitPlayerFSMAllEvent(PLAYER_FSM_DESC_PTR pRet)
	{
		for (int i = 0; i < THMAX_PLAYERFSMSTATUS; i++)
		{
			THDELETE(pRet->arrpFsmEvent[i]);
		}
		THDELETE(pRet);
		return;
	}

	thBool createPlayerFSMUnknow(THCHARACTERFSM_DESC_PTR* ppRet)
	{
		thBool bRet = THFALSE;
		THCHARACTERFSM_DESC_PTR ptFsmEvent = (THCHARACTERFSM_DESC_PTR)malloc(sizeof(THCHARACTERFSM_DESC));
		TH_PROCESS_ERROR(ptFsmEvent);

		ptFsmEvent->emStatus = THEM_CHARACTERFSM_STATUS::CMS_UNKNOW;
		ptFsmEvent->szpCharacterDesc = "Player fsm unknow";
		ptFsmEvent->fnFsmInit = tfpUnknowInit;
		ptFsmEvent->fnFsmUpdate = tfpUnknowUpdate;
		ptFsmEvent->fnFsmRelease = tfpUnknowRelease;

		*ppRet = ptFsmEvent;
		bRet = THTRUE;
	Exit0:
		return bRet;
	}

	thBool createPlayerFSMStandby(THCHARACTERFSM_DESC_PTR* ppRet)
	{
		thBool bRet = THFALSE;
		THCHARACTERFSM_DESC_PTR ptFsmEvent = (THCHARACTERFSM_DESC_PTR)malloc(sizeof(THCHARACTERFSM_DESC));
		TH_PROCESS_ERROR(ptFsmEvent);

		ptFsmEvent->emStatus = THEM_CHARACTERFSM_STATUS::CMS_STANDBY;
		ptFsmEvent->szpCharacterDesc = "Player fsm standby";
		ptFsmEvent->fnFsmInit = tfpStandbyInit;
		ptFsmEvent->fnFsmUpdate = tfpStandbyUpdate;
		ptFsmEvent->fnFsmRelease = tfpStandbyRelease;

		*ppRet = ptFsmEvent;
		bRet = THTRUE;
	Exit0:
		return bRet;
	}

	thBool createPlayerFSMLeftMove(THCHARACTERFSM_DESC_PTR* ppRet)
	{
		thBool bRet = THFALSE;
		THCHARACTERFSM_DESC_PTR ptFsmEvent = (THCHARACTERFSM_DESC_PTR)malloc(sizeof(THCHARACTERFSM_DESC));
		TH_PROCESS_ERROR(ptFsmEvent);

		ptFsmEvent->emStatus = THEM_CHARACTERFSM_STATUS::CMS_LEFTMOVE;
		ptFsmEvent->szpCharacterDesc = "Player fsm standby";
		ptFsmEvent->fnFsmInit = tfpMoveLeftInit;
		ptFsmEvent->fnFsmUpdate = tfpMoveLeftUpdate;
		ptFsmEvent->fnFsmRelease = tfpMoveLeftRelease;

		*ppRet = ptFsmEvent;
		bRet = THTRUE;
	Exit0:
		return bRet;
	}

	thBool createPlayerFSMRightMove(THCHARACTERFSM_DESC_PTR* ppRet)
	{
		thBool bRet = THFALSE;

		bRet = THTRUE;
	Exit0:
		return bRet;
	}
}



