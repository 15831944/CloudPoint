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

#ifndef __vtkIGESReader_h
#define __vtkIGESReader_h

//opencasecade
#include <TopoDS_Shape.hxx>

class vtkIGESReader
{
public:
	vtkIGESReader();
	~vtkIGESReader();

	void SetFileName(const char* fileName);
	void ReadModel();
	TopAbs_ShapeEnum GetShapeType();
	TopoDS_Shape GetTopologyShape();

protected:
	TopAbs_ShapeEnum shapeType;
	TopoDS_Shape shapeModel;
	char *m_fileName;  //file name
private:
	void ReadShapeType();
	vtkIGESReader(const vtkIGESReader&);  // Not implemented.
	void operator=(const vtkIGESReader&);  // Not implemented.
};

#endif