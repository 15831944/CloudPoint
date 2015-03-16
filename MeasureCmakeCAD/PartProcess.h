#pragma once
#include "afxwin.h"
#include "vtkStringArray.h"

// PartProcess dialog

class PartProcess : public CDialog
{
	DECLARE_DYNAMIC(PartProcess)

public:
	PartProcess(CWnd* pParent = NULL);   // standard constructor
	PartProcess(int match, CWnd* pParent = NULL);//带模式设置的对话框构造函数
	PartProcess(vtkStringArray *pScalars,vtkStringArray *pVectors,int match, CWnd* pParent = NULL);//有物理属性模型对话框构造函数
	virtual ~PartProcess();

	const PartProcess& operator =(const PartProcess& partProcess);
	void SetNumStlModel(const int num);

// Dialog Data
	enum { IDD = IDD_DIALOG_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_visible;
	CString m_model;
	CString m_type;
	CString m_num_cells;
	CString m_num_verts;
	CComboBox m_physical_atrribute;
	CComboBox m_component;
	CComboBox m_model_match_select;

	vtkStringArray *pScalarArray;//标量属性名数组
	vtkStringArray *pVectorArray;//矢量属性名数组

	int m_set_attribute;//选择的映射物理属性
	int m_set_component;//选择的映射物理属性的分量
	int m_set_match; //选择模型匹配的数值
	bool m_set_edge_selection;

	int num_stl_model;

	bool m_part_visible;//模型可见性变量
    bool m_part_legend;//物理属性bar变量
	int m_mode; //0为选择，1为匹配，2为交互，其他不处理

	bool m_has_component;//选择的映射物理属性是否含有分量的变量

	bool m_cad_surface;
	bool m_recognize_model;

	bool m_inform_all;
	bool m_inform_only;
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckLegend();
	afx_msg void OnBnClickedCheckVisible();
	afx_msg void OnCbnSelchangeComboAttribute();
	CButton m_legend;
	BOOL m_part_highlight;
	CButton m_hightlight;
	afx_msg void OnBnClickedHighlight();
	CButton m_select_surface_edge;
	bool m_edge_selection;
	afx_msg void OnBnClickedSelectEdge();
	CButton m_part_visible_all;
	CButton m_part_visible_only;
	bool m_set_visible_all;
	bool m_set_visible_only;
	afx_msg void OnBnClickedVisibleOnly();
	afx_msg void OnBnClickedVisibleAll();
	bool m_set_2d;
	afx_msg void OnBnClicked2d();
	bool m_set_axis;
	afx_msg void OnBnClickedAxis();
	bool m_set_2d_all;
	afx_msg void OnBnClicked2dall();
	CButton m_info_vis_only;
	CButton m_info_vis_all;
	afx_msg void OnBnClickedInforVisAll();
	afx_msg void OnBnClickedInforVisOnly();
};
