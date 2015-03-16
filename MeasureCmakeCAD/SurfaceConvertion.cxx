/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: SurfaceConvertion.cxx,v $

  Copyright (c) Bing,Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "SurfaceConvertion.h"

//opencascade
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <Poly_Polygon3D.hxx>
#include <Poly_Triangulation.hxx>
#include <BRep_Tool.hxx>
#include <BRepMesh.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_SurfaceOfLinearExtrusion.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <Geom_OffsetSurface.hxx>

//vtk
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>

/* --------------------------------------------------------------
   after setting the index, precision, and the TopDs model, than 
   us this function to tranfer the model into triangle model with
   the setting precsion---------------------------------------- */

bool SurfaceConvertion::DetectFaceType(const TopoDS_Shape &shape)
{
	const TopoDS_Face &face = TopoDS::Face(shape);

	if(face.IsNull())
	{
		return false;
	}

	Handle(Geom_Surface) aSurf = BRep_Tool::Surface(face);

	Handle(Standard_Type) TheType = aSurf->DynamicType();

	if ( TheType ==  STANDARD_TYPE(Geom_Plane)) {
		geometryType = string("plane");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_CylindricalSurface)) {
		geometryType = string("cylinder");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_ConicalSurface)) {
		geometryType = string("cone");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_SphericalSurface)) {
		geometryType = string("sphere");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_ToroidalSurface)) {
		geometryType = string("torus");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_SurfaceOfLinearExtrusion)) {
		geometryType = string("extrusion");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_SurfaceOfRevolution)) {
		geometryType = string("revolution");
		//Handle(Geom_SurfaceOfRevolution) rSurf = Handle(Geom_SurfaceOfRevolution)::DownCast(aSurf);
		/*Handle(Standard_Type) revType = rSurf->DynamicType();
		if(rSurf->IsKind(STANDARD_TYPE(GeomAbs_CylindricalForm)))
		{
			cout << "revolution is cylinder." << endl;
		}*/
		//r_axis = rSurf->Axis();
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_BezierSurface)) {
		geometryType = string("bezierSurf");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_BSplineSurface)) {
		geometryType = string("bsplineSurf");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_RectangularTrimmedSurface)) {
		geometryType = string("trimmedSurf");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_OffsetSurface)) {
		geometryType = string("offsetSurf");
	}
	else {
		geometryType = string("unknown surface");
	}

	return true;
}

bool SurfaceConvertion::TransferModel()
{
	//detect face type 
	if(!DetectFaceType(shapeModel))
		return false;

	//initilize the dicrete segment precision
	Initialization(shapeModel);

	//tranfer polygon
    if(!TransferToPolygon(shapeModel))
		return false;

	//sub recognize surface type
	SubRecognizeSurface();

	//set the name of current model
	SetModelName();

	return true;
}

void SurfaceConvertion::SubRecognizeSurface()
{
	if(geometryType == "bsplineSurf")
	{
		SubRecognizeBsplineSurface();
	}
	if(geometryType == "revolution")
	{
		SubRecognizeRevolutionSurface();
	}
}

void SurfaceConvertion::SubRecognizeBsplineSurface()
{
	if(this->polydata != NULL)
	{
		//computer the center of all points
		int numPoints = polydata->GetNumberOfPoints();
		double x[3];
		double center[3] = {0.0, 0.0, 0.0};
		for(int i = 0; i < numPoints; i++)
		{
			polydata->GetPoint(i, x);
			for(int j = 0; j < 3; j++)
			{
				center[j] += x[j] / numPoints;
			}
		}

		//pca method for recognize the plane type


	}
}

void SurfaceConvertion::SubRecognizeRevolutionSurface()
{
	const TopoDS_Face &face = TopoDS::Face(shapeModel);
	Handle(Geom_Surface) aSurf = BRep_Tool::Surface(face);
	Handle(Geom_SurfaceOfRevolution) rSurf = Handle(Geom_SurfaceOfRevolution)::DownCast(aSurf);
}

bool SurfaceConvertion::TransferToPolygon(const TopoDS_Shape &shape)
{
	TopLoc_Location location;

	const TopoDS_Face &face = TopoDS::Face(shape);

	Handle_Poly_Triangulation triFace = BRep_Tool::Triangulation(face, location);

	if(triFace.IsNull())
	{
		return false;
	}

	Standard_Integer nTriangles = triFace->NbTriangles();

	gp_Pnt vertex1;
	gp_Pnt vertex2;
	gp_Pnt vertex3;

	Standard_Integer nVertexIndex1 = 0;
	Standard_Integer nVertexIndex2 = 0;
	Standard_Integer nVertexIndex3 = 0;

	TColgp_Array1OfPnt nodes(1, triFace->NbNodes());
	Poly_Array1OfTriangle triangles(1, triFace->NbTriangles());

	nodes = triFace->Nodes();
	triangles = triFace->Triangles();      

	if(polydata == NULL)
	{
		polydata = vtkPolyData::New();
	}

	vtkPoints *points = vtkPoints::New();
	vtkCellArray *cells = vtkCellArray::New();
	points->Allocate(nTriangles * 3);
	cells->Allocate(nTriangles);

	int id = 0;

	for (Standard_Integer i = 1; i <= nTriangles; i++)
	{
		Poly_Triangle aTriangle = triangles.Value(i);

		aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);

		vertex1 = nodes.Value(nVertexIndex1).Transformed(location.Transformation());
		vertex2 = nodes.Value(nVertexIndex2).Transformed(location.Transformation());
		vertex3 = nodes.Value(nVertexIndex3).Transformed(location.Transformation());

		points->InsertNextPoint(vertex1.X(), vertex1.Y(), vertex1.Z());
		points->InsertNextPoint(vertex2.X(), vertex2.Y(), vertex2.Z());
		points->InsertNextPoint(vertex3.X(), vertex3.Y(), vertex3.Z());

		vtkTriangle *triangle = vtkTriangle::New();
		triangle->GetPointIds()->SetId(0,id * 3);
		triangle->GetPointIds()->SetId(1,id * 3 + 1);
		triangle->GetPointIds()->SetId(2,id *3 + 2);

		// Add the triangle to a cell array
		cells->InsertNextCell(triangle);
		id++;

		triangle->Delete();
	}

	polydata->SetPoints(points);
	polydata->SetPolys(cells);

	points->Delete();
	cells->Delete();

	return true;
}

SurfaceConvertion::SurfaceConvertion(const SurfaceConvertion &s)
{
	topologyType = s.topologyType;
	shapeModel = s.shapeModel;
	modelName = s.modelName;
	precisionOfDiscrete = s.precisionOfDiscrete;
	index = s.index;
	geometryType = s.geometryType;
	
	if(s.para != NULL)
	{
		if(para != NULL)
			delete [] para;

		paraLen = s.paraLen;

		para = new double[paraLen];
		for(int i = 0; i < paraLen; i++)
		{
			para[i] = s.para[i];
		}
	}

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(s.polydata != NULL)
		polydata->DeepCopy(s.polydata);
}

SurfaceConvertion & SurfaceConvertion::operator =(const SurfaceConvertion &s)
{
	topologyType = s.topologyType;
	shapeModel = s.shapeModel;
	modelName = s.modelName;
	precisionOfDiscrete = s.precisionOfDiscrete;
	index = s.index;
	geometryType = s.geometryType;

	if(s.para != NULL)
	{
		if(para != NULL)
			delete [] para;

		paraLen = s.paraLen;

		para = new double[paraLen];
		for(int i = 0; i < paraLen; i++)
		{
			para[i] = s.para[i];
		}
	}

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(s.polydata != NULL)
		polydata->DeepCopy(s.polydata);

	return *this;
}