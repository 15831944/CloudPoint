/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: BasicGeometryConvertion.cxx,v $

  Copyright (c) Bing,Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "VertexConvertion.h"

//opencascade 
#include <TopoDS.hxx>
#include <gp_Pnt.hxx>

//vtk
#include <vtkSphereSource.h>
#include <BRep_Tool.hxx>

bool VertexConvertion::TransferModel()
{
	//detect face type 
	geometryType = string("vertex");

	//use sphere to visulize the points
    TransferToSphereSource(shapeModel);

	//set the name of current model
	SetModelName();

	return true;
}

void VertexConvertion::TransferToSphereSource(const TopoDS_Shape &shape)
{
	const TopoDS_Vertex &vert = TopoDS::Vertex(shape);
	
	gp_Pnt pnt = BRep_Tool::Pnt(vert);

	x = pnt.X(), y  = pnt.Y(), z = pnt.Z();

	vtkSphereSource *sp = vtkSphereSource::New();
	sp->SetCenter(x, y, z);
	sp->SetRadius(radius);

	if(polydata == NULL)
	{
		polydata = vtkPolyData::New();
	}

	polydata->DeepCopy(sp->GetOutput());

	sp->Delete();
}

void VertexConvertion::SetDisplayRadiusForPoint(const float r)
{
	radius = r;
}

bool VertexConvertion::operator ==(const VertexConvertion &v)
{
	if(abs(x - v.x) < precisionOfSamePoint && abs(y - v.y) < precisionOfSamePoint  && abs(z - v.z) < precisionOfSamePoint)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float VertexConvertion::GetX()
{
	return x;
}

float VertexConvertion::GetY()
{
	return y;
}

float VertexConvertion::GetZ()
{
	return z;
}

VertexConvertion::VertexConvertion(const VertexConvertion &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	radius = v.radius;

	topologyType = v.topologyType;
	shapeModel = v.shapeModel;
	modelName = v.modelName;
	precisionOfDiscrete = v.precisionOfDiscrete;
	index = v.index;
	geometryType = v.geometryType;

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(v.polydata != NULL)
		polydata->DeepCopy(v.polydata);
}


VertexConvertion& VertexConvertion::operator =(const VertexConvertion &v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	radius = v.radius;

	topologyType = v.topologyType;
	shapeModel = v.shapeModel;
	modelName = v.modelName;
	precisionOfDiscrete = v.precisionOfDiscrete;
	index = v.index;
	geometryType = v.geometryType;

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(v.polydata != NULL)
		polydata->DeepCopy(v.polydata);

	return *this;
}