#pragma once
#include "afxwin.h"


// OutPutDlg dialog

class OutPutDlg : public CDialog
{
	DECLARE_DYNAMIC(OutPutDlg)

public:
	OutPutDlg(CWnd* pParent = NULL);   // standard constructor
	OutPutDlg(int numPointCloud, int numStl, CWnd* pParent = NULL);
	virtual ~OutPutDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT };

	//属性
public:
	int m_point_cloud_out;//设置是pointcloud模型的值
	int m_stl_out;//设置stl模型的值

private:
	int num_point_cloud;
	int num_stl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_point_cloud;
	CComboBox m_stl;
	CButton m_out_model;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedRadioPoint();
	afx_msg void OnBnClickedRadioStl();
};
