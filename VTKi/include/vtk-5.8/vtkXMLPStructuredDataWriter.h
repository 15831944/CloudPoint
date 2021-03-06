/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkXMLPStructuredDataWriter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkXMLPStructuredDataWriter - Superclass for PVTK XML structured data writers.
// .SECTION Description
// vtkXMLPStructuredDataWriter provides PVTK XML writing functionality
// that is common among all the parallel structured data formats.

#ifndef __vtkXMLPStructuredDataWriter_h
#define __vtkXMLPStructuredDataWriter_h

#include "vtkXMLPDataWriter.h"

class vtkXMLStructuredDataWriter;

class VTK_IO_EXPORT vtkXMLPStructuredDataWriter : public vtkXMLPDataWriter
{
public:
  vtkTypeMacro(vtkXMLPStructuredDataWriter,vtkXMLPDataWriter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
protected:
  vtkXMLPStructuredDataWriter();
  ~vtkXMLPStructuredDataWriter();
  
  virtual vtkXMLStructuredDataWriter* CreateStructuredPieceWriter()=0;
  void WritePrimaryElementAttributes(ostream &os, vtkIndent indent);
  void WritePPieceAttributes(int index);
  vtkXMLWriter* CreatePieceWriter(int index);
  
private:
  vtkXMLPStructuredDataWriter(const vtkXMLPStructuredDataWriter&);  // Not implemented.
  void operator=(const vtkXMLPStructuredDataWriter&);  // Not implemented.
};

#endif
