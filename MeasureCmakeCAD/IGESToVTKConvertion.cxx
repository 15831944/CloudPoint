/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: IGESToVTKConvertion.cxx,v $

  Copyright (c) Bing,Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "stdafx.h"
#include "IGESToVTKConvertion.h"
#include <TColStd_HSequenceOfTransient.hxx>
#include <IGESControl_Reader.hxx>
#include <TopExp_Explorer.hxx>
// Description:
// Instantiate object with NULL filename.

IGESToVTKConvertion::IGESToVTKConvertion()
{
	surfPrecision = 0.1;
	readyForConvertion = false;
}

IGESToVTKConvertion::~IGESToVTKConvertion()
{
	//delete surfaces list
	if(surfaces.size())
	{
		list<SurfaceConvertion*>::iterator iter;
		for(iter = surfaces.begin(); iter != surfaces.end(); iter++)
		{
			delete (*iter);
		}
	}

	//delete edge list
	/*if(edges.size())
	{
		list<EdgeConvertion*>::iterator iter;
		for(iter = edges.begin(); iter != edges.end(); iter++)
		{
			delete (*iter);
		}
	}*/

	//delete vertex list
	/*if(vertexs.size())
	{
		list<VertexConvertion*>::iterator iter;
		for(iter = vertexs.begin(); iter != vertexs.end(); iter++)
		{
			delete (*iter);
		}
	}*/
}

//void IGESToVTKConvertion::SetEdgePrecision(const float p)
//{
//	edgePrecision = p;
//}

void IGESToVTKConvertion::SetSurfacePrecision(const float p)
{
	surfPrecision = p;
}

void IGESToVTKConvertion::SetTopologyModel(const TopoDS_Shape& model)
{
	shapeModel = model;
	readyForConvertion = true;
}

TopoDS_Shape IGESToVTKConvertion::GetTopologyShape()
{
	return shapeModel;
}

void IGESToVTKConvertion::ConvertModel()
{
	if(readyForConvertion)
	{
		TopExp_Explorer surfExp, edgeExp, verExp;
		int index;

		//convert surface
		for(surfExp.Init(shapeModel, TopAbs_FACE), index = 1; surfExp.More(); surfExp.Next())
		{
			ConvertSurface(surfExp.Current(), index);
			index++;
		}

		//convert edge
		/*for(edgeExp.Init(shapeModel, TopAbs_EDGE), index = 1; edgeExp.More(); edgeExp.Next())
		{
			ConvertEdge(edgeExp.Current(), index);
			index++;
		}*/

		//convert vertex
		/*for(verExp.Init(shapeModel, TopAbs_VERTEX), index = 1; verExp.More(); verExp.Next())
		{
			ConvertVertex(verExp.Current(), index);
			index++;
		}*/
	}
}

void IGESToVTKConvertion::ConvertSurface(const TopoDS_Shape &model, int index)
{
	SurfaceConvertion *surf = new SurfaceConvertion();
	surf->SetTopologyModel(model);
	surf->SetTopologyType(TopAbs_FACE);
	surf->SetIndex(index);
	surf->SetPrecisionOfDiscrete(surfPrecision);
	
	if(surf->TransferModel())
	{
		surfaces.push_back(surf);
	}
}

//void IGESToVTKConvertion::ConvertEdge(const TopoDS_Shape &model, int index)
//{
//	EdgeConvertion *edge = new EdgeConvertion();
//	edge->SetTopologyModel(model);
//	edge->SetTopologyType(TopAbs_EDGE);
//	edge->SetIndex(index);
//	edge->SetPrecisionOfDiscrete(edgePrecision);
//	
//	
//	if(edge->TransferModel())
//	{
//		/*if(edges.size())
//		{
//			list<EdgeConvertion*>::iterator iter;
//			for(iter = edges.begin(); iter != edges.end(); iter++)
//			{
//				if((*(*iter)) == (*edge))
//				{
//					break;
//				}
//			}
//
//			if(iter == edges.end())
//			{
//				edges.push_back(edge);
//			}
//			else
//			{
//				delete edge;
//			}
//		}
//		else
//		{*/
//			edges.push_back(edge);
//		/*}*/
//
//			//debug
//			/*string name;
//			edge->GetName(name);
//			std::ofstream out("edge.txt",std::ios::app | std::ios::out);
//			out << name << std::endl;
//			out.close();*/
//	}
//}

//void IGESToVTKConvertion::ConvertVertex(const TopoDS_Shape &model, int index)
//{
//	VertexConvertion *v = new VertexConvertion();
//	v->SetTopologyModel(model);
//	v->SetTopologyType(TopAbs_VERTEX);
//	v->SetIndex(index);
//	v->TransferModel();
//
//	//avoid adding the duplicated vertex 
//	if(vertexs.size())
//	{
//		list<VertexConvertion*>::iterator iter;
//		for(iter = vertexs.begin(); iter != vertexs.end(); iter++)
//		{
//			if((*(*iter)) == (*v))
//			{
//				break;
//			}
//		}
//
//		if(iter == vertexs.end())
//		{
//			vertexs.push_back(v);
//		}
//		else
//		{
//			delete v;
//		}
//	}
//	else
//	{
//		vertexs.push_back(v);
//	}
//}