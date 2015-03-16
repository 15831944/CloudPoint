#include "StdAfx.h"
#include "TolerOutPut.h"
#include "ShapePrimitivePara.h"
#include <vector>
TolerOutPut::TolerOutPut()
{
	this->counter_of_toler = 0;
	this->value_of_toler = 0.0;
}

TolerOutPut::~TolerOutPut(void)
{
}

void TolerOutPut::Get_Tolerance(double tolerance,int counter_feature_1,int counter_feature_2,vector<string> typeName,string TolerType)
{
	this->toler_type = TolerType;
	this->ID_of_correspond_features.push_back(counter_feature_1);
	this->ID_of_correspond_features.push_back(counter_feature_2);
	for(int i=0;i<typeName.size();i++)
	{
		string feature_tmp = typeName[i];
		this->correspond_features.push_back(feature_tmp);
	}
	this->value_of_toler = tolerance;
}