// RansacPara.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "RansacPara.h"
//#include "ProgressDlg.h"


// RansacPara dialog

IMPLEMENT_DYNAMIC(RansacPara, CDialog)

RansacPara::RansacPara(CWnd* pParent /*=NULL*/)
	: CDialog(RansacPara::IDD, pParent)
{
	this->m_set_cone = false;
	this->m_set_cylinder = false;
	this->m_set_plane = false;
	this->m_set_sphere = false;
	this->m_set_torus = false;

	this->m_selected_part = -1;

	this->m_distance_ths = 0.001;
	this->m_projection_ths = 0.002;
	this->m_normal_ths = 0.988;
	this->m_probability_ths = 0.002;
	this->m_minimal_points = 7000;

	this->num_parts = 0;
}

RansacPara::~RansacPara()
{
}

void RansacPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DISTANCE_THREHOLD, m_distance_ths);
	DDX_Text(pDX, IDC_MINIMAL_POINTS, m_minimal_points);
	DDX_Text(pDX, IDC_NORMAL_THREHOLD, m_normal_ths);
	DDX_Text(pDX, IDC_PROBABILITY_THREHOLD4, m_probability_ths);
	DDX_Text(pDX, IDC_PROJECTION_THREHOLD, m_projection_ths);
	DDX_Control(pDX, IDC_PLANE, m_plane);
	DDX_Control(pDX, IDC_Cone, m_cone);
	DDX_Control(pDX, IDC_CYLINDER, m_cylinder);
	DDX_Control(pDX, IDC_SPHERE, m_sphere);
	DDX_Control(pDX, IDC_TORUS, m_torus);
	DDX_Control(pDX, IDC_PART_SELECT, m_part);
}


BEGIN_MESSAGE_MAP(RansacPara, CDialog)
	ON_BN_CLICKED(IDC_PLANE, &RansacPara::OnBnClickedPlane)
	ON_BN_CLICKED(IDC_CYLINDER, &RansacPara::OnBnClickedCylinder)
	ON_BN_CLICKED(IDC_Cone, &RansacPara::OnBnClickedCone)
	ON_BN_CLICKED(IDC_SPHERE, &RansacPara::OnBnClickedSphere)
	ON_BN_CLICKED(IDC_TORUS, &RansacPara::OnBnClickedTorus)
END_MESSAGE_MAP()


// RansacPara message handlers

BOOL RansacPara::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	if(this->num_parts)//…Ë÷√stlƒ£–Õlist
	{
		CString str;

		for(int i=0;i<this->num_parts;i++)
		{
			str.Format("Model %d",i+1);
			this->m_part.AddString(str);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void RansacPara::OnBnClickedPlane()
{
	// TODO: Add your control notification handler code here
	if(this->m_set_plane)
	{
		this->m_set_plane = false;
	}
	else
	{
		this->m_set_plane = true;
	}
}

void RansacPara::OnBnClickedCylinder()
{
	// TODO: Add your control notification handler code here
	if(this->m_set_cylinder)
	{
		this->m_set_cylinder = false;
	}
	else
	{
		this->m_set_cylinder = true;
	}
}

void RansacPara::OnBnClickedCone()
{
	// TODO: Add your control notification handler code here
	if(this->m_set_cone)
	{
		this->m_set_cone = false;
	}
	else
	{
		this->m_set_cone = true;
	}
}

void RansacPara::OnBnClickedSphere()
{
	// TODO: Add your control notification handler code here
	if(this->m_set_sphere)
	{
		this->m_set_sphere = false;
	}
	else
	{
		this->m_set_sphere = true;
	}
}

void RansacPara::OnBnClickedTorus()
{
	// TODO: Add your control notification handler code here
	if(this->m_set_torus)
	{
		this->m_set_torus = false;
	}
	else
	{
		this->m_set_torus = true;
	}
}

void RansacPara::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	m_selected_part = this->m_part.GetCurSel();

	CDialog::OnOK();

}

void RansacPara::OnBnClickedTours()
{
	// TODO: Add your control notification handler code here
}
