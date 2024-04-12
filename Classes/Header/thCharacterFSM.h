/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base character fsm event header.
********************************************************/

#ifndef __THCHARATERFSM_H__
#define __THCHARATERFSM_H__

#include "thBase.h"


enum THEM_CHARACTERFSM_STATUS
{
	CMS_UNKNOW,
	CMS_STANDBY,
	CMS_MOVE,
	CMS_JUMP,
	CMS_FLAG
};

struct _tThCharacterFSMDesc
{
	const char* szpCharacterDesc;
};
typedef struct _tThCharacterFSMDesc THCHARACTERFSM_DESC, * THCHARACTERFSM_DESC_PTR;
typedef thBool(*THCALLBACK_CFE)(
	void* pThreadParameter
	);

class CTHBaseCharacterFSM
{
public fn:
	CTHBaseCharacterFSM();
	virtual ~CTHBaseCharacterFSM();

	virtual thBool fsmInit() noexcept = 0;
	virtual thBool fsmUpdate() noexcept = 0;
	virtual thBool fsmRelease() noexcept = 0;

private:
};

/* Player event.*/
class CTHCharaterFSM:
	public CTHBaseCharacterFSM
{
public:
	CTHCharaterFSM();
	~CTHCharaterFSM();

	virtual thBool fsmInit(THCALLBACK_CFE fn, void* vpArgs) noexcept;
	virtual thBool fsmUpdate(THCALLBACK_CFE fn, void* vpArgs) noexcept;
	virtual thBool fsmRelease(THCALLBACK_CFE fn, void* vpArgs) noexcept;

private:
	thBool bWorking;
};



#endif