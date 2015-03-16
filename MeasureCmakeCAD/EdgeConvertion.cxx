/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: BasicGeometryConvertion.cxx,v $

Copyright (c) Bing,Yi
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "EdgeConvertion.h"

//opencascade
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_OffsetCurve.hxx>

//vtk
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkMath.h>

bool EdgeConvertion::DetectEdgeType(const TopoDS_Shape& shape)
{
	const TopoDS_Edge &edge = TopoDS::Edge(shape);

	if(edge.IsNull())
	{
		return false;
	}

	Standard_Real dFirst, dLast;

	Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edge, dFirst, dLast);

	if(aCurve.IsNull())
	{
		return false;
	}

	Handle(Standard_Type) TheType = aCurve->DynamicType();

	if ( TheType ==STANDARD_TYPE(Geom_Line)) {
		geometryType = string("line");
	}
	else if ( TheType ==  STANDARD_TYPE(Geom_Circle)) {
		geometryType = string("circle");
	}
	else if ( TheType == STANDARD_TYPE(Geom_Ellipse)) {
		geometryType = string("ellipse");
	}
	else if ( TheType == STANDARD_TYPE(Geom_Parabola)) {
		geometryType = string("parabola");
	}
	else if ( TheType == STANDARD_TYPE(Geom_Hyperbola)) {
		geometryType = string("hyperbola");
	}
	else if ( TheType == STANDARD_TYPE(Geom_BezierCurve)) {
		geometryType = string("bezierCurve");
	}
	else if ( TheType == STANDARD_TYPE(Geom_BSplineCurve)) {
		geometryType = string("bsplineCurve");
	}
	else if ( TheType == STANDARD_TYPE(Geom_TrimmedCurve)) {
		geometryType = string("trimmedCurve");
	}
	else if ( TheType == STANDARD_TYPE(Geom_OffsetCurve)) {
		geometryType = string("offsetCurve");
	}
	else {
		geometryType = string("unknownCurve");
	}

	return true;
}

bool EdgeConvertion::TransferToPolyline(const TopoDS_Shape &shape)
{
	const TopoDS_Edge &edge = TopoDS::Edge(shape);

	float segment;
	gp_Pnt point,uPoint;
	Standard_Real dFirst, dLast;
	TopLoc_Location aEdgeLoc;

	Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edge,aEdgeLoc, dFirst,dLast);

	if(aCurve.IsNull())
	{
		return false;
	}

	Precision::IsNegativeInfinite(dFirst) ? dFirst = -1.0 : dFirst;
	Precision::IsInfinite(dLast) ? dLast = 1.0 : dLast;

	start = dFirst;
	end = dLast;

	segment = (dLast - dFirst) * precisionOfDiscrete;

	//debug
	//cout << "first: " << start << " " << "end: " << end << " " << "segment:" << segment <<endl;

	//transformation detection
	gp_Trsf edgeTransf;
	Standard_Boolean isidtrsf = true;
	if(!aEdgeLoc.IsIdentity())  {
		isidtrsf = false;
		edgeTransf = aEdgeLoc.Transformation();
	}

	//construct polyline
	int pts[2];
	float coord[3], uCoord[3];

	if(polydata == NULL)
	{
		polydata = vtkPolyData::New();
	}

	vtkPoints *points = vtkPoints::New();
	vtkCellArray *cells = vtkCellArray::New();
	points->Allocate((dLast - dFirst) / segment);
	cells->Allocate((dLast - dFirst) / segment * 2);

	int num = (dLast - dFirst) / segment;
	
	//debug
	/*int numS = num;*/

	if(num < 2 || num > (1 / precisionOfDiscrete) * 1.1 )
	{
		return false;
	}

	//bound precision
	ComputeBound(shape, xMin, yMin, zMin, xLen, yLen, zLen);
    proPrecision = sqrt(xLen * xLen + yLen * yLen + zLen * zLen);

	for (float u = dFirst; u <= dLast; u += segment)
	{
		point = aCurve->Value(u);
		uPoint = aCurve->Value(dLast - segment * (num));

		if(!isidtrsf) {
			// apply edge transformation
			point.Transform(edgeTransf);
			uPoint.Transform(edgeTransf);
		}
		coord[0] = point.X(); coord[1] = point.Y(); coord[2] = point.Z();
		uCoord[0] = uPoint.X(); uCoord[1] = uPoint.Y(); uCoord[2] = uPoint.Z();

		//max discrete distance
		double dis =  vtkMath::Distance2BetweenPoints(coord, uCoord);
		proPrecision = proPrecision > dis ? proPrecision : dis;

		if(abs(u - dFirst) < segment / 5)
		{
			//insert first points
			pts[1] = pts[0] = points->InsertNextPoint(coord);

			//for debug
			//cout << " firt point:" << u - dFirst << endl;
		}
		else
		{
			//inset next points and line element
			pts[0] = pts[1];
			pts[1] = points->InsertNextPoint(coord);
			cells->InsertNextCell(2, pts);
		}

		num--;
	}

	polydata->SetPoints(points);
	polydata->SetLines(cells);

	points->Delete();
	cells->Delete();

	return true;
}

bool EdgeConvertion::TransferModel()
{
	//detect face type 
	if(!DetectEdgeType(shapeModel))
		return false;

	//tranfer polyline
	TransferToPolyline(shapeModel);

	//set the name of current model
	SetModelName();

	return true;
}

EdgeConvertion::EdgeConvertion(const EdgeConvertion &e)
{
	start = e.start;
	end = e.end;

	topologyType = e.topologyType;
	shapeModel = e.shapeModel;
	modelName = e.modelName;
	precisionOfDiscrete = e.precisionOfDiscrete;
	index = e.index;
	geometryType = e.geometryType;

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(e.polydata != NULL)
		polydata->DeepCopy(e.polydata);
}

EdgeConvertion & EdgeConvertion::operator =(const EdgeConvertion &e)
{
	start = e.start;
	end = e.end;

	topologyType = e.topologyType;
	shapeModel = e.shapeModel;
	modelName = e.modelName;
	precisionOfDiscrete = e.precisionOfDiscrete;
	index = e.index;
	geometryType = e.geometryType;

	if(polydata == NULL)
		polydata = vtkPolyData::New();

	if(e.polydata != NULL)
		polydata->DeepCopy(e.polydata);

	return *this;
}

//compare if two edgeconvertionsion is the same should be used after the tranfer function have been used
bool EdgeConvertion::operator ==(const EdgeConvertion &e)
{
	if(geometryType.compare(e.geometryType) == 0)
	{
		if(e.polydata != NULL && polydata != NULL)
		{
			if(e.polydata->GetNumberOfPoints() == polydata->GetNumberOfPoints())
			{
				if(e.start == start && e.end == end)
				{
					if(abs(xMin - e.xMin) < 0.01 && abs(yMin - e.yMin) < 0.01 && abs(zMin - e.zMin) < 0.01
						&& abs(xLen - e.xLen) < 0.01 && abs(yLen - e.yLen) < 0.01 && abs(zLen - e.zLen) < 0.01)
					{
						return true;
					}

					//to do with more acturate method removing the same edge
					/*int num = polydata->GetNumberOfPoints();

					double p1[3], p2[3], p3[3];
					bool sameDir = true, inverDir = true;
					double sum = 0.0, inverSum = 0.0;

					for(int i = 0; i < num; i++)
					{
						polydata->GetPoints()->GetPoint(i, p1);

						e.polydata->GetPoints()->GetPoint(i, p2);

						e.polydata->GetPoints()->GetPoint(num - i -1, p3);

						if(abs(p1[0] - p2[0]) > proPrecision || abs(p1[1] - p2[1]) > proPrecision || abs(p1[2] - p2[2]) > proPrecision)
						{
							sameDir = false;
						}

						if(abs(p1[0] - p3[0]) > proPrecision || abs(p1[1] - p3[1]) > proPrecision || abs(p1[2] - p3[2]) > proPrecision)
						{
							inverDir = false;
						}

						if(!(inverDir || sameDir))
						{
							return false;
						}
					}

					return true;*/
				}
			}
		}
	}

	return false;
}