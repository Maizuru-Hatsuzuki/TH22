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
	/*
	const short csMaxAni = 4;
	const char* szarrpAni[csMaxAni] =
	{
		"data\\CharacterConfig\\SaigyoSakura\\AniWarriorsMove.ini",
		"data\\CharacterConfig\\SaigyoSakura\\AniOpenTheDoor.ini",
		"data\\CharacterConfig\\SaigyoSakura\\AniCloseTheDoor.ini",
		"data\\CharacterConfig\\SaigyoSakura\\AniTagWarriorsDie.ini"
	};
	CHARACTER_DESC_PTR arrpChacWarrios[2] = { 0 };
	DEFTOWER_WARRIORS tWarrior = { arrpChacWarrios, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, 2 };
	CThDefTower* pHunterCabinObject = new CThDefTower;

	bFnRet = CthCcCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
		"data\\CharacterConfig\\SaigyoSakura\\ChacWarrior.ini", &(tWarrior.arrpTowerWarriorsDesc[0])
	);
	TH_PROCESS_ERROR(bFnRet);
	tWarrior.arrpTowerWarriorsDesc[1] = NULL;
	
	bFnRet = pHunterCabinObject->init(
		"data\\CharacterConfig\\SaigyoSakura\\ChacSaigyoSakura.ini",
		"data\\CharacterConfig\\SaigyoSakura\\ChacBullet.ini",
		szarrpAni,
		csMaxAni,
		&tWarrior
		);
	TH_PROCESS_ERROR(bFnRet);
	this->addChild(pHunterCabinObject);
	*/


	const char* cszpSubSoilAPlistPath = "data\\CharacterConfig\\DefTowerSubsoil\\ChacGrasslandA.ini";
	CThDefTowerSubsoil* pSubsoilA = new CThDefTowerSubsoil;



	pSubsoilA->init(cszpSubSoilAPlistPath, 270.f);
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
	// CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(tWarrior.arrpTowerWarriorsDesc[0]);
	return bRet;
}

