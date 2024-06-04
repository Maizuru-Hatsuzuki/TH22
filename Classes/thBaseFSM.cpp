/********************************************************
* Filename		: thCcAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm.
********************************************************/


#include "thBaseFSM.h"
#include "thBaseMacro.h"


CThFSM::CThFSM()
{
}

CThFSM::~CThFSM()
{
}


CThFSMCharacter::CThFSMCharacter()
{
}

CThFSMCharacter::~CThFSMCharacter()
{
}

thBool CThFSMCharacter::init(THFSM_CHARACTER_DESC_PTR* parrtFsmEvent, const short csSize, void* vpEnv)
{
	thBool bRet = THFALSE;
	m_bPause = THFALSE;
	m_sArrFsmEventSize = csSize;
	m_vpEnv = vpEnv;

	memset(m_parrtFsmEvent, 0, sizeof(THFSM_CHARACTER_DESC_PTR) * THMAX_CHARACTER_FSMSTATUS);
	for (short s = 0; s < csSize; s++)
	{
		m_parrtFsmEvent[s] = THMALLOC(THFSM_CHARACTER_DESC, sizeof(THFSM_CHARACTER_DESC));
		TH_PROCESS_ERROR(m_parrtFsmEvent[s]);
		memcpy_s(m_parrtFsmEvent[s], sizeof(THFSM_CHARACTER_DESC), parrtFsmEvent[s], sizeof(THFSM_CHARACTER_DESC));
	}
	
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCharacter::uninit()
{
	thBool bRet = THFALSE;
	
	for (short s = 0; s < m_sArrFsmEventSize; s++)
	{
		THFREE(m_parrtFsmEvent[s]);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCharacter::main(enum THEM_CHARACTER_FSM_EVENT emCurEvent)
{
	thBool bRet = THFALSE;
	thBool bFnRet = THFALSE;
	THFSM_CHARACTER_DESC_PTR pCurFsmEvent = NULL;

	for (short i = 0; i < m_sArrFsmEventSize; i++)
	{
		pCurFsmEvent = m_parrtFsmEvent[i];
		if (NULL != pCurFsmEvent && pCurFsmEvent->emStatus == emCurEvent)
		{
			bFnRet = pCurFsmEvent->fnUpdate(m_vpEnv, NULL);
			ASSERT(bFnRet);
			break;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThFSMCharacter::switchEvent(enum THEM_CHARACTER_FSM_EVENT emCurEvent, enum THEM_CHARACTER_FSM_EVENT emNextEvent, void** parrArgs)
{
	thBool bRet = THFALSE;
	THFSM_CHARACTER_DESC_PTR pCurFsmEvent = NULL;
	THCALLBACK_CFE fnRelease = NULL;
	THCALLBACK_CFE fnInit = NULL;

	for (short i = 0; i < m_sArrFsmEventSize; i++)
	{
		pCurFsmEvent = m_parrtFsmEvent[i];
		if (NULL != pCurFsmEvent && pCurFsmEvent->emStatus == emCurEvent)
		{
			fnRelease = pCurFsmEvent->fnRelease;
		}
		if (NULL != pCurFsmEvent && pCurFsmEvent->emStatus == emNextEvent)
		{
			fnInit = pCurFsmEvent->fnInit;
		}
	}

	if (NULL != fnRelease && NULL != fnInit)
	{
		bRet = fnRelease(m_vpEnv, parrArgs);
		TH_PROCESS_ERROR(bRet);
		bRet = fnInit(m_vpEnv, parrArgs);
		TH_PROCESS_ERROR(bRet);
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}





