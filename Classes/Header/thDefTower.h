/********************************************************
* Filename		: thDefTower.h
* Creator		: ac
* Date time		: 2024.04.21
* Description	: def tower header.
********************************************************/


#ifndef __THDEFTOWER_H__
#define __THDEFTOWER_H__

#include "thPlayer.h"
#include "thBase.h"


class CThHunterCabin:
	public CThBaseCharacter
{
public:
	CThHunterCabin();
	~CThHunterCabin();

	thBool init(PLAYER_DESC_PTR ptPlayer);
	void initMouseEvent(Node* pScene);
	void initAnimation();
	thBool getPlayer(Sprite** pRet);

private:
	Sprite* m_pSpHunterCabin;
	Animate* m_pAniOpenTheDoor;
	Animate* m_pAniCloseTheDoor;
};



#endif // !__THDEFTOWER_H__
