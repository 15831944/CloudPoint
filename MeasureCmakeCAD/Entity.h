//--------------------------------------------------------------
//模型实体对象，包含pCloudDataSetCollection，pStlDataSetCollection
//两个数据集合，因此能够保存数据处理和显示的信息
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

	vtkPolyDataCollection *pStlDataSetCollection;//STL模型数据集合
	vtkPolyDataCollection *pPointCloudDataSetCollection;//STL模型数据集合
	vtkPolyDataCollection *pModelDataCollection;//cad模型数据集合 yuki
	vtkStringArray *pStlCaseNameArray;
	vtkStringArray *pPointCloudCaseNameArray;
	RansacModel *recModels; //Recognized model
	vector<IgesAndVtkModel*> cadModels;

	int current_stl_part_num;//当前选择的STL模型数
	int current_point_cloud_part_num;//当前选择的point cloud模型数
    int stl_for_match_to_point; //点云当前选择计算的stl模型

	EdgeFromSurface *surfEdges; //选择的面中的边转换后数据
	int current_cad_num;
	int current_edge_num;
	int current_surface_num;   //总面数目
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