/********************************************************
* Filename		: thSceneDungeon.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: dungeon scene. header
********************************************************/

#ifndef __THSCENEDUNGEON_H__
#define __THSCENEDUNGEON_H__


#include "stdafx.h"

class thSceneDungeon:
	public cocos2d::Scene
{
public:
	thSceneDungeon();
	~thSceneDungeon();

	static cocos2d::Scene* createScene();
	bool init() override;

	thBool initBgMap();
	thBool initSceneAllPlist();

	CREATE_FUNC(thSceneDungeon);

private:

};



#endif
