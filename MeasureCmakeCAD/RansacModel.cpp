#include "StdAfx.h"
#include "RansacModel.h"
#include <vtkMath.h>
#include <vtkLineSource.h>

//ransac 
#include <RansacShapeDetector.h>
#include <PlanePrimitiveShapeConstructor.h>
#include <CylinderPrimitiveShapeConstructor.h>
#include <SpherePrimitiveShapeConstructor.h>
#include <ConePrimitiveShapeConstructor.h>
#include <TorusPrimitiveShapeConstructor.h>
#include <PointCloud.h>
#include <VectorXD.h>
#include <utility>
#include <fstream>
using namespace MiscLib;

RansacModel::RansacModel(void)
{
	m_distance_ths = 0.01;
	m_minimal_points = 100;
	m_normal_ths = 0.97;
	m_probability_ths = 0.01;
	m_projection_ths = 0.02;
	m_set_plane = true;
	m_set_cylinder = true;
	m_set_cone = false;
	m_set_sphere = false;
	m_set_torus = false;

	this->polydata = vtkPolyData::New();

	this->m_init = true;

	this->m_scale = 3.0;

}

RansacModel::~RansacModel(void)
{
	if(this->polydata)
		this->polydata->Delete();

	if(this->primitives.size() > 0)
	{
		int size = this->primitives.size();
		for(int i = 0; i < size; i++)
		{
			delete this->primitives[i];
		}
	}
}


void RansacModel::InitializeOrModification(bool init)
{
	this->m_init = init;
}

void RansacModel::SetInputData(vtkPolyData *poly)
{
	if(poly != NULL)
	{
		this->polydata->DeepCopy(poly);
	}
}

void RansacModel::SetPara(float disThrehold, float projThrehold, float probThrehold, float normThrehold, float minPoint, bool usePlane, bool useCylinder, bool useCone, bool useSphere, bool useTorus)
{
	this->m_distance_ths = disThrehold;
	this->m_projection_ths = projThrehold;
	this->m_probability_ths = probThrehold;
	this->m_normal_ths = normThrehold;
	this->m_minimal_points = minPoint;
	this->m_set_plane = usePlane;
	this->m_set_cylinder = useCylinder;
	this->m_set_cone = useCone;
	this->m_set_sphere = useSphere;
	this->m_set_torus = useTorus;
}

void RansacModel::Updata()
{
	if(this->m_init)
	{
		this->ShapeRecognition();
	}
	else
	{
		this->ShapeModification();
	}
}

// normal scale for plane info visulization ---- YiBing, 2014-12-15
void RansacModel::SetNormalScale(const float s)
{
	this->m_scale = s;
}

float RansacModel::GetNormalScale()
{
	return this->m_scale;
}


void RansacModel::ShapeRecognition()
{
	PointCloud pc;

	double *currentPoint;

	//ProgressDlg * tictoc = new ProgressDlg;
	//tictoc->Create(IDD_DIALOG_Progress);
	//tictoc->ShowWindow(SW_SHOWNORMAL);
	//tictoc->DoModal();
	//TerminateThread(tictoc->m_ThreadSpeed, 0);
	//tictoc->pro_thread_crea();

	cout<<"progress..."<<endl;

	//calculate the time
	clock_t start,end;
	double time_cal;
	start = clock();

	int numPoints = polydata->GetNumberOfPoints();

	for(int i = 0; i < numPoints; i++)
	{
		currentPoint = polydata->GetPoint(i);
		Vec3f vectorPosition(*currentPoint, *(currentPoint + 1), *(currentPoint + 2));
		Point pointPosition(vectorPosition);
		pc.push_back(pointPosition);
	}



	double length = polydata->GetLength();
	double boundbox[6];
	polydata->GetBounds(boundbox);

	pc.calcNormals(length * 0.01);
	Vec3f min(boundbox[0], boundbox[2], boundbox[4]);
	Vec3f max(boundbox[1], boundbox[3], boundbox[5]);
	pc.setBBox(min, max);

	RansacShapeDetector::Options ransacOptions;
	ransacOptions.m_epsilon = m_distance_ths * pc.getScale(); // set distance threshold to .01f of bounding box width
	// NOTE: Internally the distance threshold is taken as 3 * ransacOptions.m_epsilon!!!
	ransacOptions.m_bitmapEpsilon = m_projection_ths * pc.getScale(); // set bitmap resolution to .02f of bounding box width
	// NOTE: This threshold is NOT multiplied internally!
	ransacOptions.m_normalThresh = m_normal_ths; // this is the cos of the maximal normal deviation
	ransacOptions.m_minSupport = m_minimal_points; // this is the minimal numer of points required for a primitive
	ransacOptions.m_probability = m_probability_ths; // this is the "probability" with which a primitive is overlooked

	RansacShapeDetector detector(ransacOptions); // the detector object

	// set which primitives are to be detected by adding the respective constructors
	if(m_set_plane)
	{
		detector.Add(new PlanePrimitiveShapeConstructor());
	}
	if(m_set_cylinder)
	{
		detector.Add(new CylinderPrimitiveShapeConstructor());
	}
	if(m_set_cone)
	{
		detector.Add(new ConePrimitiveShapeConstructor());
	}
	if(m_set_sphere)
	{
		detector.Add(new SpherePrimitiveShapeConstructor());
	}
	if(m_set_torus)
	{
		detector.Add(new TorusPrimitiveShapeConstructor());
	}

	MiscLib::Vector< std::pair< MiscLib::RefCountPtr< PrimitiveShape >, size_t > > shapes; // stores the detected shapes
	size_t remaining = detector.Detect(pc, 0, pc.size(), &shapes); // run detection

	//to do handle points
	std::string surfaceType;
	int size;

	int id = pc.size() - 1;

	vtkPoints* points = vtkPoints::New();
	points->Allocate(numPoints);
	vtkIntArray* label = vtkIntArray::New();
	label->Allocate(numPoints);
	label->SetName("Type");

	int type = 0;

	int recognizedPointId = 0;

	//handle shape large than minpoints as real recognized mdoel
	for(int i = 0; i < shapes.size(); i++)
	{
		size = shapes[i].second;

		if(size >= m_minimal_points)
		{
			//initial the bouding box ---- YiBing, 2014-12-15
			float min[3], max[3];
			for(int k = 0; k < 3; k++)
			{
				min[k] = FLT_MAX;
				max[k] = -FLT_MAX;
			}

			//shape information
			shapes[i].first->Description(&surfaceType);

			ShapePrimitivePara* primitive = new ShapePrimitivePara(surfaceType, size);

			//parameter
			int p_size = shapes[i].first->SerializedFloatSize();
			float *parameter = new float[p_size];
			shapes[i].first->Serialize(parameter);

			primitive->startId = recognizedPointId;
			primitive->endId = recognizedPointId + size - 1;


			primitive->SetParameter(p_size, parameter);

			primitives.push_back(primitive);


			//this->Start_End.push_back(id);

			for(int j = 0; j < size; j++)
			{
				Point current = pc[id];

				id--;

				// add bounding box for each recognized basic geometry ---- YiBing, 2014-12-15

				float cPos[3];

				cPos[0] = current.pos[0];
				cPos[1] = current.pos[1];
				cPos[2] = current.pos[2];

				points->InsertNextPoint(current[0],current[1],current[2]);
				//points->InsertNextPoint()
				label->InsertNextTuple1(type);

				for(int k = 0; k < 3; k++)
				{
					min[k] = min[k] > cPos[k] ? cPos[k] : min[k];
					max[k] = max[k] < cPos[k] ? cPos[k] : max[k];
				}
			}
			
			//this->Start_End.push_back(id+1);

			for(int k = 0; k < 3; k++)
			{
				primitive->m_min[k] = min[k];
				primitive->m_max[k] = max[k];
			}


			type++;
			recognizedPointId += size;

			delete [] parameter;
		}
		else
		{
			id -= size;
		}
	}

	//handle shape small than minpoints as real recognized mdoel
	id = pc.size() - 1;
	for(int i = 0; i < shapes.size(); i++)
	{
		size = shapes[i].second;
		
		if(size < m_minimal_points)
		{
			for(int j = 0; j < size; j++)
			{
				Point current = pc[id];
				id--;
				points->InsertNextPoint(current[0],current[1],current[2]);
		        label->InsertNextTuple1(type);
			}
		}
		else
		{
			id -= size;
		}
	}

	//this->nums=type;
	for(int i = 0; i < remaining; i++ )
	{
		Point current = pc[i];
		points->InsertNextPoint(current[0],current[1],current[2]);
		label->InsertNextTuple1(type);
	}

	this->polydata->SetPoints(points);

	points->Delete();
	label->Delete();

	//Sleep(10000);
	//tictoc->termi_thread();
	//delete tictoc;

	end = clock();
	time_cal = (double)(end - start)/CLOCKS_PER_SEC;
	cout<<"Spending time(seconds) of recoginize the feature of Point-Cloud: "<<time_cal<<endl;

	//initialize modify and output parameter
	outputMarks.assign(primitives.size(), true);
	modifyMarks.assign(primitives.size(), false);
}

void RansacModel::ShapeModification()
{
	//to do

}

//dsiplay information of basic geometry  ---- YiBing, 2014-12-15
void RansacModel::DisplayHighLevelInfo(vtkActorCollection *actorCollection)
{
	if(actorCollection != NULL)
	{
		int size = this->primitives.size();
		int index = 0;
		for(int i = 0; i < size; i++)
		{
			int num = this->primitives[i]->numPoints;
			float pos1[3],pos2[3];

			vtkLineSource *line = vtkLineSource::New();
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
			vtkActor *actor = vtkActor::New();

			this->GetShapeInfoPositions(pos1, pos2, i);
			line->SetPoint1(pos1[0], pos1[1], pos1[2]);
			line->SetPoint2(pos2[0], pos2[1], pos2[2]);
			line->SetResolution(10);


			mapper->SetInput(line->GetOutput());
			actor->SetMapper(mapper);

			actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
			actor->GetProperty()->SetLineWidth(2);

			actorCollection->AddItem(actor);

			mapper->Delete();
			actor->Delete();
			line->Delete();
		}
	}
}

//model position  ---- YiBing, 2014-12-15
void RansacModel::GetShapeInfoPositions(float p1[3], float p2[3], const int idex)
{
	string type = this->primitives[idex]->shapeType;

	if(type == "Plane")
	{ 
		float n[3], p[3], d;
		this->primitives[idex]->GetPlanePara(n[0], n[1], n[2], d, p[0], p[1], p[2]);

		for(int i = 0; i < 3; i++)
		{
			p1[i] = p[i];
			p2[i] = p[i] + n[i] * m_scale;
		}
	}

	if(type == "Cylinder")
	{
		float n[3], p[3], r, angle;
		this->primitives[idex]->GetCylinderPara(n[0], n[1], n[2], p[0], p[1], p[2], r, angle);

		//position of cylinder
		float min[3], max[3];
		for(int i = 0; i < 3; i++)
		{
			min[i] = this->primitives[idex]->m_min[i] - p[i];
			max[i] = this->primitives[idex]->m_max[i] - p[i];
		}

		float lmin = vtkMath::Dot(min, n);
		float lmax = vtkMath::Dot(max, n);


		for(int i = 0; i < 3; i++)
		{
			p1[i] = p[i] + n[i] * lmin;
			p2[i] = p[i] + n[i] * lmax;
		}

	}

	//cone information YiBing, 2014-12-23
	if(type == "Cone")
	{
		float n[3], p[3], alpha, angle;
		this->primitives[idex]->GetConePara( p[0], p[1], p[2], n[0], n[1], n[2], alpha, angle);

		//position of cone
		float min[3], max[3];
		for(int i = 0; i < 3; i++)
		{
			min[i] = this->primitives[idex]->m_min[i] - p[i];
			max[i] = this->primitives[idex]->m_max[i] - p[i];
		}

		float lmin = vtkMath::Dot(min, n);
		float lmax = vtkMath::Dot(max, n);


		for(int i = 0; i < 3; i++)
		{
			p1[i] = p[i] + n[i] * lmin;
			p2[i] = p[i] + n[i] * lmax;
		}

	}

	//to do for cone and sphere, torus

}

void RansacModel::DisplayModel(vtkActorCollection *actorCollection)
{
	if(actorCollection != NULL)
	{
		int size = this->primitives.size();
		int index = 0;
		for(int i = 0; i < size; i++)
		{
			int num = this->primitives[i]->numPoints;
			vtkPoints *points = vtkPoints::New(); 
			vtkCellArray *polys = vtkCellArray::New();
			points->Allocate(num);
			polys->Allocate(num);
			vtkPolyData *data = vtkPolyData::New();
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
			vtkActor *actor = vtkActor::New();

			int id = 0;
			for(int j = 0; j < num; j++)
			{
				double x[3];
				polydata->GetPoints()->GetPoint(index, x);
				points->InsertNextPoint(x[0], x[1], x[2]);
				polys->InsertNextCell(1,&id);
				id++;
				index++;
			}

			data->SetPoints(points);
			data->SetVerts(polys);

			mapper->SetInput(data);
			actor->SetMapper(mapper);
			actor->GetProperty()->SetPointSize(4);
			double r = vtkMath::Random(0,1);
			double g = vtkMath::Random(0,1);
			double b = vtkMath::Random(0,1);

			actor->GetProperty()->SetColor(r, g, b);

			actorCollection->AddItem(actor);

			points->Delete();
			polys->Delete();
			data->Delete();
			mapper->Delete();
			actor->Delete();
		}
	}
}

//Output the selected feature ---- yibing 2015/1/15
void RansacModel::OutPutFile(const char* fileName)
{			
	ofstream fout(fileName);
    int prim = 0;
	double *currentPoint;
	for(int i = 0; i < outputMarks.size();i++)
	{
		if(outputMarks[i])
		{
			fout<<"# Primitive "<<prim<<'\n';
			fout<<"# Number of Points\n";
			fout<< primitives[i]->numPoints <<'\n';
	        fout<<"# Point_x point_y point_z \n";
			int start = primitives[i]->startId;
			int end = primitives[i]->endId;
			for(int j = start; j <= end; j++)
			{
				currentPoint = polydata->GetPoint(j);
		        fout<<*currentPoint<<" "<<*(currentPoint+1)<<" "<<*(currentPoint+2)<<endl;	
			}
			fout<<"#shape discriptions\n";
			fout<<"#shape type\n";
			fout<< primitives[i]->shapeType.c_str() << endl;
			fout<<"#shape parameters\n";     //output shape parameter of each primitive
			fout<<"#number of parameters\n";     //output shape parameter of each primitive
			int len = primitives[i]->paraLength;
			fout << len << endl;
			float *para = new float[len];
			primitives[i]->GetParameter(para);
			for(int j = 0; j < len; j++)
			{
				fout << para[j] << endl;
			}
			prim++;
		}
	}
	fout.close();
}

//void RansacModel::getInteractivePoints(vector<double>p)
//{
//	for(int i = 0;i<p.size();i++)
//	{
//		this->interPoints.push_back(p[i]);
//		cout<<p[i]<<endl;
//	}
//}