/********************************************************
* Filename		: thCcCharacterSkill.h
* Creator		: ac
* Date time		: 2024.06.06
* Description	: character skill.
********************************************************/


#ifndef __THCCCHARACTERSKILL_H__
#define __THCCCHARACTERSKILL_H__

#include "thCcCharacter.h"


struct _tSkill
{
	char szarrSkill[THMAX_CHAR_DESC];
	CHARACTER_FRAMEINFO_PTR pChacFrSkill;
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
	enum THEM_DEFTOWER_TYPE emCharacterType;
	enum THEM_CHARACTER_LEVEL emCharacterLevel;
	CHARACTER_SKILL_UNION_PTR puChacSkill;
};
typedef struct _tDefTowerQuickMenu DEFTOWER_QUICKMENU, * DEFTOWER_QUICKMENU_PTR;


class CThCcCharacterSkillHanlder
{
public:
	static CThCcCharacterSkillHanlder* getInstance();

	thBool setTargetSkillUnion(enum THEM_DEFTOWER_TYPE emChacType, enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet);
	thBool getGeneralSkill(CHARACTER_SKILL_UNION_PTR* ppRet);
	thBool setWarriorSkillUnion(enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet);

	thBool initWarriorSkillLv4Union(CHARACTER_SKILL_UNION_PTR* ppRet);
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
