//--------------------------------------------------------------
//实体模型形位公差计算
//--------------------------------------------------------------
#ifndef _XING_WEI_GONG_CHA_DEF_H_
#define _XING_WEI_GONG_CHA_DEF_H_
#include "vtkPolyData.h"
#include "ShapePrimitivePara.h"

class XingWeiGongCha
{
public:
	XingWeiGongCha(void);
	XingWeiGongCha(vtkPolyData *poly, ShapePrimitivePara *base, ShapePrimitivePara *surf);
	~XingWeiGongCha(void);

	//parallelism computing
	bool ComputeParallelism(double &results);

	//perpendicularity computing
	bool ComputePerpendicularity(double &results);

	//Angle computing
	bool ComputeShapeAngles(double &results);

protected:
	void ComputeParallelismPlaneToPlane(double &result, const float n[3]);

	void ComputePerpendicularityAxisToPlane(double &result, const float dir[3], const float n[3]);

	void ComputePerpendicularityPlaneToAxis(double &result, const float n[3], const float dir[3]);

	void ComputePerpendicularityPlaneToPlane(double &result, const float n1[3], const float n2[3]);

	void ComputeShapeAnglesOfDirections(double &result,  const float n1[3], const float n2[3], const bool type); //type == true for dir and dir; false for dir and normal

	void ComputePrimitivesToPlane(double &result,  const float n[3]);


public:
	vtkPolyData *polydata;
	ShapePrimitivePara *basePrimitive;
	ShapePrimitivePara *surfPrimitive;
};

#endif