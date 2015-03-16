
// MainFrm.h : CMainFrame ��Ľӿ�
//
#include "DockTool.h"
#include "PointCloudMeasureDoc.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

private:
	bool mInteractModeActor;
	
// ����
	CPointCloudMeasureDoc* GetDocument();
	void SetInteractiveStyle(Entity::ViewMode mode);

public:

	CMessageViewDockBar* GetMessageViewBar();

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
//	CMFCMenuBar       m_wndMenuBar;
//	CMFCToolBar       m_wndToolBar;
//	CMFCStatusBar     m_wndStatusBar;
//	CMFCToolBarImages m_UserImages;
	CStatusBar  m_wndStatusBar; //״̬������
	CToolBar    m_wndToolBar;   //��������
	
	CTabSheetDockBar m_leftDockBar;  //ģ��������
	CMessageViewDockBar m_OutputDockBar;//�����Ϣ����

	CToolBar    m_wndDisplayBar;//��ͼ��������

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateModeInteract(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeMatch(CCmdUI *pCmdUI);
	afx_msg void OnUpdateModeQuery(CCmdUI *pCmdUI);
	afx_msg void OnModeInteract();
	afx_msg void OnMenuInformation();
	afx_msg void OnModeQuery();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnModeMatch();
	afx_msg void OnMatchIcp();
	afx_msg void OnMatchInteract();
	afx_msg void OnMatchFeaturePoint();
	afx_msg void OnMatchInteractStart();
	afx_msg void OnMatchInteractEnd();
	afx_msg void OnMatchInteractChange();
	afx_msg void OnMeasureCompute();
	afx_msg void OnMeasureQuery();
	afx_msg void Onzhuangpeijisuan();
	afx_msg void OnXiuPeiCompute();
	afx_msg void OnZhuangPeiMotion();
	afx_msg void OnRansacCompute();
	afx_msg void OnToleranceCompute();
	afx_msg void OnEndMotionSim();
};


