#ifndef _countDir_
#define _countDir_
#include "basebrowsedir.h"

class countDir :
	public BaseBrowseDir
{
public:
	int m_nFileCount;   //�����ļ�����
	int m_nSubdirCount; //������Ŀ¼����
	int m_numPart;

	vector<string> partDirNameArray;
//	vector<string> partNameArray;
	vector<string> propertyDirNameArray;
	map<string,string> propertyAndPartMap;

	bool bInit;

public:
	//ȱʡ������
	countDir()
	{
		//��ʼ�����ݳ�Աm_nFileCount��m_nSubdirCount
		m_nFileCount=m_nSubdirCount=m_numPart=0;
		bInit = false;
	}

protected:
	//��д�麯��ProcessFile��ÿ����һ�Σ��ļ�������1
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		return true;
	}

	//��д�麯��ProcessDir��ÿ����һ�Σ���Ŀ¼������1
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