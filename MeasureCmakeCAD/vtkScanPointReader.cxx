/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkScanPointReader.cxx,v $

  Copyright (c) Bing,Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "vtkScanPointReader.h"

#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkTriangle.h>

#include <sstream>
#include <fstream>

vtkStandardNewMacro(vtkScanPointReader);

// Description:
// Instantiate object with NULL filename.
vtkScanPointReader::vtkScanPointReader()
{
  this->FileName = NULL;

  this->SetNumberOfInputPorts(0);
}

vtkScanPointReader::~vtkScanPointReader()
{
  if (this->FileName)
  {
    delete [] this->FileName;
    this->FileName = NULL;
  }
}

/*--------------------------------------------------------
the file type is:
x y z
x y z
......

---------------------------------------------------------*/

int vtkScanPointReader::RequestData(
                              vtkInformation *vtkNotUsed(request),
    vtkInformationVector **vtkNotUsed(inputVector),
                                      vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (!this->FileName) 
  {
    vtkErrorMacro(<< "A FileName must be specified.");
    return 0;
  }

  std::ifstream in;

  std::locale::global(std::locale(""));
  in.open(this->FileName);
  std::locale::global(std::locale("C"));

  if (!in) 
  {
    vtkErrorMacro(<< "File " << this->FileName << " not found");
    return 0;
  }

  vtkDebugMacro(<<"Reading file");

  // intialise some structures to store the file contents in
  vtkPoints *points = vtkPoints::New(); 
  vtkCellArray *polys = vtkCellArray::New();
  points->Allocate(1000);
  polys->Allocate(1000);

  // -- work through the file line by line, assigning into the above structures as appropriate --

//  float xyz[3];

  std::string line;

  //Read until we get to the first line that is not a comment or a blank line.
  //This line second states the number of vertices, faces, and edges. 
  while(getline(in, line))
  {
    //std::cout << line << std::endl;
    if(line.size() == 0)
      continue;
    if(line[0] == '#')
      continue;

    //if we get to here, this is the info line
    break;
  }

  //get vertex
  int VertexCounter = 0;
  while(getline(in, line))
  {
    if(line.size() == 0) //skip blank lines (they should only occur before the vertices start)
      continue;

    std::stringstream ssVertex;
    ssVertex << line;
    float x,y,z;
    ssVertex >> x >> y >> z;
    points->InsertNextPoint(x, y, z);
	polys->InsertNextCell(1,&VertexCounter);
    //std::cout << "adding vertex: " << x << " " << y << " " << z << std::endl;
    VertexCounter++;
  } // (end of vertex while loop)

  // we have finished with the file
  in.close(); 

  output->SetPoints(points);
  output->SetVerts(polys);

  points->Delete();
  polys->Delete();

  return 1;
}

void vtkScanPointReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "File Name: " 
      << (this->FileName ? this->FileName : "(none)") << "\n";
}
