/********************************************************
* Filename		: thCcCharacterSkill.h
* Creator		: ac
* Date time		: 2024.06.06
* Description	: character skill.
********************************************************/


#ifndef __THCCCHARACTERSKILL_H__
#define __THCCCHARACTERSKILL_H__

#include "thCcCharacter.h"

#define GETSK_GEN_LEVELUP(p)				p->puChacSkill->ptGeneralSkill->ptDefTowerLevelUp
#define GETSK_FRAMEINFO_GEN_LEVELUP(p)		p->puChacSkill->ptGeneralSkill->ptDefTowerLevelUp->pChacFrSkill
#define GETSK_DOLLREPAIR(p)					p->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollRepair
#define GETSK_FRAMEINFO_DOLLREPAIR(p)		p->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollRepair->pChacFrSkill
#define GETSK_DOLLSTRENGTHEM(p)				p->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollStrengthem
#define GETSK_FRAMEINFO_DOLLSTRENGTHEM(p)	p->puChacSkill->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pChacFrSkill
#define ADDCHILD_SKPRICE_AND_SKDESC(arrPrice, arrDesc)												\
do																									\
{																									\
	/* 技能售价. */																					\
	for (short s = 0; s < THMAX_TOWER_SKILL_COUNT; s++)												\
	{																								\
		TH_RUN_SUCCESS(NULL != arrPrice[s], this->addChild(arrPrice[s]->ptBg->pSpCharacter));		\
		TH_RUN_SUCCESS(NULL != arrPrice[s], this->addChild(arrPrice[s]->pLbText));					\
	}																								\
	/* 技能说明. */																					\
	for (short s = 0; s < THMAX_TOWER_SKILL_QM_COUNT; s++)											\
	{																								\
		TH_RUN_SUCCESS(NULL != arrDesc[s], this->addChild(arrDesc[s]->ptBg->pSpCharacter));			\
		TH_RUN_SUCCESS(NULL != arrDesc[s], this->addChild(arrDesc[s]->pLbTitleText));				\
		TH_RUN_SUCCESS(NULL != arrDesc[s], this->addChild(arrDesc[s]->pLbMainDesc));				\
		TH_RUN_SUCCESS(NULL != arrDesc[s], this->addChild(arrDesc[s]->pLbSubDesc));					\
	}																								\
} while (0);

struct _tSkill
{
	char szarrSkill[THMAX_CHAR_DESC];
	CHARACTER_FRAMEINFO_PTR pChacFrSkill;
	CHARACTER_FRAMEINFO_PTR arrpSkillLevelPoint[THMAX_SKILL_LEVEL];
	SpriteFrame* pSpFrameActiveSkill;
	SpriteFrame* pSpFrameDisableSkill;
	int nSkillPrice;
};

typedef _tSkill TH_SKILL, * TH_SKILL_PTR;

/* Basic universal. */
struct _tDefTowerGeneralSkill
{
	TH_SKILL_PTR ptDefTowerLevelUp;
};
typedef struct _tDefTowerGeneralSkill DEFTOWER_GENERAL_SKILL, * DEFTOWER_GENERAL_SKILL_PTR;

/* Alice Margatroid */
struct _tWarriorLv4Skill
{
	/* 轮回的西藏人型. */
	TH_SKILL_PTR ptSkDollRepair;
	/* 魔彩光的上海人型. */
	TH_SKILL_PTR ptSkDollStrengthem;
};
typedef struct _tWarriorLv4Skill ALICEMARGATROID_LV4_SKILL, * ALICEMARGATROID_LV4_SKILL_PTR;

union _uCharacterSkill
{
	DEFTOWER_GENERAL_SKILL_PTR ptGeneralSkill;
	ALICEMARGATROID_LV4_SKILL_PTR ptAliceMargatroidLv4Skill;
};
typedef union _uCharacterSkill CHARACTER_SKILL_UNION, * CHARACTER_SKILL_UNION_PTR;

struct _tDefTowerQuickMenu
{
	CHARACTER_FRAMEINFO_PTR pBg;
	CHARACTER_FRAMEINFO_PTR pCommandMovement;
	CHARACTER_FRAMEINFO_PTR pSellTower;
	enum THEM_DEFTOWER_TYPE emTowerType;
	enum THEM_CHARACTER_LEVEL emCharacterLevel;
	CHARACTER_SKILL_UNION_PTR puChacSkill;
	int nChacSkillCnt;
};
typedef struct _tDefTowerQuickMenu DEFTOWER_QUICKMENU, * DEFTOWER_QUICKMENU_PTR;


class CThCcCharacterSkillHanlder
{
public:
	static CThCcCharacterSkillHanlder* getInstance();

	void uninitSkillUnion(CHARACTER_SKILL_UNION_PTR pTag);

	thBool getSkillTextDescIni(const char* cszpSk, char* szpTitleRet) const;
	thBool getSkillTextDescXml(const char* cszpSk, char* szpTitleRet, char* szpMainDesc, char* szpSubDesc) const;
	thBool setTargetSkillUnion(enum THEM_DEFTOWER_TYPE emChacType, enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt);
	thBool setWarriorSkillUnion(enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt);

	thBool initGeneralSkill(CHARACTER_SKILL_UNION_PTR* ppRet);
	void uninitGeneralSkill(CHARACTER_SKILL_UNION_PTR p);
	thBool initWarriorSkillLv4Union(CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt);
	void uninitWarriorSkillLv4Union(CHARACTER_SKILL_UNION_PTR pUnion);

private:
	CThCcCharacterSkillHanlder();
	~CThCcCharacterSkillHanlder();
	CThCcCharacterSkillHanlder(const CThCcCharacterSkillHanlder& pSelf);
	const CThCcCharacterSkillHanlder& operator=(const CThCcCharacterSkillHanlder& pSelf);

private:
	static CThCcCharacterSkillHanlder* m_pSelf;
};



#endif
