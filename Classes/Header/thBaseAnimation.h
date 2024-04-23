/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module header.
********************************************************/

#ifndef __THBASEANIMATION_H__
#define __THBASEANIMATION_H__

#include "thCharacter.h"


class CThBaseAnimation
{
public:
	static CThBaseAnimation* getInstance();
	thBool createPlayAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet);

public:


private:
	CThBaseAnimation();
	~CThBaseAnimation();
	CThBaseAnimation(const CThBaseAnimation& pSelf);
	const CThBaseAnimation& operator=(const CThBaseAnimation& pSelf);

private:
	static CThBaseAnimation* m_pSelf;
};

#endif
