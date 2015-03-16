/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkIGESReader.h,v $

  Copyright (c) Bing, Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkIGESReader - read .igs iges files
// .SECTION Description
// vtkIGESReader is a source object that reads .igs .iges files with the
// geometry kernel of opencascade  
// files. The output of this source object is polygonal data and also geometry
// topology model of TopDS type with opencascade 
// .SECTION See Also

#ifndef __VertexConvertion_h
#define __VertexConvertion_h

#include "BasicGeometryConvertion.h"

class VertexConvertion : public BasicGeometryConvertion
{
public:
	VertexConvertion(){ radius = 1.0; x = y = z = 0.0; ; precisionOfSamePoint = 0.0001;};
	~VertexConvertion(){};
	bool TransferModel();
	void SetDisplayRadiusForPoint(const float r);
	float GetX();
	float GetY();
	float GetZ();
	bool operator==(const VertexConvertion& v);

	VertexConvertion(const VertexConvertion& v);
	VertexConvertion& operator=(const VertexConvertion& v); 

protected:
	void TransferToSphereSource(const TopoDS_Shape& shape);
private:
	float radius;
	float x;
	float y;
	float z;
	float precisionOfSamePoint;
};

#endif