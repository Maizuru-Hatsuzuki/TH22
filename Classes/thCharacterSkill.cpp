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

void CThCcCharacterSkillHanlder::uninitSkillUnion(CHARACTER_SKILL_UNION_PTR pTag)
{
	if (NULL == pTag)
	{
		goto Exit0;
	}

	/* union 用的同一块内存. */
	if (0 == strcmp(THSK_DEFTOWER_LVUP, pTag->ptGeneralSkill->ptDefTowerLevelUp->szarrSkill))
	{
		uninitGeneralSkill(pTag);
	}
	else if (0 == strcmp("DollRepair", pTag->ptAliceMargatroidLv4Skill->ptSkDollRepair->szarrSkill))
	{
		uninitWarriorSkillLv4Union(pTag);
	}

Exit0:
	return;
}

thBool CThCcCharacterSkillHanlder::setTargetSkillUnion(enum THEM_DEFTOWER_TYPE emChacType, enum THEM_CHARACTER_LEVEL emLevel, const thBool cbIsInit, CHARACTER_SKILL_UNION_PTR* ppRet, int* pnSkillCnt)
{
	thBool bRet = THFALSE;

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
	{
		bRet = initGeneralSkill(ppRet);
		TH_PROCESS_ERROR(bRet);
		(*ppRet)->ptGeneralSkill->ptDefTowerLevelUp->nSkillPrice = 90;
		*pnSkillCnt = 1;
		break;
	}
	case CHARACTER_LEVEL_2:
	{
		bRet = initGeneralSkill(ppRet);
		TH_PROCESS_ERROR(bRet);
		(*ppRet)->ptGeneralSkill->ptDefTowerLevelUp->nSkillPrice = 180;
		*pnSkillCnt = 1;
		break;
	}
	case CHARACTER_LEVEL_3:
	{
		bRet = initGeneralSkill(ppRet);
		TH_PROCESS_ERROR(bRet);
		(*ppRet)->ptGeneralSkill->ptDefTowerLevelUp->nSkillPrice = 270;
		*pnSkillCnt = 1;
		break;
	}
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

thBool CThCcCharacterSkillHanlder::initGeneralSkill(CHARACTER_SKILL_UNION_PTR* ppRet)
{
	thBool bRet = THFALSE;
	char szarrPath[MAX_PATH] = { 0 };
	CHARACTER_DESC_PTR ptChacDesc = NULL;
	CHARACTER_SKILL_UNION_PTR puRet = THMALLOC(CHARACTER_SKILL_UNION, sizeof(CHARACTER_SKILL_UNION));
	TH_PROCESS_ERROR(puRet);
	puRet->ptGeneralSkill = THMALLOC(DEFTOWER_GENERAL_SKILL, sizeof(DEFTOWER_GENERAL_SKILL));
	TH_PROCESS_ERROR(puRet->ptGeneralSkill);
	puRet->ptGeneralSkill->ptDefTowerLevelUp = THMALLOC(TH_SKILL, sizeof(TH_SKILL));
	TH_PROCESS_ERROR(puRet->ptGeneralSkill->ptDefTowerLevelUp);

	TH_GETWINRESPATH(szarrPath, "data\\CharacterConfig\\Skill\\ChacSkTowerLevelUp.ini");

	strcpy_s(puRet->ptGeneralSkill->ptDefTowerLevelUp->szarrSkill, THMAX_CHAR_DESC, THSK_DEFTOWER_LVUP);
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrPath, &ptChacDesc);
	TH_PROCESS_ERROR(bRet);
	bRet = CThBaseCharacter::initCharacterWithPlist(ptChacDesc, &puRet->ptGeneralSkill->ptDefTowerLevelUp->pChacFrSkill);
	TH_PROCESS_ERROR(bRet);

	puRet->ptGeneralSkill->ptDefTowerLevelUp->pSpFrameDisableSkill = puRet->ptGeneralSkill->ptDefTowerLevelUp->pChacFrSkill->pSpCharacter->getSpriteFrame();
	puRet->ptGeneralSkill->ptDefTowerLevelUp->pSpFrameActiveSkill = SpriteFrameCache::getInstance()->getSpriteFrameByName("Quickmenu Material_29.png");
	TH_PROCESS_ERROR(puRet->ptGeneralSkill->ptDefTowerLevelUp->pSpFrameActiveSkill);

	puRet->ptGeneralSkill->ptDefTowerLevelUp->nSkillPrice = 0;
	puRet->ptGeneralSkill->ptDefTowerLevelUp->fnCallbackSkLvUp = NULL;

	*ppRet = puRet;
	bRet = THTRUE;

Exit0:
	if (THFALSE == bRet)
	{
		THFREE(puRet->ptGeneralSkill->ptDefTowerLevelUp);
		THFREE(puRet->ptGeneralSkill);
		THFREE(puRet);
	}
	TH_UNINIT_CHACDESC(ptChacDesc);
	return bRet;
}

void CThCcCharacterSkillHanlder::uninitGeneralSkill(CHARACTER_SKILL_UNION_PTR p)
{
	THFREE(p->ptGeneralSkill->ptDefTowerLevelUp);
	THFREE(p->ptGeneralSkill);
	THFREE(p);
	return;
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
	puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->pSpFrameDisableSkill = puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->pChacFrSkill->pSpCharacter->getSpriteFrame();
	puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->pSpFrameActiveSkill = SpriteFrameCache::getInstance()->getSpriteFrameByName("Quickmenu Material_10.png");
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->pSpFrameActiveSkill);

	bRet = CthCcCharacterLoadHandler::getInstance()->getSkillFromIni(szarrTmpSkillIni, &puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->nSkillPrice);
	TH_PROCESS_ERROR(bRet);

	memset(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->arrpSkillLevelPoint, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SKILL_LEVEL);
	TH_UNINIT_CHACDESC(ptChacDesc);
	puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair->fnCallbackSkLvUp = NULL;

	/* 魔彩光的上海人型. */
	TH_GETWINRESPATH(szarrTmpSkillIni, "data\\CharacterConfig\\Skill\\ChacSkDollStrengthem.ini");
	puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem = THMALLOC(TH_SKILL, sizeof(TH_SKILL));
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem);

	strcpy_s(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->szarrSkill, THMAX_CHAR_DESC, "DollStrengthem");
	bRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(szarrTmpSkillIni, &ptChacDesc);
	TH_PROCESS_ERROR(bRet);
	bRet = CThBaseCharacter::initCharacterWithPlist(ptChacDesc, &puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pChacFrSkill);
	TH_PROCESS_ERROR(bRet);
	
	puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pSpFrameDisableSkill = puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pChacFrSkill->pSpCharacter->getSpriteFrame();
	puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pSpFrameActiveSkill = SpriteFrameCache::getInstance()->getSpriteFrameByName("Quickmenu Material_2.png");
	TH_PROCESS_ERROR(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->pSpFrameActiveSkill);

	bRet = CthCcCharacterLoadHandler::getInstance()->getSkillFromIni(szarrTmpSkillIni, &puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->nSkillPrice);
	TH_PROCESS_ERROR(bRet);

	memset(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->arrpSkillLevelPoint, 0, sizeof(CHARACTER_FRAMEINFO_PTR) * THMAX_SKILL_LEVEL);
	TH_UNINIT_CHACDESC(ptChacDesc);
	puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem->fnCallbackSkLvUp = NULL;

	*pnSkillCnt = 2;
	*ppRet = puRet;
	bRet = THTRUE;

Exit0:
	if (THFALSE == bRet)
	{
		THFREE(puRet->ptAliceMargatroidLv4Skill->ptSkDollRepair);
		THFREE(puRet->ptAliceMargatroidLv4Skill->ptSkDollStrengthem);
		THFREE(puRet->ptAliceMargatroidLv4Skill);
		THFREE(puRet);
	}
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

thBool CThCcCharacterSkillHanlder::getSkillTextDescIni(const char* cszpSk, char* szpTitleRet) const
{
	thBool bRet = THFALSE;
	char szarrTextIni[MAX_PATH] = { 0 };
	TH_GETWINRESPATH(szarrTextIni, "data\\String\\skillText.ini");
	
	GetPrivateProfileStringA(cszpSk, "Title", "NA", szpTitleRet, THMAX_CHAR_DESC, szarrTextIni);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCcCharacterSkillHanlder::getSkillTextDescXml(const char* cszpSk, char* szpTitleRet, char* szpMainDesc, char* szpSubDesc) const
{
	thBool bRet = THFALSE;
	const char* cszpXml = "data\\String\\string_skillDesc.xml";
	char szarrTmpRet[THMAX_CHAR_DESC] = { 0 };
	ValueMap pTextMap = FileUtils::getInstance()->getValueMapFromFile(cszpXml);
	std::string strRet = "";

	sprintf_s(szarrTmpRet, THMAX_CHAR_DESC, "%sTitle", cszpSk);
	strRet = pTextMap[szarrTmpRet].asString();
	const char* cszpTitle = strRet.c_str();
	strcpy_s(szpTitleRet, THMAX_CHAR_DESC, cszpTitle);

	sprintf_s(szarrTmpRet, THMAX_CHAR_DESC, "%sMainDesc", cszpSk);
	strRet = pTextMap[szarrTmpRet].asString();
	const char* cszpMainDesc = strRet.c_str();
	strcpy_s(szpMainDesc, THMAX_CHAR_DESC, cszpMainDesc);

	sprintf_s(szarrTmpRet, THMAX_CHAR_DESC, "%sSubDesc", cszpSk);
	strRet = pTextMap[szarrTmpRet].asString();
	const char* cszpSubDesc = strRet.c_str();
	strcpy_s(szpSubDesc, THMAX_CHAR_DESC, cszpSubDesc);

	bRet = THTRUE;
Exit0:
	return bRet;
}
