#include "StdAfx.h"
#include "XingWeiGongCha.h"
#include "vtkMath.h"

XingWeiGongCha::XingWeiGongCha(void)
{
	this->polydata = NULL;
	this->basePrimitive = NULL;
	this->surfPrimitive = NULL;
}

XingWeiGongCha::~XingWeiGongCha(void)
{
}

XingWeiGongCha::XingWeiGongCha(vtkPolyData *poly, ShapePrimitivePara *base, ShapePrimitivePara *surf)
{
	this->polydata = poly;
	this->basePrimitive = base;
	this->surfPrimitive = surf;
}

void XingWeiGongCha::ComputePrimitivesToPlane(double &result, const float n[])
{
	int pointStartId = this->surfPrimitive->startId;
	int pointEndId = this->surfPrimitive->endId;

	double point[3];
	double d_min, d_max, res;

	for(int i = pointStartId; i <= pointEndId; i++)
	{
		this->polydata->GetPoints()->GetPoint(i, point);
		res = (point[0] * n[0] + point[1] * n[1] + point[2] * n[2])/sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);


		if(i == pointStartId)
		{
			d_max = d_min = res;
		}
		else
		{
			if(res > d_max)
			{
				d_max = res;
			}

			if(res < d_min)
			{
				d_min = res;
			}
		}
	}

	result = abs(d_max - d_min);
}

void XingWeiGongCha::ComputeParallelismPlaneToPlane(double &result, const float n[])
{
	ComputePrimitivesToPlane(result, n);

}

void XingWeiGongCha::ComputePerpendicularityAxisToPlane(double &result, const float dir[], const float n[])
{
	double res;
	ComputePrimitivesToPlane(res, n);

	double cosTheta = vtkMath::Dot(dir, n);
	double sinTheta = sqrt(1 - cosTheta * cosTheta);

	result = abs(res * sinTheta / cosTheta);
}

void XingWeiGongCha::ComputePerpendicularityPlaneToAxis(double &result, const float n[], const float dir[])
{
	ComputePrimitivesToPlane(result, dir);
}

//n1 for base norm, n2 for surface norm;
void XingWeiGongCha::ComputePerpendicularityPlaneToPlane(double &result, const float n1[], const float n2[])
{
	float tempNorm[3], n[3];

	//the dir for interact line of two plane
	vtkMath::Cross(n1, n2, tempNorm);

	//the normal of the plane perpendicular with the base and the ineract lline
	vtkMath::Cross(n1, tempNorm, n);

    ComputePrimitivesToPlane(result, n);
}

//n1 for base, n2 for feature
void XingWeiGongCha::ComputeShapeAnglesOfDirections(double &result, const float n1[], const float n2[], const bool type)
{
	float val = vtkMath::Dot(n1, n2);

	float cosTheta = val / vtkMath::Dot(n1, n1) / vtkMath::Dot(n2,n2);

	if(type)
	{
		result = acos(cosTheta) * 180 / vtkMath::Pi();
	}
	else
	{
		if(cosTheta > 0)
		{
			result = 90 - acos(cosTheta) * 180 / vtkMath::Pi();
		}
		else
		{
			result = acos(cosTheta) * 180 / vtkMath::Pi() - 90;
		}

	}
}

//compute the Parallelism for shape primitives
bool XingWeiGongCha::ComputeParallelism(double &results)
{
	if(this->polydata != NULL && this->basePrimitive != NULL && this->surfPrimitive != NULL)
	{
		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Plane")
		{
			float n[3], p[3], d;
			this->basePrimitive->GetPlanePara(n[0], n[1], n[2], d, p[0], p[1], p[2]);
			this->ComputeParallelismPlaneToPlane(results, n);
			return true;
		}

		//to do; may be other shape type of prmitives
	}

	return false;
}

//Compute Perpendicularity of shape primitives
bool XingWeiGongCha::ComputePerpendicularity(double &results)
{
	if(this->polydata != NULL && this->basePrimitive != NULL && this->surfPrimitive != NULL)
	{
		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], d2;
			this->surfPrimitive->GetPlanePara(n2[0], n2[1], n2[2], d2, p2[0], p2[1], p2[2]);
			
			this->ComputePerpendicularityPlaneToPlane(results, n1, n2);

			return true;
		}

		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Cylinder")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], r, angle;
			this->surfPrimitive->GetCylinderPara(n2[0], n2[1], n2[2], p2[0], p2[1], p2[2], r, angle);

			
			this->ComputePerpendicularityAxisToPlane(results, n2, n1);

			return true;
		}

		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Cone")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], alpha, angle;
			this->surfPrimitive->GetConePara(p2[0], p2[1], p2[2], n2[0], n2[1], n2[2], alpha, angle);

			
			this->ComputePerpendicularityAxisToPlane(results, n2, n1);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cylinder" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->surfPrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], r, angle;
			this->basePrimitive->GetCylinderPara(n2[0], n2[1], n2[2], p2[0], p2[1], p2[2], r, angle);

			
			this->ComputePerpendicularityPlaneToAxis(results, n2, n1);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cone" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->surfPrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], alpha, angle;
			this->basePrimitive->GetConePara(p2[0], p2[1], p2[2], n2[0], n2[1], n2[2], alpha, angle);

			
			this->ComputePerpendicularityPlaneToAxis(results, n2, n1);

			return true;
		}

		//to do; may be other shape type of prmitives
	}
}

//Compute Shape Angles, axis for cylinder and cone, normal fro plane
bool XingWeiGongCha::ComputeShapeAngles(double &results)
{
	if(this->polydata != NULL && this->basePrimitive != NULL && this->surfPrimitive != NULL)
	{
		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], d2;
			this->surfPrimitive->GetPlanePara(n2[0], n2[1], n2[2], d2, p2[0], p2[1], p2[2]);
			
			this->ComputeShapeAnglesOfDirections(results, n1, n2, true);

			return true;
		}

		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Cylinder")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], r, angle;
			this->surfPrimitive->GetCylinderPara(n2[0], n2[1], n2[2], p2[0], p2[1], p2[2], r, angle);

			
			this->ComputeShapeAnglesOfDirections(results, n1, n2, false);

			return true;
		}

		if(this->basePrimitive->shapeType == "Plane" && this->surfPrimitive->shapeType == "Cone")
		{
			float n1[3], p1[3], d1;
			this->basePrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], alpha, angle;
			this->surfPrimitive->GetConePara(p2[0], p2[1], p2[2], n2[0], n2[1], n2[2], alpha, angle);

			
			this->ComputeShapeAnglesOfDirections(results, n1, n2, false);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cylinder" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->surfPrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], r, angle;
			this->basePrimitive->GetCylinderPara(n2[0], n2[1], n2[2], p2[0], p2[1], p2[2], r, angle);

			
			this->ComputeShapeAnglesOfDirections(results, n2, n1, false);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cone" && this->surfPrimitive->shapeType == "Plane")
		{
			float n1[3], p1[3], d1;
			this->surfPrimitive->GetPlanePara(n1[0], n1[1], n1[2], d1, p1[0], p1[1], p1[2]);

			float n2[3], p2[3], alpha, angle;
			this->basePrimitive->GetConePara(p2[0], p2[1], p2[2], n2[0], n2[1], n2[2], alpha, angle);

			
			this->ComputeShapeAnglesOfDirections(results, n2, n1, false);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cylinder" && this->surfPrimitive->shapeType == "Cylinder")
		{
			float n1[3], p1[3], r1, angle1;
			this->basePrimitive->GetCylinderPara(n1[0], n1[1], n1[2], p1[0], p1[1], p1[2], r1, angle1);

			float n2[3], p2[3], r2, angle2;
			this->surfPrimitive->GetCylinderPara(n2[0], n2[1], n2[2], p2[0], p2[1], p2[2], r2, angle2);

			this->ComputeShapeAnglesOfDirections(results, n1, n2, true);

			return true;
		}

		if(this->basePrimitive->shapeType == "Cone" && this->surfPrimitive->shapeType == "Cone")
		{
			float n1[3], p1[3], alpha1, angle1;
			this->basePrimitive->GetConePara(p1[0], p1[1], p1[2], n1[0], n1[1], n1[2], alpha1, angle1);

			float n2[3], p2[3], alpha2, angle2;
			this->surfPrimitive->GetConePara(p2[0], p2[1], p2[2], n2[0], n2[1], n2[2], alpha2, angle2);

			
			this->ComputeShapeAnglesOfDirections(results, n1, n2, true);

			return true;
		}


		//to do; may be other shape type of prmitives
	}
}