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


#define TH_ANITAG_ALLANI	0
#define TH_ANITAG_MOVEERROR 1
#define TH_ANITAG_MOVING	2
#define TH_ANITAG_SCALEQM	3
#define TH_ANITAG_TOWERSK_LVUP 4
#define TH_ANITAG_MAX TH_ANITAG_TOWERSK_LVUP


struct _tQmPrice
{
	CHARACTER_FRAMEINFO_PTR ptBg;
	Label* pLbText;
};

struct _tQmSkTextDesc
{
	CHARACTER_FRAMEINFO_PTR ptBg;
	Label* pLbTitleText;
	Label* pLbMainDesc;
	Label* pLbSubDesc;
	char szarrTagSk[THMAX_CHAR_DESC];
};

typedef struct _tQmPrice THQM_PRICE, * THQM_PRICE_PTR;
typedef struct _tQmSkTextDesc THQM_SKTEXT, * THQM_SKTEXT_PTR;


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
		float* arrfDstXY,
		const CHARACTER_ANI_MAP_PTR cptAniMap,
		const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo
	);
	thBool initWarriors(const CHARACTER_DESC_PTR cptSpDesc, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);
	thBool initFsmEvent();
	void uninit();

	const short getWarriorVacantPos() const;
	const float getWarriorBirthMoveAngle() const;
	const float getWarriorInPositionTime() const;
	void getWarriorPosition(float* pfX, float* pfY, float fFixedY) const;
	void getWarriorAniMap(CHARACTER_ANI_MAP_PTR* ppRet) const;
	void getWarriorFsmStatus(enum THEM_CHARACTER_FSM_EVENT* pemRet) const;
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	void setCurFsmStatus(enum THEM_CHARACTER_FSM_EVENT emStatus);
	void setWarriorBirthMoveAngle(const float fAngle);
	void setWarriorHaloIsVisible(const thBool cbIsVisible);

	/* us 前缀, 用户接口, 能够直接切换状态机状态, 暴露给其他逻辑用接口. */
	thBool usSetWarriorMove(const float cfDstX, const float cfDstY, const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);
	thBool usSetWarriorDie(const short csSpArrVacantPos, const CHARACTER_ANI_FRAMEINFO_PTR cptAniMoveTo);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);
	virtual void update(float dt);

	virtual thBool delayUninitMonitoring();

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
	float* m_arrfWarriorMovePos;
	float m_fWarriorBirthX;
	float m_fWarriorBirthY;
	double m_dInPositionTime;
	short m_sSpArrVacantPos;
	enum THEM_CHARACTER_FSM_EVENT m_emCurFsmStatus;
	thBool m_bIsFlip;
	time_t m_tLastStandFlipTime;
	CHARACTER_FRAMEINFO_PTR m_ptWarriorFrameInfo;
	CHARACTER_FRAMEINFO_PTR m_ptWarriorHaloFrameInfo;
	CHARACTER_ANI_MAP_PTR m_ptAniMap;
	CHARACTER_ANI_FRAMEINFO_PTR m_ptAniMoveTo;
	CThFSMCharacter* m_fsmWarriorObject;
	EventListenerMouse* m_pEventMouse;
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
	thBool initAnimate(char** szarrpAniDesc, const short csSize);
	thBool initWarriors(const short csCnt, short sSpArrVacantPos);
	thBool initDefTowerWarriorsDesc(const DEFTOWER_WARRIORS_PTR ptWarriors);
	thBool initBullet(float fShootAngle);
	virtual void uninit();
	void uninitDefTowerWarriorsDesc();
	void uninitBullet(Node* pNode, const short csBullet);

	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);
	enum THEM_DEFTOWER_TYPE getDefTowerType();
	enum THEM_CHARACTER_LEVEL getDefTowerCurLv();
	enum THEM_DELAY_UNINIT_FLAG getDefTowerDelayUninitType() const;
	void getAniTagByDesc(const char* cszpDesc, int* pnRet) const;
	void getAniFrameInfoByTag(const char* cszpTag, CHARACTER_ANI_FRAMEINFO_PTR* ppRet) const;
	void getWarriorExistsByAngle(const float cfAngle, thBool* pbRet) const;
	void getRandWarriorTypeDesc(CHARACTER_DESC_PTR* ppRet);
	void getIsHoverDefTower(thBool* pbRet) const;
	void getCurSkillArray(TH_SKILL_PTR* ppRet, const int cnSize);
	void getCurSkillByName(const char* cszpSk, TH_SKILL_PTR* ppRet);
	const float getQmScale() const;

	void setUninitFlag();
	void setWarriorExistsByAngle(const float cfAngle, const short csTag);
	void setSkLevelUpByName(const char* cszpSk);
	thBool setWarriorsOverallMovement(const float cfX, const float cfY);
	thBool setPlayerAniBasic();
	thBool setPlayAniTowerSummon(const short* arrnCondAniTag, const short cnSize, const thBool bIsSummoning);
	thBool setPlayAniBuildSmoke(thBool bIsBuild);

	thBool execTowerShoot(const short csBulletCnt);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual void update(float dt);
	virtual thBool globalMonitoring();
	virtual thBool delayUninitMonitoring();
	thBool globalMonitoringWarriors();

	thBool thOnClickCreateQucikMenu();
	thBool thOnClickDestoryQucikMenu();

public:
	/* static fn. get init defTower info. */
	static thBool getTowerInfo(
		enum THEM_CHARACTER_LEVEL emLevel, enum THEM_DEFTOWER_TYPE emTowerType, DEFTOWER_WARRIORS_PTR ptWarrior,
		char* szpTowerDescRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
	);
	static void getTowerInfoArcher(
		enum THEM_CHARACTER_LEVEL emLevel, char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction
	);
	static thBool getTowerInfoWarriors(
		char* szpArcherRet, char** arrpAniRet, short* psAniSizeRet, char** arrpWarriorsRet, short* psWarriorsCnt, char* szpDefTowerConstruction, char* szpLv
	);

private:
	thBool _initBasicAnimate(short* psOffset);
	void _initAllWarriorsMovePosWithAngle(const CHARACTER_DESC_PTR cptCharacterDesc, const float cfFaceAngle);
	void _initAllWarriorsMovePosWithXY(const float cfX, const float cfY);
	thBool _getSpArrayVacantPos(short* psRet);
	thBool _getWarArrayVacantPos(short* psRet);
	thBool _setSpTowerSpecialValues();
	thBool _setSpTowerSpecialValuesWarrior();
	thBool _setPlayerAniBasicWarriorTower();
	thBool _setPlayAniOpenTheDoor();
	thBool _setPlayAniCloseTheDoor();
	thBool _setPlayAniWarriorsDie(CThDefTowerWarrior_ptr pSp);
	thBool _setCreateQmWarrior(const thBool cbIsCreate);

	void thcbPlayAniSmoke();

	thBool _monitoringWarriorsHealthy(CThDefTowerWarrior_ptr pSp, pthBool pbIsRelease);

private:
	short							m_sVacantPos;
	short							m_arrsWarriorsVacantPos[THMAX_DEFTOWER_TARLEVEL_WARRIORS];
	float							m_fQmScale;
	static float					ms_fWarriorsBirthAngle;
	float							m_arrfWarriorMovePos[THMAX_DEFTOWER_TARLEVEL_WARRIORS][2];
	double							m_dLastSummonWarriors;
	double							m_dLastDieWarriors;
	double							m_dLastAttack;
	thBool							m_bIsHoverTower;
	Vec2							m_vecAnchorPoint;
	EventListenerMouse*				m_pEventMouse;
	CHARACTER_ANI_TAG				m_tAniTag;
	CHARACTER_FRAMEINFO_PTR			m_ptTower;
	CHARACTER_FRAMEINFO_PTR			m_ptSmoke;
	DEFTOWER_DESC_PTR				m_ptTowerStatus;
	enum THEM_DELAY_UNINIT_FLAG		m_emStepUninit;
	enum THEM_DEFTOWER_TYPE			m_emTowerType;
	CHARACTER_FRAMEINFO_PTR*		m_arrpSpGroup;
	CHARACTER_ANI_FRAMEINFO_PTR*	m_arrpAniGroup;
	/* 防御塔战士精灵描述，每个等级 4 种类型. */
	CHARACTER_DESC_PTR				m_arrpWarriorsDesc[THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL][THMAX_DEFTOWER_TARLEVEL_WARRIORS];
	CThDefTowerWarrior_ptr*			m_arrpWarriors;
	/* 子弹. */
	SpriteBatchNode*				m_pBatchNodeBullet;
	CHARACTER_DESC_PTR				m_ptBulletDesc;
	/* 点击菜单 */
	DEFTOWER_QUICKMENU				m_tChacFrameQuickMenuBg;
	/* 当前防御塔技能. */
	TH_SKILL_PTR					m_arrpCurSkill[THMAX_TOWER_SKILL_COUNT];
};
typedef CThDefTower* CThDefTower_ptr;


class CThDefTowerQuickMenu:
	public CThDefTower
{
public:
	thBool init();
	thBool initBasicAni();
	void uninit();
	static CThDefTowerQuickMenu* getInstance();
	thBool createBasicQm(const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, thBool bIsCreateGenSk);
	void destoryQm();
	thBool createQmGenera(enum THEM_CHARACTER_LEVEL emLv);
	thBool createQmWarrior(enum THEM_CHARACTER_LEVEL emLv, const float cfX, const float cfY, const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget);
	thBool createQmWarriorLevel4(const float cfTagScale, DEFTOWER_QUICKMENU_PTR ptDefTowerQm, CThDefTower_ptr pTaget);
	thBool getQmIsPlayAni(const int cnAniTag);
	thBool getIsClickInMoveRangeHalo(Vec2 vecPosInView);
	thBool getChacSkillPos(const int nSkillCnt, float arrarrfRet[][2]);
	thBool getChacSkillLevelPointPos(const short csPos, float* arrfRet, TH_SKILL_PTR pSk);
	void getSkTextBySkillName(const char* cszpSk, THQM_SKTEXT_PTR* ppRet);
	thBool setChacSkillLevelPoint(enum THEM_CHARACTER_LEVEL emLv, TH_SKILL_PTR pSk);
	thBool setChacSkillPrice(TH_SKILL_PTR pSk);
	thBool setChacSkillTextDesc(TH_SKILL_PTR pSk);
	enum THEM_DELAY_UNINIT_FLAG getDefTowerDelayUninitType() const;
	void setMouseCursorAni(enum THEM_QM_MOUSECURSOR emMouseType);
	void setStopUninit();

	void onMouseUp(EventMouse* pMouse);
	void onMouseDown(EventMouse* pMouse);
	void onMouseMove(EventMouse* pMouse);
	void update(float dt);

	virtual thBool delayUninitMonitoring();

private:
	thBool _getSkTextArrVancantPos(short* psRet) const;
	thBool _getSkPriceArrVancantPos(short* psRet) const;

private:
	CThDefTowerQuickMenu();
	~CThDefTowerQuickMenu();
	CThDefTowerQuickMenu(const CThDefTowerQuickMenu& pSelf);
	const CThDefTowerQuickMenu& operator=(const CThDefTowerQuickMenu& pSelf);

private:
	static CThDefTowerQuickMenu* m_pSelf;
	DEFTOWER_QUICKMENU_PTR m_ptQm;
	CThDefTower_ptr m_pTaget;
	CHARACTER_FRAMEINFO_PTR m_ptCurrentMouse;
	CHARACTER_FRAMEINFO_PTR m_ptSellHoverBorder;
	CHARACTER_FRAMEINFO_PTR m_ptMoveHoverBorder;
	CHARACTER_FRAMEINFO_PTR m_ptSkillHoverBorder;
	CHARACTER_FRAMEINFO_PTR m_ptMoveRangeHalo;
	CHARACTER_FRAMEINFO_PTR m_ptMoveSelectingMouse;
	CHARACTER_FRAMEINFO_PTR m_ptMoveSelectedMouse;
	CHARACTER_FRAMEINFO_PTR m_ptMoveSelectedErrorMouse;
	CHARACTER_FRAMEINFO_PTR m_ptTowerSkillLevelUp;
	CHARACTER_ANI_FRAMEINFO_PTR m_ptAniMovePosSelectingMouse;
	CHARACTER_ANI_FRAMEINFO_PTR m_ptAniMovePosSelectedMouse;
	CHARACTER_ANI_FRAMEINFO_PTR m_ptAniMovePosSelectedErrorMouse;
	CHARACTER_ANI_FRAMEINFO_PTR m_ptAniTowerSkillLevelUp;
	EventListenerMouse*			m_pMouse;
	enum THEM_DEFTOWER_TYPE		m_emTagTowerType;
	enum THEM_DELAY_UNINIT_FLAG m_emStepUninit;
	TH_SKILL_PTR				m_arrpCurSk[THMAX_TOWER_SKILL_COUNT];
	THQM_PRICE_PTR				m_arrpQmPrice[THMAX_TOWER_SKILL_COUNT];
	/* 多一个售出按钮的介绍位置. */
	THQM_SKTEXT_PTR				m_arrpQmSkText[THMAX_TOWER_SKILL_QM_COUNT];
	SpriteFrame*				m_pSpFrameActiveSkillPoint;
	DrawNode*					m_pMoveRangeHaloJudge;
};


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
	thBool initDefTower(enum THEM_CHARACTER_LEVEL emLevel);
	void uninit();
	void uninitDefTower();
	virtual void getCharacterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharacterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	void setDefTowerProfessionalType(enum THEM_DEFTOWER_TYPE emType);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	void onHoverSubsoil(const thBool cbIsHover);

	virtual thBool globalMonitoring();
	virtual thBool delayUninitMonitoring();
	virtual void update(float dt);

private:
	float m_fSubsoilScale;
	float m_arrfDefTowerWarriorPos[THMAX_DEFTOWER_TARLEVEL_WARRIORS][2];
	CHARACTER_FRAMEINFO_PTR m_ptSubsoil;
	CHARACTER_FRAMEINFO_PTR m_ptConstruction;
	SUBSOIL_DESC_PTR m_ptSubSoilStatus;
	CThDefTower_ptr m_pDefTower;
	SpriteFrame* m_pSpFrDefaultSubsoil;
	SpriteFrame* m_pSpFrHoverSubsoil;
	SpriteFrame* m_pSpFrActiveDefaultSubsoil;
	SpriteFrame* m_pSpFrActiveHoverSubsoil;
	ProgressTimer* m_pLoading;
	Sprite* m_pSpLoading;
	Sprite* m_pSpLoadingBg;
	enum THEM_DEFTOWER_TYPE m_emDefTowerProfessionalType;
};


#endif // !__THDEFTOWER_H__
