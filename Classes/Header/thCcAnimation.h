/********************************************************
* Filename		: thCcAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module header.
********************************************************/

#ifndef __thCcAnimation_H__
#define __thCcAnimation_H__

#include "thCcCharacter.h"


class CthCcFrameByFrameAnimation
{
public:
	static CthCcFrameByFrameAnimation* getInstance();
	thBool createAnimationWithPList(CHARACTER_ANI_DESC_PTR ptAniDesc, Animate** ppRet);
	thBool createAnimationWithPListIni(const char* cszpIniPath, CHARACTER_ANI_FRAMEINFO_PTR* ppRet);
	thBool createLoadingBar(Sprite* pSpLoading, const float cfInterval, const float cfPersent, CHARACTER_DESC_PTR ptSpLoading, ProgressTo** ppActionRet, ProgressTimer** ppLoadingPicRet);

public:


private:
	CthCcFrameByFrameAnimation();
	~CthCcFrameByFrameAnimation();
	CthCcFrameByFrameAnimation(const CthCcFrameByFrameAnimation& pSelf);
	const CthCcFrameByFrameAnimation& operator=(const CthCcFrameByFrameAnimation& pSelf);

private:
	static CthCcFrameByFrameAnimation* m_pSelf;
};

#endif
