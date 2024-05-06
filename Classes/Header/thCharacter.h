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


enum THEM_BULLET_TYPE
{
	SHOOTCHAT_TRACKING,
	SHOOTCHAT_WINDMILL,
	SHOOTCHAT_RING,
	SHOOTCHAT_LASER
};

enum THEM_CHARARCTERLEVEL_MOVESPEED
{
	MOVESPEED_LOW,
	MOVESPEED_NARMAL,
	MOVESPEED_HIGH,
	MOVESPEED_HIGHEX
};

enum THEM_CHARACTER_TYPE
{
	CHARACTER_UNKNOW,
	CHARACTER_PLAYER,
	CHARACTER_DEFTOWER,
	CHARACTER_TOWER_WARRIOR,
	CHARACTER_SUPPORT_WARRIOR,
	CHARACTER_ENEMY,
	CHARACTER_ENEMY_DEFTOWER,
	CHARACTER_BOSS,
	CHARACTER_NPC,
	CHARACTER_BULLET,
	CHARACTER_ENEMY_BULLET
};

enum THEM_CHARACTER_LEVEL
{
	CHARACTER_LEVEL_1,
	CHARACTER_LEVEL_2,
	CHARACTER_LEVEL_3,
	CHARACTER_LEVEL_4,
	CHARACTER_LEVEL_5,
	CHARACTER_MAXLEVEL
};

struct _tAniTag
{
	int nOffset;
	struct _tTag
	{
		const char* cszpDesc;
		int nTag;
	} arrTag[THMAX_ANI_TAG];
};

struct _tCharacterAniMap
{
	const char* cszpAniStandby;
	const char* cszpAniMoveTransverse;
	const char* cszpAniMoveUp;
	const char* cszpAniMoveDown;
	const char* cszpAniMoveAttack;
	const char* cszpAniMoveSkill;
	const char* cszpAniMoveDie;
	const char* cszpAniOpenTheDoor;
	const char* cszpAniCloseTheDoor;
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
	int nHP;
	int nMP;
	int nAttack;
	int nAttackCD;
	int nDefense;
	int nCDResurrection;
	int nDuration;
	DWORD nLastestDieTime;
	enum THEM_CHARARCTERLEVEL_MOVESPEED emMoveSpeed;
	enum THEM_CHARACTER_TYPE emCharacterType;
	struct _tCharacterAniMap* ptAniMap;
};

struct _tCharacterAnimationDesc
{
	unsigned int nFrameAniCount;
	unsigned int nFrameAniBegin;
	unsigned int nFrameAniEnd;
	float fDelayPerUnit;
	int nLoops;
	bool bResFirstFrame;
	char szarrBasicFrameAniPlistPath[MAX_PATH];
	char szarrPlistPngPath[64];
	char szarrAniDesc[32];
};

struct _tCharacterFrameInfo
{
	int nHP;
	int nMP;
	int nAttack;
	int nAttackCD;
	int nDefense;
	int nCDResurrection;
	int nDuration;
	DWORD nLastestDieTime;
	enum THEM_CHARARCTERLEVEL_MOVESPEED emMoveSpeed;
	enum THEM_CHARACTER_TYPE emCharacterType;
	Sprite* pSpCharacter;
	char szarrDesc[THMAX_CHAR_DESC];
};

struct _tCharacterAnimateFrameInfo
{
	Animate* pAnimate;
	char szarrDesc[64];
};

struct _tDefTowerDesc
{
	const short csMaxWarriors;
	const short csMaxLevel;
	int nAttack;
	short sCurWarriors;
	short sCurBullets;
	short sSummonWarriorsCD;
	short sActionRadius;
	enum THEM_CHARACTER_LEVEL emCurLevel;
	enum THEM_BULLET_TYPE emBulletType;
};

typedef struct _tAniTag						CHARACTER_ANI_TAG, * CHARACTER_ANI_TAG_PTR;
typedef struct _tCharacterDesc				CHARACTER_DESC, * CHARACTER_DESC_PTR;
typedef struct _tCharacterAniMap			CHARACTER_ANI_MAP, * CHARACTER_ANI_MAP_PTR;
typedef struct _tCharacterAnimationDesc		CHARACTER_ANI_DESC, * CHARACTER_ANI_DESC_PTR;
typedef struct _tCharacterFrameInfo			CHARACTER_FRAMEINFO, * CHARACTER_FRAMEINFO_PTR;
typedef struct _tCharacterAnimateFrameInfo	CHARACTER_ANI_FRAMEINFO, * CHARACTER_ANI_FRAMEINFO_PTR;
typedef struct _tDefTowerDesc				DEFTOWER_DESC, * DEFTOWER_DESC_PTR;


class CThBaseCharacter:
	public Node
{
public:
	CThBaseCharacter();
	virtual ~CThBaseCharacter();

	thBool	initCharacter(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet, thBool bInitSp) const;
	void	uninitCharacter(CHARACTER_FRAMEINFO_PTR pCharater);
	thBool	initCharacterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet) const ;

	thBool getCharacterAnimateFrameInfo(CHARACTER_ANI_DESC_PTR pAniDesc, CHARACTER_ANI_FRAMEINFO_PTR ppRet) const;
	void getCharacterMoveSpeed(const float cfPlayerX, const float cfPlayerY, const float cfDstX, const float cfDstY, enum THEM_CHARARCTERLEVEL_MOVESPEED emSpeed, float* pfRet);
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)								= 0;
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)	= 0;

	void setPlayerRunAction(Action* pAction, Sprite* pSp);
	void setPlayerStopAllAction(Sprite* pSp);

	virtual void onMouseUp(EventMouse* pEvent)													= 0;
	virtual void onMouseDown(EventMouse* pEvent)												= 0;
	virtual void onMouseMove(EventMouse* pEvent)												= 0;

	virtual thBool globalMonitoring()															= 0;

private:

};


class CThBaseCharacterAction
{
public:
	static CThBaseCharacterAction* getInstance();
	
	thBool createActionMoveTo(float fSpeed, float fDstX, float fDstY, FiniteTimeAction** arrfnCallback, const short csCallbackSize, Sequence** ppRet) noexcept;


private:
	CThBaseCharacterAction();
	~CThBaseCharacterAction();
	CThBaseCharacterAction(const CThBaseCharacterAction& pSelf);
	const CThBaseCharacterAction& operator=(const CThBaseCharacterAction& pSelf);

private:
	static CThBaseCharacterAction* m_pSelf;
};



#endif // !__THCHARACTER_H__
