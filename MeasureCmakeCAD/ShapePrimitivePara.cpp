#include "StdAfx.h"
#include "ShapePrimitivePara.h"

ShapePrimitivePara::ShapePrimitivePara(string type, int num)
{
	this->shapeType = type;
	this->numPoints = num;

	this->para = NULL;
}

ShapePrimitivePara::~ShapePrimitivePara(void)
{
	if(this->para)
		delete [] para;
}

//make sure the input parameter p has length len
void ShapePrimitivePara::SetParameter(int len, float *p)
{
	if(len > 0)
	{
		this->paraLength = len;

		para = new float[len];

		for(int i = 0; i < len; i++)
		{
			para[i] = p[i];
		}
	}
}

//make sure the input parameter p has length len
void ShapePrimitivePara::GetParameter(float *p)
{
	if(paraLength > 0)
	{
		for(int i = 0; i < paraLength; i++)
		{
			p[i] = para[i];
		}
	}
}

//get plane parameter
bool ShapePrimitivePara::GetPlanePara(float &n1, float &n2, float &n3, float &d, float &p1, float &p2, float &p3)
{
	if(paraLength != 7)
	{
		return false;
	}
	n1 = para[0];
	n2 = para[1];
	n3 = para[2];
	d = para[3];
	p1 = para[4];
	p2 = para[5];
	p3 = para[6];

	return true;
}

//get cylinder parameter
bool ShapePrimitivePara::GetCylinderPara(float &a1, float &a2, float &a3, float &p1, float &p2, float &p3, float &r, float &angle)
{
	if(paraLength != 8)
	{
		return false;
	}

	a1 = para[0];
	a2 = para[1];
	a3 = para[2];
	p1 = para[3];
	p2 = para[4];
	p3 = para[5];

	r = para[6];
	angle = para[7];

	return true;
}

//get cone parameter
bool ShapePrimitivePara::GetConePara(float &c1, float &c2, float &c3, float &a1, float &a2, float &a3, float &alpha, float &angle)
{
	if(paraLength != 8)
	{
		return false;
	}

	c1 = para[0];
	c2 = para[1];
	c3 = para[2];
	a1 = para[3];
	a2 = para[4];
	a3 = para[5];

	alpha = para[6];
	angle = para[7];

	return true;
}

//get sphere parameter
bool ShapePrimitivePara::GetSpherePara(float &c1, float &c2, float &c3, float &r)
{
	if(paraLength != 4)
	{
		return false;
	}

	c1 = para[0];
	c2 = para[1];
	c3 = para[2];
	r = para[3];

	return true;
}

//get torus parameter
bool ShapePrimitivePara::GetTorusPara(float &n1, float &n2, float &n3, float &c1, float &c2, float &c3, float &r_min, float &r_max)
{
	if(paraLength != 8)
	{
		return false;
	}

	n1 = para[0];
	n2 = para[1];
	n3 = para[2];
	c1 = para[3];
	c2 = para[4];
	c3 = para[5];

	r_min = para[6];
	r_max = para[7];

	return true;
}