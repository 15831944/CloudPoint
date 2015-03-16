
// PointCloudMeasureView.h : CPointCloudMeasureView 类的接口
//
#include <vtkMFCWindow.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkTextProperty.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkPointData.h>
#include <vtkSTLWriter.h>

#include "PointCloudMeasureDoc.h"

#pragma once


class CPointCloudMeasureView : public CView
{
protected: // 仅从序列化创建
	CPointCloudMeasureView();
	DECLARE_DYNCREATE(CPointCloudMeasureView)

// 属性
public:
	CPointCloudMeasureDoc* GetDocument() const;
	vtkRenderer* GetRenderer() { ASSERT(pvtkRenderer); return pvtkRenderer; }
	vtkMFCWindow* GetMFCWindow() {ASSERT(pvtkMFCWindow); return pvtkMFCWindow; }

private:
	vtkRenderer            *pvtkRenderer;
    vtkMFCWindow          *pvtkMFCWindow;

	vtkActor      *pAxesActor;
	vtkOrientationMarkerWidget *pWidget;


	double m_viewAngle;
	double m_azimuthAngle;
	double m_dis;	

	double m_zoomIn;
	double m_zoomOut;
	double m_rBag;
	double m_gBag;
	double m_bBag;
	double m_elevation;

// 操作
public:

private:
	void CreateAxes();
	void CreateWidget();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPointCloudMeasureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewBack();
	afx_msg void OnViewBottom();
	afx_msg void OnViewFront();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewNeIsometric();
	afx_msg void OnViewNwIsometric();
	afx_msg void OnViewSeIsometric();
	afx_msg void OnViewSwIsometric();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomall();
	afx_msg void OnViewShade();
	afx_msg void OnCameraOperation();
};

#ifndef _DEBUG  // PointCloudMeasureView.cpp 中的调试版本
inline CPointCloudMeasureDoc* CPointCloudMeasureView::GetDocument() const
   { return reinterpret_cast<CPointCloudMeasureDoc*>(m_pDocument); }
#endif

