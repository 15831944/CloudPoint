
// XPMotionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "XPMotionDlg.h"


// XPMotionDlg dialog

IMPLEMENT_DYNAMIC(XPMotionDlg, CDialog)

XPMotionDlg::XPMotionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XPMotionDlg::IDD, pParent)
{
	m_set_motion1=-1;
	m_set_motion2=-1;
	m_set_motion3=-1;
}

XPMotionDlg::~XPMotionDlg()
{
}

void XPMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MPART1, m_model_motion_select1);
	DDX_Control(pDX, IDC_COMBO_MPART2, m_model_motion_select2);
	DDX_Control(pDX, IDC_COMBO_MPART3, m_model_motion_select3);

}


BEGIN_MESSAGE_MAP(XPMotionDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MPART1, &XPMotionDlg::OnCbnSelchangeComboMpart1)
	ON_CBN_SELCHANGE(IDC_COMBO_MPART2, &XPMotionDlg::OnCbnSelchangeComboMpart2)
	ON_CBN_SELCHANGE(IDC_COMBO_MPART3, &XPMotionDlg::OnCbnSelchangeComboMpart3)
END_MESSAGE_MAP()


// XPMotionDlg message handlers

BOOL XPMotionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str1;
	CString str2;
	CString str3;
	
	std::list<CString>::iterator iterName;


	for(iterName = this->model_name.begin(); iterName != this->model_name.end(); iterName++)
	{
		this->m_model_motion_select1.AddString((*iterName));
	}

	for(iterName = this->model_name.begin(); iterName != this->model_name.end(); iterName++)
	{
		this->m_model_motion_select2.AddString((*iterName));
	}

	for(iterName = this->model_name.begin(); iterName != this->model_name.end(); iterName++)
	{
		this->m_model_motion_select3.AddString((*iterName));
	}

	return TRUE;
}

void XPMotionDlg::SetNumModel(const int num)
{
	num_model = num;
}

void XPMotionDlg::OnCbnSelchangeComboMpart1()
{
	// TODO: Add your control notification handler code here
	this->m_set_motion1=this->m_model_motion_select1.GetCurSel();

}

void XPMotionDlg::OnCbnSelchangeComboMpart2()
{
	// TODO: Add your control notification handler code here
	this->m_set_motion2=this->m_model_motion_select2.GetCurSel();

}

void XPMotionDlg::OnCbnSelchangeComboMpart3()
{
	// TODO: Add your control notification handler code here
	this->m_set_motion3=this->m_model_motion_select3.GetCurSel();

}
