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
#include "BasicGeometryConvertion.h"
#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBndLib.hxx>
#include <TopoDS.hxx>

#include <sstream>

#define MAX2(X, Y)	(  Abs(X) > Abs(Y)? Abs(X) : Abs(Y) )
#define MAX3(X, Y, Z)	( MAX2 ( MAX2(X,Y) , Z) )

// Description:
// Instantiate object with NULL filename.
BasicGeometryConvertion::BasicGeometryConvertion()
{
    topologyType = TopAbs_VERTEX;
	precisionOfDiscrete = 0.1;
	index = 0;
	polydata = NULL;
}

BasicGeometryConvertion::~BasicGeometryConvertion()
{
	if(polydata)
	{
		polydata->Delete();
	}
}

//after set the model index and get the basic geometry type
void BasicGeometryConvertion::SetModelName()
{
	if(!geometryType.empty())
	{
		stringstream id;
		id << index;

		modelName = geometryType + "  " + id.str();
	}
}

void BasicGeometryConvertion::GetGeometryType(string &name)
{
	name = geometryType;
}

//void BasicGeometryConvertion::GetRAxis(gp_Ax1 &axis)
//{
//	//gp_Ax1 tmp;
//	//axis = r_axis;
//	//axis = &tmp;
//}

TopoDS_Shape BasicGeometryConvertion::GetTopologyShape()
{
	return shapeModel;
}

void BasicGeometryConvertion::GetName(std::string &name)
{
	name = modelName;
}

void BasicGeometryConvertion::SetTopologyModel(const TopoDS_Shape &model)
{
	shapeModel = model;
}

//get the dicrete model for visulization or other users
void BasicGeometryConvertion::GetPolyData(vtkPolyData *poly)
{
	if( poly != NULL)
	{
		if(polydata != NULL)
			poly->DeepCopy(polydata);
	}

	/*if(polydata != NULL)
		poly->ShallowCopy(polydata);*/
}

void BasicGeometryConvertion::SetIndex(const int ind)
{
	index = ind;
}

void BasicGeometryConvertion::SetPrecisionOfDiscrete(const double precision)
{
	precisionOfDiscrete = precision;
}

void BasicGeometryConvertion::SetTopologyType(TopAbs_ShapeEnum type)
{
	topologyType = type;
}

TopAbs_ShapeEnum BasicGeometryConvertion::GetTopolyogyType()
{
	return topologyType;
}

void BasicGeometryConvertion::Initialization(const TopoDS_Shape& shape)
{
	// Mesh the shape if necessary
	Standard_Boolean alreadymesh = Standard_True;
	
	TopExp_Explorer ex;
	TopLoc_Location aLoc;

	for (ex.Init(shape, TopAbs_FACE); ex.More(); ex.Next()) {
		const TopoDS_Face& aFace = TopoDS::Face(ex.Current());
		Handle(Poly_Triangulation) aPoly = BRep_Tool::Triangulation(aFace,aLoc);
		if(aPoly.IsNull()) { alreadymesh = Standard_False; break; }
	}

	if(!alreadymesh) {
		if(precisionOfDiscrete <= 0) {
			// Compute default deflection
			float x, y, z;
			ComputeBound(shape, x, y, z);
			precisionOfDiscrete = MAX3( x, y, z) * 0.001 *4;
		}
		BRepMesh_IncrementalMesh MESH(shape,precisionOfDiscrete);
	}
}

void BasicGeometryConvertion::ComputeBound(const TopoDS_Shape& shape, float &x, float &y, float &z)
{
	Bnd_Box B;
	BRepBndLib::Add(shape, B);
	Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
	B.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);
	x = aXmax-aXmin,y = aYmax-aYmin, z = aZmax-aZmin;
}

void BasicGeometryConvertion::ComputeBound(const TopoDS_Shape& shape, float &xMin, float &yMin, float &zMin, float &xLen, float &yLen, float &zLen)
{
	Bnd_Box B;
	BRepBndLib::Add(shape, B);
	Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;
	B.Get(aXmin, aYmin, aZmin, aXmax, aYmax, aZmax);
	xLen = aXmax-aXmin,yLen = aYmax-aYmin, zLen = aZmax-aZmin;
	xMin = aXmin, yMin = aYmin, zMin = aZmin;
}