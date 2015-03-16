// MeasurementDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "MeasurementDialog.h"


// MeasurementDialog dialog

IMPLEMENT_DYNAMIC(MeasurementDialog, CDialog)

MeasurementDialog::MeasurementDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MeasurementDialog::IDD, pParent)
{
	this->m_cad_surf_out = -1;
	this->m_cad_edge_out = -1;
	this->m_stl_out = -1;

	this->m_use_cad_for_measure = true;
	this->m_use_cad_surf_or_edge = true;

	this->numStl = 0;

	this->m_initial_cad_edge = 0;
	this->m_initial_cad_surf = 0;
	this->m_initial_stl = 0;
}

MeasurementDialog::MeasurementDialog(const int nStl,CWnd* pParent /*=NULL*/)
	: CDialog(MeasurementDialog::IDD, pParent)
{
	this->m_cad_surf_out = -1;
	this->m_cad_edge_out = -1;
	this->m_stl_out = -1;

	this->m_use_cad_for_measure = false;
	this->m_use_cad_surf_or_edge = false;

	this->numStl = nStl;
}

MeasurementDialog::MeasurementDialog(const int nStl, const vector<string> & surf, const vector<string> &edge,CWnd* pParent /*=NULL*/)
	: CDialog(MeasurementDialog::IDD, pParent)
{
	this->m_cad_surf_out = -1;
	this->m_cad_edge_out = -1;
	this->m_stl_out = -1;

	this->m_use_cad_for_measure = true;
	this->m_use_cad_surf_or_edge = true;

	this->numStl = nStl;

	this->surfName = surf;
	this->edgeName = edge;
}

MeasurementDialog::~MeasurementDialog()
{
}

void MeasurementDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAD_COMPUTE, m_computing_type);
	DDX_Control(pDX, IDC_COMBO_CAD_EDGE_COMPUTE, m_cad_edge);
	DDX_Control(pDX, IDC_COMBO_CAD_SURFACE_COMPUTE, m_cad_surface);
	DDX_Control(pDX, IDC_COMBO_STL_COMPUTE, m_stl);
	DDX_Control(pDX, IDC_CHECK_SURFOREDGE, m_cad_surf_or_edge);
}


BEGIN_MESSAGE_MAP(MeasurementDialog, CDialog)
	ON_BN_CLICKED(IDC_CAD_COMPUTE, &MeasurementDialog::OnBnClickedCadCompute)
	ON_BN_CLICKED(IDC_RADIO_STL_COMPUTE, &MeasurementDialog::OnBnClickedRadioStlCompute)
	ON_BN_CLICKED(IDC_CHECK_SURFOREDGE, &MeasurementDialog::OnBnClickedCheckSurforedge)
END_MESSAGE_MAP()


// MeasurementDialog message handlers

void MeasurementDialog::OnBnClickedCadCompute()
{
	// TODO: Add your control notification handler code here
	this->m_stl.EnableWindow(FALSE);

	if(this->m_cad_surf_or_edge)
	{
		this->m_cad_surface.EnableWindow(TRUE);
	}
	else
	{
		this->m_cad_edge.EnableWindow(TRUE);
	}
	
	this->m_cad_surf_or_edge.EnableWindow(TRUE);
	this->m_use_cad_for_measure = true;
}

void MeasurementDialog::OnBnClickedRadioStlCompute()
{
	// TODO: Add your control notification handler code here
	this->m_stl.EnableWindow(TRUE);
	this->m_cad_edge.EnableWindow(FALSE);
	this->m_cad_surface.EnableWindow(FALSE);
	this->m_cad_surf_or_edge.EnableWindow(FALSE);

	this->m_use_cad_for_measure = false;
}

BOOL MeasurementDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	this->m_computing_type.SetCheck(1);//设置初始导出模型为enisight
	this->m_cad_surf_or_edge.SetCheck(1);
	CString str;

	if(this->numStl)//设置stl模型list
	{
		for(int i=0;i<this->numStl;i++)
		{
			str.Format("Stl Model %d",i+1);
			this->m_stl.AddString(str);
		}
	}

	//for feature surface
	if(this->surfName.size() > 0)
	{
		int num = this->surfName.size();
		for(int i = 0; i < num; i++)
		{
			this->m_cad_surface.AddString(surfName[i].c_str());
		}
	}

	//for feature edge
	if(this->edgeName.size() > 0)
	{
		int num = this->edgeName.size();
		for(int i = 0; i < num; i++)
		{
			this->m_cad_edge.AddString(edgeName[i].c_str());
		}
	}

	this->m_stl.SetCurSel(this->m_initial_stl);
	this->m_cad_edge.SetCurSel(this->m_initial_cad_edge);
	this->m_cad_surface.SetCurSel(this->m_initial_cad_surf);

	this->m_stl.EnableWindow(FALSE);
	this->m_cad_edge.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void MeasurementDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	this->m_cad_edge_out = this->m_cad_edge.GetCurSel();
	this->m_cad_surf_out = this->m_cad_surface.GetCurSel();
	this->m_stl_out = this->m_stl.GetCurSel();
	CDialog::OnOK();
}

void MeasurementDialog::OnBnClickedCheckSurforedge()
{
	// TODO: Add your control notification handler code here
	if(this->m_use_cad_surf_or_edge)
	{
		this->m_use_cad_surf_or_edge = false;
		this->m_cad_surface.EnableWindow(FALSE);
		this->m_cad_edge.EnableWindow(TRUE);
	}
	else
	{
		this->m_use_cad_surf_or_edge = true;
		this->m_cad_surface.EnableWindow(TRUE);
		this->m_cad_edge.EnableWindow(FALSE);
	}
}
