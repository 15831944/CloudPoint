/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: IGESToVTKConvertion.h,v $

  Copyright (c) Bing, Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME IGESToVTKConvertion - read .igs iges files
// .SECTION Description
// IGESToVTKConvertion is a source object that reads .igs .iges files with the
// geometry kernel of opencascade  
// files. The output of this source object is polygonal data and also geometry
// topology model of TopDS type with opencascade 
// .SECTION See Also

#ifndef __IGESToVTKConvertion_h
#define __IGESToVTKConvertion_h

#include "VertexConvertion.h"
#include "EdgeConvertion.h"
#include "SurfaceConvertion.h"

#include <list>

//opencasecade
#include <TopoDS_Shape.hxx>

using namespace std;

class IGESToVTKConvertion
{
public:
	IGESToVTKConvertion();
	~IGESToVTKConvertion();

	void ConvertModel();

	void SetTopologyModel(const TopoDS_Shape& model);
	TopoDS_Shape GetTopologyShape();

//	void SetEdgePrecision(const float p);
	void SetSurfacePrecision(const float p);

public:
//	list<VertexConvertion*> vertexs;
//	list<EdgeConvertion*> edges;
	list<SurfaceConvertion*> surfaces;

private:
	TopoDS_Shape shapeModel;
//	float edgePrecision;
	float surfPrecision;
	bool readyForConvertion;

private:
    void ConvertSurface(const TopoDS_Shape& model, int index);
//	void ConvertEdge(const TopoDS_Shape& model, int index);
//	void ConvertVertex(const TopoDS_Shape& model, int index);

	IGESToVTKConvertion(const IGESToVTKConvertion&);  // Not implemented.
	void operator=(const IGESToVTKConvertion&);  // Not implemented.
};

#endif