/********************************************************
* Filename		: thSceneDungeon.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: dungeon scene.
********************************************************/


#include "thBaseMacro.h"
#include "thSceneDungeon.h"
#include "thCcDeviceControl.h"
#include "thCcDbg.h"
#include "thCcDefTower.h"


thSceneDungeon::thSceneDungeon()
{
}

thSceneDungeon::~thSceneDungeon()
{
}

Scene* thSceneDungeon::createScene()
{
	return thSceneDungeon::create();
}

thBool initSceneAllPlist()
{
	thBool bRet = THFALSE;

	bRet = THTRUE;
Exit0:
	return bRet;
}

thBool thSceneDungeon::initBgMap()
{
	thBool bRet = THFALSE;
	Layer* pBgLayer = Layer::create();
	Sprite* pBgMap = Sprite::create("image\\maps\\Stage5.png");
	TH_PROCESS_ERROR(pBgMap);
	pBgMap->setScale(0.5);
	//pBgMap->setAnchorPoint(Vec2(0, 0));
	pBgMap->setPosition(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2);
	pBgLayer->addChild(pBgMap);
	this->addChild(pBgLayer);

	bRet = THTRUE;
Exit0:
	return bRet;
}


void initAni()
{

}

bool thSceneDungeon::init()
{
	bool bRet = THFALSE;
	bool bFnRet = THFALSE;
	const int cnDbgTag = 1;

	CTHCcBaseHandler::getInstance()->setSceneAllSpPlist(THEM_SCENE::THSCENE_DUNGEON, THTRUE);
	char szarrSubSoilAPlistPath[MAX_PATH] = "data\\CharacterConfig\\DefTowerSubsoil\\ChacGrasslandA.ini";
	CThDefTowerSubsoil* pSubsoilA = new CThDefTowerSubsoil;

	pSubsoilA->init(szarrSubSoilAPlistPath, 10.f);
	pSubsoilA->setDefTowerProfessionalType(THEM_DEFTOWER_TYPE::DEFTOWERTYPE_WARRIORS);
	this->addChild(pSubsoilA);

#ifdef _DEBUG
	/* 初始化角色 debug 信息. */
	if (NULL == this->getChildByTag(cnDbgTag))
	{
		TDBG_CHARACTER::getInstance()->createShowCharacterDbgFrameInfo();
		this->addChild(TDBG_CHARACTER::getInstance(), 0, cnDbgTag);
	}
#endif // _DEBUG

	

	bRet = THTRUE;
Exit0:

	return bRet;
}

