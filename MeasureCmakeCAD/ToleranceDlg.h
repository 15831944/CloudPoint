#pragma once

#include "RansacModel.h"
#include "TolerOutPut.h"
#include <PointCloud.h>
#include <vector>

using namespace std;


// ToleranceDlg 对话框

class ToleranceDlg : public CDialog
{
	DECLARE_DYNAMIC(ToleranceDlg)

public:
	ToleranceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ToleranceDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOLERANCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	bool b_flatness_set;
	bool b_parallelism_set;
	bool b_perpendicularity_set;
	bool b_angle_set;
	bool b_size_set;
	CComboBox ccb_feature_1;
	CComboBox ccb_feature_2;
	int counter_feature_1;
	int counter_feature_2;
	double tolerance;

	RansacModel *model;

public:
	afx_msg void OnBnClickedFlatness();
	afx_msg void OnBnClickedParallelism();
	afx_msg void OnBnClickedPerpendicularity();
	afx_msg void OnBnClickedAngle();
	afx_msg void OnBnClickedSize();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCalculator();
public:
	vector<TolerOutPut> record_of_toler;
	afx_msg void OnMenuExportTorlerance();
	void OutPutFile(const char* fileName);
	afx_msg void OnBnClickedButtonExportFile();
};
