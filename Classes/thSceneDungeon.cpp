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
	PLAYER_DESC tPlayer =
	{
		"ichigoW0",
		"ichigoW0-removebg-preview",
		"ichigoW0-removebg-preview1",
		760.f,
		Director::getInstance()->getWinSize().height - 228.f,
		0.5f,
		false,
		false
	};
	PLAYER_DESC tHunterCabin =
	{
		"HunterCabin",
		NULL,
		NULL,
		435.f,
		Director::getInstance()->getWinSize().height - 165.f,
		0.3f,
		false,
		false
	};
	CThPlayer* pIchigoObject = new CThPlayer;
	CThHunterCabin* pHunterCabinObject = new CThHunterCabin;
	//LayerColor* pBgTmp = LayerColor::create(ccc4(0xff, 0x0, 0x0, 0x80), 640, 480);

	pIchigoObject->init(&tPlayer);
	pHunterCabinObject->init(&tHunterCabin);
	pHunterCabinObject->initMouseEvent(this);

	CThKeyboard::getInstance()->init(pIchigoObject);
	CThKeyboard::getInstance()->listener(this);

	//this->addChild(pBgTmp);
	bFnRet = initBgMap();
	TH_PROCESS_ERROR(bFnRet);
	this->addChild(pIchigoObject);
	this->addChild(pHunterCabinObject);

	bRet = THTRUE;
Exit0:
	return bRet;
}

