//--------------------------------------------------------------
//实体模型形位公差计算
//--------------------------------------------------------------
#ifndef _IGES_AND_VTK_MODEL_DEF_H_
#define _IGES_AND_VTK_MODEL_DEF_H_

#include "IGESToVTKConvertion.h"
#include <vtkActorCollection.h>

#include <vector>
#include <string>
using namespace std;

//opencasecade
#include <TopoDS_Shape.hxx>

class IgesAndVtkModel
{
public:
	IgesAndVtkModel(void);
	~IgesAndVtkModel(void);

	void ConvetModel(const TopoDS_Shape& model, const float p, const int startId);
	void SetModelName(string name);

	//should updata first
	void DisplayModel(vtkActorCollection* actorCollection, const double r = 0.0, const double g = 0.0, const double b = 1.0);

public:
	IGESToVTKConvertion convert; //cad model with igs file type  vector<IGESToVTKConvertion*>
	string modelName;

	vector<string> surfaceNames;

	int startSurfId;
	int endSurfId;

private:
	bool hasConverted;
};


#endif