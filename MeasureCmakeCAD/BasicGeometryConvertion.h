/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: BasicGeometryConvertion.h,v $

  Copyright (c) Bing, Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME BasicGeometryConvertion - convert the basic geometry models
// .SECTION Description
// BasicGeometryConvertion is a source object that convert the opencascade 
// models into the vtk system
// .SECTION See Also

#ifndef __BasicGeometryConvertion_h
#define __BasicGeometryConvertion_h

//opencasecade
#include <TopoDS_Shape.hxx>
#include <gp_Ax1.hxx>


//vtk
#include <vtkPolyData.h>


#include <string>

using namespace std;

class BasicGeometryConvertion
{
public:
	BasicGeometryConvertion();
	~BasicGeometryConvertion();

	void SetTopologyModel(const TopoDS_Shape& model);
	void SetIndex(const int index);
	void SetPrecisionOfDiscrete(const double precision);
	void SetTopologyType(TopAbs_ShapeEnum type);
	void GetName(string &name);
	void GetPolyData(vtkPolyData *poly);
	void GetGeometryType(string &name);
	//void GetRAxis(gp_Ax1 &axis);
	TopoDS_Shape GetTopologyShape();
	TopAbs_ShapeEnum GetTopolyogyType();

	virtual bool TransferModel() = 0;

protected:
	void Initialization(const TopoDS_Shape& shape);
	void SetModelName();
	void ComputeBound(const TopoDS_Shape& shape, float &x, float &y, float &z);
	void ComputeBound(const TopoDS_Shape& shape, float &xMin, float &yMin, float &zMin, float &xLen, float &yLen, float &zLen);

protected:
	TopAbs_ShapeEnum topologyType;
	TopoDS_Shape shapeModel;
	string modelName;  //model name
	vtkPolyData *polydata;
	double precisionOfDiscrete;
	int index;
	string geometryType; //the string name of the surface type
	//gp_Ax1 r_axis;
	
private:
	BasicGeometryConvertion(const BasicGeometryConvertion&);  // Not implemented.
	void operator=(const BasicGeometryConvertion&);  // Not implemented.
};

#endif