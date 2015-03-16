#pragma once


// XPComputeDlg dialog

class XPComputeDlg : public CDialog
{
	DECLARE_DYNAMIC(XPComputeDlg)

public:
	XPComputeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~XPComputeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_XP };
	double L;//杆长
	int count;//装配步长
	double temp;			//与间隙有关常量
	double px, py;;		//点P
	double cx, cy, cr;			//点O，及以杆内径D为半径的圆
	double pi;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditL();
	afx_msg void OnEnChangeEditCount();
	afx_msg void OnEnChangeEditPi();
	afx_msg void OnEnChangeEditTemp();
	afx_msg void OnEnChangeEditPx();
	afx_msg void OnEnChangeEditPy();
	afx_msg void OnEnChangeEditCx();
	afx_msg void OnEnChangeEditCy();
	afx_msg void OnEnChangeEditCr();
};
