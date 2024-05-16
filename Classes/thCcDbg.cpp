/********************************************************
* Filename		: thCCDbg.cpp
* Creator		: ac
* Date time		: 2024.05.09
* Description	: th cocos2dx engine debug.
********************************************************/


#include "thCcDbg.h"
#include "thBaseMacro.h"


CThCCDbgWithCharacter* CThCCDbgWithCharacter::m_pSelf;


CThCCDbg::CThCCDbg()
{
}

CThCCDbg::~CThCCDbg()
{
}

thBool CThCCDbg::getDbgInfoInScreenObject(const char* cszpText, Label** ppRet)
{
	thBool bRet = THFALSE;
	Color3B tFontColor;
	tFontColor.r = 255;
	tFontColor.g = 255;
	tFontColor.b = 255;
	Label* pFont = Label::createWithTTF(cszpText, "fonts\\Marker Felt.ttf", 18);
	TH_PROCESS_ERROR(pFont);

	pFont->setAnchorPoint(Vec2(0, 1));
	pFont->setPosition(20, Director::getInstance()->getVisibleSize().height);
	pFont->setColor(tFontColor);

	*ppRet = pFont;
	bRet = THTRUE;
Exit0:
	return bRet;
}


CThCCDbgWithCharacter::CThCCDbgWithCharacter()
{
}

CThCCDbgWithCharacter::~CThCCDbgWithCharacter()
{
}

CThCCDbgWithCharacter* CThCCDbgWithCharacter::getInstance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new (std::nothrow) CThCCDbgWithCharacter();
		m_pSelf->init();
	}
	return m_pSelf;
}

thBool CThCCDbgWithCharacter::init()
{
	thBool bRet = THFALSE;
	m_pLbDbg = NULL;

	strcpy_s(m_szarrDbgInfo, 2048, "Character debug info");

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCCDbgWithCharacter::createShowCharacterDbgFrameInfo()
{
	thBool bRet = THFALSE;

	bRet = getDbgInfoInScreenObject(m_szarrDbgInfo, &m_pLbDbg);
	TH_PROCESS_ERROR(bRet);

	this->addChild(m_pLbDbg, 1);

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool CThCCDbgWithCharacter::setDbgFrameInfo(CHARACTER_FRAMEINFO_PTR ptInfo)
{
	thBool bRet = THFALSE;
	char szarrText[2048] = { 0 };
	
	sprintf_s(
		szarrText,
		2048,
		"Character debug info\n"
		"HP: %d\n"
		"MP: %d\n"
		"Level: %d\n"
		"Des: %s\n"
		"Type: %d\n",
		ptInfo->nHP,
		ptInfo->nMP,
		ptInfo->emCurLevel,
		ptInfo->szarrDesc,
		ptInfo->emCharacterType
	);

	strcpy_s(m_szarrDbgInfo, strlen(szarrText) + 1, szarrText);
	m_pLbDbg->setString(m_szarrDbgInfo);

	CCLOG(m_szarrDbgInfo);

	bRet = THTRUE;
Exit0:
	return bRet;
}
