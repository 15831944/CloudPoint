#if !defined(AFX_PARTTREEVIEW_H__88CD4C98_457B_4E0C_A9B5_BAF601686270__INCLUDED_)
#define AFX_PARTTREEVIEW_H__88CD4C98_457B_4E0C_A9B5_BAF601686270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PartTreeView.h : header file
//

#include <afxcview.h>
#include "IgesAndVtkModel.h"

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView view
class CPointCloudMeasureDoc;
class CPartTreeView : public CTreeView
{
protected:
	CPartTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPartTreeView)


	void ProcessSurfaceEdge(IgesAndVtkModel* model, int item);
// Attributes
public:

// Operations
public:
	CPointCloudMeasureDoc * GetDocument();//模型树视图得到文档类的指针

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPartTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView *pSender,LPARAM iHint,CObject *pHint);//模型树视图更新函数
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPartTreeView();

	int part_num;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ImageList;//模型树项图表
//	void ChangeEntityName(CEntity *ent);
//	void ChangeEntityColor(CEntity *ent);
//	void ChangeEntityVisible(CEntity *ent);
	//{{AFX_MSG(CPartTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);//模型树项响应鼠标双击函数
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTTREEVIEW_H__88CD4C98_457B_4E0C_A9B5_BAF601686270__INCLUDED_)
