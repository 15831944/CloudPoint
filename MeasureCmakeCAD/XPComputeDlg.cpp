// XPComputeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "XPComputeDlg.h"


// XPComputeDlg dialog

IMPLEMENT_DYNAMIC(XPComputeDlg, CDialog)

XPComputeDlg::XPComputeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XPComputeDlg::IDD, pParent)
{

}

XPComputeDlg::~XPComputeDlg()
{
}

void XPComputeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(XPComputeDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_L, &XPComputeDlg::OnEnChangeEditL)
	ON_EN_CHANGE(IDC_EDIT_COUNT, &XPComputeDlg::OnEnChangeEditCount)
	ON_EN_CHANGE(IDC_EDIT_PI, &XPComputeDlg::OnEnChangeEditPi)
	ON_EN_CHANGE(IDC_EDIT_TEMP, &XPComputeDlg::OnEnChangeEditTemp)
	ON_EN_CHANGE(IDC_EDIT_PX, &XPComputeDlg::OnEnChangeEditPx)
	ON_EN_CHANGE(IDC_EDIT_PY, &XPComputeDlg::OnEnChangeEditPy)
	ON_EN_CHANGE(IDC_EDIT_CX, &XPComputeDlg::OnEnChangeEditCx)
	ON_EN_CHANGE(IDC_EDIT_CY, &XPComputeDlg::OnEnChangeEditCy)
	ON_EN_CHANGE(IDC_EDIT_CR, &XPComputeDlg::OnEnChangeEditCr)
END_MESSAGE_MAP()


// XPComputeDlg message handlers

void XPComputeDlg::OnEnChangeEditL()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_1;
	CString str;
	char *pstr;
	p_para_1=(CEdit*) GetDlgItemText(IDC_EDIT_L,str);
	pstr=str.GetBuffer(str.GetLength());
	L=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();

}

void XPComputeDlg::OnEnChangeEditCount()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_2;
	CString str;
	char *pstr;
	p_para_2=(CEdit*) GetDlgItemText(IDC_EDIT_COUNT,str);
	pstr=str.GetBuffer(str.GetLength());
	count=atoi(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditPi()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_3;
	CString str;
	char *pstr;
	p_para_3=(CEdit*) GetDlgItemText(IDC_EDIT_PI,str);
	pstr=str.GetBuffer(str.GetLength());
	pi=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditTemp()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_4;
	CString str;
	char *pstr;
	p_para_4=(CEdit*) GetDlgItemText(IDC_EDIT_TEMP,str);
	pstr=str.GetBuffer(str.GetLength());
	temp=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditPx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_5;
	CString str;
	char *pstr;
	p_para_5=(CEdit*) GetDlgItemText(IDC_EDIT_PX,str);
	pstr=str.GetBuffer(str.GetLength());
	px=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditPy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_6;
	CString str;
	char *pstr;
	p_para_6=(CEdit*) GetDlgItemText(IDC_EDIT_PY,str);
	pstr=str.GetBuffer(str.GetLength());
	py=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditCx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_7;
	CString str;
	char *pstr;
	p_para_7=(CEdit*) GetDlgItemText(IDC_EDIT_CX,str);
	pstr=str.GetBuffer(str.GetLength());
	cx=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditCy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_8;
	CString str;
	char *pstr;
	p_para_8=(CEdit*) GetDlgItemText(IDC_EDIT_CY,str);
	pstr=str.GetBuffer(str.GetLength());
	cy=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}

void XPComputeDlg::OnEnChangeEditCr()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* p_para_9;
	CString str;
	char *pstr;
	p_para_9=(CEdit*) GetDlgItemText(IDC_EDIT_CR,str);
	pstr=str.GetBuffer(str.GetLength());
	cr=atof(pstr);//cout<<para1<<endl;
	str.ReleaseBuffer();
}
