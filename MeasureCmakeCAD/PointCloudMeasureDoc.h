
// PointCloudMeasureDoc.h : CPointCloudMeasureDoc 类的接口
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
protected: // 仅从序列化创建
	CPointCloudMeasureDoc();
	DECLARE_DYNCREATE(CPointCloudMeasureDoc)

// 属性
public:
    vtkActorCollection      *pStlActorColletion;//StlActorColletion集合
	vtkActorCollection      *pCloudActorColletion;//EnsightActorColletion集合
	vtkActorCollection      *pSurfacesCADActorColletion;//cad surface集合
	vtkActorCollection      *pModelCADActorColletion;//cad model集合  yuki20141217
	vtkActorCollection      *pEdgesCADActorColletion;//cad edge 集合

	vtkActorCollection      *pPrimitiveActorColletion;//recognize primitves Colletion集合
	vtkActorCollection      *pPrimitiveParaActorColletion;//recognized primitves info rColletion集合

	vtkLookupTable *pLookUpTable;//物理属性BAR变量转化表设置类变量
	vtkScalarBarActor *pScalarBarActor;//物理属性BAR显示类变量
	vtkScalarBarWidget *pBarWidget;//可交互式物理属性BAR显示类变量

	Entity *pEntity;//建立实体类对象

	bool m_widget_on;//是否显示物理属性BAR变量

private:
	//读取数据对象
	vtkScanPointReader *pcloudSetReader;//sp类型文件读取类变量
	vtkSTLReader  *pstlReader; //STL类型文件读取类变量

	//模型显示对象
	vtkPolyDataMapper *pcloudMapper;//pointCloud数据的Mapper
	vtkPolyDataMapper *pstlPolyDataMapper;//STL数据的Mapper
	//vtkPolyDataMapper *psurfacesMapper;  //surface feature mapper of cad model
	//vtkPolyDataMapper *pedgesMapper;  //surface feature mapper of cad model
   
	vtkActor      *pcloudActor;//VTK数据模型的Actor
	vtkActor      *pstlActor;//STL数据模型的Actor  
	/*vtkActor      *psurfaceActor;
	vtkActor      *pedgesActor;*/

	vtkActor     *pSourceActor;//SourceActor用于交互式匹配


    vtkActor2D      *pvtkActor2D;//二维数据模型的Actor

	vtkMotionTimerCallback* motCallback; // for motion
	int timeID;                          //time ID

	vector<vtkSmartPointer<vtkCaptionWidget>> captionWidgets;

// 操作
public:
	void ChangToActorHighlight(vtkActorCollection *pActorCollection,int num,bool highLight, int mode); //set the model highlight according to the feature type
	void ChangeToActorVisibility(vtkActorCollection *pActorCollection,int num,bool visibility);//设置ActorCollection中相应Actor项的可见性
	void ChangeToDataSetMapper(vtkActorCollection *pActorCollection,int num,const char *arrayName, int component);//设置ActorCollection中相应Actor项的物理属性映射可视化
    void ReorderActors();//重新对视图类的Actors进行处理
	int  GetActorVisibility(vtkActorCollection *pActorCollection,int num);//得到ActorCollection中相应Actor的可见性
	void CreateScalarBar(void);//创建物理属性BAR
	void DisplayWidget(void);//显示物理属性BAR
	void MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData,CString &str,double m_range[2],int num);//用标量物理属性变量映射pActorCollection中的相应项，进行可视化
	void MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData,CString &str,double m_range[2],int num,int component);//用矢量物理属性变量映射pActorCollection中的相应项，进行可视化
	void DisplayMatchModels(); //显示匹配模型
	void AddCurrentPolyDataInRenderer(vtkPolyDataCollection *pDatasetColletion, int num, bool source); //将polydata模型添加显示
	void AddSingleActorToRender(vtkActor *pActor);//将actor添加render显示
	void InteractMatchUpdatePolyData(); //根据交互式的结果，更新模型
	void ICPMatchUpdatePolyData(); //根据ICP更新模型
	void ComputeMeasurement(); //计算测量误差
	void ComputeMeasurementWithFeature(const int ind, const bool surfOrEdge); 
	vtkDataArray* GetAttributeArrayFromMeasuredData(); //获得误差属性数组
	vtkDataArray* GetDisplaceArrayFromMeasuredData(); //获得方向属性误差数组
	void DataExport(int numPointCloud, int numStl);
	void ActorVisibleAll(vtkActorCollection *pActorCollection);
	void ActorVisibleOnly(vtkActorCollection *pActorCollection,int num);
	void DisplayRecognizedModel();
	void DisplayRecognizedModelInfo();
	void DisplayRecognizedSingleInfo(int num);
	void RANSACBasedShapeRecognition(vtkPolyData *pPolyData, float disThrehold, float projThrehold, float probThrehold, float normThrehold, float minPoint, bool usePlane, bool useCylinder, bool useCone, bool useSphere, bool useTorus);

	void AddCaptionWidget(CString str, double position[3]); //显示标签

	void AddMotionActionObserver(const int index);                        //cad模型运动显示
	void RemoveMotionActionObserver();                                    //移除运动显示状态

	void DataChangeToPointcloudData();


private:
	void ExecutePipeline();//运行处理流
	void RemoveActors();//移除视图中的ACTORS
	void ClearlAllOldActors();//移除视图中的所有的ACTORS
	void DataChangeToActor();//模型数据改变之后处理相应的ACTORS的函数
	void PolyDataCollectionToActor(vtkPolyDataCollection *pPolyDataColletion,vtkActorCollection *pActorCollection,vtkRenderer *pRender, bool cloud);//对STL几何模型数据集合转换成ACTORS类，并且显示到视图类中
	void ActorCollectionToDisplay(vtkActorCollection *pActorCollection,vtkRenderer *pRender);//对ActorCollection进行显示
	void ActorCollectionRemove(vtkActorCollection *pActorCollection,vtkRenderer *pRender);//移除视图中的ActorCollection集合单项Actor
//	void PhysicalPropertyPolyDataSetCollectionToActor(vtkPolyDataCollection *pDatasetColletion,vtkActorCollection *pActorCollection,int num);
//	void CopyActorCollection(vtkActorCollection *pActorCollection, vtkActorCollection *pOldActorCollection); //复制actor collection

	TopoDS_Shape GetFeatureSurface(const int ind);
	TopoDS_Shape GetFeatureEdge(const int ind);

	void IntializeMotionActionObserver(); //初始化模型运动模块

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CPointCloudMeasureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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


