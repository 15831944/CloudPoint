//-----------------------------------
#include "stdafx.h"
#include "Entity.h"
#pragma comment(lib,"libeng.lib")
#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmat.lib")
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

	recModels = new RansacModel;
	
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

void Entity::getInteractivePoints(vector<double>p)
{
	for(int i = 0;i<p.size();i++)
	{
		this->interPoints.push_back(p[i]);
		cout<<"Selected STL Point: "<<p[i]<<endl;
	}
}

void Entity::CloudPoint2Base()
{
	double * norms = NULL;
	norms = this->CrossDot();
	vtkPolyData *pTargetData = NULL;
	this->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=this->current_point_cloud_part_num;i++)//得到数据集中相应模型数据的指针
	{
		pTargetData = this->pPointCloudDataSetCollection->GetNextItem();
		//vtkCellArray * tmp_vert = NULL;
		//tmp_vert = pTargetData->GetVerts();
		//double x,y,z;
		//vtkIdType id_cell = tmp_vert->GetSize();

	}
	double cos_tmp = norms[0]*norms[3]+norms[1]*norms[4]+norms[3]*norms[5];
	double tmp = abs(sqrt(norms[0]*norms[0]+norms[1]*norms[1]+norms[2]*norms[2]))*abs(sqrt(norms[3]*norms[3]
	+norms[4]*norms[4]+norms[5]*norms[5]));
	double cos_value = cos_tmp/tmp;
	double x = this->interPoints[0] - this->interPointsPC[0];
	double y = this->interPoints[1] - this->interPointsPC[1];
	double z = this->interPoints[2] - this->interPointsPC[2];
	double h = sqrt(x*x + y*y + z*z);
	h = h*cos_value;
	if(cos_tmp<0)
		h = -h;
	cout<<"Translation About: "<<h<<endl;
	double * currentPoint;
	int numPoints = pTargetData->GetNumberOfPoints();
	for(int i = 0; i < numPoints; i++)
	{
		currentPoint = pTargetData->GetPoint(i);
		//Vec3f vectorPosition(*currentPoint, *(currentPoint + 1), *(currentPoint + 2));
		//Point pointPosition(vectorPosition);
		//pc.push_back(pointPosition);
		//cout<<"333"<<endl;
		//Engine *ep;
		//cout<<"333"<<endl;
		//mxArray *T = NULL,*results = NULL;
		//cout<<"444"<<endl;
		//char buffer[257];
		//double time[4]={norms[3],norms[4],norms[5],1};
		//if(!(ep==engOpen("\0")))
		//{
		//	fprintf(stderr,"\nCan't start MATLAB engine\n");
		//}
		//T = mxCreateDoubleMatrix(1,4,mxREAL);
		//memcpy((void*)mxGetPr(T),(void*)time,sizeof(time));

		//engPutVariable(ep,"T",T);
		//engEvalString(ep,"D = pinv(T);");

		*currentPoint = *currentPoint + h;
		*(currentPoint + 1) = *(currentPoint + 1) + h;
		*(currentPoint + 2) = *(currentPoint + 2) + h;
		if(i%1000 == 0)cout<<". ";

	}
	cout<<endl<<"2 Base Computing End..."<<endl;
}

double* Entity::CrossDot()
{
	double a1 = this->interPoints[6] - this->interPoints[0];
	double a2 = this->interPoints[7] - this->interPoints[1];
	double a3 = this->interPoints[8] - this->interPoints[2];
	double b1 = this->interPoints[3] - this->interPoints[0];
	double b2 = this->interPoints[4] - this->interPoints[1];
	double b3 = this->interPoints[5] - this->interPoints[2];

	double a11 = this->interPointsPC[6] - this->interPointsPC[0];
	double a12 = this->interPointsPC[7] - this->interPointsPC[1];
	double a13 = this->interPointsPC[8] - this->interPointsPC[2];
	double b11 = this->interPointsPC[3] - this->interPointsPC[0];
	double b12 = this->interPointsPC[4] - this->interPointsPC[1];
	double b13 = this->interPointsPC[5] - this->interPointsPC[2];

	double * norm = new double[6];
	norm[0] = a2*b3-a3*b2;
	norm[1] = a3*b1-a1*b3;
	norm[2] = a1*b2-a2*b1;

	norm[3] = a12*b13-a13*b12;
	norm[4] = a13*b11-a11*b13;
	norm[5] = a11*b12-a12*b11;
	return norm;
}

void Entity::getInteractivePointsPC(vector<double>p)
{
	for(int i = 0;i<p.size();i++)
	{
		this->interPointsPC.push_back(p[i]);
		cout<<"Selected Cloud Point: "<<p[i]<<endl;
	}
}


