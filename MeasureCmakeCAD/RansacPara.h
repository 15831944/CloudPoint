#pragma once
#include "afxwin.h"


// RansacPara dialog

class RansacPara : public CDialog
{
	DECLARE_DYNAMIC(RansacPara)

public:
	RansacPara(CWnd* pParent = NULL);   // standard constructor
	virtual ~RansacPara();

// Dialog Data
	enum { IDD = IDD_DIALOG_RANSAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_distance_ths;
	int m_minimal_points;
	float m_normal_ths;
	float m_probability_ths;
	float m_projection_ths;
	CButton m_plane;
	CButton m_cone;
	CButton m_cylinder;
	CButton m_sphere;
	CButton m_torus;
	CComboBox m_part;
	int m_selected_part;
	int num_parts;

	bool m_set_plane;
	bool m_set_cylinder;
	bool m_set_cone;
	bool m_set_sphere;
	bool m_set_torus;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPlane();
	afx_msg void OnBnClickedCylinder();
	afx_msg void OnBnClickedCone();
	afx_msg void OnBnClickedSphere();
	afx_msg void OnBnClickedTorus();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedTours();
};
