#include "StdAfx.h"
#include "XingZhuangGongCha.h"
#include <vtkMath.h>

XingZhuangGongCha::XingZhuangGongCha(void)
{
	this->polydata = NULL;
	this->primitive = NULL;
}

XingZhuangGongCha::~XingZhuangGongCha(void)
{
}


XingZhuangGongCha::XingZhuangGongCha(vtkPolyData *poly, ShapePrimitivePara *shape)
{
	this->polydata = poly;
	this->primitive = shape;
}

//计算平面度
bool XingZhuangGongCha::ComputePingMianDu(double &results)
{
	if(this->polydata != NULL && this->primitive != NULL)
	{
		if(this->primitive->shapeType == "Plane")
		{
			//plane parameter
		     float n[3], p[3], d;
			 this->primitive->GetPlanePara(n[0], n[1], n[2], d, p[0], p[1], p[2]);

			int pointStartId = this->primitive->startId;
			int pointEndId = this->primitive->endId;

			double point[3];
			double d_min, d_max, res;
			
			for(int i = pointStartId; i <= pointEndId; i++)
			{
				this->polydata->GetPoints()->GetPoint(i, point);
			    res = (point[0] * n[0] + point[1] * n[1] + point[2] * n[2] + d)/sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
				

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
			
            results = d_max - d_min;

			return true;
		}
	}

	return false;
}


//计算圆柱度
bool XingZhuangGongCha::ComputeYuanZhuDu(double &results)
{
	if(this->polydata != NULL && this->primitive != NULL)
	{
		if(this->primitive->shapeType == "Cylinder")
		{
			float n[3], p[3], r, angle;
		    this->primitive->GetCylinderPara(n[0], n[1], n[2], p[0], p[1], p[2], r, angle);

			int pointStartId = this->primitive->startId;
			int pointEndId = this->primitive->endId;

			double point[3];
			double r_min, r_max, res, len, proLen;
			float pointDir[3];
			
			for(int i = pointStartId; i <= pointEndId; i++)
			{
				this->polydata->GetPoints()->GetPoint(i, point);

				//position dir of cylinder point
				for(int i = 0; i < 3; i++)
				{
					pointDir[i] = point[i] - p[i];
				}
			    
				len = vtkMath::Dot(pointDir, pointDir);
				proLen = vtkMath::Dot(pointDir, n) / sqrt(vtkMath::Dot(n, n));
				
				
				res = sqrt(len * len - proLen * proLen);
				

				if(i == pointStartId)
				{
					r_max = r_min = res;
				}
				else
				{
					if(res > r_max)
					{
						r_max = res;
					}

					if(res < r_min)
					{
						r_min = res;
					}
				}
			}
			
            results = r_max - r_min;

			return true;
		}
	}

	return false;
}