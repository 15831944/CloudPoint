//-----------------------------------
#include "stdafx.h"
#include "Entity.h"

//实体类构造函数，初始化所有变量
Entity::Entity()
{
	this->pStlDataSetCollection = vtkPolyDataCollection::New();
	this->pPointCloudDataSetCollection = vtkPolyDataCollection::New();
	this->pPointCloudCaseNameArray = vtkStringArray::New();
	this->pStlCaseNameArray = vtkStringArray::New();

	this->surfEdges = NULL;

	current_stl_part_num = 0;
	current_point_cloud_part_num = 0;

	current_cad_num = 0;
	current_edge_num = 0;

	stl_for_match_to_point = 0;

	numTotalSurf = 0;

	recModels = NULL;
	
	mMode = Interact;
}

//实体类析构函数，删除相应变量
Entity::~Entity()
{
	if(this->pStlDataSetCollection)
	{
		this->pStlDataSetCollection->Delete();
	}
	if(this->pPointCloudDataSetCollection)
	{
		this->pPointCloudDataSetCollection->Delete();
	}
	if(this->pPointCloudCaseNameArray)
	{
		this->pPointCloudCaseNameArray->Delete();
	}
	if(this->pStlCaseNameArray)
	{
		this->pStlCaseNameArray->Delete();
	}

	if(this->surfEdges)
	{
		delete this->surfEdges;
	}

	if(cadModels.size() > 0)
	{
		vector<IgesAndVtkModel*>::iterator iter;
		for(iter = cadModels.begin(); iter != cadModels.end(); iter++)
		{
			delete (*iter);
		}
	}

	if(recModels)
		delete recModels;
}

void Entity::setInteractiveStyleMode(Entity::ViewMode mode)
{
	mMode = mode;
}

Entity::ViewMode Entity::getInteractiveStyleMode()
{
	return mMode;
}