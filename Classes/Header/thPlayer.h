/********************************************************
* Filename		: thPlayerAnimation.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: player module header.
********************************************************/

#ifndef __THPLAYERANIMATION_H__
#define __THPLAYERANIMATION_H__

#include "thCharacter.h"
#include "thBaseFSM.h"


struct _tPlayerDesc
{
	const char* cszpSpriteName;
	char* cszpPListName;
	const char* cszpFirstFrame;
	float fPosX;
	float fPosY;
	float fScale;
	bool bFlipX;
	bool bFlipY;
};
typedef struct _tPlayerDesc PLAYER_DESC, * PLAYER_DESC_PTR;


class CThPlayer:
	public CThBaseCharacter
{
public:
	CThPlayer();
	~CThPlayer();
	
	thBool init(PLAYER_DESC_PTR ptPlayer, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	virtual void update(float dt);
	thBool doPlayAnimationStandby(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd);
	thBool doPlayAnimationMove(char* szpFrameAni, unsigned int nAniCount, unsigned int nAniBegin, unsigned nAniEnd, THEM_CHARACTERFSM_STATUS emMoveDirection);
	void doFsmCtrlUpdate();
	
	void getPlayer(Sprite** pRet);
	void getPlayerDesc(PLAYER_DESC_PTR* pptRet);

	thBool setPlayerMoveTo(const float fX, const float fY);
	thBool setPlayerStopAllAction();
	void setPlayerFsmCurStatus(enum THEM_CHARACTERFSM_STATUS emStatus);
	
private:
	Sprite * m_pPlayer;
	Animate* m_pAniStandby;
	Animate* m_pAniMove;
	PLAYER_DESC_PTR m_ptPlayerDesc;
	PLAYER_FSM_DESC_PTR m_ptPlayerFsmDesc;
	enum THEM_CHARACTERFSM_STATUS m_emCurStatus;
	CThFSMCtrl* m_pPlayerFsmCtrl;
};


#endif
