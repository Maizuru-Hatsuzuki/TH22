/********************************************************
* Filename		: thBaseAnimation.h
* Creator		: ac
* Date time		: 2024.04.11
* Description	: base fsm header.
********************************************************/

#ifndef __THBASEFSM_H__
#define __THBASEFSM_H__


#include "thBase.h"


enum THEM_FSMTYPE
{

};

class CTHBaseFSM
{
public:
	CTHBaseFSM();
	~CTHBaseFSM();

	thBool init();
	thBool thFsmDriver();
	thBool thFsmSwitch();
	thBool thFsmPause();

private:
	thBool m_bWorking;
	const char* szFSMDesc;
};



#endif // !__THBASEFSM_H__
