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
	double L;//�˳�
	int count;//װ�䲽��
	double temp;			//���϶�йس���
	double px, py;;		//��P
	double cx, cy, cr;			//��O�����Ը��ھ�DΪ�뾶��Բ
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
