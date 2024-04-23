/********************************************************
* Filename		: thCharacter.h
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/


#ifndef __THCHARACTER_H__
#define __THCHARACTER_H__

#include "stdafx.h"
#include "thBase.h"


enum THEM_CHARARCTERLEVEL_MOVESPEED
{
	MOVESPEED_LOW,
	MOVESPEED_NARMAL,
	MOVESPEED_HIGH,
	MOVESPEED_HIGHEX
};

struct _tCharacterDesc
{
	const char* cszpSpriteName;
	const char* cszpSpriteTexPath;
	float fPosX;
	float fPosY;
	float fScale;
	bool bFlipX;
	bool bFlipY;
};

struct _tCharacterAnimationDesc
{
	unsigned int nFrameAniCount;
	unsigned int nFrameAniBegin;
	unsigned int nFrameAniEnd;
	float fDelayPerUnit;
	float fRevolve;
	int nLoops;
	bool bResFirstFrame;
	char szarrBasicFrameAniPath[MAX_PATH];
	char szarrAniDesc[32];
};

struct _tCharacterFrameInfo
{
	int nHP;
	int nMP;
	int nAttack;
	int nDefense;
	int nCDResurrection;
	int nDuration;
	DWORD nLastestDieTime;
	enum THEM_CHARARCTERLEVEL_MOVESPEED emMoveSpeed;
	Sprite* pSpCharater;
	char szarrDesc[THMAX_CHAR_DESC];
};

struct _tCharacterAnimateFrameInfo
{
	Animate* pAnimate;
	char szarrDesc[64];
};

typedef struct _tCharacterDesc				CHARACTER_DESC, * CHARACTER_DESC_PTR;
typedef struct _tCharacterAnimationDesc		CHARACTER_ANI_DESC, * CHARACTER_ANI_DESC_PTR;
typedef struct _tCharacterFrameInfo			CHARACTER_FRAMEINFO, * CHARACTER_FRAMEINFO_PTR;
typedef struct _tCharacterAnimateFrameInfo	CHARACTER_ANI_FRAMEINFO, * CHARACTER_ANI_FRAMEINFO_PTR;


class CThBaseCharacter:
	public Node
{
public:
	CThBaseCharacter();
	~CThBaseCharacter();

	virtual thBool init(CHARACTER_DESC_PTR pDesc)												= 0;
	virtual thBool initCharater(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual thBool initCharaterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet);
	virtual void uninit()																		= 0;
	virtual void uninitCharater(CHARACTER_FRAMEINFO_PTR pCharater);

	virtual void getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)							= 0;
	virtual void getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet) = 0;
	virtual thBool getCharaterAnimateFrameInfo(CHARACTER_ANI_DESC_PTR pAniDesc, CHARACTER_ANI_FRAMEINFO_PTR ppRet) const;
	virtual void setPlayerRunAction(Action* pAction)											= 0;
	virtual void setPlayerStopAllAction()														= 0;

	virtual void onMouseUp(EventMouse* pEvent)													= 0;
	virtual void onMouseDown(EventMouse* pEvent)												= 0;
	virtual void onMouseMove(EventMouse* pEvent)												= 0;

private:
	thBool initCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR pRet);
};


class CThBaseCharacterAction
{
public:
	static CThBaseCharacterAction* getInstance();
	
	thBool createActionMoveTo(float fSpeed, float fDstX, float fDstY, FiniteTimeAction** arrfnCallback, Sequence** ppRet) noexcept;


private:
	CThBaseCharacterAction();
	~CThBaseCharacterAction();
	CThBaseCharacterAction(const CThBaseCharacterAction& pSelf);
	const CThBaseCharacterAction& operator=(const CThBaseCharacterAction& pSelf);

private:
	static CThBaseCharacterAction* m_pSelf;
};



#endif // !__THCHARACTER_H__
