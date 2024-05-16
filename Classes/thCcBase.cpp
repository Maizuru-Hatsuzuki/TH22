/********************************************************
* Filename		: thCcBase.cpp
* Creator		: ac
* Date time		: 2024.04.29
* Description	: all the base for touhou cocos.
********************************************************/


#include "stdafx.h"
#include "dirent.h"
#include "thBase.h"
#include "thBaseMacro.h"


CTHCcBaseHandler* CTHCcBaseHandler::m_pSelf;


CTHCcBaseHandler::CTHCcBaseHandler()
{
}

CTHCcBaseHandler::~CTHCcBaseHandler()
{
}

CTHCcBaseHandler* CTHCcBaseHandler::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new CTHCcBaseHandler();
	}
	return m_pSelf;
}

thBool CTHCcBaseHandler::getSpriteFrameExists(const char* cszpSpName)
{
	thBool bRet = THFALSE;
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();
	SpriteFrame* pSpFrame = NULL;

	pSpFrame = pSpFrameCache->getSpriteFrameByName(cszpSpName);
	if (NULL != pSpFrame)
	{
		bRet = THTRUE;
	}
	else
	{
		bRet = THFALSE;
	}
	return bRet;
}

thBool CTHCcBaseHandler::setSceneAllSpPlist(enum THEM_SCENE emScene, thBool bIsLoad)
{
	thBool bRet = THFALSE;
	char szarrPlistPath[MAX_PATH] = { 0 };
	int nPlistCnt = 0;

	switch (emScene)
	{
	case THSCENE_DUNGEON:
		strcpy_s(szarrPlistPath, MAX_PATH, "image\\ScenePlist\\Dungeon");
		bRet = scanPlistFiles(szarrPlistPath, &nPlistCnt, bIsLoad);
		TH_PROCESS_ERROR(bRet);
		CCLOG("loaded %d plist.", nPlistCnt);
		break;

	default:
		break;
	}

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCcBaseHandler::scanPlistFiles(const char* cszpPath, int* pnPlistCnt, thBool bIsLoad)
{
	thBool bRet = THFALSE;
	int nLoadingCnt = 0;
	char szarrFilename[MAX_PATH] = { 0 };
	char szarrPlist[MAX_PATH] = { 0 };
	char szarrPlistPng[MAX_PATH] = { 0 };
	struct dirent* ptFile = { 0 };
	DIR* ptDir = opendir(cszpPath);
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::sharedSpriteFrameCache();

	if (NULL != ptDir)
	{
		while (NULL != (ptFile = readdir(ptDir)))
		{
			if (0 != strcmp(ptFile->d_name, ".") && 0 != strcmp(ptFile->d_name, ".."))
			{
				bRet = splitFileSuffix(ptFile->d_name, ".plist", szarrFilename);
				if (THTRUE == bRet)
				{
					sprintf_s(szarrPlist, MAX_PATH, "%s\\%s.plist", cszpPath, szarrFilename);
					sprintf_s(szarrPlistPng, MAX_PATH, "%s\\%s.png", cszpPath, szarrFilename);
					if (bIsLoad == THTRUE)
					{
						pSpFrameCache->addSpriteFramesWithFile(szarrPlist, szarrPlistPng);
						CCLOG("Loaded plist: %s.", szarrPlist);
					}
					else
					{
						pSpFrameCache->removeSpriteFramesFromFile(szarrPlist);
					}
					nLoadingCnt++;
				}
			}
		}
	}

	*pnPlistCnt = nLoadingCnt;
	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CTHCcBaseHandler::splitFileSuffix(char* szpFile, const char* cszpSuffix, char* szpFilenameRet)
{
	thBool bRet = THFALSE;
	char* filename = "";
	char* szpSuffix = strstr(szpFile, cszpSuffix);
	if (NULL != szpSuffix)
	{
		/* 截断suffix后面的内容, 截断后缀 */
		*szpSuffix = '\0';
		strcpy_s(szpFilenameRet, strlen(szpFile) + 1, szpFile);
		bRet = THTRUE;
	}

	return bRet;
}
