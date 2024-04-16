/********************************************************
* Filename		: thSceneDungeon.cpp
* Creator		: ac
* Date time		: 2024.04.11
* Description	: dungeon scene.
********************************************************/


#include "thBaseMacro.h"
#include "thPlayer.h"
#include "thSceneDungeon.h"
#include "thDeviceControl.h"


USING_NS_CC;

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

bool thSceneDungeon::init()
{
	bool bRet = THFALSE;
	bool bFnRet = THFALSE;
	PLAYER_DESC tPlayer =
	{
		"ichigoW0",
		"ichigoW0-removebg-preview",
		"ichigoW0-removebg-preview1.png",
		350.f,
		250.f,
		4.f
	};
	CThPlayer* pIchigoObject = new CThPlayer;
	
	LayerColor* pBgTmp = LayerColor::create(ccc4(0xff, 0x0, 0x0, 0x80), 640, 480);
	Sprite* pIchigo = NULL;

	pIchigoObject->init(&tPlayer, 4, 1, 5);
	pIchigoObject->getPlayer(&pIchigo);
	TH_PROCESS_ERROR(pIchigo);
	

	CThKeyboard::getInstance()->init(pIchigoObject);
	CThKeyboard::getInstance()->listener(this);

	this->addChild(pBgTmp);
	this->addChild(pIchigoObject);

	bRet = THTRUE;
Exit0:
	return bRet;
}

