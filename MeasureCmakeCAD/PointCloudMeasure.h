
// PointCloudMeasure.h : PointCloudMeasure Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CPointCloudMeasureApp:
// �йش����ʵ�֣������ PointCloudMeasure.cpp
//

class CPointCloudMeasureApp : public CWinAppEx
{
public:
	CPointCloudMeasureApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPointCloudMeasureApp theApp;
