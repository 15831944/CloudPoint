#include "StdAfx.h"
#include "EdgeFromSurface.h"
#include <TColStd_HSequenceOfTransient.hxx>
#include <TopExp_Explorer.hxx>

EdgeFromSurface::EdgeFromSurface()
{
	edgePrecision = 0.01;
	readyForConvertion = false;
}

EdgeFromSurface::~EdgeFromSurface()
{
	//delete edge list
	if(edges.size())
	{
		list<EdgeConvertion*>::iterator iter;
		for(iter = edges.begin(); iter != edges.end(); iter++)
		{
			delete (*iter);
		}
	}
}

void EdgeFromSurface::SetEdgePrecision(const float p)
{
	edgePrecision = p;
}

void EdgeFromSurface::SetTopologyModel(const TopoDS_Shape& model)
{
	shapeModel = model;
	readyForConvertion = true;
}

TopoDS_Shape EdgeFromSurface::GetTopologyShape()
{
	return shapeModel;
}

void EdgeFromSurface::ConvertModel()
{
	if(readyForConvertion)
	{
		TopExp_Explorer surfExp, edgeExp, verExp;
		int index;

		//convert edge
		for(edgeExp.Init(shapeModel, TopAbs_EDGE), index = 1; edgeExp.More(); edgeExp.Next())
		{
			ConvertEdge(edgeExp.Current(), index);
			index++;
		}
	}
}

void EdgeFromSurface::ConvertEdge(const TopoDS_Shape &model, int index)
{
	EdgeConvertion *edge = new EdgeConvertion();
	edge->SetTopologyModel(model);
	edge->SetTopologyType(TopAbs_EDGE);
	edge->SetIndex(index);
	edge->SetPrecisionOfDiscrete(edgePrecision);
	
	
	if(edge->TransferModel())
	{
		edges.push_back(edge);
	}
}