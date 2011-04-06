#include "stdafx.h"
#include "MainWindow.h"

typedef CWTLApplicationT<CMainWindow> CMainApp;
CMainApp g_MainApp;

int WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR lpCmdLine,int nShowCmd)
{
	g_MainApp.WinMain(hInstance,lpCmdLine,nShowCmd);
}