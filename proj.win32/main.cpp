/********************************************************
* Filename		: thCharacter.cpp
* Creator		: ac
* Date time		: 2024.04.14
* Description	: main.
********************************************************/


#include "cocos2d.h"
#include "main.h"
#include "AppDelegate.h"
#include "thBaseFSM.h"


USING_NS_CC;

thBool thMainLogicFrame()
{
    thBool bRet = THFALSE;
   

    bRet = THTRUE;
Exit0:
    return bRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    FILE* pConsole = NULL;
    AllocConsole();
    pConsole = freopen("CONIN$", "r", stdin);
    pConsole = freopen("CONOUT$", "w", stdout);
    pConsole = freopen("CONOUT$", "w", stderr);
#endif

    int nRet = -1;
    srand(time(0));

    AppDelegate app;
    nRet = Application::getInstance()->run();

    nRet = 0;
    return nRet;
}
