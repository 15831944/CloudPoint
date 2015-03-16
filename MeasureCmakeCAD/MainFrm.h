
// MainFrm.h : CMainFrame 类的接口
//
#include "DockTool.h"
#include "PointCloudMeasureDoc.h"

#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

private:
	bool mInteractModeActor;
	
// 操作
	CPointCloudMeasureDoc* GetDocument();
	void SetInteractiveStyle(Entity::ViewMode mode);

public:

	CMessageViewDockBar* GetMessageViewBar();

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
//	CMFCMenuBar       m_wndMenuBar;
//	CMFCToolBar       m_wndToolBar;
//	CMFCStatusBar     m_wndStatusBar;
//	CMFCToolBarImages m_UserImages;
	CStatusBar  m_wndStatusBar; //状态工具条
	CToolBar    m_wndToolBar;   //主工具条
	
	CTabSheetDockBar m_leftDockBar;  //模型树窗口
	CMessageViewDockBar m_OutputDockBar;//输出信息窗口

	CToolBar    m_wndDisplayBar;//视图处理工具条

// 生成的消息映射函数
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


