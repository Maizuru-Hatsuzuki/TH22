/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base character fsm event header.
********************************************************/

#ifndef __THCHARATERFSM_H__
#define __THCHARATERFSM_H__

#include "thBase.h"
#include "thCharacter.h"


enum THEM_CHARACTERFSM_STATUS
{
	CMS_UNKNOW,
	CMS_STANDBY,
	CMS_MOVE,
	CMS_LEFTMOVE,
	CMS_RIGHTMOVE,
	CMS_JUMP,
	CMS_FLAG
};

/* CFE -> character fsm event */
typedef thBool(*THCALLBACK_CFE)(
	void* vpArgs
	);

struct _tThCharacterFSMDesc
{
	enum THEM_CHARACTERFSM_STATUS emStatus;
	const char* szpCharacterDesc;
	THCALLBACK_CFE fnFsmInit;
	THCALLBACK_CFE fnFsmUpdate;
	THCALLBACK_CFE fnFsmRelease;
	void* vpTagArgs;
};
typedef struct _tThCharacterFSMDesc THCHARACTERFSM_DESC, * THCHARACTERFSM_DESC_PTR;



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


/* Character event.*/
class CTHCharaterFSM:
	public CTHBaseCharacterFSM
{
public:
	CTHCharaterFSM();
	~CTHCharaterFSM();

	thBool init(THCHARACTERFSM_DESC_PTR pFsmDesc);
	virtual thBool fsmInit() noexcept;
	virtual thBool fsmUpdate() noexcept;
	virtual thBool fsmRelease() noexcept;

	void setCurFsmArgs(void* vpArgs, size_t ullSrcSize) noexcept;

private:
	void* m_vpCurFsmArgs;
	thBool bWorking;
	THCHARACTERFSM_DESC_PTR m_pFsmDesc;
};


#endif