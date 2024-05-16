/********************************************************
* Filename		: thCcAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module header.
********************************************************/

#ifndef __thCcAnimation_H__
#define __thCcAnimation_H__

#include "thCcCharacter.h"


class CthCcAnimation
{
public:
	static CthCcAnimation* getInstance();
	thBool createPlayAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet);
	thBool createLoadingBar(Sprite* pSpLoading, const float cfInterval, const float cfPersent, CHARACTER_DESC_PTR ptSpLoading, ProgressTo** ppActionRet, ProgressTimer** ppLoadingPicRet);

public:


private:
	CthCcAnimation();
	~CthCcAnimation();
	CthCcAnimation(const CthCcAnimation& pSelf);
	const CthCcAnimation& operator=(const CthCcAnimation& pSelf);

private:
	static CthCcAnimation* m_pSelf;
};

#endif