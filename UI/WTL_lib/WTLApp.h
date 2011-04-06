#ifndef _WTLAPP_H_
#define _WTLAPP_H_

#pragma once
#include <Windows.h>
#include <ATLHelper.h>

#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")

#if (_MSC_VER>=1600)  //VC2010 or higher
#include <type_traits>
#endif

//Single instance application support
#pragma data_seg("Shared")
__declspec(selectany) volatile HWND _g_hMainWindow = NULL;
#pragma data_seg()

#pragma comment(linker,"/Section:Shared,RWS")

namespace WTL
{

//Get the global AppModule pointer
__declspec(selectany) CAppModule* _g_pAppModule = NULL;
inline CAppModule& AtlGetAppModule()
{
	return *_g_pAppModule;
}

//////////////////////////////////////////////////////////////////////////
//
//CWTLAppTraits class
//
enum WTLAppInitOption
{
	WTLAPP_INIT_NONE = 0x00,
	WTLAPP_INIT_GDIPLUS = 0x01,
	WTLAPP_INIT_COM = 0x02
};

template<DWORD t_InitOption>
class CWTLAppTraits
{
public:
	static DWORD GetInitOption()
	{
		return t_InitOption;
	}
};

typedef CWTLAppTraits<WTLAPP_INIT_NONE> CWTLAppTraitsInitNone;
typedef CWTLAppTraits<WTLAPP_INIT_GDIPLUS> CWTLAppTraitsInitGdiplus;
typedef CWTLAppTraits<WTLAPP_INIT_COM> CWTLAppTraitsInitCom;
typedef CWTLAppTraits<WTLAPP_INIT_GDIPLUS | WTLAPP_INIT_COM> CWTLAppTraitsInitGdiplusCom;

//////////////////////////////////////////////////////////////////////////
//
//CWTLApplicationImpl class
//
template<typename T, typename TMainWindow, typename TWTLAppTraits = CWTLAppTraitsInitNone>
class ATL_NO_VTABLE CWTLApplicationImpl :
	public CAppModule
{
private:
	ULONG_PTR m_GdiplusToken;

public:
	//Call this method in the application's entry point[_tWinMain(...)]
	int WinMain(HINSTANCE hInstance,LPTSTR lpCmdLine,int nShowCmd)
	{
#if (_MSC_VER>=1600)  //VC2010 or higher
		//Ensure that TMainWindow is derived from CWindow
		static_assert(std::is_base_of<CWindow,TMainWindow>::value,
			"TMainWindow class needs to be CWindow based.");
#endif
		UNREFERENCED_PARAMETER(lpCmdLine);

		_g_pAppModule = this;

		T* pT = static_cast<T*>(this);
		//Do additional work on starting up, if any
		if(!(pT->OnStartup()))
			return 0;

		//Initialize COM
		BOOL initCom = TWTLAppTraits::GetInitOption() & WTLAPP_INIT_COM;
		HRESULT hr = S_OK;
		if (initCom)
		{
			// If you are running on NT 4.0 or higher you can use the following 
			// call instead to make the EXE free threaded. This means that calls 
			// come in on a random RPC thread.
			//	hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
			hr = ::CoInitialize(NULL);
			CHECKHR(hr);
			if (FAILED(hr))
				return 0;
		}

		//Initialize GDI+
		BOOL initGdiplus = TWTLAppTraits::GetInitOption() & WTLAPP_INIT_GDIPLUS;
		if (initGdiplus)
		{
			Gdiplus::GdiplusStartupInput input;
			Gdiplus::GdiplusStartupOutput output;
			Gdiplus::GdiplusStartup(&m_GdiplusToken,&input,&output);
		}

		// This resolves ATL window thunking problem when Microsoft Layer 
		// for Unicode (MSLU) is used
		::DefWindowProc(NULL, 0, 0, 0L);

		pT->OnInitCommonControl();

		//CAppModule::Init()
		hr = Init(NULL, hInstance);
		CHECKHR(hr);
		int nRet = 0;
		{
			CMessageLoop msgLoop;
			//CAppModule::AddMessageLoop()
			AddMessageLoop(&msgLoop);

			TMainWindow wndMain;
			_g_hMainWindow = wndMain.Create(NULL,CWindow::rcDefault);
			if(_g_hMainWindow == NULL)
			{
				ATLTRACE(_T("Failed to create Main window!\n"));
				return 0;
			}
			wndMain.ShowWindow(nShowCmd);
			wndMain.UpdateWindow();

			nRet = msgLoop.Run();
			//CAppModule::RemoveMessageLoop()
			RemoveMessageLoop();

			_g_hMainWindow = NULL;
		}

		//CAppModule::Term()
		Term();

		//Uninitialize GDI+
		if (initGdiplus)
			Gdiplus::GdiplusShutdown(m_GdiplusToken);

		//Uninitialize COM
		if (initCom)
			::CoUninitialize();

		//Do additional work on shutting down, if any
		pT->OnShutdown();

		_g_pAppModule = NULL;

		return nRet;
	}

protected:
	//Override this method to do additional work on starting up
	BOOL OnStartup()
	{
		return TRUE;
	}

	//Override this method to do additional work on shutting down
	void OnShutdown()
	{}

	//Override this method to support other native controls
	void OnInitCommonControl()
	{
		AtlInitCommonControls(ICC_STANDARD_CLASSES|ICC_COOL_CLASSES|ICC_BAR_CLASSES|ICC_WIN95_CLASSES);
	}

protected:
	//Single instance application support
	BOOL IsAnotherInstanceRunning()
	{
		return (_g_hMainWindow!=NULL);
	}

	void TakeAnotherInstanceForeground()
	{
		if (_g_hMainWindow)
		{
			CWindow mainWnd(_g_hMainWindow);
			if (mainWnd.IsIconic())
				mainWnd.ShowWindow(SW_RESTORE);
			mainWnd.SetForegroundWindow();
		}
	}
}; // class CWTLApplicationImpl

//////////////////////////////////////////////////////////////////////////
//
//CWTLApplicationT class
//
template<typename TMainWindow, typename TWTLAppTraits = CWTLAppTraitsInitNone>
class CWTLApplicationT :
	public CWTLApplicationImpl<CWTLApplicationT<TMainWindow, TWTLAppTraits>, TMainWindow, TWTLAppTraits>
{};

} // namespace WTL

#endif