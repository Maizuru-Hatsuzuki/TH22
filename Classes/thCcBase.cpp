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
	SpriteFrameCache* pSpFrameCache = SpriteFrameCache::getInstance();
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

void CTHCcBaseHandler::setShowWinMouseCursor(thBool bIsShow)
{
	int nCnt = 0;
	if (THTRUE == bIsShow)
	{
		do 
		{
			nCnt = ShowCursor(TRUE);
		} while (nCnt < 0);
	}
	else
	{
		do 
		{
			nCnt = ShowCursor(FALSE);
		} while (nCnt >= 0);
	}
	return;
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
	thBool bRet							= THFALSE;
	DWORD dwFnRet						= 0;
	int nLoadingCnt						= 0;
	char szarrWinPath[MAX_PATH]			= { 0 };
	char szarrFilename[MAX_PATH]		= { 0 };
	char szarrPlist[MAX_PATH]			= { 0 };
	char szarrPlistPng[MAX_PATH]		= { 0 };
	char szarrCurPath[MAX_PATH]			= { 0 };
	struct dirent* ptFile				= { 0 };
	SpriteFrameCache* pSpFrameCache		= SpriteFrameCache::getInstance();
	/* ����Ҫ��Ӹ� Resources ·��, cc ����·���� windows ��һ��. */
	sprintf_s(szarrWinPath, "Resources\\%s", cszpPath);
	DIR* ptDir = opendir(szarrWinPath);

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
	else
	{
		CCLOG("Dir is null: %s. cur path: %s.", cszpPath, szarrCurPath);
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
		/* �ض�suffix���������, �ضϺ�׺ */
		*szpSuffix = '\0';
		strcpy_s(szpFilenameRet, strlen(szpFile) + 1, szpFile);
		bRet = THTRUE;
	}

	return bRet;
}
