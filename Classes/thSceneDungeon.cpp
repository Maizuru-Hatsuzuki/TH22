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

bool thSceneDungeon::init()
{
	bool bRet = THFALSE;
	bool bFnRet = THFALSE;

	CThDefTower* pHunterCabinObject = new CThDefTower;
	/* tower */
	CHARACTER_DESC tTowerDesc =
	{
		"Hunter cabin",
		"image\\sprite\\kr\\hunterCabin1.png",
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.3f, false, false, 100, 100, 10, 10, 10, 10, 0,
		THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_NARMAL,
		THEM_CHARACTER_TYPE::CHARACTER_DEFTOWER
	};
	CHARACTER_ANI_DESC_PTR arrpAniDesc[THMAX_ANI_COUNT] = { 0 };
	DEFTOWER_DESC tTowerStatusDesc = { 3, 4, 1, 10, 10, 50, 0 };

	/* warriors */
	CHARACTER_DESC tTowerWarriorA =
	{
		"hunter cabin warrior A",
		"image\\sprite\\kr\\batMove1.png",
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.3f, false, false, 100, 100, 10, 10, 10, 10, 0,
		THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_NARMAL,
		THEM_CHARACTER_TYPE::CHARACTER_TOWER_WARRIOR
	};
	CHARACTER_DESC_PTR arrpWarriors[THMAX_TARLEVEL_DEFTOWER_WARRIORS] = { &tTowerWarriorA, NULL, NULL };
	
	bFnRet = pHunterCabinObject->init(&tTowerDesc, arrpAniDesc, &tTowerStatusDesc);
	TH_PROCESS_ERROR(bFnRet);
	bFnRet = pHunterCabinObject->initDefTowerWarriorsDesc(arrpWarriors, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, 1);
	TH_PROCESS_ERROR(bFnRet);

	bFnRet = initBgMap();
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(pHunterCabinObject);

	bRet = THTRUE;
Exit0:
	return bRet;
}



//LayerColor* pBgTmp = LayerColor::create(ccc4(0xff, 0x0, 0x0, 0x80), 640, 480);
//this->addChild(pBgTmp);