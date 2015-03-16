#pragma once

//opencascade
#include <TopoDS_Shape.hxx>

//vtk
#include <vtkPolyData.h>

class MeasureWithBasicGeometry
{
public:
	MeasureWithBasicGeometry(void);
	~MeasureWithBasicGeometry(void);

	void SetInputPolydata(vtkPolyData *poly);
	void SetFeatureSurfaceModel(const TopoDS_Shape &shape);
	virtual void Compute() = 0;
	void GetOutput(vtkPolyData *poly);

protected:
	vtkPolyData *polydata;
	TopoDS_Shape shapeModel;
};
