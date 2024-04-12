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

class CThPlayer
{
public fn:
	CThPlayer();
	~CThPlayer();
	
	thBool init(PLAYER_DESC_PTR ptPlayer, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	thBool doPlayAnimationStandby(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	//thBool doPlayAnimationMove();
	
	void getPlayer(Sprite** pRet);
	

private var:
	Sprite * m_pPlayer;
	Animate* m_pAniStandby;
	Animate* m_pAniMove;
	CTHCharaterFSM* arrpPlayerStatus[THMAX_PLAYERFSMSTATUS];
};


#endif
