/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base animation module header.
********************************************************/

#ifndef __THBASEANIMATION_H__
#define __THBASEANIMATION_H__

#include "thBase.h"


struct _tThAnimationDesc
{
	char szarrFrameAni[64];
	unsigned int nFrameAniCount;
	unsigned int nFrameAniBegin;
	unsigned int nFrameAniEnd;
	float fDelayPerUnit;
	float fRevolve;
	int nLoops;
	bool bResFirstFrame;
	bool bReverseReturn;
};
typedef struct _tThAnimationDesc THANIMATION_DESC, * THANIMATION_DESC_PTR;

class CThBaseAnimation
{
public fn:
	static CThBaseAnimation* getInstance();
	thBool createPlayAnimationWithPList(THANIMATION_DESC_PTR ptAniDesc, Animate** ppRet);

public var:


private fn:
	CThBaseAnimation();
	~CThBaseAnimation();
	CThBaseAnimation(const CThBaseAnimation& pSelf);
	const CThBaseAnimation& operator=(const CThBaseAnimation& pSelf);

private var:
	static CThBaseAnimation* m_pSelf;
};

#endif
