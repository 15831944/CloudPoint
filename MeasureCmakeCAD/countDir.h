#ifndef _countDir_
#define _countDir_
#include "basebrowsedir.h"

class countDir :
	public BaseBrowseDir
{
public:
	int m_nFileCount;   //保存文件个数
	int m_nSubdirCount; //保存子目录个数
	int m_numPart;

	vector<string> partDirNameArray;
//	vector<string> partNameArray;
	vector<string> propertyDirNameArray;
	map<string,string> propertyAndPartMap;

	bool bInit;

public:
	//缺省构造器
	countDir()
	{
		//初始化数据成员m_nFileCount和m_nSubdirCount
		m_nFileCount=m_nSubdirCount=m_numPart=0;
		bInit = false;
	}

protected:
	//覆写虚函数ProcessFile，每调用一次，文件个数加1
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		return true;
	}

	//覆写虚函数ProcessDir，每调用一次，子目录个数加1
	virtual void ProcessDir
		(const char *currentdir,const char *parentdir)
	{
		if(bInit)
		{
			string dirName(currentdir);
			string parentDirName(parentdir);

			if(parentDirName.compare(orgDir) == 0)
			{
				partDirNameArray.push_back(currentdir);
				m_numPart++;
			}
			else
			{
				propertyDirNameArray.push_back(dirName);
				propertyAndPartMap.insert(pair<string,string>(dirName,parentDirName));
				m_nSubdirCount++;
			}
		}

		bInit = true;
	}
};

#endif