/********************************************************
* Filename		: thSceneDungeon.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: dungeon scene.
********************************************************/


#include "thBaseMacro.h"
#include "thSceneDungeon.h"
#include "thDeviceControl.h"
#include "thDefTower.h"


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
	const char* szarrpAni[3] =
	{
		"data\\CharacterConfig\\SaigyoSakura\\AniWarriorsMove.ini",
		"data\\CharacterConfig\\SaigyoSakura\\AniOpenTheDoor.ini",
		"data\\CharacterConfig\\SaigyoSakura\\AniCloseTheDoor.ini"
	};
	DEFTOWER_WARRIORS tWarrior = { NULL, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, 2 };
	CThDefTower* pHunterCabinObject = new CThDefTower;

	bFnRet = CThCharacterLoadHandler::getInstance()->getCharaterDescFromIni(
		"data\\CharacterConfig\\SaigyoSakura\\ChacWarrior.ini", &(tWarrior.arrpTowerWarriorsDesc[0])
	);
	TH_PROCESS_ERROR(bFnRet);
	tWarrior.arrpTowerWarriorsDesc[1] = NULL;
	
	bFnRet = pHunterCabinObject->init(
		"data\\CharacterConfig\\SaigyoSakura\\ChacSaigyoSakura.ini",
		"data\\CharacterConfig\\SaigyoSakura\\ChacBullet.ini",
		szarrpAni,
		&tWarrior
		);
	TH_PROCESS_ERROR(bFnRet);

	bFnRet = initBgMap();
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(pHunterCabinObject);

	bRet = THTRUE;
Exit0:
	CThCharacterLoadHandler::getInstance()->uninitCharacterDesc(tWarrior.arrpTowerWarriorsDesc[0]);
	return bRet;
}

