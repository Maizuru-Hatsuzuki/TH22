/********************************************************
* Filename		: thCharacter.h
* Creator		: ac
* Date time		: 2024.04.14
* Description	: base character.
********************************************************/


#ifndef __THCHARACTER_H__
#define __THCHARACTER_H__

#include "stdafx.h"
#include "thBase.h"




enum THEM_PLAYERLEVEL_MOVESPEED
{
	MOVESPEED_LOW,
	MOVESPEED_NARMAL,
	MOVESPEED_HIGH,
	MOVESPEED_HIGHEX
};

class CThBaseCharacter:
	public Node
{
public:
	CThBaseCharacter();
	~CThBaseCharacter();

private:

};


class CThBaseCharacterAction
{
public:
	static CThBaseCharacterAction* getInstance();
	
	thBool createActionMoveTo(float fSpeed, float fDstX, float fDstY, FiniteTimeAction** arrfnCallback, Sequence** ppRet) noexcept;

private:
	CThBaseCharacterAction();
	~CThBaseCharacterAction();
	CThBaseCharacterAction(const CThBaseCharacterAction& pSelf);
	const CThBaseCharacterAction& operator=(const CThBaseCharacterAction& pSelf);

private:
	static CThBaseCharacterAction* m_pSelf;
};



#endif // !__THCHARACTER_H__
