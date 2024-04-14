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

thBool CTHCharaterFSM::init(THCHARACTERFSM_DESC_PTR pFsmDesc)
{
	thBool bRet = THFALSE;
	TH_PROCESS_ERROR(pFsmDesc->fnFsmInit);
	TH_PROCESS_ERROR(pFsmDesc->fnFsmUpdate);
	TH_PROCESS_ERROR(pFsmDesc->fnFsmRelease);

	m_pFsmDesc = pFsmDesc;
	/* Init vp args. */
	m_vpCurFsmArgs = pFsmDesc->vpTagArgs;

	bRet = THTRUE;
Exit0:
	return bRet;
}

void CTHCharaterFSM::setCurFsmArgs(void* vpArgs, size_t ullSize) noexcept
{
	memcpy_s(m_vpCurFsmArgs, ullSize, vpArgs, ullSize);
	return;
}

thBool CTHCharaterFSM::fsmInit() noexcept
{
	thBool bRet = THFALSE;
	bRet = m_pFsmDesc->fnFsmInit(m_pFsmDesc->vpTagArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCharaterFSM::fsmUpdate() noexcept
{
	thBool bRet = THFALSE;
	bRet = m_pFsmDesc->fnFsmUpdate(m_pFsmDesc->vpTagArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCharaterFSM::fsmRelease() noexcept
{
	thBool bRet = THFALSE;
	bRet = m_pFsmDesc->fnFsmRelease(m_pFsmDesc->vpTagArgs);
	TH_PROCESS_ERROR(bRet);

	bRet = THTRUE;
Exit0:
	return bRet;
}
