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

	thBool init(CHARACTER_DESC_PTR pDesc, CHARACTER_ANI_DESC_PTR* arrpAniDesc, DEFTOWER_DESC_PTR ptTowerDesc);
	thBool initCharaterAnimate(CHARACTER_ANI_DESC_PTR pAniDesc, const int cnGroupPos);
	virtual void uninit();

	virtual void getCharaterFrameInfo(CHARACTER_FRAMEINFO_PTR* ppRet);
	virtual void getCharaterFrameInfoInGroup(const char* cszpTag, CHARACTER_FRAMEINFO_PTR* ppRet);
	
	thBool initDefTowerWarriorsDesc(CHARACTER_DESC_PTR* arrpCharacterDesc, enum THEM_CHARACTER_LEVEL emLevel, const short csSize);

	virtual void onMouseUp(EventMouse* pEvent);
	virtual void onMouseDown(EventMouse* pEvent);
	virtual void onMouseMove(EventMouse* pEvent);

	virtual void update(float dt);
	virtual thBool globalMonitoring();

private:
	thBool _initBaiscAnimate(CHARACTER_ANI_DESC_PTR* arrpAniDesc);
	void _getSpArrayVacantPos(short* psRet);

private:
	CHARACTER_FRAMEINFO_PTR m_pTower;
	CHARACTER_FRAMEINFO_PTR* m_arrpSpGroup;
	CHARACTER_ANI_FRAMEINFO_PTR* m_arrpAniGroup;
	DEFTOWER_DESC_PTR m_ptTowerStatus;
	/* 防御塔战士精灵描述，每个等级 3 种类型. */
	CHARACTER_DESC_PTR m_arrpWarriorsDesc[THEM_CHARACTER_LEVEL::CHARACTER_MAXLEVEL][THMAX_TARLEVEL_DEFTOWER_WARRIORS];
};



#endif // !__THDEFTOWER_H__
