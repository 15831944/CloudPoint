// PartProcess.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "PartProcess.h"


// PartProcess dialog

IMPLEMENT_DYNAMIC(PartProcess, CDialog)

PartProcess::PartProcess(CWnd* pParent /*=NULL*/)
: CDialog(PartProcess::IDD, pParent)
, m_model(_T(""))
, m_type(_T(""))
, m_num_cells(_T(""))
, m_num_verts(_T(""))
, m_part_highlight(FALSE)
, m_edge_selection(false)
, m_set_visible_all(false)
, m_set_2d(false)
, m_set_axis(false)
, m_set_2d_all(false)
, m_set_visible_only(false)
{

	m_part_legend = FALSE;
	m_part_visible = TRUE;
	m_part_highlight = FALSE;
	m_mode = 0;

	this->pScalarArray = vtkStringArray::New();
	this->pVectorArray = vtkStringArray::New();

	m_set_attribute = -1;
	m_set_component = -1;
	m_set_match = -1;

	num_stl_model = 0;

	m_has_component = FALSE;

	m_edge_selection = FALSE;

	m_set_edge_selection = FALSE;

	m_set_visible_only = FALSE;

	m_set_visible_all = FALSE;

	m_set_2d = FALSE;

	m_set_2d_all = FALSE;

	m_set_axis = FALSE;

	m_cad_surface = FALSE;
	m_recognize_model = FALSE;
	
	m_inform_all = FALSE;
	m_inform_only = FALSE;
}

//带模式的构造函数
PartProcess::PartProcess(int match, CWnd* pParent)
: CDialog(PartProcess::IDD, pParent)
, m_model(_T(""))
, m_type(_T(""))
, m_num_cells(_T(""))
, m_num_verts(_T(""))
{
	m_part_legend = FALSE;
	m_part_visible = TRUE;
	m_part_highlight = FALSE;
	m_mode = match;

	this->pScalarArray = vtkStringArray::New();
	this->pVectorArray = vtkStringArray::New();

	m_set_attribute = -1;
	m_set_component = -1;
	m_set_match = -1;

	num_stl_model = 0;

	m_has_component = FALSE;

	m_edge_selection = FALSE;

	m_set_edge_selection = FALSE;

	m_set_visible_only = FALSE;

	m_set_visible_all = FALSE;

	m_set_2d = FALSE;
	
	m_set_2d_all = FALSE;

	m_set_axis = FALSE;

	m_cad_surface = FALSE;
	m_recognize_model = FALSE;
	
	m_inform_all = FALSE;
	m_inform_only = FALSE;
}

//带物理属性与类型的构造函数
PartProcess::PartProcess(vtkStringArray *pScalars,vtkStringArray *pVectors,int match, CWnd* pParent)
: CDialog(PartProcess::IDD, pParent)
, m_model(_T(""))
, m_type(_T(""))
, m_num_cells(_T(""))
, m_num_verts(_T(""))
{
	m_part_legend = FALSE;
	m_part_visible = TRUE;
	m_part_highlight = FALSE;
	m_mode = match;

	this->pScalarArray = vtkStringArray::New();
	this->pVectorArray = vtkStringArray::New();

	this->pScalarArray->DeepCopy(pScalars);
	this->pVectorArray->DeepCopy(pVectors);

	m_set_attribute = -1;
	m_set_component = -1;
	m_set_match = -1;

	num_stl_model = 0;

	m_has_component = FALSE;

	m_edge_selection = FALSE;

	m_set_edge_selection = FALSE;

	m_set_visible_only = FALSE;

	m_set_visible_all = FALSE;

	m_set_2d = FALSE;

	m_set_2d_all = FALSE;

	m_set_axis = FALSE;

	m_cad_surface = FALSE;
	m_recognize_model = FALSE;
	
	m_inform_all = FALSE;
	m_inform_only = FALSE;
}

PartProcess::~PartProcess()
{
	if(this->pScalarArray)
	{
		this->pScalarArray->Delete();
	}
	if(this->pVectorArray)
	{
		this->pVectorArray->Delete();
	}
}

void PartProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_VISIBLE, m_visible);
	DDX_Text(pDX, IDC_STATIC_MODEL, m_model);
	DDX_Text(pDX, IDC_STATIC_TYPE, m_type);
	DDX_Text(pDX, IDC_STATIC_NUM_CELLS, m_num_cells);
	DDX_Text(pDX, IDC_STATIC_NUM_VERTS, m_num_verts);
	DDX_Control(pDX, IDC_COMBO_ATTRIBUTE, m_physical_atrribute);
	DDX_Control(pDX, IDC_COMBO_COMPONENT, m_component);
	DDX_Control(pDX, IDC_COMBO_MATCH_SELECT, m_model_match_select);
	DDX_Control(pDX, IDC_CHECK_LEGEND, m_legend);
	DDX_Check(pDX, IDC_HIGHLIGHT, m_part_highlight);
	DDX_Control(pDX, IDC_HIGHLIGHT, m_hightlight);
	DDX_Control(pDX, IDC_SELECT_EDGE, m_select_surface_edge);
	DDX_Control(pDX, IDC_VISIBLE_ALL, m_part_visible_all);
	DDX_Control(pDX, IDC_VISIBLE_ONLY, m_part_visible_only);
	DDX_Control(pDX, IDC_INFOR_VIS_ONLY, m_info_vis_only);
	DDX_Control(pDX, IDC_INFOR_VIS_ALL, m_info_vis_all);
}


BEGIN_MESSAGE_MAP(PartProcess, CDialog)
	ON_BN_CLICKED(IDC_CHECK_LEGEND, &PartProcess::OnBnClickedCheckLegend)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE, &PartProcess::OnBnClickedCheckVisible)
	ON_CBN_SELCHANGE(IDC_COMBO_ATTRIBUTE, &PartProcess::OnCbnSelchangeComboAttribute)
	ON_BN_CLICKED(IDC_HIGHLIGHT, &PartProcess::OnBnClickedHighlight)
	ON_BN_CLICKED(IDC_SELECT_EDGE, &PartProcess::OnBnClickedSelectEdge)
	ON_BN_CLICKED(IDC_VISIBLE_ONLY, &PartProcess::OnBnClickedVisibleOnly)
	ON_BN_CLICKED(IDC_VISIBLE_ALL, &PartProcess::OnBnClickedVisibleAll)
	ON_BN_CLICKED(IDC_2D, &PartProcess::OnBnClicked2d)
	ON_BN_CLICKED(IDC_AXIS, &PartProcess::OnBnClickedAxis)
	ON_BN_CLICKED(IDC_2DALL, &PartProcess::OnBnClicked2dall)
	ON_BN_CLICKED(IDC_INFOR_VIS_ALL, &PartProcess::OnBnClickedInforVisAll)
	ON_BN_CLICKED(IDC_INFOR_VIS_ONLY, &PartProcess::OnBnClickedInforVisOnly)
END_MESSAGE_MAP()


// PartProcess message handlers

void PartProcess::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_mode == 2)
	{
		this->m_set_attribute = this->m_physical_atrribute.GetCurSel();//设置最终的物理属性结果
		this->m_set_component = this->m_component.GetCurSel();//设置物理属性分量结果
	}
	if(m_mode == 1)
	{
		this->m_set_match = this->m_model_match_select.GetCurSel(); //设置选择的模型匹配值
	}

	CDialog::OnOK();
}

BOOL PartProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	if(this->m_part_visible)//根据模型可见性设置可见性控件的初始值
	{
		this->m_visible.SetCheck(1);

		if(m_part_highlight)
			this->m_hightlight.SetCheck(1);

		if(m_mode == 0)
		{
			this->m_physical_atrribute.EnableWindow(0);
			this->m_component.EnableWindow(0);
			this->GetDlgItem(IDC_CHECK_LEGEND)->EnableWindow(0);
			this->m_model_match_select.EnableWindow(0);
		}

		if(m_mode == 1)
		{
			CString str;

			this->m_physical_atrribute.EnableWindow(0);
			this->m_component.EnableWindow(0);
			this->GetDlgItem(IDC_CHECK_LEGEND)->EnableWindow(0);

			//添加模型到match combox 从而选择匹配模型
			if(this->num_stl_model)//设置stl模型list
			{
				for(int i=0;i<this->num_stl_model;i++)
				{
					str.Format("Stl Model %d",i+1);
					this->m_model_match_select.AddString(str);
				}
			}
		}

		if(m_mode == 2)
		{
			this->m_model_match_select.EnableWindow(0);

			if(this->pVectorArray->GetNumberOfTuples())//初始化设置物理属性的分量
			{
				this->m_component.AddString("X");
				this->m_component.AddString("Y");
				this->m_component.AddString("Z"); 
			}

			this->m_component.EnableWindow(0);

			if(this->pScalarArray->GetNumberOfTuples()||this->pVectorArray->GetNumberOfTuples())//初始化设置物理属性值
			{
				int num_scalar=this->pScalarArray->GetNumberOfTuples();
				CString str;
				for(int i=0;i<num_scalar;i++)
				{
					str=this->pScalarArray->GetValue(i);
					this->m_physical_atrribute.AddString(str);
				}

				int num_vector=this->pVectorArray->GetNumberOfTuples();
				for(int j=0;j<num_vector;j++)
				{
					str=this->pVectorArray->GetValue(j);
					this->m_physical_atrribute.AddString(str);
				}
			}

			//设置初始legend
			if(this->m_part_legend)
			{
				this->m_legend.SetCheck(1);
			}
		}
	}
	else
	{
		this->m_physical_atrribute.EnableWindow(0);
		this->m_component.EnableWindow(0);
		this->GetDlgItem(IDC_CHECK_LEGEND)->EnableWindow(0);
		this->m_model_match_select.EnableWindow(0);
	}

	

	if(m_cad_surface)
	{
		if(m_edge_selection)
		{
			this->m_select_surface_edge.EnableWindow(1);
			this->m_part_visible_all.EnableWindow(1);
		}
		else
		{
			this->m_select_surface_edge.EnableWindow(0);
		}

		this->m_part_visible_all.EnableWindow(1);
		this->m_part_visible_only.EnableWindow(1);
		this->m_hightlight.EnableWindow(1);
	}
	else
	{
		this->m_select_surface_edge.EnableWindow(0);
		this->m_part_visible_all.EnableWindow(0);
		this->m_part_visible_only.EnableWindow(0);
		this->m_hightlight.EnableWindow(0);
	}

	if(m_recognize_model)
	{
		this->m_info_vis_all.EnableWindow(1);
		this->m_info_vis_only.EnableWindow(1);
	}
	else
	{
		this->m_info_vis_all.EnableWindow(0);
		this->m_info_vis_only.EnableWindow(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PartProcess::OnBnClickedCheckLegend()
{
	// TODO: Add your control notification handler code here
	if(this->m_part_legend)
	{
		this->m_part_legend=FALSE;
	}
	else
	{
		this->m_part_legend=TRUE;
	}
}

void PartProcess::OnBnClickedCheckVisible()
{
	// TODO: Add your control notification handler code here
	if(m_part_visible)
	{
		m_part_visible=FALSE;
	}
	else
	{
		m_part_visible=TRUE;
	}
}

void PartProcess::OnCbnSelchangeComboAttribute()
{
	// TODO: Add your control notification handler code here
	int m_selectnum=this->m_physical_atrribute.GetCurSel();
	int num_scalar=this->pScalarArray->GetNumberOfTuples();

	if(m_selectnum>=num_scalar)//选择为矢量物理属性时设置分量结果
	{
		this->m_component.EnableWindow(1);
		this->m_has_component=TRUE;
	}
	else//选择为标量物理属性时设置为无分量结果
	{
		this->m_component.EnableWindow(0);
		this->m_has_component=FALSE;
	}
}

void PartProcess::SetNumStlModel(const int num)
{
	num_stl_model = num;
}

void PartProcess::OnBnClickedHighlight()
{
	// TODO: Add your control notification handler code here
	if(m_part_highlight)
	{
		m_part_highlight = FALSE;
	}
	else
	{
		m_part_highlight = TRUE;
	}
}

void PartProcess::OnBnClickedSelectEdge()
{
	// TODO: Add your control notification handler code here
	if(m_set_edge_selection)
	{
		m_set_edge_selection = FALSE;
	}
	else
	{
		m_set_edge_selection = TRUE;
	}
}

void PartProcess::OnBnClickedVisibleOnly()
{
	// TODO: Add your control notification handler code here
	if(m_set_visible_only)
	{
		m_set_visible_only = FALSE;
	}
	else
	{
		m_set_visible_only = TRUE;
	}
}

void PartProcess::OnBnClickedVisibleAll()
{
	// TODO: Add your control notification handler code here
	if(m_set_visible_all)
	{
		m_set_visible_all = FALSE;
	}
	else
	{
		m_set_visible_all = TRUE;
	}
}

void PartProcess::OnBnClicked2d()
{
	// TODO: Add your control notification handler code here
	if(m_set_2d)
	{
		m_set_2d = FALSE;
	}
	else
	{
		m_set_2d = TRUE;
	}
}

void PartProcess::OnBnClickedAxis()
{
	// TODO: Add your control notification handler code here
	if(m_set_axis)
	{
		m_set_axis = FALSE;
	}
	else
	{
		m_set_axis = TRUE;
	}
}

void PartProcess::OnBnClicked2dall()
{
	// TODO: Add your control notification handler code here
		if(m_set_2d_all)
	{
		m_set_2d_all = FALSE;
	}
	else
	{
		m_set_2d_all = TRUE;
	}
}

void PartProcess::OnBnClickedInforVisAll()
{
	// TODO: Add your control notification handler code here
	if(m_inform_all)
	{
		m_inform_all = FALSE;
	}
	else
	{
		m_inform_all = TRUE;
	}
}

void PartProcess::OnBnClickedInforVisOnly()
{
	// TODO: Add your control notification handler code here
	if(m_inform_only)
	{
		m_inform_only = FALSE;
	}
	else
	{
		m_inform_only = TRUE;
	}
}
