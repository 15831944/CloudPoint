/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkScanPointReader.h,v $

  Copyright (c) Bing, Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkScanPointReader - read .sp files
// .SECTION Description
// vtkScanPointReader is a source object that reads .sp
// files. The output of this source object is polygonal data.
// .SECTION See Also

#ifndef __vtkScanPointReader_h
#define __vtkScanPointReader_h

#include <vtkPolyDataAlgorithm.h>

class vtkScanPointReader : public vtkPolyDataAlgorithm
{
public:
  static vtkScanPointReader *New();
  vtkTypeMacro(vtkScanPointReader,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Specify file name of .off file.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

protected:
  vtkScanPointReader();
  ~vtkScanPointReader();

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  char *FileName;
private:
  vtkScanPointReader(const vtkScanPointReader&);  // Not implemented.
  void operator=(const vtkScanPointReader&);  // Not implemented.
};

#endif
