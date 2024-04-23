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


class CThDefTower:
	public CThBaseCharacter
{
public:
	CThDefTower();
	~CThDefTower();

	virtual thBool init(CHARACTER_DESC_PTR pDesc, CHARACTER_ANI_DESC_PTR* arrpAniDesc);
	thBool initCharaterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos);
	virtual void uninit();

	virtual void getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual void update(float dt);

private:
	thBool _initBaiscAnimate(CHARACTER_ANI_DESC_PTR* arrpAniDesc);

private:
	CHARACTER_FRAMEINFO_PTR m_pTower;
	CHARACTER_FRAMEINFO_PTR* m_arrpSpGroup;
	CHARACTER_ANI_FRAMEINFO_PTR* m_arrpAniGroup;
};



//class CThHunterCabin:
//	public CThBaseCharacter
//{
//public:
//	CThHunterCabin();
//	~CThHunterCabin();
//
//	thBool init(PLAYER_DESC_PTR ptPlayer);
//	void initMouseEvent(Node* pScene);
//	void initAnimation();
//	void onMouseUp(EventMouse* pEvent);
//	thBool createFighter();
//	thBool getPlayer(Sprite** pRet);
//
//private:
//	Sprite* m_pSpHunterCabin;
//	Animate* m_pAniOpenTheDoor;
//	Animate* m_pAniCloseTheDoor;
//};



#endif // !__THDEFTOWER_H__
