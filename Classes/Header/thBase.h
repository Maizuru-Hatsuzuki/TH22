/********************************************************
* Filename		: thBase.h
* Creator		: ac
* Date time		: 2024.04.29
* Description	: all the base.
********************************************************/


#pragma warning(disable: 4102)
#pragma warning(disable: 26409)
#pragma warning(disable: 26429)
#pragma warning(disable: 26430)
#pragma warning(disable: 26433)
#pragma warning(disable: 26440)
#pragma warning(disable: 26446)
#pragma warning(disable: 26481)
#pragma warning(disable: 26482)
#pragma warning(disable: 26496)
#pragma warning(disable: 26812)


#ifndef __KBASE_H__
#define __KBASE_H__


#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)


#define THDLLEXPORT
#ifdef THDLLEXPORT
#define TH_DLLEXPORT __declspec(dllexport)
#else
#define TH_DLLEXPORT __declspec(dllimport)
#endif

#define THCALLBACK static

/* CC use. */
#define THMAX_CHARACTER_FSMSTATUS			16
#define THMAX_SEQACTION						16
#define THMAX_ANI_COUNT						32
#define THMAX_ANI_TAG						16
#define THMAX_SP_COUNT						64
#define THMAX_CHAR_DESC						128
#define THMAX_DEFTOWER_TARLEVEL_WARRIORS	4
#define THMAX_DEFTOWER_SYNC_ANI				32
#define THMAX_SKILL_LEVEL					3
#define THMAX_TOWER_SKILL_COUNT				4
#define THMAX_TOWER_SKILL_QM_COUNT			(THMAX_TOWER_SKILL_COUNT + 1)

#define THMOVESPEED_LOW_PLAYER 5.0
#define THMOVESPEED_NORMAL_PLAYER 4.5
#define THMOVESPEED_HIGH_PLAYER 3.5
#define THMOVESPEED_HIGHEX_PLAYER 3.0

#define THSP_FLAG_DIE 0
#define THSP_FLAG_CLEAN -1
#define THSP_FLAG_ENABLE 1
#define THSP_FLAG_DISABLE 2
/* 战士之间距离. */
#define THSP_WARRIOR_SEPARATION 25.f
/* 战士站位角度. */
#define THSP_WARRIOR_SEPARATION_ANGLE 40.f

#define TH_EVENTPRIORITY_QUICKMENU 1
#define TH_EVENTPRIORITY_DEFTOWER 2
#define TH_EVENTPRIORITY_DEFTOWERWARRIOR 3

#define THINI_DEFAULT_STR "NA"

#define THMALLOC(t, s) (t*)malloc(s)
#define THFREE(p) { if (p) { free(p); (p) = NULL; }}

#define TH_GETX(arr) arr[0]
#define TH_GETY(arr) arr[1]


#ifdef __cplusplus

typedef bool thBool;

#define THTRUE true
#define THFALSE false
#define THNEW_CLASS(p) new p()
#define THDELETE(p) { if (p) { delete (p); (p) = NULL; }}
#define TH_UNINIT_CHACDESC(p) { if (p) { CthCcCharacterLoadHandler::getInstance()->uninitCharacterDesc(p); (p) = NULL; }}
#define TH_GETBOTTOMY(p) ( p->getPositionY() - p->getBoundingBox().size.height * p->getAnchorPoint().y )
#define TH_GETTOPY(p) ( p->getPositionY() + p->getBoundingBox().size.height * p->getAnchorPoint().y )
#define TH_CREATE_CN_LABELTEXT(p, str, size, tColor, fPosX, fPosY, bVis, emAli, nWidth)	\
do																						\
{																						\
	p = Label::createWithTTF(str, "fonts\\FZKai.ttf", size);							\
	TH_PROCESS_ERROR(p);																\
	p->setColor(tColor);																\
	p->setPosition(fPosX, fPosY);														\
	p->setVisible(bVis);																\
	p->setWidth(nWidth);																\
	p->setAlignment(emAli);																\
} while (0);

#define TH_CHAC_LEVELUP(emCurLv)														\
do																						\
{																						\
	switch (emCurLv)																	\
	{																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_0:										\
	{																					\
		emCurLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1;								\
		break;																			\
	}																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_1:										\
	{																					\
		emCurLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_2;								\
		break;																			\
	}																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_2:										\
	{																					\
		emCurLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_3;								\
		break;																			\
	}																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_3:										\
	{																					\
		emCurLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4;								\
		break;																			\
	}																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_4:										\
	{																					\
		emCurLv = THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_5;								\
		break;																			\
	}																					\
	case THEM_CHARACTER_LEVEL::CHARACTER_LEVEL_5:										\
	{																					\
		break;																			\
	}																					\
	default:																			\
	{																					\
		break;																			\
	}																					\
	}																					\
} while (0);


class CTHCcBaseHandler
{
public:
	static CTHCcBaseHandler* getInstance();

	thBool getSpriteFrameExists(const char* cszpSpName);
	void setShowWinMouseCursor(thBool bIsShow);
	thBool setSceneAllSpPlist(enum THEM_SCENE emScene, thBool bIsLoad);

	thBool scanPlistFiles(const char* cszpPath, int* pnPlistCnt, thBool bIsLoad);
	thBool splitFileSuffix(char* szpFile, const char* cszpSuffix, char* szpFilenameRet);

private:
	CTHCcBaseHandler();
	~CTHCcBaseHandler();
	CTHCcBaseHandler(const CTHCcBaseHandler& pSelf);
	const CTHCcBaseHandler& operator=(const CTHCcBaseHandler& pSelf);

private:
	static CTHCcBaseHandler* m_pSelf;
};

#else

typedef int thBool;

#define THTRUE 1
#define THFALSE 0

#endif	

typedef thBool* pthBool;


enum THEM_SCENE
{
	THSCENE_DUNGEON
};

enum THEM_DELAY_UNINIT_FLAG
{
	FLAG_NOTNEED_UNINIT,
	FLAG_NEED_UNINIT,
	FLAG_UNITING,
	FLAG_UNINIT,
	FLAG_UNINIT_COMPLETE
};



#endif
