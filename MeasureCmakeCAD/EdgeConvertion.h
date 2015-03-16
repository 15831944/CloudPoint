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

#ifndef __EdgeConvertion_h
#define __EdgeConvertion_h

#include "BasicGeometryConvertion.h"

class EdgeConvertion : public BasicGeometryConvertion
{
public:
	EdgeConvertion(){};
	~EdgeConvertion(){}
	bool TransferModel();
	float GetStart();
	float GetEnd();

	bool operator==(const EdgeConvertion&);
	EdgeConvertion(const EdgeConvertion&);
	EdgeConvertion& operator=(const EdgeConvertion&);

protected:
	bool TransferToPolyline(const TopoDS_Shape& shape);
	bool DetectEdgeType(const TopoDS_Shape& shape);

private:
	float start;
	float end;

	float xMin;
	float yMin;
	float zMin;
	float xLen;
	float yLen;
	float zLen;

	double proPrecision;
};

#endif