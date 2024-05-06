/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm header.
********************************************************/

#ifndef __THBASEFSM_H__
#define __THBASEFSM_H__

#include "stdafx.h"
#include "thBase.h"


enum THEM_CHARACTER_FSM_EVENT
{
	FSM_EVENT_UNKNOW,
	FSM_EVENT_STAND,
	FSM_EVENT_MOVE,
	FSM_EVENT_DIE,
	FSM_EVENT_ATTACK
};

/* CFE -> character fsm event */
typedef thBool(*THCALLBACK_CFE)(
	void* vpEnv
	);

struct _tThCharacterFsmDesc
{
	enum THEM_CHARACTER_FSM_EVENT emStatus;
	char szarrDesc[THMAX_CHAR_DESC];
	THCALLBACK_CFE fnInit;
	THCALLBACK_CFE fnUpdate;
	THCALLBACK_CFE fnRelease;
};
typedef _tThCharacterFsmDesc THFSM_CHARACTER_DESC, * THFSM_CHARACTER_DESC_PTR;


class CThFSM
{
public:
	CThFSM();
	~CThFSM();

private:

};


class CThFSMCharacter:
	public CThFSM
{
public:
	CThFSMCharacter();
	~CThFSMCharacter();

	virtual thBool init(THFSM_CHARACTER_DESC_PTR* parrtFsmEvent, const short csSize);
	virtual thBool uninit();
	virtual thBool main(enum THEM_CHARACTER_FSM_EVENT emCurEvent);
	thBool switchEvent(enum THEM_CHARACTER_FSM_EVENT emCurEvent, enum THEM_CHARACTER_FSM_EVENT emNextEvent);

private:
	thBool m_bPause;
	short m_sArrFsmEventSize;
	THFSM_CHARACTER_DESC_PTR m_parrtFsmEvent[THMAX_CHARACTER_FSMSTATUS];
};



#endif // !__THBASEFSM_H__
