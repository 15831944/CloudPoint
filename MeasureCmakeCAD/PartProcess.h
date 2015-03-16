#pragma once
#include "afxwin.h"
#include "vtkStringArray.h"

// PartProcess dialog

class PartProcess : public CDialog
{
	DECLARE_DYNAMIC(PartProcess)

public:
	PartProcess(CWnd* pParent = NULL);   // standard constructor
	PartProcess(int match, CWnd* pParent = NULL);//��ģʽ���õĶԻ����캯��
	PartProcess(vtkStringArray *pScalars,vtkStringArray *pVectors,int match, CWnd* pParent = NULL);//����������ģ�ͶԻ����캯��
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

	vtkStringArray *pScalarArray;//��������������
	vtkStringArray *pVectorArray;//ʸ������������

	int m_set_attribute;//ѡ���ӳ����������
	int m_set_component;//ѡ���ӳ���������Եķ���
	int m_set_match; //ѡ��ģ��ƥ�����ֵ
	bool m_set_edge_selection;

	int num_stl_model;

	bool m_part_visible;//ģ�Ϳɼ��Ա���
    bool m_part_legend;//��������bar����
	int m_mode; //0Ϊѡ��1Ϊƥ�䣬2Ϊ����������������

	bool m_has_component;//ѡ���ӳ�����������Ƿ��з����ı���

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
