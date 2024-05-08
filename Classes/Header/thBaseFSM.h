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


#define CHACFSM_SWITCH_STAND(p, fo) \
do \
{	\
	p->fo->switchEvent(p->m_emCurFsmStatus, THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND, NULL); \
	p->setCurFsmStatus(THEM_CHARACTER_FSM_EVENT::FSM_EVENT_STAND); \
} while (0);

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
	/* vpEnv: 类指针. 一般传 this. */
	/* parrArgs: 函数参数，2 个参数; 0: CC Sequence action; 1: CHARACTER_ANI_FRAMEINFO_PTR  */
	void* vpEnv,
	void** parrArgs
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

	virtual thBool init(THFSM_CHARACTER_DESC_PTR* parrtFsmEvent, const short csSize, void* vpEnv);
	virtual thBool uninit();
	virtual thBool main(enum THEM_CHARACTER_FSM_EVENT emCurEvent);
	thBool switchEvent(enum THEM_CHARACTER_FSM_EVENT emCurEvent, enum THEM_CHARACTER_FSM_EVENT emNextEvent, void** parrArgs);

private:
	thBool m_bPause;
	short m_sArrFsmEventSize;
	void* m_vpEnv;
	THFSM_CHARACTER_DESC_PTR m_parrtFsmEvent[THMAX_CHARACTER_FSMSTATUS];
};



#endif // !__THBASEFSM_H__
