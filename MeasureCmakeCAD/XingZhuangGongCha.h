//--------------------------------------------------------------
//实体模型形状公差计算
//--------------------------------------------------------------
#ifndef _XING_ZHUANG_GONG_CHA_DEF_H_
#define _XING_ZHUANG_GONG_CHA_DEF_H_
#include "vtkPolyData.h"
#include "ShapePrimitivePara.h"

class XingZhuangGongCha
{
public:
	XingZhuangGongCha(void);
	XingZhuangGongCha(vtkPolyData *poly, ShapePrimitivePara *shape);
	~XingZhuangGongCha(void);

	bool ComputePingMianDu(double &results);
	bool ComputeYuanZhuDu(double &results);

public:
	vtkPolyData *polydata;
	ShapePrimitivePara *primitive;
};

#endif
