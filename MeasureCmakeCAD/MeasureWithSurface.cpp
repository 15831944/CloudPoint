#include "StdAfx.h"
#include "MeasureWithSurface.h"
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

//opencascade
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <Geom_Surface.hxx>
#include <gp_Pnt.hxx>
#include <gp.hxx>
#include <Precision.hxx>
#include <TopLoc_Location.hxx>
#include <Extrema_ExtPS.hxx>
#include <GeomAdaptor_Surface.hxx>

MeasureWithSurface::MeasureWithSurface(void)
{
}

MeasureWithSurface::~MeasureWithSurface(void)
{
}

MeasureWithSurface::MeasureWithSurface(vtkPolyData *poly, const TopoDS_Shape &shape, double u, double v)
{
	if(poly != NULL && polydata != NULL)
	{
		polydata->DeepCopy(poly);
	}

	shapeModel = shape;

	tU = u;
	tV = v;
}

void MeasureWithSurface::SetUVTolerent(const double u, const double v)
{
	tU = u;
	tV = v;
}

void MeasureWithSurface::Compute()
{
	double x[3], dis[3], cPoint[3], result;
	int num = this->polydata->GetNumberOfPoints();
	vtkDoubleArray *distanceAToB = vtkDoubleArray::New();
	distanceAToB->SetNumberOfComponents(1);
	distanceAToB->SetNumberOfTuples(num);
	distanceAToB->SetName( "Distance" );

	vtkDoubleArray *clostPointAToB = vtkDoubleArray::New();
	clostPointAToB->SetNumberOfComponents(3);
	clostPointAToB->SetNumberOfTuples(num);
	clostPointAToB->SetName( "ClosePoint" );

	vtkDoubleArray *displaceAToB = vtkDoubleArray::New();
	displaceAToB->SetNumberOfComponents(3);
	displaceAToB->SetNumberOfTuples(num);
	displaceAToB->SetName( "Displace" );

	//estimate distance for every points
	for(int i = 0; i < num; i++)
	{
		this->polydata->GetPoints()->GetPoint(i, x);

		bool res = IterativeEstimateDistance(shapeModel, x, tU, tV, result, cPoint, dis);

		if(!res)
		{
			result = ApproximateEstimateDistance(shapeModel, x, tU, tV, cPoint, dis);
		}

	    distanceAToB->SetTuple1(i, result);
		clostPointAToB->SetTuple3(i, cPoint[0], cPoint[1], cPoint[2]);
		displaceAToB->SetTuple3(i, dis[0], dis[1], dis[2]);
	}

	//output data
	this->polydata->GetPointData()->AddArray(clostPointAToB);
	this->polydata->GetPointData()->AddArray(displaceAToB);
	this->polydata->GetPointData()->AddArray(distanceAToB);

	distanceAToB->Delete();
	clostPointAToB->Delete();
	displaceAToB->Delete();
}

//can use kdtree to accerate the agrothim to O(logn), now is O(n) 
float MeasureWithSurface::ApproximateEstimateDistance(const TopoDS_Shape& shape, const double x[3],const double TolU, const double TolV, double closePoint[3], double dis[3])
{
	const TopoDS_Face &face = TopoDS::Face(shape);

	TopLoc_Location aSurfLoc;
	gp_Pnt point;

	Handle(Geom_Surface) aSurface = BRep_Tool::Surface(face, aSurfLoc);

	double result = FLT_MAX;

	Standard_Real uFirst = 0.0;
	Standard_Real vFirst = 0.0;
	Standard_Real uLast = 0.0;
	Standard_Real vLast = 0.0;

	aSurface->Bounds(uFirst, uLast, vFirst, vLast);

	Precision::IsNegativeInfinite(uFirst) ? uFirst = -1.0 : uFirst;
	Precision::IsInfinite(uLast) ? uLast = 1.0 : uLast;

	Precision::IsNegativeInfinite(vFirst) ? vFirst = -1.0 : vFirst;
	Precision::IsInfinite(vLast) ? vLast = 1.0 : vLast;

	gp_Trsf surfTransf;
	Standard_Boolean isidtrsf = true;
	if(!aSurfLoc.IsIdentity())  {
		isidtrsf = false;
		surfTransf = aSurfLoc.Transformation();
	}

	for (Standard_Real u = uFirst; u <= uLast; u += TolU)
	{
		for (Standard_Real v = vFirst; v <= vLast; v += TolV)
		{
			point = aSurface->Value(u, v);

			if(!isidtrsf) {
				// apply edge transformation
				point.Transform(surfTransf);
			}

			double res = sqrt((x[0] - point.X() ) * (x[0] - point.X() ) + (x[1] - point.Y() ) * (x[1] - point.Y() ) + (x[2] - point.Z() ) * (x[2] - point.Z() ) );

			if(res < result)
			{
				result = res;
				closePoint[0] = point.X(), closePoint[1] = point.Y(), closePoint[2] = point.Z();
				dis[0] = x[0] - point.X(), dis[1] = x[1] - point.Y(), dis[2] = x[2] - point.Z();
			}
		}
	}

	return result;
}

bool MeasureWithSurface::IterativeEstimateDistance(const TopoDS_Shape& shape, const double x[3], const double TolU, const double TolV, double &result, double closePoint[3], double dis[3])
{
	const TopoDS_Face &face = TopoDS::Face(shape);

	gp_Pnt point;

	Handle(Geom_Surface) aSurface = BRep_Tool::Surface(face);

	Standard_Real uFirst = 0.0;
	Standard_Real vFirst = 0.0;
	Standard_Real uLast = 0.0;
	Standard_Real vLast = 0.0;

	aSurface->Bounds(uFirst, uLast, vFirst, vLast);

	Precision::IsNegativeInfinite(uFirst) ? uFirst = -1.0 : uFirst;
	Precision::IsInfinite(uLast) ? uLast = 1.0 : uLast;

	Precision::IsNegativeInfinite(vFirst) ? vFirst = -1.0 : vFirst;
	Precision::IsInfinite(vLast) ? vLast = 1.0 : vLast;

	result = FLT_MAX;

	gp_Pnt P1, newP1;
	P1.SetX(x[0]), P1.SetY(x[1]), P1.SetZ(x[2]);

	GeomAdaptor_Surface surf(aSurface);
	Extrema_ExtPS ext1(P1, surf, 0.001, 0.001);
	if ( ext1.IsDone() && ext1.NbExt() > 0 )
	{
		for ( Standard_Integer i = 1 ; i <= ext1.NbExt() ; i++ )
		{
			newP1 = ext1.Point(i).Value();

			double res = sqrt((P1.X() - newP1.X() ) * (P1.X() - newP1.X() ) + (P1.Y() - newP1.Y() ) * (P1.Y() - newP1.Y() ) + (P1.Z() - newP1.Z() ) * (P1.Z() - newP1.Z() ) );

			if(res < result)
			{
				result = res;
				closePoint[0] = newP1.X(), closePoint[1] = newP1.Y(), closePoint[2] = newP1.Z();
				dis[0] = P1.X() - newP1.X(), dis[1] = P1.Y() - newP1.Y(), dis[2] = P1.Z() - newP1.Z();
			}
		}

		return true;
	}

	return false;
}