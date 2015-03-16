//--------------------------------------------------------------
//ģ��ʵ����󣬰���pCloudDataSetCollection��pStlDataSetCollection
//�������ݼ��ϣ�����ܹ��������ݴ������ʾ����Ϣ
//--------------------------------------------------------------
#ifndef _ENTITY_VTK_DEF_H_
#define _ENTITY_VTK_DEF_H_

#include "vtkStringArray.h"
#include "vtkPolyDataCollection.h" 
#include <vector>
#include "IGESToVTKConvertion.h"
#include "EdgeFromSurface.h"
#include "RansacModel.h"
#include "IgesAndVtkModel.h"
#include <string>
using namespace std;

class Entity
{
public:
	Entity();
	~Entity();

	vtkPolyDataCollection *pStlDataSetCollection;//STLģ�����ݼ���
	vtkPolyDataCollection *pPointCloudDataSetCollection;//STLģ�����ݼ���
	vtkPolyDataCollection *pModelDataCollection;//cadģ�����ݼ��� yuki
	vtkStringArray *pStlCaseNameArray;
	vtkStringArray *pPointCloudCaseNameArray;
	RansacModel *recModels; //Recognized model
	vector<IgesAndVtkModel*> cadModels;

	int current_stl_part_num;//��ǰѡ���STLģ����
	int current_point_cloud_part_num;//��ǰѡ���point cloudģ����
    int stl_for_match_to_point; //���Ƶ�ǰѡ������stlģ��

	EdgeFromSurface *surfEdges; //ѡ������еı�ת��������
	int current_cad_num;
	int current_edge_num;
	int current_surface_num;   //������Ŀ
	vector<bool> highLightEdge;
	vector<string> edgeNames;
	vector<bool> highLightSurface;
	vector<int> modelEndId;
	int numTotalSurf;

	list<CString> t_fileName;  //file name yuki
	vector<int> surfacesize;//wenjianchang yuki
	//IGESToVTKConvertion convert; //cad model with igs file type  vector<IGESToVTKConvertion*>



	enum ViewMode{Interact = 0, Match, Query} mMode;

	void setInteractiveStyleMode(ViewMode mode);
	ViewMode getInteractiveStyleMode();
protected:
private:
};

#endif