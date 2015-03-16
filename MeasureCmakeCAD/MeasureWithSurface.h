#pragma once
#include "MeasureWithBasicGeometry.h"

class MeasureWithSurface : public MeasureWithBasicGeometry
{
public:
	MeasureWithSurface(void);
	MeasureWithSurface(vtkPolyData *poly, const TopoDS_Shape &shape, double u = 0.0001, double v = 0.0001);
    ~MeasureWithSurface(void);

	void SetUVTolerent(const double u, const double v);
	void Compute();

private:
	bool IterativeEstimateDistance(const TopoDS_Shape& shape, const double x[3], const double TolU, const double TolV, double &result, double closePoint[3], double dis[3]);
	float ApproximateEstimateDistance(const TopoDS_Shape& shape, const double x[3],const double TolU, const double TolV, double closePoint[3], double dis[3]);


private:
	double tU;
	double tV;
};
