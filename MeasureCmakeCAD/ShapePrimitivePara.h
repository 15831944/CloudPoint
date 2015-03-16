#pragma once
#include <string>

using namespace std;

class ShapePrimitivePara
{
public:
	ShapePrimitivePara(string type, int numPoints);
	~ShapePrimitivePara(void);

	void SetParameter(int length, float* p);
	void GetParameter(float* p);

	//get basic geomtry info ---- YiBing, 2014-12-15
	bool GetPlanePara(float &n1, float &n2, float &n3, float &d, float &p1, float &p2, float &p3);
	bool GetCylinderPara(float &a1, float &a2, float &a3, float &p1, float &p2, float &p3, float &r, float &angle);
	bool GetConePara(float &c1, float &c2, float &c3, float &a1, float &a2, float &a3, float &alpha, float &angle);
	bool GetSpherePara(float &c1, float &c2, float &c3, float &r);
	bool GetTorusPara(float &n1, float &n2, float &n3, float &c1, float &c2, float &c3, float &r_min, float &r_max);

public:
	string shapeType;
	int numPoints;
	int paraLength;

	int startId;
	int endId;

	float m_min[3];
	float m_max[3];

private:
	float* para;
	//to add senmatic means of para
//plane:
//		normal: array[0] array[1] array[2]
//		position: array[4] array[5] array[6]
//		distance: array[3]
//cylinder:
//cone:
//sphere:
//torus:
};
