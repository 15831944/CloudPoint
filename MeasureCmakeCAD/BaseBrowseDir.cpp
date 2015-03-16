#include "stdafx.h"
#include "BaseBrowseDir.h"

BaseBrowseDir::BaseBrowseDir()
{
    //�õ�ǰĿ¼��ʼ��m_szInitDir
    getcwd(m_szInitDir,_MAX_PATH);
	getcwd(m_originDir,_MAX_PATH);
     
    //���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");
}
 
bool BaseBrowseDir::SetInitDir(const char *dir)
{
    //�Ȱ�dirת��Ϊ����·��
    if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
        return false;
     
    //�ж�Ŀ¼�Ƿ����
    if (_chdir(m_szInitDir) != 0)
        return false;
     
    //���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
    int len=strlen(m_szInitDir);
    if (m_szInitDir[len-1] != '\\')
        strcat(m_szInitDir,"\\");

	//��ó�ʼ·�����ұ���
	string inDir(m_szInitDir);
	orgDir = inDir;
     
    return true;
}
 
bool BaseBrowseDir::BeginBrowse(const char *filespec)
{
    ProcessDir(m_szInitDir,NULL);
	int val = BrowseDir(m_szInitDir,filespec);

	//���ô�����Ŀ¼Ϊ֮ǰ��Ŀ¼�ļ�
	_chdir(m_originDir);

    return val;
}
 
bool BaseBrowseDir::BrowseDir(const char *dir,const char *filespec)
{
    _chdir(dir);
     
    //���Ȳ���dir�з���Ҫ����ļ�
    long hFile;
    _finddata_t fileinfo;
    if ((hFile=_findfirst(filespec,&fileinfo)) != -1)
    {
        do
        {
            //����ǲ���Ŀ¼
            //�������,����д���
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                char filename[_MAX_PATH];
                strcpy(filename,dir);
                strcat(filename,fileinfo.name);
                cout << filename << endl;
                if (!ProcessFile(filename))
                    return false;
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
    }
    //����dir�е���Ŀ¼
    //��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
    //��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
    //ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
    //��_findnextû��Ӱ�졣
    
	_chdir(dir);

    if ((hFile=_findfirst("*.*",&fileinfo)) != -1)
    {
        do
        {
            //����ǲ���Ŀ¼
            //�����,�ټ���ǲ��� . �� .. 
            //�������,���е���
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name,".") != 0 && strcmp
                    (fileinfo.name,"..") != 0)
                {
                    char subdir[_MAX_PATH];
                    strcpy(subdir,dir);
                    strcat(subdir,fileinfo.name);
                    strcat(subdir,"\\");
                    ProcessDir(subdir,dir);
                    if (!BrowseDir(subdir,filespec))
                        return false;
                }
            }
        } while (_findnext(hFile,&fileinfo) == 0);
        _findclose(hFile);
    }
    return true;
}

bool BaseBrowseDir::ProcessFile(const char *filename)
{
    return true;
}
 
void BaseBrowseDir::ProcessDir(const char 
                            *currentdir,const char *parentdir)
{
}