/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: vtkScanPointWriter.h,v $

Copyright (c) Bing, Yi
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkScanPointWriter - output .sp files
// .SECTION Description
// vtkScanPointWriter is a source object that output .sp
// files. 
// .SECTION See Also

#ifndef __vtkScanPointWriter_h
#define __vtkScanPointWriter_h

#include <fstream>
#include "vtkPolyData.h"

class vtkScanPointWriter
{
public:
	vtkScanPointWriter();
	~vtkScanPointWriter();

	void SetFileName(const char* fileName);
	void SetInputPolyData(vtkPolyData* polyData);
	void WriteData();

protected:
	void WritePoints(vtkPolyData* poly);

	char *m_fileName;
	vtkPolyData* m_data;
private:
	vtkScanPointWriter(const vtkScanPointWriter&);  // Not implemented.
	void operator=(const vtkScanPointWriter&);  // Not implemented.
};

#endif
