#include "StdAfx.h"
#include "MeasureWithBasicGeometry.h"

MeasureWithBasicGeometry::MeasureWithBasicGeometry(void)
{
	polydata = vtkPolyData::New();
}

MeasureWithBasicGeometry::~MeasureWithBasicGeometry(void)
{
	if(polydata != NULL)
	{
		polydata->Delete();
	}
}

void MeasureWithBasicGeometry::SetInputPolydata(vtkPolyData *poly)
{
	if(poly != NULL)
	{
		polydata->DeepCopy(poly);
	}
}

void MeasureWithBasicGeometry::GetOutput(vtkPolyData *poly)
{
	if(poly != NULL)
	{
		poly->DeepCopy(polydata);
	}
}

void MeasureWithBasicGeometry::SetFeatureSurfaceModel(const TopoDS_Shape &shape)
{
	shapeModel = shape;
}