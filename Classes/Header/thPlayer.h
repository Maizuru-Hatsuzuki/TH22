/********************************************************
* Filename		: thPlayerAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: player module header.
********************************************************/

#ifndef __THPLAYERANIMATION_H__
#define __THPLAYERANIMATION_H__


#include "thBase.h"
#include "thCharacterFSM.h"




struct _tPlayerDesc
{
	const char* cszpSpriteName;
	char* cszpPListName;
	const char* cszpFirstFrame;
	float fPosX;
	float fPosY;
	float fScale;
};
typedef struct _tPlayerDesc PLAYER_DESC, * PLAYER_DESC_PTR;




class CThPlayer:
	public CTHBaseCharacter
{
public fn:
	CThPlayer();
	~CThPlayer();
	
	thBool init(PLAYER_DESC_PTR ptPlayer, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	thBool initPlayerFsm();
	thBool doPlayAnimationStandby(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	thBool doPlayAnimationMove(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	
	void getPlayer(Sprite** pRet);
	void getPlayerDesc(PLAYER_DESC_PTR* pptRet);
	
private fn:
	/* Standby */
	static thBool _playerFsmStandbyInit(void* vpArgs);
	static thBool _playerFsmStandbyUpdate(void* vpArgs);
	static thBool _playerFsmStandbyRelease(void* vpArgs);
	/* Move */
	static thBool _playerFsmMoveInit(void* vpArgs);
	static thBool _playerFsmMoveUpdate(void* vpArgs);
	static thBool _playerFsmMoveRelease(void* vpArgs);

private var:
	Sprite * m_pPlayer;
	Animate* m_pAniStandby;
	Animate* m_pAniMove;
	CTHCharaterFSM* m_arrpPlayerFsm[THMAX_PLAYERFSMSTATUS];
	PLAYER_DESC_PTR m_ptPlayerDesc;
};


struct _tPlayerFsmArgs
{
	CThPlayer* pEnv;
	THEM_CHARACTERFSM_STATUS emStatus;
};
typedef struct _tPlayerFsmArgs PLAYER_FSM_ARGS, * PLAYER_FSM_ARGS_PTR;

#endif
