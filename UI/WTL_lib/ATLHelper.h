#ifndef _ATLHELPER_H_
#define _ATLHELPER_H_

#pragma once
#if _MSC_VER>=1500		//VC2008 or higher
#include <sdkddkver.h>	//Definition of WINVER/_WIN32_WINNT/_WIN32_IE/_RICHEDIT_VER
#else
#define WIN_VER			0x0501
#define _WIN32_WINNT	0x0501	//Windows XP or higher
#define _WIN32_IE		0x0600
#define _RICHEDIT_VER	0x0300
#endif

#include <atlstr.h>     //ATL:: CString
#include <atltypes.h>   //ATL:: CPoint,CRect,CSzie

#include <atlbase.h>
#include <atlapp.h>

#include <vector>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof((a))/sizeof((a[0]))
#endif

#ifndef IFR
#if defined(_DEBUG)||defined(DEBUG)
	#define IFR(expr)						\
	{										\
		HRESULT _hr_IFR_ = (expr);			\
		ATLASSERT(SUCCEEDED((_hr_IFR_)));	\
	}
#else
	#define IFR(expr) (expr);
#endif
#endif

#ifndef CHECKHR
#define CHECKHR(hr) ATLASSERT(SUCCEEDED((hr)))
#endif

#ifndef CHECK_PTR
#define CHECK_PTR(p) ATLASSERT((p)!=NULL)
#endif

#ifndef CHECK_OUTPUT_PTR
#define CHECK_OUTPUT_PTR(pp) ATLASSERT(((pp)!=NULL) && ((*(pp))==NULL))
#endif

#ifndef CHECK_HANDLE
#define CHECK_HANDLE(hWnd) ATLASSERT((hWnd)!=NULL)
#endif

#ifndef ARRAY_ARGS
#define ARRAY_ARGS(a)  (a),ARRAYSIZE((a))
#endif

namespace ATLHelper
{

//////////////////////////////////////////////////////////////////////////
//
// Helper functions
// 
template< typename T >
bool IsValidOutputPtr(T** ppT)
{
	return ((ppT!=nullptr) && ((*ppT)==nullptr));
}

template< typename T >
void AssignOutputPtr(T* pTSrc, T** ppTDest)
{
	CHECK_OUTPUT_PTR(ppTDest);
	*ppTDest = pTSrc;
}

template< typename T >
void AssignOutputPtr(T TSrc, T* pTDest)
{
	CHECK_OUTPUT_PTR(ppTDest);
	*pTDest = TSrc;
}

template< typename T >
void SafeRelease(T** ppT)
{
	CHECK_PTR(ppT);
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//
// CComPtrArray template class
//
#pragma warning(push)
#pragma warning(disable:4018)

template<typename T>
class CComPtrArray
{
public:
	CComPtrArray() {}

	~CComPtrArray()
	{
		if (!m_TPtrArray.empty())
			clear();
	}

public:
	T* operator[](int i)
	{
		ATLASSERT(i>=0 && i<m_TPtrArray.size());
		return m_TPtrArray[i];
	}

	int size() { return m_TPtrArray.size(); }

	bool empty() { return m_TPtrArray.empty(); }

	void push_back(T* pT)
	{
		ATLASSERT(pT!=NULL);
		m_TPtrArray.push_back(pT);
	}

	void pop_back()
	{
		ATLASSERT(!m_TPtrArray.empty());
		int size = m_TPtrArray.size();
		T* pT = m_TPtrArray[size-1];
		ATLHelper::SafeRelease<T>(&pT);
		m_TPtrArray.pop_back();
	}

	void clear()
	{
		for (auto p=m_TPtrArray.begin();p!=m_TPtrArray.end();p++)
			ATLHelper::SafeRelease<T>(&(*p));

		m_TPtrArray.clear();
	}

private:
	std::vector<T*> m_TPtrArray;
}; // class CComPtrArray

#pragma warning(pop)

} // namespace ATLHelper

#endif