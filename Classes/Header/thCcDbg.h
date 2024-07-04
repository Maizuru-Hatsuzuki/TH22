/********************************************************
* Filename		: thCCDbg.h
* Creator		: ac
* Date time		: 2024.05.09
* Description	: th cocos2dx engine debug.
********************************************************/

#ifndef __THCCDBG_H__
#define __THCCDBG_H__

#include "thCcCharacter.h"


#ifndef THDBG_FLAG_MOVERANGEHALO
#define THDBG_FLAG_MOVERANGEHALO 0
#endif

#ifndef THDBG_FLAG_QMMOUSEUP
#define THDBG_FLAG_QMMOUSEUP 0
#endif


class CThCCDbg :
	public Node
{
public:
	CThCCDbg();
	virtual ~CThCCDbg();

	thBool getDbgInfoInScreenObject(const char* cszpText, Label** ppRet);

private:

};


class CThCCDbgWithCharacter :
	public CThCCDbg
{

public:
	static CThCCDbgWithCharacter* getInstance();
	thBool init();
	thBool createShowCharacterDbgFrameInfo();
	thBool setDbgFrameInfo(CHARACTER_FRAMEINFO_PTR ptInfo);

public:


private:
	CThCCDbgWithCharacter();
	~CThCCDbgWithCharacter();
	CThCCDbgWithCharacter(const CThCCDbgWithCharacter& pSelf);
	const CThCCDbgWithCharacter& operator=(const CThCCDbgWithCharacter& pSelf);

private:
	Label* m_pLbDbg;
	static CThCCDbgWithCharacter* m_pSelf;
	char m_szarrDbgInfo[2048];
};

typedef CThCCDbgWithCharacter TDBG_CHARACTER;

#endif
