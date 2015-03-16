#include "stdafx.h"
#include "PhysicalFileBrowseDir.h"

PhysicalFileBrowseDir::PhysicalFileBrowseDir(void)
{

}

PhysicalFileBrowseDir::~PhysicalFileBrowseDir(void)
{

}
bool PhysicalFileBrowseDir::ProcessFile(const char *filename)
{
	string name(filename);
	//size_t firstDot = name.find_first_of('.');
	/*size_t lastDot = name.find_last_of('.');
	size_t length = 4;
	string time = name.substr(lastDot - length, length);
	float t = atof(time.c_str());
	timeArray.push_back(t);*/
	fileNameArray.push_back(filename);
    
	return true;
}

void PhysicalFileBrowseDir::ProcessDir(const char *currentdir, const char *parentdir)
{
}