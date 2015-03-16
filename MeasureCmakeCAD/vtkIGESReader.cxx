/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkIGESReader.cxx,v $

  Copyright (c) Bing,Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "vtkIGESReader.h"
#include <TColStd_HSequenceOfTransient.hxx>
#include <IGESControl_Reader.hxx>

// Description:
// Instantiate object with NULL filename.
vtkIGESReader::vtkIGESReader()
{
	shapeType = TopAbs_VERTEX;
}

vtkIGESReader::~vtkIGESReader()
{
	if (this->m_fileName)
	{
		delete [] this->m_fileName;
		this->m_fileName = NULL;
	}
}

void vtkIGESReader::SetFileName(const char* fileName)
{
	int num = strlen(fileName) + 1;
	this->m_fileName = new char[num];
	strcpy(m_fileName, fileName);
}

void vtkIGESReader::ReadModel()
{
	if ( this->m_fileName == NULL)
	{
		return;
	}
	
	//read shape model
	IGESControl_Reader Reader;
	Standard_Integer nIgesFaces,nTransFaces;

    Standard_Integer status = Reader.ReadFile( m_fileName );

	Handle(TColStd_HSequenceOfTransient) myList =  Reader.GiveList("iges-faces");
	nIgesFaces = myList->Length();  
    nTransFaces = Reader.TransferList(myList); 

	cout<<"IGES Faces: "<<nIgesFaces<<" Transferred:"<<nTransFaces<<endl;

    shapeModel = Reader.OneShape(); 

    //detect shape type	
	if(!shapeModel.IsNull())
	{
		ReadShapeType();
	}
}

TopAbs_ShapeEnum vtkIGESReader::GetShapeType()
{
	return shapeType;
}

TopoDS_Shape vtkIGESReader::GetTopologyShape()
{
	return shapeModel;
}

void vtkIGESReader::ReadShapeType()
{
	// Get shape type
	TopAbs_ShapeEnum shapeType = shapeModel.ShapeType();

	// Print out shape type
	switch (shapeType)
	{
	case TopAbs_VERTEX:
		cout << "VERTEX" << endl;
		break;

	case TopAbs_COMPOUND:
		cout << "COMPOUND" << endl;
		break;

	case TopAbs_COMPSOLID:
		cout << "COMPSOLID" << endl;
		break;

	case TopAbs_EDGE:
		cout << "EDGE" << endl;
		break;

	case TopAbs_FACE:
		cout << "FACE" << endl;
		break;

	case TopAbs_SHAPE:
		cout << "SHAPE" << endl;
		break;

	case TopAbs_SHELL:
		cout << "SHELL" << endl;
		break;

	case TopAbs_SOLID:
		cout << "SOLID" << endl;
		break;

	case TopAbs_WIRE:
		cout << "WIRE" << endl;
		break;

	default:
		cerr << "ERROR: " << ": Internal error." << endl;
		return;

	} // switch inputShapeType
}