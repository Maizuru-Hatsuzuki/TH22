/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base character fsm event header.
********************************************************/

#ifndef __THCHARATERFSM_H__
#define __THCHARATERFSM_H__


#include "thBaseMacro.h"
#include "thBaseFSM.h"
#include "thCharacter.h"


namespace TFC
{
	thBool initPlayerFSMAllEvent(PLAYER_FSM_DESC_PTR* ppRet);
	void uninitPlayerFSMAllEvent(PLAYER_FSM_DESC_PTR pRet);
	void getCharacterFsmArrayVacantPos(THCHARACTERFSM_DESC_PTR* arrpArray, short* psRet);

	thBool createPlayerFSMUnknow(THCHARACTERFSM_DESC_PTR* ppRet);
	thBool createPlayerFSMStandby(THCHARACTERFSM_DESC_PTR* ppRet);
	thBool createPlayerFSMLeftMove(THCHARACTERFSM_DESC_PTR* ppRet);
	thBool createPlayerFSMRightMove(THCHARACTERFSM_DESC_PTR* ppRet);
}





#endif