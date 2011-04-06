#pragma once
#include "stdafx.h"
#include "resource.h"
#include <ImageHelpers.h>
#include <atlwinmisc.h>
#include "ChatRecord.h"

//#define NOT_USE_SYS_FRAME

#ifdef NOT_USE_SYS_FRAME
enum
{
	TITLEBAR_HEIGHT = 25
};

typedef CWinTraits<WS_POPUP|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_MAXIMIZEBOX> CMainWinTraits;
#else
typedef CWinTraits<WS_OVERLAPPEDWINDOW> CMainWinTraits;
#endif

class CMainWindow :
	public CWindowImpl<CMainWindow,CWindow,CMainWinTraits>
{
public:
	DECLARE_WND_CLASS(_T("GroupTalk_MainWindow"))
	BEGIN_MSG_MAP(CMainWindow)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
		MSG_WM_CREATE(OnCreate)
		COMMAND_HANDLER_EX(IDC_BUTTON_SENDMESSAGE,BN_CLICKED,OnSendMessageBtnClicked)
		COMMAND_HANDLER_EX(IDC_BUTTON_CLOSE,BN_CLICKED,OnCloseBtnClicked)
		COMMAND_RANGE_HANDLER_EX(ID_GROUP_COMMUNITY,ID_GROUP_SETTING,OnCmd)
		NOTIFY_CODE_HANDLER_EX(TTN_GETDISPINFO,OnGetDispInfo)
#ifdef NOT_USE_SYS_FRAME
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
#endif
	END_MSG_MAP()

	CMainWindow()
	{}

	int OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		//Load background images
		m_bkgndTop = ::AtlLoadGdiplusImage(IDB_TOP,_T("PNG"));
		m_bkgndMid = ::AtlLoadGdiplusImage(IDB_MID,_T("PNG"));
		m_bkgndBottom = ::AtlLoadGdiplusImage(IDB_BOTTOM,_T("PNG"));
		m_bkgndRightTop = ::AtlLoadGdiplusImage(IDB_RIGHTTOP,_T("PNG"));
		m_bkgndTop.GetSize(m_sizeTop);
		m_bkgndMid.GetSize(m_sizeMid);
		m_bkgndBottom.GetSize(m_sizeBottom);
		m_bkgndRightTop.GetSize(m_sizeRightTop);

		//Create top toolbar
		m_imagelistToolbarTop.CreateFromImage(IDB_TOOLBAR_TOP,32,0,CLR_NONE,
			IMAGE_BITMAP,LR_CREATEDIBSECTION);
		ATLASSERT(m_imagelistToolbarTop);

		DWORD style = WS_CHILD|WS_VISIBLE
			|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_LIST
			|CCS_NOPARENTALIGN|CCS_NORESIZE|CCS_NODIVIDER;
		DWORD styleEx = 0 ;
		m_ToolbarTop.Create(m_hWnd,rcDefault,_T(""),style,styleEx,IDC_TOOLBAR_TOP);
		//m_ToolbarTop.SetExtendedStyle(TBSTYLE_EX_MIXEDBUTTONS);
		m_ToolbarTop.SetImageList(m_imagelistToolbarTop);
		m_ToolbarTop.SetIndent(0);
		m_ToolbarTop.SetButtonSize(32,32);
		m_ToolbarTop.SetMaxTextRows(0);
		TBBUTTON buttonsTop[] = {
			{0, ID_GROUP_COMMUNITY,	TBSTATE_ENABLED, BTNS_BUTTON|BTNS_AUTOSIZE,0,0,(INT_PTR)_T("群社区")},
			{1, ID_GROUP_SHARE,		TBSTATE_ENABLED, BTNS_BUTTON|BTNS_AUTOSIZE,0,0,(INT_PTR)_T("群共享")},
			{2, ID_GROUP_ALBUM,		TBSTATE_ENABLED, BTNS_BUTTON|BTNS_AUTOSIZE,0,0,(INT_PTR)_T("群相册")},
			{4, ID_GROUP_GAME,		TBSTATE_ENABLED, BTNS_BUTTON|BTNS_AUTOSIZE,0,0,(INT_PTR)_T("小游戏")},
			{8, ID_GROUP_SETTING,	TBSTATE_ENABLED, BTNS_BUTTON|BTNS_AUTOSIZE,0,0,(INT_PTR)_T("群设置")}
		};
		BOOL res = m_ToolbarTop.AddButtons(ARRAYSIZE(buttonsTop),buttonsTop);
		ATLASSERT(res);

		//Create mid toolbar
		m_imagelistToolbarMid.CreateFromImage(IDB_TOOLBAR_MID,24,0,CLR_NONE,
			IMAGE_BITMAP,LR_CREATEDIBSECTION);
		ATLASSERT(m_imagelistToolbarMid);

		m_ToolbarMid.Create(m_hWnd,rcDefault,_T(""),style,styleEx,IDC_TOOLBAR_TOP);
		m_ToolbarMid.SetImageList(m_imagelistToolbarMid);
		m_ToolbarMid.SetIndent(0);
		m_ToolbarMid.SetButtonSize(24,24);
		TBBUTTON buttonsMid[] = {
			{0, ID_INPUT_TEXTFORMAT,	TBSTATE_ENABLED, BTNS_AUTOSIZE,0,0,(INT_PTR)_T("设置字体格式")},
			{1, ID_INPUT_EMOTION,		TBSTATE_ENABLED, BTNS_AUTOSIZE,0,0,(INT_PTR)_T("发送表情")},
			{3, ID_INPUT_PICTURE,		TBSTATE_ENABLED, BTNS_AUTOSIZE,0,0,(INT_PTR)_T("发送图片")}
		};
		res = m_ToolbarMid.AddButtons(ARRAYSIZE(buttonsMid),buttonsMid);
		ATLASSERT(res);

		//Create control font
		m_font = ::AtlCreateControlFont();

		//Create input edit
		style = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL;
		m_EditInput.Create(m_hWnd,rcDefault,_T("Type message here..."),style,0,IDC_EDIT_INPUT);
		m_EditInput.SetFont(m_font);

		//Create output edit
		::LoadLibrary(CRichEditCtrl::GetLibraryName());
		style = WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|ES_MULTILINE|ES_READONLY|ES_AUTOVSCROLL|WS_VSCROLL;
		m_EditOutput.Create(m_hWnd,rcDefault,_T(""),style,0,IDC_EDIT_OUTPUT);
		m_EditOutput.SetFont(m_font);
		CHECK_HANDLE(m_EditOutput);

		style = WS_CHILDWINDOW|WS_VISIBLE|WS_VSCROLL|LBS_NOTIFY|LBS_NOINTEGRALHEIGHT;
		styleEx = WS_EX_CLIENTEDGE|WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR;
		m_ListboxMemberList.Create(m_hWnd,rcDefault,_T(""),style,styleEx,IDC_LISTBOX_MEMBER);
		m_ListboxMemberList.SetFont(m_font);
		for (int i=0;i<8;i++)
		{
			m_ListboxMemberList.InsertString(-1,_T("踏雪无痕"));
			m_ListboxMemberList.InsertString(-1,_T("pearry"));
			m_ListboxMemberList.InsertString(-1,_T("喜之狼"));
			m_ListboxMemberList.InsertString(-1,_T("米虫"));
			m_ListboxMemberList.InsertString(-1,_T("nbboy"));
		}

		// Create "Send message" button and "Close" button
		style = WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON;
		m_btnSendMessage.Create(m_hWnd,rcDefault,_T("发送"),style,0,IDC_BUTTON_SENDMESSAGE);
		m_btnSendMessage.SetFont(m_font);
		m_btnClose.Create(m_hWnd,rcDefault,_T("关闭"),style,0,IDC_BUTTON_CLOSE);
		m_btnClose.SetFont(m_font);

		SetWindowPos(HWND_DESKTOP,0,0,586,480,SWP_NOZORDER);
		CenterWindow();

		return 0;
	}

	void OnFinalMessage(HWND)
	{
		PostQuitMessage(0);
	}

	BOOL OnEraseBkgnd(CDCHandle dc)
	{
		CRect rc;
		GetClientRect(&rc);

		//Default back ground color
		const COLORREF BKGND_COLOR = RGB(232,242,249);
		dc.FillSolidRect(&rc,BKGND_COLOR);

		CDC dcBmp;
		dcBmp.CreateCompatibleDC(dc);
		HBITMAP hOldBmp = dcBmp.SelectBitmap(m_bkgndTop);
		dc.StretchBlt(0,0,rc.Width(),m_sizeTop.cy,
			dcBmp,0,0,m_sizeTop.cx,m_sizeTop.cy,SRCCOPY);

		dcBmp.SelectBitmap(m_bkgndMid);
		dc.StretchBlt(0,rc.Height()-140,m_cxLeft-5,m_sizeMid.cy,
			dcBmp,0,0,m_sizeMid.cx,m_sizeMid.cy,SRCCOPY);

		dcBmp.SelectBitmap(m_bkgndBottom);
		dc.StretchBlt(0,rc.Height()-m_sizeBottom.cy,rc.Width(),m_sizeBottom.cy,
			dcBmp,0,0,m_sizeBottom.cx,m_sizeBottom.cy,SRCCOPY);

		dcBmp.SelectBitmap(m_bkgndRightTop);
		dc.StretchBlt(m_cxLeft,m_sizeTop.cy+5,m_cxRight,m_sizeRightTop.cy,
			dcBmp,0,0,m_sizeRightTop.cx,m_sizeRightTop.cy,SRCCOPY);
		dcBmp.SelectBitmap(hOldBmp);

		return TRUE;
	}

	void OnSize(UINT /*nType*/, CSize size)
	{
		m_cxRight = 200;
		m_cxLeft = size.cx-m_cxRight;
		if (m_EditInput!=NULL && m_EditOutput!=NULL && m_ListboxMemberList!=NULL 
			&& m_ToolbarTop!=NULL && m_ToolbarMid!=NULL)
		{
			CDeferWindowPos dwp(7);
			dwp.DeferWindowPos(m_EditInput,2,size.cy-140+m_sizeMid.cy,m_cxLeft-5,140-m_sizeMid.cy-m_sizeBottom.cy);
			dwp.DeferWindowPos(m_EditOutput,2,m_sizeTop.cy,m_cxLeft-5,size.cy-m_sizeTop.cy-140);
			int y = m_sizeTop.cy+m_sizeRightTop.cy+5;
			dwp.DeferWindowPos(m_ListboxMemberList,m_cxLeft,y,m_cxRight-2,size.cy-y-5);

			CRect rc;
			m_ToolbarTop.GetRect(ID_GROUP_COMMUNITY,&rc);
			dwp.DeferWindowPos(m_ToolbarTop,2,m_sizeTop.cy-rc.Height(),m_cxLeft-5,rc.Height());
			m_ToolbarMid.GetRect(ID_INPUT_TEXTFORMAT,&rc);
			dwp.DeferWindowPos(m_ToolbarMid,2,size.cy-141,m_cxLeft-5,rc.Height());

			rc.left = 0;
			rc.right = 70;
			rc.top = 0;
			rc.bottom = 24;
			rc.OffsetRect(m_cxLeft-rc.Width()-10,size.cy-rc.Height()-4);
			dwp.DeferWindowPos(m_btnSendMessage,rc);
			rc.OffsetRect(-rc.Width()-10,0);
			dwp.DeferWindowPos(m_btnClose,rc);
		}
	}

	//Toolbar tips notify handler
	LRESULT OnGetDispInfo(LPNMHDR pnmh)
	{
		LPNMTTDISPINFO ptti = (LPNMTTDISPINFO)pnmh;
		if(!(ptti->uFlags & TTF_IDISHWND))
		{
			ptti->hinst = AtlGetAppModule().GetResourceInstance();
			ptti->lpszText = MAKEINTRESOURCE(ptti->hdr.idFrom);
		}
		return 0;
	}

	//Toolbar command handler
	void OnCmd(UINT /*uNotifyCode*/, int nID, CWindow /*wndCtl*/)
	{
		ATLTRACE2(_T("ID_GROUP_COMMUNITY+%d\n"),nID);
	}

	void OnSendMessageBtnClicked(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
	{
		CChatRecord cr;
		cr.ID = 365407170;
		cr.NickName = _T("喜之狼");
		cr.TimeStamp = CTime::GetCurrentTime();
		m_EditInput.GetWindowText(cr.Message);

		// title paragraph format
		PARAFORMAT2 tpf;
		ZeroMemory(&tpf,sizeof(tpf));
		tpf.cbSize = sizeof(tpf);
		tpf.dwMask = PFM_SPACEBEFORE|PFM_SPACEAFTER;
		tpf.dySpaceBefore = 0;
		tpf.dySpaceAfter = 0;

		// title char format
		CHARFORMAT2 tcf;
		ZeroMemory(&tcf,sizeof(tcf));
		tcf.cbSize = sizeof(tcf);
		tcf.dwMask = CFM_COLOR|CFM_SIZE|CFM_FACE;
		tcf.crTextColor = RGB(255,0,0);
		SecureHelper::strcpy_x(tcf.szFaceName,32,_T("Microsoft Yahei"));
		tcf.yHeight = 20*10;

		// content paragraph format
		PARAFORMAT2 cpf;
		ZeroMemory(&cpf,sizeof(cpf));
		cpf.cbSize = sizeof(cpf);
		cpf.dwMask = PFM_SPACEBEFORE|PFM_SPACEAFTER;
		cpf.dySpaceBefore = 0;
		cpf.dySpaceAfter = 0;

		// content paragraph format
		CHARFORMAT2 ccf;
		ZeroMemory(&ccf,sizeof(ccf));
		ccf.cbSize = sizeof(ccf);
		ccf.dwMask = CFM_COLOR|CFM_SIZE|CFM_FACE;
		ccf.crTextColor = RGB(0,0,255);
		SecureHelper::strcpy_x(ccf.szFaceName,32,_T("Courier New"));
		ccf.yHeight = 20*12;

		m_EditOutput.SetSel(-1,-1);
		//m_EditOutput.SetParaFormat(tpf);
		m_EditOutput.SetSelectionCharFormat(tcf);
		m_EditOutput.AppendText(cr.GetTitleText());
		m_EditOutput.AppendText(_T("\n"));

		m_EditOutput.SetSel(-1,-1);
		//m_EditOutput.SetParaFormat(cpf);
		m_EditOutput.SetSelectionCharFormat(ccf);
		m_EditOutput.AppendText(cr.Message);
		m_EditOutput.AppendText(_T("\n"));

		m_EditOutput.PostMessage(WM_VSCROLL,SB_BOTTOM);
	}

	void OnCloseBtnClicked(UINT /*uNotifyCode*/, int /*nID*/, CWindow /*wndCtl*/)
	{
		PostMessage(WM_CLOSE);
	}

#ifdef NOT_USE_SYS_FRAME
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//UINT nFlags = (UINT)wParam;
		CPoint point = _WTYPES_NS::CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		if (point.y<TITLEBAR_HEIGHT)
		{
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,lParam);
		}
		return 0;
	}

	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//UINT nFlags = (UINT)wParam;
		CPoint point = _WTYPES_NS::CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		if (point.y<TITLEBAR_HEIGHT)
		{
			ReleaseCapture();
			SendMessage(WM_NCLBUTTONDBLCLK,HTCAPTION,lParam);
		}
		return 0;
	}
#endif

private:
	CBitmap m_bkgndTop;
	CBitmap m_bkgndMid;
	CBitmap m_bkgndBottom;
	CBitmap m_bkgndRightTop;

	CSize m_sizeTop;
	CSize m_sizeMid;
	CSize m_sizeBottom;
	CSize m_sizeRightTop;

	int m_cxLeft;
	int m_cxRight;

	CImageList m_imagelistToolbarTop;
	CToolBarCtrl m_ToolbarTop;

	CImageList m_imagelistToolbarMid;
	CToolBarCtrl m_ToolbarMid;

	CEdit m_EditInput;
	CRichEditCtrl m_EditOutput;
	CListBox m_ListboxMemberList;

	CButton m_btnSendMessage;
	CButton m_btnClose;

	CFont m_font;
};