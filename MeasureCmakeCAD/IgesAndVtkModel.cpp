#include "StdAfx.h"
#include "IgesAndVtkModel.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"
#include "vtkActor.h"
#include "vtkProperty.h"

IgesAndVtkModel::IgesAndVtkModel(void)
{
	hasConverted = false;

	startSurfId = 0;
	endSurfId = 0;
}

IgesAndVtkModel::~IgesAndVtkModel(void)
{
}

void IgesAndVtkModel::ConvetModel(const TopoDS_Shape &model, const float p,  const int startId)
{
	convert.SetTopologyModel(model);
	convert.SetSurfacePrecision(p);
    convert.ConvertModel();

	hasConverted = true;

	startSurfId = startId;
	endSurfId = startSurfId + convert.surfaces.size() - 1;
}

void IgesAndVtkModel::SetModelName(string name)
{
	modelName = name;
}

void IgesAndVtkModel::DisplayModel(vtkActorCollection *actorCollection, const double r, const double g, const double b)
{
	if(actorCollection != NULL && hasConverted)
	{
		list<SurfaceConvertion*>::iterator iter;
		for(iter = convert.surfaces.begin(); iter != convert.surfaces.end(); iter++)
		{
			string name;
			vtkPolyData *poly = vtkPolyData::New();
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
			vtkActor *actor = vtkActor::New();
			
			(*iter)->GetPolyData(poly);
			(*iter)->GetName(name);
			surfaceNames.push_back(name);
			
			mapper->SetInput( poly );
			actor->SetMapper (mapper);
			actor->GetProperty()->SetColor(r,g,b);

			actorCollection->AddItem(actor);

			mapper->Delete();
			actor->Delete();
			poly->Delete();
		}
	}
}