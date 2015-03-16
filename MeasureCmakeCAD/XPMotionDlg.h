#include <list>
#include <string>

#pragma once


// XPMotionDlg dialog

class XPMotionDlg : public CDialog
{
	DECLARE_DYNAMIC(XPMotionDlg)

public:
	XPMotionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~XPMotionDlg();

	void SetNumModel(const int num);


// Dialog Data
	enum { IDD = IDD_DIALOG_MOTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_model_motion_select1;
	CComboBox m_model_motion_select2;
	CComboBox m_model_motion_select3;

	int m_set_motion1;
	int m_set_motion2;
	int m_set_motion3;

	int num_model;

	std::list<CString> model_name;
protected:
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboMpart1();
	afx_msg void OnCbnSelchangeComboMpart2();
	afx_msg void OnCbnSelchangeComboMpart3();
};
