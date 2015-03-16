#pragma once
#include "afxwin.h"
#include <string>
#include <vector>

using namespace std;
// MeasurementDialog dialog

class MeasurementDialog : public CDialog
{
	DECLARE_DYNAMIC(MeasurementDialog)

public:
	MeasurementDialog(CWnd* pParent = NULL);   // standard constructor
	MeasurementDialog(const int nStl,CWnd* pParent = NULL);
	MeasurementDialog(const int nStl, const vector<string> & surf, const vector<string> &edge, CWnd* pParent = NULL);
	virtual ~MeasurementDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_COMPUTE };

	int m_cad_surf_out;
	int m_cad_edge_out;
	int m_stl_out;

	bool m_use_cad_for_measure;
	bool m_use_cad_surf_or_edge; //1 for surf, 0 for edge
	
	int m_initial_cad_surf;
	int m_initial_cad_edge;
	int m_initial_stl;

private:
	vector<string> surfName;
	vector<string> edgeName;

	int numStl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_computing_type;
	CComboBox m_cad_edge;
	CComboBox m_cad_surface;
	CComboBox m_stl;
	afx_msg void OnBnClickedCadCompute();
	afx_msg void OnBnClickedRadioStlCompute();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CButton m_cad_surf_or_edge;
	afx_msg void OnBnClickedCheckSurforedge();
};
