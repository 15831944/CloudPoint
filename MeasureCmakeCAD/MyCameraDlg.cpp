// MyCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "MyCameraDlg.h"


// CMyCameraDly dialog

IMPLEMENT_DYNAMIC(CMyCameraDlg, CDialog)

CMyCameraDlg::CMyCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyCameraDlg::IDD, pParent)
	, m_viewangle(0)
	, m_azimuthangle(0)
	
	, m_xcenter(0)
	, m_ycenter(0)
	, m_elevation(0)
	, m_xpoisition(0)
	, m_ypoisition(0)
	, m_zpoisition(0)
	, m_VX(0)
	, m_VY(0)
	, m_VZ(0)
	, m_xfocal(0)
	, m_yfocal(0)
	, m_zfocal(0)
	, m_xnormal(0)
	, m_ynormal(0)
	, m_znormal(0)
	
	, m_dis(0)
	, m_rBag(0)
	, m_gBag(0)
	, m_bBag(0)
	, m_zoomIn(0)
	, m_zoomOut(0)
{
	 m_zoomIn=1.2;
     m_zoomOut=0.8;
}

CMyCameraDlg::~CMyCameraDlg()
{
}

void CMyCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_viewangle, m_viewangle);
	DDX_Text(pDX, IDC_EDIT_azimuth, m_azimuthangle);

	DDX_Text(pDX, IDC_EDIT_xcenter, m_xcenter);
	DDX_Text(pDX, IDC_EDIT_ycenter, m_ycenter);
	DDX_Text(pDX, IDC_EDIT_pitch, m_elevation);
	DDX_Text(pDX, IDC_EDIT_xpoisition, m_xpoisition);
	DDX_Text(pDX, IDC_EDIT_ypoisition, m_ypoisition);
	DDX_Text(pDX, IDC_EDIT_zpoisition, m_zpoisition);
	DDX_Text(pDX, IDC_EDIT_VX, m_VX);
	DDX_Text(pDX, IDC_EDIT_VY, m_VY);
	DDX_Text(pDX, IDC_EDIT_VZ, m_VZ);
	DDX_Text(pDX, IDC_EDIT_xfocal, m_xfocal);
	DDX_Text(pDX, IDC_EDIT_yfocal, m_yfocal);
	DDX_Text(pDX, IDC_EDIT_zfocal, m_zfocal);
	DDX_Text(pDX, IDC_EDIT_xnormal, m_xnormal);
	DDX_Text(pDX, IDC_EDIT_ynormal, m_ynormal);
	DDX_Text(pDX, IDC_EDIT_znormal, m_znormal);

	DDX_Text(pDX, IDC_EDIT_dis, m_dis);
	DDX_Text(pDX, IDC_EDIT_rBag, m_rBag);
	DDX_Text(pDX, IDC_EDIT_gBag, m_gBag);

	DDX_Text(pDX, IDC_EDIT_bBag, m_bBag);
	DDX_Text(pDX, IDC_EDIT_zoomIn, m_zoomIn);
	DDX_Text(pDX, IDC_EDIT_zoomOut, m_zoomOut);
}


BEGIN_MESSAGE_MAP(CMyCameraDlg, CDialog)
END_MESSAGE_MAP()


// CMyCameraDly message handlers
