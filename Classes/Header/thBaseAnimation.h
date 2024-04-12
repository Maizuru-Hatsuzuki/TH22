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
	char szarrFrameAni[32];
	unsigned int nFrameAniCount;
	unsigned int nFrameAniBegin;
	unsigned int nFrameAniEnd;
	float fDelayPerUnit;
	int nLoops;
	bool bResFirstFrame;
	bool bReverseReturn;
};
typedef struct _tThAnimationDesc THANIMATION_DESC, * THANIMATION_DESC_PTR;

class CTHBaseAnimation
{
public fn:
	static CTHBaseAnimation* getInstance();
	thBool createPlayAnimationWithPList(THANIMATION_DESC_PTR ptAniDesc, Animate** ppRet);

public var:


private fn:
	CTHBaseAnimation();
	~CTHBaseAnimation();
	CTHBaseAnimation(const CTHBaseAnimation& pSelf);
	const CTHBaseAnimation& operator=(const CTHBaseAnimation& pSelf);

private var:
	static CTHBaseAnimation* m_pSelf;
};

#endif
