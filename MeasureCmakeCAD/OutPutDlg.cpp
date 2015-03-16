// OutPutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "OutPutDlg.h"


// OutPutDlg dialog

IMPLEMENT_DYNAMIC(OutPutDlg, CDialog)

OutPutDlg::OutPutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OutPutDlg::IDD, pParent)
{
	this->num_point_cloud = 0;
	this->num_stl = 0;
	this->m_point_cloud_out = -1;
	this->m_stl_out = -1;
}

OutPutDlg::OutPutDlg(int numPointCloud, int numStl, CWnd* pParent /*=NULL*/)
	: CDialog(OutPutDlg::IDD, pParent)
{
	this->num_point_cloud = numPointCloud;
	this->num_stl = numStl;
	this->m_point_cloud_out = -1;
	this->m_stl_out = -1;
}

OutPutDlg::~OutPutDlg()
{
}

void OutPutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_POINT, m_point_cloud);
	DDX_Control(pDX, IDC_COMBO_STL, m_stl);
	DDX_Control(pDX, IDC_RADIO_POINT, m_out_model);
}


BEGIN_MESSAGE_MAP(OutPutDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_POINT, &OutPutDlg::OnBnClickedRadioPoint)
	ON_BN_CLICKED(IDC_RADIO_STL, &OutPutDlg::OnBnClickedRadioStl)
END_MESSAGE_MAP()


// OutPutDlg message handlers

BOOL OutPutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->m_out_model.SetCheck(1);//设置初始导出模型为enisight
	CString str;

	if(this->num_point_cloud)//设置pointCloud模型list
	{
		for(int i=0;i<this->num_point_cloud;i++)
		{
			str.Format("Point Cloud Model %d",i+1);
			this->m_point_cloud.AddString(str);
		}
	}

	if(this->num_stl)//设置stl模型list
	{
		for(int i=0;i<this->num_stl;i++)
		{
			str.Format("Stl Model %d",i+1);
			this->m_stl.AddString(str);
		}
	}

	this->m_stl.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void OutPutDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	this->m_point_cloud_out = this->m_point_cloud.GetCurSel();//设置最后的point cloud导出模型数
	this->m_stl_out = this->m_stl.GetCurSel();//设置最后的stl导出模型数
	
	CDialog::OnOK();
}

void OutPutDlg::OnBnClickedRadioPoint()
{
	// TODO: Add your control notification handler code here
	this->m_point_cloud.EnableWindow(TRUE);
	this->m_stl.EnableWindow(FALSE);
}

void OutPutDlg::OnBnClickedRadioStl()
{
	// TODO: Add your control notification handler code here
	this->m_point_cloud.EnableWindow(FALSE);
	this->m_stl.EnableWindow(TRUE);
}
