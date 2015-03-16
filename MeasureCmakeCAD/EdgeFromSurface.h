#ifndef __EdgeFromSurface_h
#define __EdgeFromSurface_h

#include "EdgeConvertion.h"

#include <list>

//opencasecade
#include <TopoDS_Shape.hxx>

using namespace std;

class EdgeFromSurface
{
public:
	EdgeFromSurface();
	~EdgeFromSurface();

	void ConvertModel();

	void SetTopologyModel(const TopoDS_Shape& model);
	TopoDS_Shape GetTopologyShape();

	void SetEdgePrecision(const float p);

public:
	list<EdgeConvertion*> edges;

private:
	TopoDS_Shape shapeModel;
	float edgePrecision;
	bool readyForConvertion;

private:
	void ConvertEdge(const TopoDS_Shape& model, int index);

	EdgeFromSurface(const EdgeFromSurface&);  // Not implemented.
	void operator=(const EdgeFromSurface&);  // Not implemented.
};

#endif