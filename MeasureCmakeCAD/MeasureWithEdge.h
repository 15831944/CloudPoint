#pragma once
#include "MeasureWithBasicGeometry.h"

class MeasureWithEdge : public MeasureWithBasicGeometry
{
public:
	MeasureWithEdge(void);
	MeasureWithEdge(vtkPolyData *poly, const TopoDS_Shape &shape, double u = 0.0001);
	~MeasureWithEdge(void);

	void SetUTolerent(const double u);
	void Compute();

private:
	bool IterativeEstimateDistance(const TopoDS_Shape& shape, const double x[3], const double TolU, double &result, double closePoint[3], double dis[3]);
	float ApproximateEstimateDistance(const TopoDS_Shape& shape, const double x[3],const double TolU, double closePoint[3], double dis[3]);

private:
	double tU;
};
