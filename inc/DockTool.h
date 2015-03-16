// LeftDockBar.h: interface for the CLeftDockBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_)
#define AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
//#include "resource.h"

#include "CoolDialogBar.h"

//AFX_EXT_API void WINAPI InitClass();

class AFX_EXT_CLASS CTabSheet : public CTabCtrl
{
// Construction
public:
	CTabSheet();
	CArray<CWnd*,CWnd*> m_arrView;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheet)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	//}}AFX_VIRTUAL

// Implementation
public:
	void RemoveAllPage();
	void SetMargin(int margin);
	void SetActiveView(int nView);
	
	BOOL AddView(LPTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext=NULL);
	BOOL AddView(LPTSTR name, CWnd* pView);

	virtual ~CTabSheet();

	// Generated message map functions
protected:
	int		m_iMargin;
	CWnd*	m_curView;
	CRect	m_rcView;
	CFont	m_TabFont;
	virtual void InitFont();
	//{{AFX_MSG(CTabSheet)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType,int cx,int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class AFX_EXT_CLASS CTabSheetDockBar : public CCoolDialogBar
{
	DECLARE_DYNCREATE(CTabSheetDockBar)
public:
	CTabSheetDockBar();
	virtual ~CTabSheetDockBar();
	virtual BOOL Create( CWnd* pParentWnd,LPCTSTR pTitle);

	// Dialog Data
	//{{AFX_DATA(CTabSheetDockBar)
	//}}AFX_DATA
	CTabSheet	m_TabCtrl;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheetDockBar)
	//}}AFX_VIRTUAL

	void	SetFloatSize(const CSize& size);
	void	SetDockSize(const CSize& size);
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSheetDockBar)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class AFX_EXT_CLASS CMessageViewDockBar : public CCoolDialogBar  
{
	DECLARE_DYNCREATE(CMessageViewDockBar)
protected:
	RECT	m_rcView;
	CView*	m_pOutputView;
public:
	CMessageViewDockBar();
	virtual ~CMessageViewDockBar();
	virtual BOOL Create( CWnd* pParentWnd,LPCTSTR pTitle);

	void	AddMessageLine(LPCTSTR pStr);
	void	RemoveAllMessageLines();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageViewDockBar)
	//}}AFX_VIRTUAL

	void	SetFloatSize(const CSize& size);
	void	SetDockSize(const CSize& size);

protected:
	BOOL	AddView(CRuntimeClass *pViewClass, CCreateContext *pContext);

	// Generated message map functions
	//{{AFX_MSG(COutputDockBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LEFTDOCKBAR_H__7A0EF27C_C98F_46FE_A9C9_FE977A73627E__INCLUDED_)
