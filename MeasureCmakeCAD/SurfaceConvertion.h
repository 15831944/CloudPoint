/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: SurfaceConvertion.h,v $

  Copyright (c) Bing, Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME SurfaceConvertion - read .igs iges files
// .SECTION Description
// SurfaceConvertion is a source object that reads .igs .iges files with the
// geometry kernel of opencascade  
// files. The output of this source object is polygonal data and also geometry
// topology model of TopDS type with opencascade 
// .SECTION See Also

#ifndef __SurfaceConvertion_h
#define __SurfaceConvertion_h

#include "BasicGeometryConvertion.h"

class SurfaceConvertion : public BasicGeometryConvertion
{
public:
	SurfaceConvertion()
	{
		para = NULL;
		paraLen = 0;
	};
	~SurfaceConvertion()
	{
		if(para != NULL)
			delete [] para;
	};
	bool TransferModel();

	SurfaceConvertion(const SurfaceConvertion&);
	SurfaceConvertion& operator=(const SurfaceConvertion&);
protected:
	bool TransferToPolygon(const TopoDS_Shape& shape);
	bool DetectFaceType(const TopoDS_Shape& shape);

	void SubRecognizeRevolutionSurface();
	void SubRecognizeBsplineSurface();
	void SubRecognizeSurface();

	double *para; //parameter for trimmed or extrused basic surface
	int paraLen;
};

#endif