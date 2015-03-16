#pragma once
#include <vector>
#include <iostream>
using namespace std;
class TolerOutPut
{
public:
	TolerOutPut(void);
	~TolerOutPut(void);
	void Get_Tolerance(double tolerance,int counter_feature_1,int counter_feature_2,vector<string> typeName,string TolerType);
	int counter_of_toler;    //count the number of tolerances that had been calculated
	inline string tolerType(){return toler_type;};    //tolerance type
	inline double tolerance(){return value_of_toler;}
	inline vector<string> correspond_fea(){return correspond_features;}
	inline vector<int> ID_fea(){return ID_of_correspond_features;}

private:
	vector<int> ID_of_correspond_features;
	vector<string> correspond_features;    //the first feature is the base!
	double value_of_toler;
	string toler_type;
};
