/********************************************************
* Filename		: thDeviceControl.h
* Creator		: ac
* Date time		: 2024.04.15
* Description	: device ctrl.
********************************************************/


#ifndef __THDEVICECONTROL_H__
#define __THDEVICECONTROL_H__

#include "thBase.h"
#include "thPlayer.h"


class CThKeyboard
{
public:

	thBool init(CThPlayer* pCurTag);
	thBool listener(Node* pLister);

	static CThKeyboard* getInstance();

private:
	thBool _KeyDownEvent(EventListenerKeyboard* pKeyboard);
	thBool _KeyUpEvent(EventListenerKeyboard* pKeyboard);

	CThKeyboard();
	~CThKeyboard();
	CThKeyboard(const CThKeyboard& pSelf);
	const CThKeyboard& operator=(const CThKeyboard& pSelf);

private:
	CThPlayer * m_pListener;
	static CThKeyboard* m_pSelf;
};


#endif
