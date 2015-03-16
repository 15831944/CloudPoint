#pragma once


// CMyCameraDlg dialog

class CMyCameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyCameraDlg)

public:
	CMyCameraDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyCameraDlg();

	double m_viewangle;
	float m_azimuthangle;
	
	double m_dolly;
	double m_yaw;
	double m_roll;
	double m_xcenter;
	double m_ycenter;
	double m_elevation;
	double m_xpoisition;
	double m_ypoisition;
	double m_zpoisition;
	double m_VX;
	double m_VY;
	double m_VZ;
	double m_xfocal;
	double m_yfocal;
	double m_zfocal;
	double m_xnormal;
	double m_ynormal;
	double m_znormal;
	
	double m_dis;
	double m_rBag;
	double m_gBag;
	
	double m_bBag;
	double m_zoomIn;
	double m_zoomOut;

// Dialog Data
	enum { IDD = IDD_DIALOG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditxcenter();
	afx_msg void OnEnChangeEditviewangle();
};
