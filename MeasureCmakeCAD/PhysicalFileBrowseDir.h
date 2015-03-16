#ifndef _PhysicalFileBrowseDir_
#define _PhysicalFileBrowseDir_

#include "basebrowsedir.h"

class PhysicalFileBrowseDir :
	public BaseBrowseDir
{
public:
	PhysicalFileBrowseDir(void);
	~PhysicalFileBrowseDir(void);

	vector<string> fileNameArray;
//	vector<float> timeArray;

protected:
	virtual bool ProcessFile(const char *filename);

	virtual void ProcessDir(const char *currentdir,const char *parentdir);
};

#endif