/********************************************************
* Filename		: thCcCharacter.h
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/


#ifndef __thCcCharacter_H__
#define __thCcCharacter_H__

#include "stdafx.h"
#include "thBase.h"


enum THEM_QM_MOUSECURSOR
{
	CUR_UNKNOW,
	CUR_SELECTING_POSITION,
	CUR_SELECTED_POSITION,
	CUR_SELECTERR_POSITION,
};

enum THEM_BULLET
{
	BULLET_BUTTERFLY,
	BULLET_SWORD
};

enum THEM_BULLET_TYPE
{
	SHOOTCHAT_TRACKING,
	SHOOTCHAT_WINDMILL,
	SHOOTCHAT_RING,
	SHOOTCHAT_LASER
};

enum THEM_DEFTOWER_TYPE
{
	DEFTOWERTYPE_UNKNOW,
	DEFTOWERTYPE_ARCHER,
	DEFTOWERTYPE_WARRIORS,
	DEFTOWERTYPE_ARCHER_WARRIORS,
};

enum THEM_CHARARCTERLEVEL_MOVESPEED
{
	MOVESPEED_LOWEX,
	MOVESPEED_LOW,
	MOVESPEED_NARMAL,
	MOVESPEED_HIGH,
	MOVESPEED_HIGHEX,
	MOVESPEED_NOTMOVE
};

enum THEM_CHARACTER_TYPE
{
	CHARACTER_UNKNOW,
	CHARACTER_PLAYER,
	CHARACTER_DEFTOWER,
	CHARACTER_DEFTOWER_SUBSOIL,
	CHARACTER_DEFTOWER_WARRIOR,
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
	CHARACTER_LEVEL_1 = 1,
	CHARACTER_LEVEL_2,
	CHARACTER_LEVEL_3,
	CHARACTER_LEVEL_4,
	CHARACTER_LEVEL_5,
	CHARACTER_MAXLEVEL
};

struct _tAniTag
{
	short sOffset;
	struct _tTag
	{
		const char* cszpDesc;
		short sTag;
	} arrTag[THMAX_ANI_TAG];
};

struct _tCharacterAniMap
{
	char szarrAniStandby[MAX_PATH];
	char szarrAniMoveTransverse[MAX_PATH];
	char szarrAniMoveUp[MAX_PATH];
	char szarrAniMoveDown[MAX_PATH];
	char szarrAniAttack[MAX_PATH];
	char szarrAniSkill[MAX_PATH];
	char szarrAniDie[MAX_PATH];
	char szarrAniOpenTheDoor[MAX_PATH];
	char szarrAniCloseTheDoor[MAX_PATH];
};

struct _tCharacterDesc
{
	char szarrSpriteName[64];
	char szarrSpriteTex[64];
	char szarrDefaultTexPlistPos[64];
	float fPosX;
	float fPosY;
	float fScale;
	bool bFlipX;
	bool bFlipY;
	int nDefaultTexPlistPos;
	int nHP;
	int nMP;
	int nAttack;
	int nAttackCD;
	int nAttackRadius;
	int nDefense;
	int nCDResurrection;
	int nSupportDuration;
	enum THEM_CHARARCTERLEVEL_MOVESPEED emMoveSpeed;
	enum THEM_CHARACTER_TYPE emCharacterType;
	enum THEM_CHARACTER_LEVEL emCurLevel;
	enum THEM_CHARACTER_LEVEL emMaxLevel;
	struct _tCharacterAniMap* ptAniMap;
};

struct _tCharacterAnimationDesc
{
	int nFrameAniCount;
	unsigned int nFrameAniBegin;
	unsigned int nFrameAniEnd;
	float fDelayPerUnit;
	int nLoops;
	bool bResFirstFrame;
	char szarrSpriteTex[64];
	char szarrAniDesc[32];
};

struct _tCharacterFrameInfo
{
	int nHP;
	int nMP;
	int nAttack;
	int nAttackCD;
	int nAttackRadius;
	int nDefense;
	int nCDResurrection;
	int nSupportDuration;
	int nLastestDieTime;
	enum THEM_CHARARCTERLEVEL_MOVESPEED emMoveSpeed;
	enum THEM_CHARACTER_TYPE emCharacterType;
	enum THEM_CHARACTER_LEVEL emMaxLevel;
	enum THEM_CHARACTER_LEVEL emCurLevel;
	Sprite* pSpCharacter;
	char szarrDesc[THMAX_CHAR_DESC];
};

struct _tCharacterAnimateFrameInfo
{
	Animate* pAnimate;
	char szarrDesc[64];
};

struct _tSubsoilDesc
{
	int nDefaultTexPlistPos;
	int nHoverTexPlistPos;
	int nActiveDefaultTexPlistPos;
	int nActiveHoverTexPlistPos;
	float fFacingEnemyAngle;
	char szarrActiveTex[64];
};

struct _tDefTowerDesc
{
	short sMaxWarriors;
	short sCurWarriors;
	short sSummonWarriorsCD;
	short sMaxBullets;
	int nDefTowerProfessional;
	enum THEM_BULLET_TYPE emBulletType;
};

struct _tTowerWarriors
{
	struct _tCharacterDesc** arrpTowerWarriorsDesc;
	enum THEM_CHARACTER_LEVEL emLevel;
	short sSize;
};

struct _tDefTowerQuickMenu
{
	struct _tCharacterFrameInfo* pBg;
	struct _tCharacterFrameInfo* pCommandMovement;
	struct _tCharacterFrameInfo* pSellTower;

};

typedef struct _tAniTag						CHARACTER_ANI_TAG, * CHARACTER_ANI_TAG_PTR;
typedef struct _tCharacterDesc				CHARACTER_DESC, * CHARACTER_DESC_PTR;
typedef struct _tCharacterAniMap			CHARACTER_ANI_MAP, * CHARACTER_ANI_MAP_PTR;
typedef struct _tCharacterAnimationDesc		CHARACTER_ANI_DESC, * CHARACTER_ANI_DESC_PTR;
typedef struct _tCharacterFrameInfo			CHARACTER_FRAMEINFO, * CHARACTER_FRAMEINFO_PTR;
typedef struct _tCharacterAnimateFrameInfo	CHARACTER_ANI_FRAMEINFO, * CHARACTER_ANI_FRAMEINFO_PTR;
typedef struct _tSubsoilDesc				SUBSOIL_DESC, * SUBSOIL_DESC_PTR;
typedef struct _tDefTowerDesc				DEFTOWER_DESC, * DEFTOWER_DESC_PTR;
typedef struct _tTowerWarriors				DEFTOWER_WARRIORS, * DEFTOWER_WARRIORS_PTR;
typedef struct _tDefTowerQuickMenu			DEFTOWER_QUICKMENU, * DEFTOWER_QUICKMENU_PTR;


class CThBaseCharacter:
	public Node
{
public:
	CThBaseCharacter();
	virtual ~CThBaseCharacter();

	thBool	initCharacter(std::string strSpFile, CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet, thBool bInitSp);
	thBool	initCharacterWithPlist(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR* ppRet);
	thBool	initCharacterWithPlistSimple(const char* cszpDesc, const char* cszpSpTex, const int cnPos, CHARACTER_FRAMEINFO_PTR* ppRet);
	void	uninitCharacter(CHARACTER_FRAMEINFO_PTR pCharater);
	thBool	initCharacterAnimation(CHARACTER_ANI_DESC_PTR pAniDesc, Animate** ppRet) const ;

	thBool getCharacterAnimateFrameInfo(CHARACTER_ANI_DESC_PTR pAniDesc, CHARACTER_ANI_FRAMEINFO_PTR ppRet) const;
	void getCharacterMoveSpeed(const float cfPlayerX, const float cfPlayerY, const float cfDstX, const float cfDstY, enum THEM_CHARARCTERLEVEL_MOVESPEED emSpeed, float* pfRet);
	void getBulletInfo(enum THEM_BULLET emBullet, char* szpTexPathRet, char* szpBulletIniPathRet);
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet)								= 0;
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet)	= 0;
	static thBool getIsHoverSprite(Sprite* pSpTag, Vec2 vecPosInView, const thBool cbIsArChange);

	void setPlayerRunAction(Action* pAction, Sprite* pSp);
	static void setPlayerStopAllAction(Sprite* pSp);
	
	virtual void onMouseUp(EventMouse* pEvent)													= 0;
	virtual void onMouseDown(EventMouse* pEvent)												= 0;
	virtual void onMouseMove(EventMouse* pEvent)												= 0;

	virtual thBool globalMonitoring()															= 0;
	virtual thBool delayUninitMonitoring()														= 0;

private:
	void _initCharacterDescInfo(CHARACTER_DESC_PTR pDesc, CHARACTER_FRAMEINFO_PTR ptCharFrame);
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


class CthCcCharacterLoadHandler
{
public:
	static CthCcCharacterLoadHandler* getInstance();

	thBool getCharaterDescFromIni(const char* cszpFilename, CHARACTER_DESC_PTR* ppRet);
	thBool getCharacterAniDescFromIni(const char* cszpFilename, CHARACTER_ANI_DESC_PTR* ppRet);
	thBool getDefTowerDescFromIni(const char* cszpFilename, DEFTOWER_DESC_PTR* ppRet);
	thBool getSubsoilFromIni(const char* cszpFilename, SUBSOIL_DESC_PTR* ppRet);

	void uninitCharacterDesc(CHARACTER_DESC_PTR p);
	void uninitCharacterAniDesc(CHARACTER_ANI_DESC_PTR P);
	void uninitDefTowerDesc(DEFTOWER_DESC_PTR p);
	void uninitSubsoilDesc(SUBSOIL_DESC_PTR p);

private:
	CthCcCharacterLoadHandler();
	~CthCcCharacterLoadHandler();
	CthCcCharacterLoadHandler(const CthCcCharacterLoadHandler& pSelf);
	const CthCcCharacterLoadHandler& operator=(const CthCcCharacterLoadHandler& pSelf);

private:
	static CthCcCharacterLoadHandler* m_pSelf;
};



#endif // !__thCcCharacter_H__
