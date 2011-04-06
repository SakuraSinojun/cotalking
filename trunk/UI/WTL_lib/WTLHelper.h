#ifndef _WTLHELPER_H_
#define _WTLHELPER_H_

#pragma once

#ifndef STRICT
#define STRICT
#endif

#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES

#include <sdkddkver.h>      //Definition of WINVER/_WIN32_WINNT/_WIN32_IE/_RICHEDIT_VER

//ATL header files
#include <ATLHelper.h>

//WTL header files
#include <atlwin.h>     //CWindowImpl
#include <atlframe.h>   //CFrameWindowImpl
#include <atlcrack.h>   //BEGIN_MSG_MAP_EX, message crack
#include <atlctrls.h>   //Wrap for Win32 native controls
#include <atlctrlw.h>   //CCommandBarCtrl
#include <atlctrlx.h>   //CBitmapButton,CHyperLink,CCheckListViewCtrl(Additional WTL controls)
#include <atltheme.h>   //Theme support
#include <atldlgs.h>    //WTL dialogs,common dialogs, file/folder dialogs
#include <atlddx.h>     //DDX support
#include <atlsplit.h>	//Splitter window support
#include <atlmisc.h>    //CString,CPoint,CSize...
#include <atlimage.h>   //CImage...

#if _WIN32_WINNT >= 0x0600
#include <atldwm.h>     //CDwm,CDwmImpl,CDwmThumbnailT,CAeroControlImpl
#endif

#ifndef WTLHELPER_NOT_USE_COMMON_CONTROL_STYLE
#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//User defined message crack
#ifndef PRE_PROCESS_MESSAGE
//LRESULT OnPreProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
#define PRE_PROCESS_MESSAGE(func) \
    { \
        SetMsgHandled(TRUE); \
        lResult = func(uMsg, wParam, lParam); \
        if(IsMsgHandled()) \
            return TRUE; \
    }
#endif

#ifndef REGISTERED_MESSAGE_HANDLER
//LRESULT OnPreProcessMessage(UINT uRegisteredMsg, WPARAM wParam, LPARAM lParam)
#define REGISTERED_MESSAGE_HANDLER(uRegisteredMsg, func) \
    if(uMsg == uRegisteredMsg) \
    { \
        SetMsgHandled(TRUE); \
        lResult = func(uMsg, wParam, lParam); \
        if(IsMsgHandled()) \
            return TRUE; \
    }
#endif

//WinTraits
typedef CWinTraits<WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN> CSimpleWinTraits;

#endif