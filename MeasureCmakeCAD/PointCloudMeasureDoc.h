
// PointCloudMeasureDoc.h : CPointCloudMeasureDoc ��Ľӿ�
//
#pragma once

#include "vtkScanPointReader.h"
#include "vtkActor2D.h"
#include "vtkTextMapper.h"
#include "vtkActor.h"
#include "vtkSTLReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkActorCollection.h"
#include "vtkLookupTable.h"
#include "vtkScalarBarActor.h" 
#include "vtkScalarBarWidget.h"
#include "vtkProperty.h"
#include "Entity.h"
#include <vtkCaptionWidget.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include "vtkMotionTimerCallback.h"

//opencasecade
#include <TopoDS_Shape.hxx>

class CPointCloudMeasureDoc : public CDocument
{
protected: // �������л�����
	CPointCloudMeasureDoc();
	DECLARE_DYNCREATE(CPointCloudMeasureDoc)

// ����
public:
    vtkActorCollection      *pStlActorColletion;//StlActorColletion����
	vtkActorCollection      *pCloudActorColletion;//EnsightActorColletion����
	vtkActorCollection      *pSurfacesCADActorColletion;//cad surface����
	vtkActorCollection      *pModelCADActorColletion;//cad model����  yuki20141217
	vtkActorCollection      *pEdgesCADActorColletion;//cad edge ����

	vtkActorCollection      *pPrimitiveActorColletion;//recognize primitves Colletion����
	vtkActorCollection      *pPrimitiveParaActorColletion;//recognized primitves info rColletion����

	vtkLookupTable *pLookUpTable;//��������BAR����ת�������������
	vtkScalarBarActor *pScalarBarActor;//��������BAR��ʾ�����
	vtkScalarBarWidget *pBarWidget;//�ɽ���ʽ��������BAR��ʾ�����

	Entity *pEntity;//����ʵ�������

	bool m_widget_on;//�Ƿ���ʾ��������BAR����

private:
	//��ȡ���ݶ���
	vtkScanPointReader *pcloudSetReader;//sp�����ļ���ȡ�����
	vtkSTLReader  *pstlReader; //STL�����ļ���ȡ�����

	//ģ����ʾ����
	vtkPolyDataMapper *pcloudMapper;//pointCloud���ݵ�Mapper
	vtkPolyDataMapper *pstlPolyDataMapper;//STL���ݵ�Mapper
	//vtkPolyDataMapper *psurfacesMapper;  //surface feature mapper of cad model
	//vtkPolyDataMapper *pedgesMapper;  //surface feature mapper of cad model
   
	vtkActor      *pcloudActor;//VTK����ģ�͵�Actor
	vtkActor      *pstlActor;//STL����ģ�͵�Actor  
	/*vtkActor      *psurfaceActor;
	vtkActor      *pedgesActor;*/

	vtkActor     *pSourceActor;//SourceActor���ڽ���ʽƥ��


    vtkActor2D      *pvtkActor2D;//��ά����ģ�͵�Actor

	vtkMotionTimerCallback* motCallback; // for motion
	int timeID;                          //time ID

	vector<vtkSmartPointer<vtkCaptionWidget>> captionWidgets;

// ����
public:
	void ChangToActorHighlight(vtkActorCollection *pActorCollection,int num,bool highLight, int mode); //set the model highlight according to the feature type
	void ChangeToActorVisibility(vtkActorCollection *pActorCollection,int num,bool visibility);//����ActorCollection����ӦActor��Ŀɼ���
	void ChangeToDataSetMapper(vtkActorCollection *pActorCollection,int num,const char *arrayName, int component);//����ActorCollection����ӦActor�����������ӳ����ӻ�
    void ReorderActors();//���¶���ͼ���Actors���д���
	int  GetActorVisibility(vtkActorCollection *pActorCollection,int num);//�õ�ActorCollection����ӦActor�Ŀɼ���
	void CreateScalarBar(void);//������������BAR
	void DisplayWidget(void);//��ʾ��������BAR
	void MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData,CString &str,double m_range[2],int num);//�ñ����������Ա���ӳ��pActorCollection�е���Ӧ����п��ӻ�
	void MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData,CString &str,double m_range[2],int num,int component);//��ʸ���������Ա���ӳ��pActorCollection�е���Ӧ����п��ӻ�
	void DisplayMatchModels(); //��ʾƥ��ģ��
	void AddCurrentPolyDataInRenderer(vtkPolyDataCollection *pDatasetColletion, int num, bool source); //��polydataģ�������ʾ
	void AddSingleActorToRender(vtkActor *pActor);//��actor���render��ʾ
	void InteractMatchUpdatePolyData(); //���ݽ���ʽ�Ľ��������ģ��
	void ICPMatchUpdatePolyData(); //����ICP����ģ��
	void ComputeMeasurement(); //����������
	void ComputeMeasurementWithFeature(const int ind, const bool surfOrEdge); 
	vtkDataArray* GetAttributeArrayFromMeasuredData(); //��������������
	vtkDataArray* GetDisplaceArrayFromMeasuredData(); //��÷��������������
	void DataExport(int numPointCloud, int numStl);
	void ActorVisibleAll(vtkActorCollection *pActorCollection);
	void ActorVisibleOnly(vtkActorCollection *pActorCollection,int num);
	void DisplayRecognizedModel();
	void DisplayRecognizedModelInfo();
	void DisplayRecognizedSingleInfo(int num);
	void RANSACBasedShapeRecognition(vtkPolyData *pPolyData, float disThrehold, float projThrehold, float probThrehold, float normThrehold, float minPoint, bool usePlane, bool useCylinder, bool useCone, bool useSphere, bool useTorus);

	void AddCaptionWidget(CString str, double position[3]); //��ʾ��ǩ

	void AddMotionActionObserver(const int index);                        //cadģ���˶���ʾ
	void RemoveMotionActionObserver();                                    //�Ƴ��˶���ʾ״̬

	void DataChangeToPointcloudData();


private:
	void ExecutePipeline();//���д�����
	void RemoveActors();//�Ƴ���ͼ�е�ACTORS
	void ClearlAllOldActors();//�Ƴ���ͼ�е����е�ACTORS
	void DataChangeToActor();//ģ�����ݸı�֮������Ӧ��ACTORS�ĺ���
	void PolyDataCollectionToActor(vtkPolyDataCollection *pPolyDataColletion,vtkActorCollection *pActorCollection,vtkRenderer *pRender, bool cloud);//��STL����ģ�����ݼ���ת����ACTORS�࣬������ʾ����ͼ����
	void ActorCollectionToDisplay(vtkActorCollection *pActorCollection,vtkRenderer *pRender);//��ActorCollection������ʾ
	void ActorCollectionRemove(vtkActorCollection *pActorCollection,vtkRenderer *pRender);//�Ƴ���ͼ�е�ActorCollection���ϵ���Actor
//	void PhysicalPropertyPolyDataSetCollectionToActor(vtkPolyDataCollection *pDatasetColletion,vtkActorCollection *pActorCollection,int num);
//	void CopyActorCollection(vtkActorCollection *pActorCollection, vtkActorCollection *pOldActorCollection); //����actor collection

	TopoDS_Shape GetFeatureSurface(const int ind);
	TopoDS_Shape GetFeatureEdge(const int ind);

	void IntializeMotionActionObserver(); //��ʼ��ģ���˶�ģ��

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CPointCloudMeasureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnStlImport();
	afx_msg void OnFileSave();
	afx_msg void OnIgsImport();
	afx_msg void OnMenuIgesAll();
	afx_msg void OnMuneExportRansac();
};


