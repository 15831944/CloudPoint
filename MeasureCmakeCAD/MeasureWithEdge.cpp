#include "StdAfx.h"
#include "MeasureWithEdge.h"
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>

//opencascade 
#include <Geom_Curve.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <Extrema_ExtPC.hxx>
#include <gp_Pnt.hxx>
#include <gp.hxx>
#include <Precision.hxx>
#include <TopLoc_Location.hxx>
#include <GeomAdaptor_Curve.hxx>

MeasureWithEdge::MeasureWithEdge(void)
{
}

MeasureWithEdge::~MeasureWithEdge(void)
{
}

MeasureWithEdge::MeasureWithEdge(vtkPolyData *poly, const TopoDS_Shape &shape, double u)
{
	if(poly != NULL && polydata != NULL)
	{
		polydata->DeepCopy(poly);
	}

	shapeModel = shape;

	tU = u;
}

void MeasureWithEdge::SetUTolerent(const double u)
{
	tU = u;
}

void MeasureWithEdge::Compute()
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

		bool res = IterativeEstimateDistance(shapeModel, x, tU, result, cPoint, dis);

		if(!res)
		{
			result = ApproximateEstimateDistance(shapeModel, x, tU,  cPoint, dis);
		}

		distanceAToB->SetTuple1(i, result);
		clostPointAToB->SetTuple3(i, cPoint[0], cPoint[1], cPoint[2]);
		displaceAToB->SetTuple3(i, dis[0], dis[1], dis[2]);
	}

	//output data
	this->polydata->GetPointData()->AddArray(clostPointAToB);
	this->polydata->GetPointData()->AddArray(displaceAToB);
	this->polydata->GetPointData()->AddArray(distanceAToB);

	//for debug
	double r[2];
	distanceAToB->GetRange(r);

	distanceAToB->Delete();
	clostPointAToB->Delete();
	displaceAToB->Delete();
}

//can use kdtree to accerate the agrothim to O(logn), now is O(n) 
float MeasureWithEdge::ApproximateEstimateDistance(const TopoDS_Shape& shape, const double x[3],const double TolU, double closePoint[3], double dis[3])
{
	const TopoDS_Edge &edge = TopoDS::Edge(shape);

	Standard_Real dFirst, dLast;
	TopLoc_Location aEdgeLoc;
	gp_Pnt point;

	Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edge, aEdgeLoc, dFirst, dLast);

	double result = FLT_MAX;

	Precision::IsNegativeInfinite(dFirst) ? dFirst = -1.0 : dFirst;
	Precision::IsInfinite(dLast) ? dLast = 1.0 : dLast;

	gp_Trsf edgeTransf;
	Standard_Boolean isidtrsf = true;
	if(!aEdgeLoc.IsIdentity())  {
		isidtrsf = false;
		edgeTransf = aEdgeLoc.Transformation();
	}

	for (float u = dFirst; u <= dLast; u += TolU)
	{
		point = aCurve->Value(u);

		if(!isidtrsf) {
			// apply edge transformation
			point.Transform(edgeTransf);
		}

		double res = sqrt((x[0] - point.X() ) * (x[0] - point.X() ) + (x[1] - point.Y() ) * (x[1] - point.Y() ) + (x[2] - point.Z() ) * (x[2] - point.Z() ) );

		if(res < result)
		{
			result = res;
			closePoint[0] = point.X(), closePoint[1] = point.Y(), closePoint[2] = point.Z();
			dis[0] = x[0] - point.X(), dis[1] = x[1] - point.Y(), dis[2] = x[2] - point.Z();
		}
	}

	return result;
}

bool MeasureWithEdge::IterativeEstimateDistance(const TopoDS_Shape& shape, const double x[3], const double TolU, double &result, double closePoint[3], double dis[3])
{
	const TopoDS_Edge &edge = TopoDS::Edge(shape);

	Standard_Real dFirst, dLast;

	Handle(Geom_Curve) aCurve = BRep_Tool::Curve(edge, dFirst, dLast);

	gp_Pnt P1, newP1;
	P1.SetX(x[0]), P1.SetY(x[1]), P1.SetZ(x[2]);

	GeomAdaptor_Curve curve(aCurve);
	Extrema_ExtPC ext1(P1,curve,dFirst,dLast);

	result = FLT_MAX;

	if ( ext1.IsDone() && ext1.NbExt() > 0 )
	{
		for ( Standard_Integer i = 1 ; i <= ext1.NbExt() ; i++ )
		{
			if ( ext1.IsMin(i) )
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
		}

		return true;
	}

	return false;
}