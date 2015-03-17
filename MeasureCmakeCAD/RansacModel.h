#pragma once

#include "vtkPolyData.h"
#include "ShapePrimitivePara.h"
#include <vector>
#include <vtkActorCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <PointCloud.h>
using namespace std;

class RansacModel
{
public:
	RansacModel(void);
	~RansacModel(void);

	void SetPara(float disThrehold, float projThrehold, float probThrehold, float normThrehold, float minPoint, bool usePlane, bool useCylinder, bool useCone, bool useSphere, bool useTorus);
	void SetInputData(vtkPolyData *poly);
	void InitializeOrModification(bool init);

	//before use this function should set parameter and type first
	void Updata();

	//should updata first
	void DisplayModel(vtkActorCollection* actorCollection);

	//normal for plane, axis for cylinder
	void DisplayHighLevelInfo(vtkActorCollection* actorCollection);

	//set the scale factor for normal visulization
	void SetNormalScale(const float s);
	float GetNormalScale();

	void OutPutFile(const char* fileName);

private:
	void ShapeRecognition();
	void ShapeModification();

	//shape information
	void GetShapeInfoPositions(float p1[3], float p2[3], const int idex);

private:
	//parameter
	float m_distance_ths;
	int m_minimal_points;
	float m_normal_ths;
	float m_probability_ths;
	float m_projection_ths;
	bool m_set_plane;
	bool m_set_cylinder;
	bool m_set_cone;
	bool m_set_sphere;
	bool m_set_torus;

	//scale for normal
	float m_scale;

	//initialize or modify
	bool m_init;


public:
	//data
	vtkPolyData *polydata;
	vector<ShapePrimitivePara*> primitives;
	vector<bool> outputMarks;
    vector<bool> modifyMarks;
	//static double p1x;
	//static double p1y;
	//static double p1z;
	//static double p2x;
	//static double p2y;
	//static double p2z;
	//static double p3x;
	//static double p3y;
	//static double p3z;
	//void getInteractivePoints(vector<double>p);
};
//double RansacModel::p1x=0;