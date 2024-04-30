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


	CThDefTower* pHunterCabinObject = new CThDefTower;

	/* tower */
	CHARACTER_ANI_MAP tBatAniMap = { 0 };
	tBatAniMap.cszpAniMoveTransverse = "batMove";
	CHARACTER_ANI_MAP tTowerAniMap = { 0 };
	tTowerAniMap.cszpAniOpenTheDoor = "TowerInitWarriorsOpenDoor";
	tTowerAniMap.cszpAniCloseTheDoor = "TowerInitWarriorsCloseDoor";
	CHARACTER_DESC tTowerDesc =
	{
		"Hunter cabin",
		"image\\sprite\\kr\\hunterCabin2.png",
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.3f, false, false, 100, 100, 10, 1, 10, 10, 10, 0,
		THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_NARMAL,
		THEM_CHARACTER_TYPE::CHARACTER_DEFTOWER,
		& tTowerAniMap
	};

	CHARACTER_DESC tBullet =
	{
		"bullet",
		"image\\sprite\\kr\\smallstar.png",
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.7f, false, false, 100, 100, 10, 1, 10, 10, 10, 0,
		THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_NARMAL,
		THEM_CHARACTER_TYPE::CHARACTER_BULLET,
		&tTowerAniMap
	};

	CHARACTER_ANI_DESC tAniBatMove =
	{
		2,
		3,
		4,
		0.35f,
		-1,
		false,
		"image\\sprite\\kr\\BatMovePlist",
		"batMove",
		"batMove"
	};
	DEFTOWER_DESC tTowerStatusDesc = { 4, 4, 1, 0, 16, 2, 100, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, THEM_BULLET_TYPE::SHOOTCHAT_RING };

	/* warriors */
	CHARACTER_DESC tTowerWarriorA =
	{
		"hunter cabin warrior A",
		"image\\sprite\\kr\\batMove1.png",
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.25f, false, false, 100, 100, 10, 3, 10, 10, 10, 0,
		THEM_CHARARCTERLEVEL_MOVESPEED::MOVESPEED_NARMAL,
		THEM_CHARACTER_TYPE::CHARACTER_TOWER_WARRIOR,
		& tBatAniMap
	};
	/* open the door */
	CHARACTER_ANI_DESC tAniOpD =
	{
		4,
		2,
		5,
		0.07f,
		1,
		false,
		"image\\sprite\\kr\\hunterCabinPlist",
		"hunterCabin",
		"TowerInitWarriorsOpenDoor"
	};

	/* close the door */
	CHARACTER_ANI_DESC tAniClD =
	{
		4,
		5,
		2,
		0.07f,
		1,
		false,
		"image\\sprite\\kr\\hunterCabinPlist",
		"hunterCabin",
		"TowerInitWarriorsCloseDoor"
	};

	CHARACTER_ANI_DESC_PTR arrpAniDesc[THMAX_ANI_COUNT] = { &tAniBatMove, &tAniOpD, &tAniClD };
	CHARACTER_DESC_PTR arrpWarriors[THMAX_DEFTOWER_TARLEVEL_WARRIORS] = { &tTowerWarriorA, NULL, NULL };
	
	bFnRet = pHunterCabinObject->init(&tTowerDesc, &tBullet, arrpAniDesc, &tTowerStatusDesc, arrpWarriors, THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1, 1);
	TH_PROCESS_ERROR(bFnRet);

	bFnRet = initBgMap();
	TH_PROCESS_ERROR(bFnRet);

	this->addChild(pHunterCabinObject);

	bRet = THTRUE;
Exit0:
	return bRet;
}

