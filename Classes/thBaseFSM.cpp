/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm.
********************************************************/


#include "thBaseFSM.h"

CTHBaseFSM::CTHBaseFSM()
{
}

CTHBaseFSM::~CTHBaseFSM()
{
}


thBool CTHBaseFSM::init()
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHBaseFSM::thFsmDriver() noexcept
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHBaseFSM::thFsmSwitch() noexcept
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHBaseFSM::thFsmPause() noexcept
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}
