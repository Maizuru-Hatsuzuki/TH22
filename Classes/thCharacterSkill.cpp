/********************************************************
* Filename		: thCcCharacterSkill.cpp
* Creator		: ac
* Date time		: 2024.06.06
* Description	: character skill.
********************************************************/


#include "thCcCharacterSkill.h"
#include "thBaseMacro.h"


CThCcCharacterSkillHanlder* CThCcCharacterSkillHanlder::m_pSelf;


CThCcCharacterSkillHanlder::CThCcCharacterSkillHanlder()
{
}

CThCcCharacterSkillHanlder::~CThCcCharacterSkillHanlder()
{
}

CThCcCharacterSkillHanlder* CThCcCharacterSkillHanlder::getInstance()
{
	thBool bRet = THFALSE;
	if (NULL == m_pSelf)
	{
		m_pSelf = THNEW_CLASS(CThCcCharacterSkillHanlder);
		TH_PROCESS_ERROR(m_pSelf);
	}

	bRet = THTRUE;
Exit0:
	ASSERT(bRet);
	return m_pSelf;
}

thBool CThCcCharacterSkillHanlder::setTargetSkillUnion(enum THEM_DEFTOWER_TYPE emChacType, enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt)
{
	thBool bRet = THFALSE;

	if (THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_3 >= emLevel)
	{
		/* 不足 3 级的统一返回通用技能. */
		bRet = getGeneralSkill(ppRet);
		TH_PROCESS_ERROR(bRet);
	}
	else
	{
		switch (emChacType)
		{
		case DEFTOWERTYPE_UNKNOW:
			break;
		case DEFTOWERTYPE_ARCHER:
			break;
		case DEFTOWERTYPE_WARRIORS:
			bRet = setWarriorSkillUnion(emLevel, THTRUE, ppRet, pnSkillCnt);
			break;
		case DEFTOWERTYPE_ARCHER_WARRIORS:
			break;
		default:
			break;
		}
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCcCharacterSkillHanlder::getGeneralSkill(CHARACTER_SKILL_UNION_PTR* ppRet)
{
	thBool bRet = THFALSE;
	CHARACTER_SKILL_UNION_PTR puRet = THMALLOC(CHARACTER_SKILL_UNION, sizeof(CHARACTER_SKILL_UNION));
	TH_PROCESS_ERROR(puRet);

	

	*ppRet = puRet;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCcCharacterSkillHanlder::setWarriorSkillUnion(enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt)
{
	thBool bRet = THFALSE;
	
	switch (emLevel)
	{
	case CHARACTER_LEVEL_1:
		break;
	case CHARACTER_LEVEL_2:
		break;
	case CHARACTER_LEVEL_3:
		break;
	case CHARACTER_LEVEL_4:
	{
		if (THTRUE == cbIsInit)
		{
			bRet = initWarriorSkillLv4Union(ppRet, pnSkillCnt);
			TH_PROCESS_ERROR(bRet);
		}
		else
		{
			uninitWarriorSkillLv4Union(*ppRet);
		}
		break;
	}
	case CHARACTER_LEVEL_5:
		break;
	case CHARACTER_MAXLEVEL:
		break;
	default:
		break;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCcCharacterSkillHanlder::initWarriorSkillLv4Union(CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt)
{
	thBool bRet = THFALSE;
	char szarrTmpSkillIni[MAX_PATH] = { 0 };
	CHARACTER_DESC_PTR ptChacDesc = NULL;
	CHARACTER_SKILL_UNION_PTR puRet = THMALLOC(CHARACTER_SKILL_UNION, sizeof(CHARACTER_SKILL_UNION));
	TH_PROCESS_ERROR(puRet);

	puRet->ptAliceMargatroidLv4Skill = THMALLOC(ALICEMARGATROID_LV4_SKILL, sizeof(ALICEMARGATROID_LV4_SKILL));
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill);

	/* 轮回的西藏人型. */
	TH_GETWINRESPATH(szarrTmpSkillIni, "data\\CharacterConfig\\Skill\\ChacSkDollRepair.ini");
	puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair = THMALLOC(TH_SKILL, sizeof(TH_SKILL));
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair);

	strcpy_s(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->szarrSkill, THMAX_CHAR_DESC, "DollRepair");
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrTmpSkillIni, &ptChacDesc);
	TH_PROCESS_ERROR(bRet);
	bRet = CThBaseCharacter::initCharacterWithPlist(ptChacDesc, &puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->pChacFrSkill);
	TH_PROCESS_ERROR(bRet);

	bRet = CthCcCharacterLoadHandler::getInstance()->getSkillFromIni(szarrTmpSkillIni, &puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->nSkillPrice);
	TH_PROCESS_ERROR(bRet);

	memset(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->arrpSkillLevelPoint, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SKILL_LEVEL);
	TH_UNINIT_CHACDESC(ptChacDesc);

	/* 魔彩光的上海人型. */
	TH_GETWINRESPATH(szarrTmpSkillIni, "data\\CharacterConfig\\Skill\\ChacSkDollStrengthem.ini");
	puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem = THMALLOC(TH_SKILL, sizeof(TH_SKILL));
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem);

	strcpy_s(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->szarrSkill, THMAX_CHAR_DESC, "DollStrengthem");
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrTmpSkillIni, &ptChacDesc);
	TH_PROCESS_ERROR(bRet);
	bRet = CThBaseCharacter::initCharacterWithPlist(ptChacDesc, &puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pChacFrSkill);
	TH_PROCESS_ERROR(bRet);

	bRet = CthCcCharacterLoadHandler::getInstance()->getSkillFromIni(szarrTmpSkillIni, &puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->nSkillPrice);
	TH_PROCESS_ERROR(bRet);

	memset(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->arrpSkillLevelPoint, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SKILL_LEVEL);
	TH_UNINIT_CHACDESC(ptChacDesc);

	*pnSkillCnt = 2;
	*ppRet = puRet;
	bRet = THTRUE;
Exit0:
	TH_UNINIT_CHACDESC(ptChacDesc);
	return bRet;
}

void CThCcCharacterSkillHanlder::uninitWarriorSkillLv4Union(CHARACTER_SKILL_UNION_PTR pUnion)
{
	THFREE(pUnion->ptAliceMargatroidLv4Skill->ptSkDollRepair->pChacFrSkill);
	THFREE(pUnion->ptAliceMargatroidLv4Skill->ptSkDollRepair);
	THFREE(pUnion->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pChacFrSkill);
	THFREE(pUnion->ptAliceMargatroidLv4Skill->ptSkDollStrengthem);
	THFREE(pUnion->ptAliceMargatroidLv4Skill);
	THFREE(pUnion);
	return;
}


