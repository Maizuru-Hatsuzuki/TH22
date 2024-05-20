/********************************************************
* Filename		: thCcDefTower.h
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower header.
********************************************************/


#ifndef __THDEFTOWER_H__
#define __THDEFTOWER_H__

#include "thCcPlayer.h"
#include "thBase.h"


class CThDefTowerWarrior :
	public CThBaseCharacter
{
public:
	CThDefTowerWarrior();
	~CThDefTowerWarrior();

	thBool init(
		const CHARACTER_DESC_PTR cptSpDesc,
		const short csSpArrVacantPos,
		const float cfWarriorsBirthAngle,
		const float cfWarriorsBirthX,
		const float cfWarriorsBirthY,
		const short csActionRadius,
		const CHARACTER_ANI_MAP_PTR cptAniMap,
		const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo
	);
	thBool initWarriors(const CHARACTER_DESC_PTR cptSpDesc, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);
	thBool initFsmEvent();
	void uninit();

	const float getWarriorBirthMoveAngle() const;
	const float getWarriorInPositionTime() const;
	void getWarriorAniMap(CHARACTER_ANI_MAP_PTR* ppRet) const;
	void getWarriorFsmStatus(enum THEM_CHARACTER_FSM_EVENT* pemRet) const;
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	void setCurFsmStatus(enum THEM_CHARACTER_FSM_EVENT emStatus);
	void setWarriorBirthMoveAngle(const float fAngle);

	/* us 前缀, 用户接口, 能够直接切换状态机状态, 暴露给其他逻辑用接口. */
	thBool usSetWarriorMove(const float cfDstX, const float cfDstY, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);
	thBool usSetWarriorDie(const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);
	virtual void update(float dt);

	virtual thBool globalMonitoring();

	static thBool fsmEventInitStand(void* vpEnv, void** parrArgs);
	static thBool fsmEventUpdateStand(void* vpEnv, void** parrArgs);
	static thBool fsmEventReleaseStand(void* vpEnv, void** parrArgs);
	static thBool fsmEventInitMove(void* vpEnv, void** parrArgs);
	static thBool fsmEventUpdateMove(void* vpEnv, void** parrArgs);
	static thBool fsmEventReleaseMove(void* vpEnv, void** parrArgs);
	static thBool fsmEventInitDie(void* vpEnv, void** parrArgs);
	static thBool fsmEventUpdateDie(void* vpEnv, void** parrArgs);
	static thBool fsmEventReleaseDie(void* vpEnv, void** parrArgs);

private:
	float m_fWarriorBirthMoveAngle;
	float m_fWarriorBirthX;
	float m_fWarriorBirthY;
	double m_dInPositionTime;
	short m_sActionRadius;
	short m_csSpArrVacantPos;
	enum THEM_CHARACTER_FSM_EVENT m_emCurFsmStatus;
	thBool m_bIsFlip;
	CHARACTER_FRAMEINFO_PTR m_ptWarriorFrameInfo;
	CHARACTER_ANI_MAP_PTR m_ptAniMap;
	CThFSMCharacter* m_fsmWarriorObject;
};

typedef CThDefTowerWarrior* CThDefTowerWarrior_ptr;


class CThDefTower:
	public CThBaseCharacter
{
public:
	CThDefTower();
	~CThDefTower();

	thBool init(
		const char* cszpDefTowerCharacterDescPath,
		enum THEM_BULLET emBullet,
		char** szarrpAniDesc,
		const short csAniDescSize,
		const DEFTOWER_WARRIORS_PTR ptWarriors,
		float fFaceAngle
	);
	thBool initCharacterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos);
	thBool initBaiscAnimate(char** szarrpAniDesc, const short csSize);
	thBool initWarriors(const short csCnt, short sSpArrVacantPos);
	thBool initDefTowerWarriorsDesc(const DEFTOWER_WARRIORS_PTR ptWarriors);
	thBool initBullet(float fShootAngle);
	virtual void uninit();
	void uninitDefTowerWarriorsDesc();
	void uninitBullet(Node* pNode, const short csBullet);

	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);
	void getAniTagByDesc(const char* cszpDesc, int* pnRet);
	void getAniFrameInfoByTag(const char* cszpTag, CHARACTER_ANI_FRAMEINFO_PTR* ppRet);
	void getWarriorExistsByAngle(const float cfAngle, thBool* pbRet);

	void setWarriorExistsByAngle(const float cfAngle, const short csTag);
	thBool setPlayAniTowerSummon(const short* arrnCondAniTag, const short cnSize, const thBool bIsSummoning);

	thBool execTowerShoot(const short csBulletCnt);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual void update(float dt);
	virtual thBool globalMonitoring();
	thBool globalMonitoringWarriors();

public:
	/* static fn. get init defTower info. */
	static void getTowerInfoArcher(
		enum THEM_CHARACTER_LEVEL emLevel, char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
	);
	static void getTowerInfoArcherWarriors(
		enum THEM_CHARACTER_LEVEL emLevel, char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
	);

private:
	thBool _getSpArrayVacantPos(short* psRet);
	thBool _getWarArrayVacantPos(short* psRet);
	thBool _setPlayAniOpenTheDoor();
	thBool _setPlayAniCloseTheDoor();
	thBool _setPlayAniWarriorsDie(CThDefTowerWarrior_ptr pSp);

	thBool _monitoringWarriorsHealthy(CThDefTowerWarrior_ptr pSp, pthBool pbIsRelease);

private:
	short							m_sVacantPos;
	static float					ms_fWarriorsBirthAngle;
	double							m_dLastSummonWarriors;
	double							m_dLastDieWarriors;
	double							m_dLastAttack;
	CHARACTER_ANI_TAG				m_tAniTag;
	CHARACTER_FRAMEINFO_PTR			m_ptTower;
	DEFTOWER_DESC_PTR				m_ptTowerStatus;
	enum THEM_DEFTOWER_TYPE			m_emTowerType;
	CHARACTER_FRAMEINFO_PTR*		m_arrpSpGroup;
	CHARACTER_ANI_FRAMEINFO_PTR*	m_arrpAniGroup;
	/* 防御塔战士精灵描述，每个等级 4 种类型. */
	CHARACTER_DESC_PTR				m_arrpWarriorsDesc[THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL][THMAX_DEFTOWER_TARLEVEL_WARRIORS];
	CThDefTowerWarrior_ptr*			m_arrpWarriors;
	/* 子弹. */
	SpriteBatchNode*				m_pBatchNodeBullet;
	CHARACTER_DESC_PTR				m_ptBulletDesc;
};

typedef CThDefTower* CThDefTower_ptr;


class CThDefTowerSubsoil :
	public CThBaseCharacter
{
public:
	CThDefTowerSubsoil();
	~CThDefTowerSubsoil();

	thBool init(char* szpSubsoilCharacterDescPath, const float cfFacingEnemyAngle);
	thBool initDefTowerConstructionLoadingBar();
	thBool initDefTowerConstruction();
	void uninitDefTowerConstruction();
	thBool initDefTower();
	void uninit();
	void uninitDefTower();
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	void setDefTowerProfessionalType(enum THEM_DEFTOWER_TYPE emType);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual thBool globalMonitoring();
	virtual void update(float dt);

private:
	float m_arrfDefTowerWarriorPos[THMAX_DEFTOWER_TARLEVEL_WARRIORS][2];
	CHARACTER_FRAMEINFO_PTR m_ptSubsoil;
	CHARACTER_FRAMEINFO_PTR m_ptConstruction;
	SUBSOIL_DESC_PTR m_ptSubSoilStatus;
	CThDefTower_ptr m_pDefTower;
	SpriteFrame* m_pDefaultSubsoil;
	SpriteFrame* m_pHoverSubsoil;
	SpriteFrame* m_pActiveDefaultSubsoil;
	SpriteFrame* m_pActiveHoverSubsoil;
	ProgressTimer* m_pLoading;
	Sprite* m_pSpLoading;
	Sprite* m_pSpLoadingBg;
	enum THEM_DEFTOWER_TYPE m_emDefTowerProfessionalType;
};


#endif // !__THDEFTOWER_H__
