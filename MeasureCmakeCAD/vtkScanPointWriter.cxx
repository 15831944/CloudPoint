/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: vtkScanPointWriter.cxx,v $

Copyright (c) Bing,Yi
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "vtkScanPointWriter.h"
#include "vtkFloatArray.h"
#include "vtkErrorCode.h"

#include <sstream>
#include <fstream>

// Description:
// Instantiate object with NULL filename.
vtkScanPointWriter::vtkScanPointWriter()
{
	this->m_fileName = NULL;

	this->m_data = NULL;
}

vtkScanPointWriter::~vtkScanPointWriter()
{
	if (this->m_fileName)
	{
		delete [] this->m_fileName;
		this->m_fileName = NULL;
	}

	this->m_data = NULL;
}

/*--------------------------------------------------------
the file type is:
x y z
x y z
......

---------------------------------------------------------*/

void vtkScanPointWriter::SetFileName(const char* fileName)
{
	int num = strlen(fileName) + 1;
	this->m_fileName = new char[num];
	strcpy(m_fileName, fileName);
}

void vtkScanPointWriter::SetInputPolyData(vtkPolyData* polyData)
{
	this->m_data = polyData;
}

void vtkScanPointWriter::WriteData()
{
	vtkPoints *pts;

	pts = this->m_data->GetPoints();
	if (pts == NULL)
	{
		return;
	}

	if ( this->m_fileName == NULL)
	{
		return;
	}

	this->WritePoints(m_data);
}

void vtkScanPointWriter::WritePoints(vtkPolyData *poly)
{
	ofstream outFile;

	std::locale::global(std::locale(""));
	outFile.open(this->m_fileName, ios::out);
	std::locale::global(std::locale("C"));

	if(outFile.fail())
	{
		cout << "creat file for out error!" << endl;
		return;
	}
	int numPoint = poly->GetNumberOfPoints();

	for(int i = 0; i < numPoint; i++)
	{
		double x[3];
		poly->GetPoint(i,x);
		outFile << x[0] << "        " << x[1] << "        " << x[2] << "        " << endl;
	}

	outFile.close();
}

