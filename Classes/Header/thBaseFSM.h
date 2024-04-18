/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm header.
********************************************************/

#ifndef __THBASEFSM_H__
#define __THBASEFSM_H__


#include "thBase.h"


enum THEM_CHARACTERFSM_STATUS
{
	CMS_UNKNOW = 1,
	CMS_STANDBY,
	CMS_LEFTMOVE,
	CMS_RIGHTMOVE,
	CMS_JUMP,
	CMS_FLAG
};

enum THEM_FSMTYPE
{

};

/* CFE -> character fsm event */
typedef thBool(*THCALLBACK_CFE)(
	void* vpEnv
	);

struct _tThCharacterFSMDesc
{
	enum THEM_CHARACTERFSM_STATUS emStatus;
	const char* szpCharacterDesc;
	THCALLBACK_CFE fnFsmInit;
	THCALLBACK_CFE fnFsmUpdate;
	THCALLBACK_CFE fnFsmRelease;
};
typedef struct _tThCharacterFSMDesc THCHARACTERFSM_DESC, * THCHARACTERFSM_DESC_PTR;

struct _tThPlayerFSMDesc
{
	THCHARACTERFSM_DESC_PTR arrpFsmEvent[THMAX_PLAYERFSMSTATUS];
	enum THEM_CHARACTERFSM_STATUS emCurPlayerStatus;
	enum THEM_CHARACTERFSM_STATUS emLastPlayerStatus;
	void* vpEnv;
};
typedef _tThPlayerFSMDesc PLAYER_FSM_DESC, * PLAYER_FSM_DESC_PTR;


class CThFSMCtrl:
	public cocos2d::Ref
{
public:
	CThFSMCtrl();
	~CThFSMCtrl();

	thBool init();
	thBool thFsmDriver() noexcept;
	thBool thFsmSwitch(PLAYER_FSM_DESC_PTR pFsmDesc) noexcept;
	thBool thFsmPause() noexcept;
	void thFsmMain(PLAYER_FSM_DESC_PTR pFsmDesc);

private:
	thBool m_bWorking;
	char* szpFSMDesc;
};



#endif // !__THBASEFSM_H__
