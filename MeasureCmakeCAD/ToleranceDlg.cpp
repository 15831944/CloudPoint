// ToleranceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "ToleranceDlg.h"
#include <math.h>
#include "XingZhuangGongCha.h"
#include "XingWeiGongCha.h"
// ToleranceDlg 对话框

IMPLEMENT_DYNAMIC(ToleranceDlg, CDialog)

ToleranceDlg::ToleranceDlg(CWnd* pParent /*=NULL*/)
: CDialog(ToleranceDlg::IDD, pParent)
{
	this->b_flatness_set = false;
	this->b_parallelism_set = false;
	this->b_perpendicularity_set = false;
	this->b_size_set = false;
	this->b_angle_set = false;

	this->counter_feature_1 = -1;
	this->counter_feature_2 = -1;
	this->tolerance = 0.01;

	this->model = NULL;
}

ToleranceDlg::~ToleranceDlg()
{
}

void ToleranceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOLERANCE, tolerance);
	DDX_Control(pDX, IDC_COMBO1, ccb_feature_1);
	DDX_Control(pDX, IDC_COMBO2, ccb_feature_2);
}


BEGIN_MESSAGE_MAP(ToleranceDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, &ToleranceDlg::OnBnClickedFlatness)
	ON_BN_CLICKED(IDC_CHECK2, &ToleranceDlg::OnBnClickedParallelism)
	ON_BN_CLICKED(IDC_CHECK3, &ToleranceDlg::OnBnClickedPerpendicularity)
	ON_BN_CLICKED(IDC_CHECK4, &ToleranceDlg::OnBnClickedAngle)
	ON_BN_CLICKED(IDC_CHECK5, &ToleranceDlg::OnBnClickedSize)
	ON_BN_CLICKED(IDC_BUTTON1, &ToleranceDlg::OnBnClickedCalculator)
	ON_COMMAND(ID_MENU_EXPORT_TORLERANCE, &ToleranceDlg::OnMenuExportTorlerance)
	ON_BN_CLICKED(IDC_BUTTON2, &ToleranceDlg::OnBnClickedButtonExportFile)
END_MESSAGE_MAP()


// ToleranceDlg 消息处理程序

void ToleranceDlg::OnBnClickedFlatness()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->b_flatness_set)
	{
		this->b_flatness_set = false;
	}
	else
	{
		this->b_flatness_set = true;
	}
}

void ToleranceDlg::OnBnClickedParallelism()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->b_parallelism_set)
	{
		this->b_parallelism_set = false;
	}
	else
	{
		this->b_parallelism_set = true;
	}
}

void ToleranceDlg::OnBnClickedPerpendicularity()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->b_perpendicularity_set)
	{
		this->b_perpendicularity_set = false;
	}
	else
	{
		this->b_perpendicularity_set = true;
	}
}

void ToleranceDlg::OnBnClickedAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->b_angle_set)
	{
		this->b_angle_set = false;
	}
	else
	{
		this->b_angle_set = true;
	}
}

void ToleranceDlg::OnBnClickedSize()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->b_size_set)
	{
		this->b_size_set = false;
	}
	else
	{
		this->b_size_set = true;
	}
}

BOOL ToleranceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//if(this->feature_num)//设置stl模型list
	//{
	//	cout<<feature_num<<endl;
	//	CString str;

	//	for(int i=0;i<this->feature_num;i++)
	//	{
	//		//str.Format("%s%d", this->_feature_name[i], i+1);
	//		char *c_i = new char[3];
	//		itoa(i+1, c_i, 10);
	//		string tmp = _feature_name[i] + c_i;
	//		CString cstr_tmp(tmp.c_str());
	//		str = cstr_tmp;
	//		this->ccb_feature_1.AddString(str);
	//		this->ccb_feature_2.AddString(str);
	//		delete [] c_i;
	//	}
	//}

	if(this->model != NULL)
	{
		int numPrims = this->model->primitives.size();

		for(int i = 0; i < numPrims; i++)
		{
			CString str;
			str.Format("%s%d",  this->model->primitives[i]->shapeType.c_str(), i+1);
			this->ccb_feature_1.AddString(str);
			this->ccb_feature_2.AddString(str);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ToleranceDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::OnOK();
	counter_feature_1 = this->ccb_feature_1.GetCurSel();
	counter_feature_2 = this->ccb_feature_2.GetCurSel();
}
void ToleranceDlg::OnBnClickedCalculator()
{
	// TODO: 在此添加控件通知处理程序代码
	string tolerance_type;
	if(this->model != NULL)
	{
		if(b_flatness_set)//calulate the flatness
		{
			cout<<"Flatness Testing......\n";
			cout<<"Select the plane feature in first feature!\n";
			int selectPrim = ccb_feature_1.GetCurSel();
			if( selectPrim > -1)
			{
				cout<<">>> Begin...\n";
				//plane parameter
				XingZhuangGongCha planeFlat(this->model->polydata, this->model->primitives[selectPrim]);
				planeFlat.ComputePingMianDu(this->tolerance);
				cout<<"DONE!"<<endl;
				UpdateData(FALSE);
				cout<<">>> End Testing!";
				tolerance_type = "Flatness";
			}
		}

		if(b_parallelism_set)//calculate the parallelism
		{
			cout<<"Parallenlism Testing......\n";
			cout<<"Select the two planes features and first feature is base!\n";
			int basePrim = ccb_feature_1.GetCurSel();
			int surfPrim = ccb_feature_2.GetCurSel();

			if(basePrim > -1 && surfPrim > -1)
			{
				cout<<">>> Begin...\n";

				XingWeiGongCha primPara(this->model->polydata, this->model->primitives[basePrim], this->model->primitives[surfPrim]);
				primPara.ComputeParallelism(this->tolerance);
				UpdateData(FALSE);
				cout<<">>> End Testing\n";
				tolerance_type = "Parallenlism";
			}
		}

		if(b_perpendicularity_set)//calculate the perpendicularity
		{
			cout<<"Perpendicularity Testing......\n";
			cout<<"Select the two features and first feature is base!\n";
			int basePrim = ccb_feature_1.GetCurSel();
			int surfPrim = ccb_feature_2.GetCurSel();

			if(basePrim > -1 && surfPrim > -1)
			{
				cout<<">>> Begin...\n";

				XingWeiGongCha primPara(this->model->polydata, this->model->primitives[basePrim], this->model->primitives[surfPrim]);
				primPara.ComputePerpendicularity(this->tolerance);
				UpdateData(FALSE);
				cout<<">>> End Testing\n";
				tolerance_type = "Perpendicularity";
			}
		}

		if(b_angle_set)
		{
			cout<<"Angle Testing......\n";
			cout<<"Select the two features and first feature is base!\n";

			int basePrim = ccb_feature_1.GetCurSel();
			int surfPrim = ccb_feature_2.GetCurSel();

			if(basePrim > -1 && surfPrim > -1)
			{
				cout<<">>> Begin...\n";

				XingWeiGongCha primPara(this->model->polydata, this->model->primitives[basePrim], this->model->primitives[surfPrim]);
				primPara.ComputeShapeAngles(this->tolerance);
				UpdateData(FALSE);
				cout<<">>> End Testing\n";
				tolerance_type = "Angle";
			}
		}

		//record the result of calculation
		int base = ccb_feature_1.GetCurSel();
		int surf = ccb_feature_2.GetCurSel();

		TolerOutPut tmp_toler_record;
		vector<string> feature_name;
		if(base>-1)
			feature_name.push_back(this->model->primitives[base]->shapeType);
		if(surf>-1)
			feature_name.push_back(this->model->primitives[surf]->shapeType);

		tmp_toler_record.Get_Tolerance(tolerance,base,surf,feature_name,tolerance_type);
		this->record_of_toler.push_back(tmp_toler_record);
	}
	//for(int i=0;i<record_of_toler.size();i++)
	//{
	//	record_of_toler[i].get_toler();
	//}
}

void ToleranceDlg::OnMenuExportTorlerance()
{
	// TODO: 在此添加命令处理程序代码
	cout<<"enter the func"<<endl;
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"recognized model files(*.ra)|*.ra||",NULL);//创建输出文件对话框，设置输出文件名
	if(dlg.DoModal()==IDOK)
	{ 
		cout<<"enter if"<<endl;
		CString strName=dlg.GetPathName();//得到文件路径
		if(this->record_of_toler.size()>0)
		{
			this->OutPutFile(strName);
		}
	}

}

void ToleranceDlg::OutPutFile(const char* fileName)
{
	ofstream fout(fileName);
	for(int i = 0; i < this->record_of_toler.size();i++)
	{
		fout<<"# Tolerance "<<i+1<<'\n';
		fout<<"# Name of Tested Tolerance \n";
		fout<< record_of_toler[i].tolerType() <<'\n';
		fout<<"# Correspond Features \n";
		vector<string> tmp_fea = record_of_toler[i].correspond_fea();
		vector<int> tmp_id = record_of_toler[i].ID_fea();
		cout<<tmp_fea[0]<<endl;
		cout<<tmp_id[0]<<endl;
		for(int j=0;j<record_of_toler[i].correspond_fea().size();j++)
		{
			fout<<record_of_toler[i].correspond_fea()[j]<<" "<<tmp_id[j]+1<<endl;
		}
		fout<<"# Base Feature \n";
		fout<<record_of_toler[i].correspond_fea()[0]<<" "<<tmp_id[0]+1<<endl;
		fout<<"# Tolerance Value \n";
		fout<<record_of_toler[i].tolerance()<<endl;
	}
	fout.close();
}
void ToleranceDlg::OnBnClickedButtonExportFile()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"calculated tolerance *.txt||",NULL);//创建输出文件对话框，设置输出文件名
	if(dlg.DoModal()==IDOK)
	{ 
		CString strName=dlg.GetPathName();//得到文件路径
		if(this->record_of_toler.size()>0)
		{
			this->OutPutFile((LPCTSTR)strName);
		}
	}
}
