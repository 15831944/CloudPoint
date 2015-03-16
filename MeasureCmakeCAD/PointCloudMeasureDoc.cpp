
// PointCloudMeasureDoc.cpp : CPointCloudMeasureDoc ���ʵ��
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "PointCloudMeasureView.h"
#include "PointCloudMeasureDoc.h"
#include "vtkHausdorffDistancePointSetFilter.h"
#include "OutPutDlg.h"
#include "vtkScanPointWriter.h"
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkRenderWindow.h>
#include "vtkIGESReader.h"
#include "IGESToVTKConvertion.h"
#include <TopoDS_Shape.hxx>
#include <vtkProperty.h>
#include "BasicGeometryConvertion.h"
#include "MeasureWithSurface.h"
#include "MeasureWithEdge.h"
#include "RansacModel.h"
#include "IgesAndVtkModel.h"
#include "PhysicalFileBrowseDir.h"
#include <vtkMath.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPointCloudMeasureDoc

IMPLEMENT_DYNCREATE(CPointCloudMeasureDoc, CDocument)

BEGIN_MESSAGE_MAP(CPointCloudMeasureDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CPointCloudMeasureDoc::OnFileOpen)
	ON_COMMAND(ID_STL_IMPORT, &CPointCloudMeasureDoc::OnStlImport)
	ON_COMMAND(ID_FILE_SAVE, &CPointCloudMeasureDoc::OnFileSave)
	ON_COMMAND(ID_IGS_IMPORT, &CPointCloudMeasureDoc::OnIgsImport)
	ON_COMMAND(ID_MENU_IGES_ALL, &CPointCloudMeasureDoc::OnMenuIgesAll)
	ON_COMMAND(ID_MUNE_EXPORT_RANSAC, &CPointCloudMeasureDoc::OnMuneExportRansac)
END_MESSAGE_MAP()


// CPointCloudMeasureDoc ����/����

CPointCloudMeasureDoc::CPointCloudMeasureDoc()
{
	// TODO: �ڴ����һ���Թ������
	this->pstlReader = NULL;
	this->pcloudSetReader = NULL;
	this->pstlPolyDataMapper = NULL;
	this->pcloudMapper = NULL;
	this->pstlActor = NULL;
	this->pcloudActor = NULL;

	this->pEntity=new Entity;

	this->pStlActorColletion = vtkActorCollection::New();
	this->pCloudActorColletion = vtkActorCollection::New();
	this->pSurfacesCADActorColletion = vtkActorCollection::New();
	this->pModelCADActorColletion = vtkActorCollection::New();//yuki
	this->pEdgesCADActorColletion = vtkActorCollection::New();
	this->pPrimitiveActorColletion =  vtkActorCollection::New();
	this->pPrimitiveParaActorColletion =  vtkActorCollection::New();
	this->pSourceActor = vtkActor::New();

	this->pScalarBarActor = NULL;
	this->pLookUpTable = NULL;
	this->pBarWidget = NULL;

	this->motCallback = NULL;

	this->m_widget_on=FALSE;

	this->CreateScalarBar();

}

CPointCloudMeasureDoc::~CPointCloudMeasureDoc()
{
	if(this->pEntity)
	{
		delete this->pEntity;
	}
	if(this->pCloudActorColletion)
	{
		this->pCloudActorColletion->RemoveAllItems();
		this->pCloudActorColletion->Delete();
	}
	if(this->pStlActorColletion)
	{
		this->pStlActorColletion->RemoveAllItems();
		this->pStlActorColletion->Delete();
	}
	if(this->pSurfacesCADActorColletion)
	{
		this->pSurfacesCADActorColletion->RemoveAllItems();
		this->pSurfacesCADActorColletion->Delete();
	}
	if(this->pEdgesCADActorColletion)
	{
		this->pEdgesCADActorColletion->RemoveAllItems();
		this->pEdgesCADActorColletion->Delete();
	}
	if(this->pModelCADActorColletion)
	{
		this->pModelCADActorColletion->RemoveAllItems();
		this->pModelCADActorColletion->Delete();
	}
	if(this->pPrimitiveActorColletion)
	{
		this->pPrimitiveActorColletion->RemoveAllItems();
		this->pPrimitiveActorColletion->Delete();
	}
	if(this->pPrimitiveParaActorColletion)
	{
		this->pPrimitiveParaActorColletion->RemoveAllItems();
		this->pPrimitiveParaActorColletion->Delete();
	}
	if(this->pLookUpTable)
	{
		this->pLookUpTable->Delete();
	}
	if(this->pScalarBarActor)
	{
		this->pScalarBarActor->Delete();
	}
	if(this->pBarWidget)
	{
		this->pBarWidget->Delete();
	}
	if(this->pSourceActor)
	{
		this->pSourceActor->Delete();
	}

	if(this->motCallback)
		this->motCallback->Delete();
}

BOOL CPointCloudMeasureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	// remove old actors
	// RemoveActors();

	// execute object pipeline
	ExecutePipeline();

	return TRUE;
}




// CPointCloudMeasureDoc ���л�

void CPointCloudMeasureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CPointCloudMeasureDoc ���

#ifdef _DEBUG
void CPointCloudMeasureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPointCloudMeasureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPointCloudMeasureDoc ����

//���д���
void CPointCloudMeasureDoc::ExecutePipeline()
{
	// get our renderer first
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if(pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(pstlReader) // have file
	{
		pCPointCloudMeasureView->GetRenderer()->AddActor(this->pstlActor);
		pCPointCloudMeasureView->GetRenderer()->ResetCamera();
	}
	else if(pcloudSetReader)
	{
		pCPointCloudMeasureView->GetRenderer()->AddActor(this->pcloudActor);
		pCPointCloudMeasureView->GetRenderer()->ResetCamera();
	}
	else  // have no file
	{	
		pCPointCloudMeasureView->GetRenderer()->SetBackground(1,1,1);
		pCPointCloudMeasureView->GetRenderer()->ResetCamera();
	}
}

//�Ƴ�actor
//�Ƴ���ͼ�е�ACTORS
void CPointCloudMeasureDoc::RemoveActors()
{
	// get our renderer first
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	// remove old actors, it is not good for here to use this, which will remove the 
	//axis and widget for x y z coordinate axis.
	//pCPointCloudMeasureView->GetRenderer()->RemoveAllViewProps();

}

//��ȡ�����ļ�
void CPointCloudMeasureDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,"sp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"scan point(*.sp)|*.sp||",NULL);//��������sp�ļ����˵��ļ�����ѡ��Ի���

	if(dlg.DoModal()==IDOK)
	{
		CString strName=dlg.GetPathName();//�õ��ļ�·��
		this->pcloudSetReader = vtkScanPointReader::New();
		this->pcloudActor = vtkActor::New();
		this->pcloudSetReader->SetFileName((LPCTSTR)strName);//ͨ��STLREADER��spģ�Ͷ���
		this->pcloudMapper = vtkPolyDataMapper::New();
		this->pcloudMapper->SetInput(this->pcloudSetReader->GetOutput());
		this->pcloudActor->SetMapper(this->pcloudMapper);
		this->pcloudActor->GetProperty()->SetColor(1,0,0);
		this->pcloudActor->GetProperty()->SetPointSize(4);
		this->pCloudActorColletion->AddItem(this->pcloudActor);//��STLACTOR����pCloudActorColletion��
		this->pEntity->pPointCloudDataSetCollection->AddItem(this->pcloudSetReader->GetOutput());//��ģ�����ݼ���pEntity��pStlDataSetCollection������
		ExecutePipeline();
		if(this->pcloudSetReader)this->pcloudSetReader->Delete();
		if(this->pcloudMapper)this->pcloudMapper->Delete();
		if(this->pcloudActor)this->pcloudActor->Delete();
		this->pcloudSetReader=NULL;

		this->UpdateAllViews(NULL);//����������ͼ
	}
}

void CPointCloudMeasureDoc::OnStlImport()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,"stl",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"STL graphic File(*.stl)|*.stl||",NULL);//��������STL�ļ����˵��ļ�����ѡ��Ի���
	if(dlg.DoModal()==IDOK)
	{
		RemoveActors();//�Ƴ�VTKACTORS
		CString strName=dlg.GetPathName();//�õ��ļ�·��
		this->pstlReader=vtkSTLReader::New();
		this->pstlActor=vtkActor::New();
		this->pstlReader->SetFileName((LPCTSTR)strName);//ͨ��STLREADER��STLģ�Ͷ���
		this->pstlPolyDataMapper=vtkPolyDataMapper::New();
		this->pstlPolyDataMapper->SetInput(this->pstlReader->GetOutput());
		this->pstlActor->SetMapper(this->pstlPolyDataMapper);
		this->pstlActor->GetProperty()->SetColor(0.9,0.9,0.9);
		this->pstlActor->DragableOff();
		this->pstlActor->PickableOff();
		this->pStlActorColletion->AddItem(this->pstlActor);//��STLACTOR����pStlActorColletion��
		this->pEntity->pStlDataSetCollection->AddItem(this->pstlReader->GetOutput());//��ģ�����ݼ���pEntity��pStlDataSetCollection������
		ExecutePipeline();
		if(this->pstlReader)this->pstlReader->Delete();
		if(this->pstlPolyDataMapper)this->pstlPolyDataMapper->Delete();
		if(this->pstlActor)this->pstlActor->Delete();
		this->pstlReader=NULL;

		this->UpdateAllViews(NULL);//����������ͼ
	}
}


//����ActorCollection����ӦActor��Ŀɼ���
void CPointCloudMeasureDoc::ChangeToActorVisibility(vtkActorCollection *pActorCollection, int num, bool visibility)
{
	vtkActor *pCurrentActor = NULL;
	pActorCollection->InitTraversal();
	for(int i=0;i<=num;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();
	}
	if(visibility)//���õ�ǰActor�Ŀɼ���
	{
		pCurrentActor->VisibilityOn();
	}
	else
	{
		pCurrentActor->VisibilityOff();
	}
}

//����ActorCollection����ӦActor�����������ӳ����ӻ�
void CPointCloudMeasureDoc::ChangeToDataSetMapper(vtkActorCollection *pActorCollection,int num,const char *arrayName, int component)
{
	vtkActor *pCurrentActor = NULL;
	pActorCollection->InitTraversal();
	for(int i=0;i<=num;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();
	}
	vtkMapper *pCurrentMapper=pCurrentActor->GetMapper();//�õ���ǰActor��Mapperָ��
	pCurrentMapper->ColorByArrayComponent(arrayName,component);//���õ�ǰMapper������ӳ������ֵ
}

//�õ�ActorCollection����ӦActor�Ŀɼ���
int CPointCloudMeasureDoc::GetActorVisibility(vtkActorCollection *pActorCollection, int num)
{
	int visible = 0;
	vtkActor *pCurrentActor = NULL;
	pActorCollection->InitTraversal();
	for(int i=0;i<=num;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();
	}
	visible=pCurrentActor->GetVisibility();//�õ���ɼ���
	return visible;
}

//�Ƴ���ͼ�е����е�ACTORS
void CPointCloudMeasureDoc::ClearlAllOldActors()
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	// remove old actors
	vtkActorCollection *pActorCollection=pCPointCloudMeasureView->GetRenderer()->GetActors();//�õ���ǰ��ͼ��ActorCollectionָ��
	pActorCollection->InitTraversal();
	int m_NumOfActors=pActorCollection->GetNumberOfItems();
	for(int i=0;i<m_NumOfActors;i++)//�Ƴ����е�Actor
	{
		this->ActorCollectionRemove(pActorCollection,pCPointCloudMeasureView->GetRenderer());
	}
}

//�Ƴ���ͼ�е�ActorCollection�����еĵ���Actor
void CPointCloudMeasureDoc::ActorCollectionRemove(vtkActorCollection *pActorCollection, vtkRenderer *pRender)
{
	vtkActor *pCurrentActor=pActorCollection->GetNextActor();
	pRender->RemoveActor(pCurrentActor); 
}

//���¶���ͼ���Actors���д���
void CPointCloudMeasureDoc::ReorderActors()
{
	this->ClearlAllOldActors();//�����ǰ��ͼ�е�����Actors
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(this->pCloudActorColletion->GetNumberOfItems()>0)//��ʾpStlActorColletionģ������
	{
		this->pCloudActorColletion->InitTraversal();
		for(int i=0;i<this->pCloudActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pCloudActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pStlActorColletion->GetNumberOfItems()>0)//��ʾpStlActorColletionģ������
	{
		this->pStlActorColletion->InitTraversal();
		for(int i=0;i<this->pStlActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pStlActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pSurfacesCADActorColletion->GetNumberOfItems()>0)//��ʾpSurfacesCADActorColletionģ������
	{
		this->pSurfacesCADActorColletion->InitTraversal();
		for(int i=0;i<this->pSurfacesCADActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pSurfacesCADActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pModelCADActorColletion->GetNumberOfItems()>0)//��ʾpModelCADActorColletionģ������  yuki20141222
	{
		this->pModelCADActorColletion->InitTraversal();
		for(int i=0;i<this->pModelCADActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pModelCADActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pEdgesCADActorColletion->GetNumberOfItems()>0)//��ʾpEdgesCADActorColletionģ������
	{
		this->pEdgesCADActorColletion->InitTraversal();
		for(int i=0;i<this->pEdgesCADActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pEdgesCADActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pPrimitiveActorColletion->GetNumberOfItems()>0)//��ʾransacģ������
	{
		this->pPrimitiveActorColletion->InitTraversal();
		for(int i=0;i<this->pPrimitiveActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pPrimitiveActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}

	if(this->pPrimitiveParaActorColletion->GetNumberOfItems()>0)//��ʾransac infoģ������
	{
		this->pPrimitiveParaActorColletion->InitTraversal();
		for(int i=0;i<this->pPrimitiveParaActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pPrimitiveParaActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}
}

//��ActorCollection������ʾ
void CPointCloudMeasureDoc::ActorCollectionToDisplay(vtkActorCollection *pActorCollection, vtkRenderer *pRender)
{
	vtkActor *pCurrentActor=pActorCollection->GetNextActor();
	pRender->AddActor(pCurrentActor);
}

void CPointCloudMeasureDoc::DataChangeToPointcloudData()
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(this->pCloudActorColletion->GetNumberOfItems()!=0)
	{
		this->pCloudActorColletion->RemoveAllItems();
	}

	if(this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()!=0)//��ʾpCloudActorColletionģ������
	{
		this->pEntity->pPointCloudDataSetCollection->InitTraversal();
		for(int i=0;i<this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();i++)
		{
			this->PolyDataCollectionToActor(this->pEntity->pPointCloudDataSetCollection,this->pCloudActorColletion,pCPointCloudMeasureView->GetRenderer(), TRUE);
		}
	}
}

//ģ�����ݸı�֮������Ӧ��ACTORS�ĺ���
void CPointCloudMeasureDoc::DataChangeToActor()
{
	this->ClearlAllOldActors();//�����ǰ��ͼ�е�����Actors
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if(this->pCloudActorColletion->GetNumberOfItems()!=0)
	{
		this->pCloudActorColletion->RemoveAllItems();
	}
	if(this->pStlActorColletion->GetNumberOfItems()!=0)
	{
		this->pStlActorColletion->RemoveAllItems();
	}

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()!=0)//��ʾpCloudActorColletionģ������
	{
		this->pEntity->pPointCloudDataSetCollection->InitTraversal();
		for(int i=0;i<this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();i++)
		{
			this->PolyDataCollectionToActor(this->pEntity->pPointCloudDataSetCollection,this->pCloudActorColletion,pCPointCloudMeasureView->GetRenderer(), TRUE);
		}
	}

	if(this->pEntity->pStlDataSetCollection->GetNumberOfItems()!=0)//��ʾpStlActorColletionģ������
	{
		this->pEntity->pStlDataSetCollection->InitTraversal();
		for(int i=0;i<this->pEntity->pStlDataSetCollection->GetNumberOfItems();i++)
		{
			this->PolyDataCollectionToActor(this->pEntity->pStlDataSetCollection,this->pStlActorColletion,pCPointCloudMeasureView->GetRenderer(), FALSE);
		}
	}
}

//��STL����ģ�����ݼ���ת����ACTORS�࣬������ʾ����ͼ����
void CPointCloudMeasureDoc::PolyDataCollectionToActor(vtkPolyDataCollection *pPolyDataColletion,vtkActorCollection *pActorCollection,vtkRenderer *pRender, bool cloud)
{
	vtkPolyData *pCurrentData = NULL;
	vtkPolyDataMapper *pStlMapper=vtkPolyDataMapper::New();
	vtkActor *pStlActor=vtkActor::New();
	pCurrentData=pPolyDataColletion->GetNextItem();
	pStlMapper->SetInput(pCurrentData);
	pStlActor->SetMapper(pStlMapper);

	//for cloud, set the vertex for 4 vixel to display
	if(cloud)
	{
		pStlActor->GetProperty()->SetPointSize(4);
		pStlActor->GetProperty()->SetColor(0,0,1);
	}

	pRender->AddActor(pStlActor);
	pActorCollection->AddItem(pStlActor);
	pStlMapper->Delete();
	pStlActor->Delete();
}

//������������BAR
void CPointCloudMeasureDoc::CreateScalarBar()
{
	this->pLookUpTable=vtkLookupTable::New();
	this->pScalarBarActor=vtkScalarBarActor::New();
	this->pLookUpTable->SetNumberOfTableValues(256);
	this->pLookUpTable->SetHueRange(0.667,0);
	this->pLookUpTable->SetTableRange(0,1);
	this->pLookUpTable->SetNumberOfColors(256);
	this->pLookUpTable->Build();

	this->pScalarBarActor->SetLookupTable(this->pLookUpTable);
	this->pScalarBarActor->SetTitle("Scalar & Color");
	this->pScalarBarActor->GetPositionCoordinate()->SetValue(0.1,0.1);
	this->pScalarBarActor->SetOrientationToVertical();
	this->pScalarBarActor->SetWidth(0.075);
	this->pScalarBarActor->SetHeight(0.5);

	this->pBarWidget=vtkScalarBarWidget::New();
	this->pBarWidget->SetScalarBarActor(this->pScalarBarActor);
}

//��ʾ��������BAR
void CPointCloudMeasureDoc::DisplayWidget()
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;
	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}
	if(this->pBarWidget)
	{
		this->pBarWidget->SetInteractor(pCPointCloudMeasureView->GetMFCWindow()->GetInteractor());
		this->pBarWidget->EnabledOn();
		this->pBarWidget->RepositionableOn();
		if(this->m_widget_on)
		{
			this->pBarWidget->On();
		}
		else
		{
			this->pBarWidget->Off();
		}
	}

	if(this->captionWidgets.size())
	{
		for(int i = 0; i < this->captionWidgets.size(); i++)
		{
			vtkSmartPointer<vtkCaptionWidget> widget = captionWidgets[i];
			widget->SetInteractor(pCPointCloudMeasureView->GetMFCWindow()->GetInteractor());
			widget->EnabledOn();
			widget->ResizableOn();
			widget->On();
		}
	}
}

//�ñ����������Ա���ӳ��pActorCollection�е���Ӧ����п��ӻ�
void CPointCloudMeasureDoc::MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData, CString &str,double m_range[],int num)
{
	if(this->pLookUpTable)
	{
		this->pLookUpTable->Delete();
		this->pLookUpTable=NULL;
	}

	if(this->pScalarBarActor)
	{
		this->pScalarBarActor->Delete();
		this->pScalarBarActor=NULL;
	}

	if(this->pBarWidget)
	{
		this->pBarWidget->Delete();
		this->pBarWidget=NULL;
	}

	vtkPolyDataMapper *pCurrentMapper = vtkPolyDataMapper::New();
	vtkActor *pActor = vtkActor::New();

	//vtkUnstructuredGrid *pCurrentData=vtkUnstructuredGrid::New();
	//pCurrentData->DeepCopy(pUnstructuredGrid);
	//pCurrentData->Update();

	this->pLookUpTable=vtkLookupTable::New();//����pLookUpTable�����������Ի���
	this->pScalarBarActor=vtkScalarBarActor::New();
	this->pLookUpTable->SetNumberOfTableValues(256);
	this->pLookUpTable->SetHueRange(0.667,0);
	this->pLookUpTable->SetTableRange(m_range[0],m_range[1]);
	this->pLookUpTable->SetNumberOfColors(256);
	this->pLookUpTable->Build();

	this->pScalarBarActor->SetLookupTable(this->pLookUpTable);//������������BAR�Ĳ���
	this->pScalarBarActor->SetTitle(str);
	this->pScalarBarActor->GetPositionCoordinate()->SetValue(0.1,0.1);
	this->pScalarBarActor->SetOrientationToVertical();
	this->pScalarBarActor->SetWidth(0.075);
	this->pScalarBarActor->SetHeight(0.5);

	this->pBarWidget=vtkScalarBarWidget::New();
	this->pBarWidget->SetScalarBarActor(this->pScalarBarActor);//�����ɽ���ʽ��������BAR

	pCurrentMapper->SetInput(pPolyData);//��������������ӳ�䵽����ģ�ͣ����п��ӻ�
	pCurrentMapper->SetLookupTable(this->pLookUpTable);
	//pCurrentMapper->SetInterpolateScalarsBeforeMapping(1);
	pCurrentMapper->SetColorModeToMapScalars();
	pCurrentMapper->SetScalarModeToUsePointData();
	pCurrentMapper->SetScalarRange(m_range[0],m_range[1]);
	pCurrentMapper->SelectColorArray(str);
	pCurrentMapper->SetUseLookupTableScalarRange(0);
	pActor->SetMapper(pCurrentMapper);
	pActor->GetProperty()->SetPointSize(4);

	pActorCollection->ReplaceItem(num, pActor);

	pCurrentMapper->Delete();
	pActor->Delete();
	//pCurrentData->Delete();
}

//��ʸ���������Ա���ӳ��pActorCollection�е���Ӧ����п��ӻ�
void CPointCloudMeasureDoc::MapperChangeToDisplay(vtkActorCollection *pActorCollection,vtkPolyDataCollection *pDatasetColletion,vtkPolyData *pPolyData, CString &str,double m_range[],int num,int component)
{
	if(this->pLookUpTable)
	{
		this->pLookUpTable->Delete();
		this->pLookUpTable=NULL;
	}

	if(this->pScalarBarActor)
	{
		this->pScalarBarActor->Delete();
		this->pScalarBarActor=NULL;
	}

	if(this->pBarWidget)
	{
		this->pBarWidget->Delete();
		this->pBarWidget=NULL;
	}

	CString m_bar_title=str;
	if(component==0)
	{
		m_bar_title+="_X";
	}
	else if(component==1)
	{
		m_bar_title+="_Y";
	}
	else if(component==2)
	{
		m_bar_title+="_Z";
	}

	vtkPolyDataMapper *pCurrentMapper = vtkPolyDataMapper::New();
	vtkActor *pActor = vtkActor::New();

	//vtkUnstructuredGrid *pCurrentData=vtkUnstructuredGrid::New();
	//pCurrentData->DeepCopy(pUnstructuredGrid);
	//pCurrentData->Update();

	this->pLookUpTable=vtkLookupTable::New();//����pLookUpTable�����������Ի���
	this->pScalarBarActor=vtkScalarBarActor::New();
	this->pLookUpTable->SetNumberOfTableValues(256);
	this->pLookUpTable->SetHueRange(0.667,0);
	this->pLookUpTable->SetTableRange(m_range[0],m_range[1]);
	this->pLookUpTable->SetNumberOfColors(256);
	this->pLookUpTable->SetVectorModeToComponent();
	this->pLookUpTable->SetVectorComponent(component);
	this->pLookUpTable->Build();

	this->pScalarBarActor->SetLookupTable(this->pLookUpTable);//������������BAR�Ĳ���
	this->pScalarBarActor->SetTitle(m_bar_title);
	this->pScalarBarActor->GetPositionCoordinate()->SetValue(0.1,0.1);
	this->pScalarBarActor->SetOrientationToVertical();
	this->pScalarBarActor->SetWidth(0.075);
	this->pScalarBarActor->SetHeight(0.5);

	this->pBarWidget=vtkScalarBarWidget::New();
	this->pBarWidget->SetScalarBarActor(this->pScalarBarActor);//�����ɽ���ʽ��������BAR

	pCurrentMapper->SetInput(pPolyData);
	pCurrentMapper->SetLookupTable(this->pLookUpTable);
	//pCurrentMapper->SetInterpolateScalarsBeforeMapping(1);
	pCurrentMapper->SetColorModeToMapScalars();//��ʸ���������Եķ���ӳ�䵽����ģ�ͣ����п��ӻ�
	pCurrentMapper->SetScalarModeToUsePointData();
	pCurrentMapper->SetScalarRange(m_range[0],m_range[1]);
	pCurrentMapper->SelectColorArray(str);
	pCurrentMapper->ColorByArrayComponent(str,component);
	pCurrentMapper->SetUseLookupTableScalarRange(0);
	pActor->SetMapper(pCurrentMapper);
	pActor->GetProperty()->SetPointSize(4);

	pActorCollection->ReplaceItem(num, pActor);

	pCurrentMapper->Delete();
	pActor->Delete();
	//pCurrentData->Delete();
}

//void CopyActorCollection(vtkActorCollection *pActorCollection, vtkActorCollection *pOldActorCollection)
//{
//}

//ƥ��ģ����ʾ
void CPointCloudMeasureDoc::DisplayMatchModels()
{
	this->ClearlAllOldActors();

	int numCurrentCloud = this->pEntity->current_point_cloud_part_num;
	int numCurrentStl = this->pEntity->stl_for_match_to_point;

	this->AddCurrentPolyDataInRenderer(this->pEntity->pPointCloudDataSetCollection, numCurrentCloud, TRUE);
	this->AddCurrentPolyDataInRenderer(this->pEntity->pStlDataSetCollection, numCurrentStl, FALSE);
}

//��polydataģ�������Ȼ����ʾ
void CPointCloudMeasureDoc::AddCurrentPolyDataInRenderer(vtkPolyDataCollection *pDatasetColletion, int num, bool source)
{
	vtkPolyData *pCurrentData;
	pDatasetColletion->InitTraversal();
	for(int i=0;i<=num;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pCurrentData = pDatasetColletion->GetNextItem();
	}

	vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
	mapper->SetInput(pCurrentData);

	if(source)
	{
		//��������actor
		if(this->pSourceActor)
		{
			this->pSourceActor->Delete();
			this->pSourceActor = vtkActor::New();
		}
		this->pSourceActor->SetMapper(mapper);
		this->pSourceActor->GetProperty()->SetColor(1,0,0);
		this->pSourceActor->GetProperty()->SetPointSize(4);
		this->AddSingleActorToRender(this->pSourceActor);
	}
	else
	{
		vtkActor *actor = vtkActor::New();
		actor->SetMapper(mapper);
		actor->PickableOff();
		actor->DragableOff();
		this->AddSingleActorToRender(actor);
		actor->Delete();
	}

	mapper->Delete();
}

//������actor��ӵ�renderer�н�����ʾ
void CPointCloudMeasureDoc::AddSingleActorToRender(vtkActor *pActor)
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;
	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	pCPointCloudMeasureView->GetRenderer()->AddActor(pActor);
}

//���ݽ���ʽƥ��Ľ������ģ��
void CPointCloudMeasureDoc::InteractMatchUpdatePolyData()
{
	int numCurrentCloud = this->pEntity->current_point_cloud_part_num;

	vtkPolyData *pCurrentData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numCurrentCloud;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pCurrentData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	vtkMatrix4x4 *m = pSourceActor->GetMatrix();
	vtkTransform *translation = vtkTransform::New();
	translation->SetMatrix(m);

	// Transform the source points by the matrix
	vtkTransformPolyDataFilter *pTransformFilter = vtkTransformPolyDataFilter::New();
	pTransformFilter->SetInput(pCurrentData);
	pTransformFilter->SetTransform(translation);
	pTransformFilter->Update();

	this->pEntity->pPointCloudDataSetCollection->ReplaceItem(numCurrentCloud, pTransformFilter->GetOutput());

	translation->Delete();
	pTransformFilter->Delete();
}

//����ICP����ģ��
void CPointCloudMeasureDoc::ICPMatchUpdatePolyData()
{
	int numSource = this->pEntity->current_point_cloud_part_num;
	int numTarget = this->pEntity->stl_for_match_to_point;

	vtkPolyData *pSourceData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numSource;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pSourceData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	vtkPolyData *pTargetData;
	this->pEntity->pStlDataSetCollection->InitTraversal();
	for(int i=0;i<=numTarget;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pTargetData = this->pEntity->pStlDataSetCollection->GetNextItem();
	}

	vtkIterativeClosestPointTransform *icp = vtkIterativeClosestPointTransform::New();
	icp->SetSource(pSourceData);
	icp->SetTarget(pTargetData);
	icp->GetLandmarkTransform()->SetModeToRigidBody();
	//need to be changed to set with users
	icp->SetMaximumNumberOfIterations(100);
	icp->Modified();
	icp->Update();

	// Get the resulting transformation matrix (this matrix takes the source points to the target points)
	vtkMatrix4x4 *m = icp->GetMatrix();
	vtkTransform *translation = vtkTransform::New();
	translation->SetMatrix(m);

	// Transform the source points by the ICP solution
	vtkTransformPolyDataFilter *pTransformFilter = vtkTransformPolyDataFilter::New();
	pTransformFilter->SetInput(pSourceData);
	pTransformFilter->SetTransform(translation);
	pTransformFilter->Update();

	this->pEntity->pPointCloudDataSetCollection->ReplaceItem(numSource, pTransformFilter->GetOutput());

	icp->Delete();
	translation->Delete();
	pTransformFilter->Delete();

	DisplayMatchModels();
}

//����������
void CPointCloudMeasureDoc::ComputeMeasurement()
{
	int numSource = this->pEntity->current_point_cloud_part_num;
	int numTarget = this->pEntity->stl_for_match_to_point;

	vtkPolyData *pSourceData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numSource;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pSourceData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	vtkPolyData *pTargetData;
	this->pEntity->pStlDataSetCollection->InitTraversal();
	for(int i=0;i<=numTarget;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pTargetData = this->pEntity->pStlDataSetCollection->GetNextItem();
	}

	//��òο�ģ�͵����ͣ��Ӷ�ȷ��������ʽ
	bool bPointTwoCell = true;
	if(pTargetData->GetNumberOfPoints() == pTargetData->GetNumberOfCells())
	{
		 bPointTwoCell = false;
	}

	//����㵽�����̾��������
	vtkHausdorffDistancePointSetFilter *filter = vtkHausdorffDistancePointSetFilter::New();
	filter->SetInput(0, pSourceData);
	filter->SetInput(1, pTargetData);
	if(bPointTwoCell)
	{
		filter->SetTargetDistanceMethod( vtkHausdorffDistancePointSetFilter::POINT_TO_CELL );
	}
	else
	{
		filter->SetTargetDistanceMethod( vtkHausdorffDistancePointSetFilter::POINT_TO_POINT );
	}
	filter->Update();

	vtkDataArray *dis = filter->GetOutput(0)->GetPointData()->GetArray("Distance");
	vtkDataArray *closePoint = filter->GetOutput(0)->GetPointData()->GetArray("ClosePoint");
	vtkDataArray *displace = filter->GetOutput(0)->GetPointData()->GetArray("Displace");
	pSourceData->GetPointData()->AddArray(dis);
	pSourceData->GetPointData()->AddArray(closePoint);
	pSourceData->GetPointData()->AddArray(displace);
	pSourceData->GetPointData()->SetActiveScalars("Distance");

	filter->Delete();

	DataChangeToPointcloudData();
}

vtkDataArray* CPointCloudMeasureDoc::GetAttributeArrayFromMeasuredData()
{
	int numSource = this->pEntity->current_point_cloud_part_num;

	vtkPolyData *pSourceData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numSource;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pSourceData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	vtkDataArray * value = pSourceData->GetPointData()->GetArray("Distance");

	return value;
}

vtkDataArray* CPointCloudMeasureDoc::GetDisplaceArrayFromMeasuredData()
{
	int numSource = this->pEntity->current_point_cloud_part_num;

	vtkPolyData *pSourceData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numSource;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pSourceData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	vtkDataArray * value = pSourceData->GetPointData()->GetArray("Displace");

	return value;
}

void CPointCloudMeasureDoc::OnFileSave()
{
	// TODO: Add your command handler code here
	if(this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()!=0 || \
		this->pEntity->pStlDataSetCollection->GetNumberOfItems()!=0)
	{
		int num_point_cloud = this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();
		int num_stl = this->pEntity->pStlDataSetCollection->GetNumberOfItems();
		OutPutDlg dlg(num_point_cloud, num_stl, NULL);//��������Ի���
		if(dlg.DoModal()==IDOK)
		{
			int current_point_cloud = dlg.m_point_cloud_out;
			int current_stl = dlg.m_stl_out;
			if(current_point_cloud != -1 || current_stl!= -1)//�����ļ�
			{
				DataExport(current_point_cloud, current_stl);
			}
		}
	}
	this->UpdateAllViews(NULL);
}

void CPointCloudMeasureDoc::DataExport(int numPointCloud, int numStl)
{
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"files(*.*)|*.*||",NULL);//��������ļ��Ի�����������ļ���
	if(dlg.DoModal()==IDOK)
	{ 
		vtkPolyData *pCurrentData;

		CString fileFullPath = dlg.GetPathName();
		CString filePath = fileFullPath.Left(fileFullPath.ReverseFind('\\'));
		CString fileName = dlg.GetFileTitle();

		CString pointFileName = filePath + "\\" + fileName + ".sp";
		CString stlFileName = filePath + "\\" + fileName + ".stl";
		CString pngFileName = filePath + "\\" + fileName + ".png";
		CString atrFileName = filePath + "\\" + fileName + ".txt";

		if( 0 <= numPointCloud && numPointCloud < (this->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()))
		{
			this->pEntity->pPointCloudDataSetCollection->InitTraversal();
			for(int i=0;i<= numPointCloud;i++)
			{
				pCurrentData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
			}

			vtkScanPointWriter *pWriter = new vtkScanPointWriter();
			pWriter->SetInputPolyData(pCurrentData);
			pWriter->SetFileName((LPCTSTR)pointFileName);
			pWriter->WriteData();
			delete pWriter;

			//��������ļ�
			vtkDataArray *displace = pCurrentData->GetPointData()->GetArray("Displace");
			vtkDataArray *closePoint = pCurrentData->GetPointData()->GetArray("ClosePoint");
			vtkPoints *points = pCurrentData->GetPoints();


			if(displace && closePoint)
			{
				ofstream outFile;

				std::locale::global(std::locale(""));
				outFile.open((LPCTSTR) atrFileName, ios::out);
				std::locale::global(std::locale("C"));

				if(outFile.fail())
				{
					//cout << "creat file for out error!" << endl; to do display error message
					return;
				}
				int numPoint = displace->GetNumberOfTuples();

				for(int i = 0; i < numPoint; i++)
				{
					double *cDisplace = displace->GetTuple3(i);
					double *cClosePoint = closePoint->GetTuple3(i);
					double *cPoint = points->GetPoint(i);
					outFile << *cPoint << "         " << *(cPoint+1) << "         " << *(cPoint+2) << endl;
					outFile << *cClosePoint << "         " << *(cClosePoint+1) << "         " << *(cClosePoint+2) << endl;
					outFile << *cDisplace << "         " << *(cDisplace+1) << "         " << *(cDisplace+2) << endl;
				}

				outFile.close();
			}

		}

		if( 0 <= numStl && numStl < (this->pEntity->pStlDataSetCollection->GetNumberOfItems()))
		{
			vtkSTLWriter *stlWriter = vtkSTLWriter::New();
			//	 vtkCleanUnstructuredGrid *pCleanUnstructuredGrid=vtkCleanUnstructuredGrid::New();//��PLOYDATA����ģ��ת����UNSTRUCTUREDGRID����ģ��
			this->pEntity->pStlDataSetCollection->InitTraversal();
			for(int i=0;i<= numStl;i++)
			{
				pCurrentData = this->pEntity->pStlDataSetCollection->GetNextItem();
			}
			stlWriter->SetFileName((LPCTSTR)stlFileName);
			stlWriter->SetInput(pCurrentData);
			stlWriter->SetFileTypeToBinary();
			stlWriter->Write();
			stlWriter->Delete();
		}


		//���浱ǰͼƬ���
		POSITION pos = this->GetFirstViewPosition();
		CPointCloudMeasureView *pCPointCloudMeasureView = NULL;
		if (pos)
		{
			pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
		}
		else  // return
		{
			ASSERT(FALSE);
			return;
		}

		vtkWindowToImageFilter *windowToImageFilter = vtkWindowToImageFilter::New();
		windowToImageFilter->SetInput(pCPointCloudMeasureView->GetRenderer()->GetRenderWindow());
		windowToImageFilter->SetMagnification(2); //set the resolution of the output image (3 times the current resolution of vtk render window)
		windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();

		vtkPNGWriter *writer = vtkPNGWriter::New();
		writer->SetFileName((LPCTSTR) pngFileName);
		writer->SetInputConnection(windowToImageFilter->GetOutputPort());
		writer->Write();
	}
}

void CPointCloudMeasureDoc::AddCaptionWidget(CString str, double position[])
{
	//���浱ǰͼƬ���
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;
	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	vtkSmartPointer<vtkCaptionRepresentation> captionRepresentation = vtkSmartPointer<vtkCaptionRepresentation>::New();
	captionRepresentation->GetCaptionActor2D()->SetCaption((LPCTSTR)str);
	captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(100);
	captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetColor(1,0,0);
	captionRepresentation->SetAnchorPosition(position);
	captionRepresentation->SetPosition(0.05,0.5);
	captionRepresentation->SetPosition2(0.4,0.075);

	vtkSmartPointer<vtkCaptionWidget> captionWidget = vtkSmartPointer<vtkCaptionWidget>::New();
	captionWidget->SetInteractor(pCPointCloudMeasureView->GetMFCWindow()->GetInteractor());
	captionWidget->SetRepresentation(captionRepresentation);

	captionWidgets.push_back(captionWidget);

	this->UpdateAllViews(NULL);
}

//import cad model
void CPointCloudMeasureDoc::OnIgsImport()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,"igs",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"igs file of CAD model(*.igs)|*.igs||",NULL);//��������igs�ļ����˵��ļ�����ѡ��Ի���
	if(dlg.DoModal()==IDOK)
	{
		RemoveActors();//�Ƴ�VTKACTORS
		CString strName=dlg.GetPathName();//�õ��ļ�·��

		//read iges file
		vtkIGESReader reader;
        reader.SetFileName( (LPCTSTR)strName );
        reader.ReadModel();
  
        TopoDS_Shape shape = reader.GetTopologyShape();

		if(shape.IsNull())
		{
			return; 
		}
        
		IgesAndVtkModel* model = new IgesAndVtkModel();
		model->ConvetModel(shape, 0.1, this->pEntity->numTotalSurf);
		model->SetModelName((LPCTSTR)strName);
		model->DisplayModel(pSurfacesCADActorColletion);

		this->pEntity->cadModels.push_back(model);

		//for hightlight initialize
		int size = model->convert.surfaces.size();
		for(int i = 0; i < size; i++)
		{
			this->pEntity->highLightSurface.push_back(false);
		}

		this->pEntity->numTotalSurf += size;

			//vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();

			//vtkPolyData *poly = vtkPolyData::New();

			//(*iter)->GetPolyData(poly);

			//mapper->SetInput( poly );

			//vtkActor *actor = vtkActor::New();
			//actor->SetMapper (mapper);
			//actor->GetProperty()->SetColor(0,0,1);

			//this->pSurfacesCADActorColletion->AddItem(actor);
			//this->pEntity->highLightSurface.push_back(false);

			//string name;
   //         (*iter)->GetName(name);

			//this->pEntity->surfaceNames.push_back(name);

			//mapper->Delete();
			//actor->Delete();
			//poly->Delete();

			//con++;
		//}

		//add actor of the basic geometry for visulization
		//list<EdgeConvertion*>::iterator iter1;
		//
		////int num = this->pEntity->convert.edges.size();
		////int i = 0;
		//for(iter1 = this->pEntity->convert.edges.begin(); iter1 != this->pEntity->convert.edges.end(); iter1++)
		//{
		//	vtkPolyData *poly = vtkPolyData::New();
		//	//	vtkPolyData *poly;
		//	(*iter1)->GetPolyData(poly);

		//	if(poly != NULL)
		//	{
		//		vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
		//		mapper->SetInput( poly );

		//		vtkActor *actor = vtkActor::New();
		//		actor->SetMapper (mapper);
		//		actor->GetProperty()->SetColor(0,1,0);
		//		actor->GetProperty()->SetLineWidth(2);

		//		this->pEdgesCADActorColletion->AddItem(actor);
		//		this->pEntity->highLightEdge.push_back(false);

		//		string name;
		//		(*iter1)->GetName(name);
		//		this->pEntity->edgeNames.push_back(name);

		//		mapper->Delete();
		//		actor->Delete();
		//	}
		//	
		//	poly->Delete();

		//}

		this->ReorderActors();

		this->UpdateAllViews(NULL);//����������ͼ
	}
}

//set the highlight model according to the feature type
void CPointCloudMeasureDoc::ChangToActorHighlight(vtkActorCollection *pActorCollection, int num, bool highLight, int mode)
{
	vtkActor *pCurrentActor = NULL;
	pActorCollection->InitTraversal();
	for(int i=0;i<=num;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();
	}
	
	if(highLight)
	{
		pCurrentActor->GetProperty()->SetColor(1,0,0);
	}
	else
	{
		switch(mode)
		{
		case 0:
			pCurrentActor->GetProperty()->SetColor(0,0,1);  //for the surface
			break;
		case 1:
			pCurrentActor->GetProperty()->SetColor(0,1,0);  //for edge
			break;
		case 2:
			pCurrentActor->GetProperty()->SetColor(0.9,0.9,0.9);  //for the stl
			break;
		default:
			break;
		}
	}
}

TopoDS_Shape CPointCloudMeasureDoc::GetFeatureSurface(const int ind)
{
	IgesAndVtkModel* model = pEntity->cadModels[pEntity->current_cad_num-1];
	
	ASSERT(ind < model->convert.surfaces.size());

	list<SurfaceConvertion*>::iterator iter;
	iter = model->convert.surfaces.begin();
	for(int i=0;i<ind;i++)
		iter++;  

	return (*iter)->GetTopologyShape();
}

TopoDS_Shape CPointCloudMeasureDoc::GetFeatureEdge(const int ind)
{
	ASSERT(ind < pEntity->surfEdges->edges.size());

	list<EdgeConvertion*>::iterator iter;
	iter = pEntity->surfEdges->edges.begin();
	for(int i=0;i<ind;i++)
		iter++;  

	return (*iter)->GetTopologyShape();
}

//use cad feature to compute the distance
void CPointCloudMeasureDoc::ComputeMeasurementWithFeature(const int ind, const bool surfOrEdge)
{
	//��ȡ����ģ��
	BasicGeometryConvertion *geom;
	TopoDS_Shape shape;

	if(surfOrEdge)
	{
		shape = GetFeatureSurface(ind);
	}
	else
	{
		shape = GetFeatureEdge(ind);
	}

	//��ò���������
	int numSource = this->pEntity->current_point_cloud_part_num;

	vtkPolyData *pSourceData;
	this->pEntity->pPointCloudDataSetCollection->InitTraversal();
	for(int i=0;i<=numSource;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
	{
		pSourceData = this->pEntity->pPointCloudDataSetCollection->GetNextItem();
	}

	//����㵽��������߱ߵľ��룬���һ�����
	vtkPolyData *poly = vtkPolyData::New();

	if(surfOrEdge)
	{
		MeasureWithSurface surf(pSourceData, shape);
		surf.Compute();
		surf.GetOutput(poly);
	}
	else
	{
		MeasureWithEdge edge(pSourceData, shape);
		edge.Compute();
		edge.GetOutput(poly);
	}

	vtkDataArray *dis = poly->GetPointData()->GetArray("Distance");
	vtkDataArray *closePoint = poly->GetPointData()->GetArray("ClosePoint");
	vtkDataArray *displace = poly->GetPointData()->GetArray("Displace");
	pSourceData->GetPointData()->AddArray(dis);
	pSourceData->GetPointData()->AddArray(closePoint);
	pSourceData->GetPointData()->AddArray(displace);
	pSourceData->GetPointData()->SetActiveScalars("Distance");

	//for debug 
	/*double x[2];
	dis->GetRange(x);*/

	poly->Delete();
}

//visible all the actor
void CPointCloudMeasureDoc::ActorVisibleAll(vtkActorCollection *pActorCollection)
{
	vtkActor *pCurrentActor = NULL;
	int num = pActorCollection->GetNumberOfItems();
	pActorCollection->InitTraversal();
	for(int i=0;i< num;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();
		pCurrentActor->VisibilityOn();
	}
}

//only visible the current actor
void CPointCloudMeasureDoc::ActorVisibleOnly(vtkActorCollection *pActorCollection,int num)
{
	vtkActor *pCurrentActor = NULL;
	int N = pActorCollection->GetNumberOfItems();
	pActorCollection->InitTraversal();
	for(int i=0;i< N;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pActorCollection->GetNextActor();

		if(i == num)
		{
			pCurrentActor->VisibilityOn();
		}
		else
		{
			pCurrentActor->VisibilityOff();
		}
	}
}

void CPointCloudMeasureDoc::RANSACBasedShapeRecognition(vtkPolyData *pPolyData, float disThrehold, float projThrehold, float probThrehold, float normThrehold, float minPoint, bool usePlane, bool useCylinder, bool useCone, bool useSphere, bool useTorus)
{
	this->pEntity->recModels = new RansacModel();
	this->pEntity->recModels->SetInputData(pPolyData);
	this->pEntity->recModels->InitializeOrModification(true);
	this->pEntity->recModels->SetPara(disThrehold, projThrehold, probThrehold, normThrehold, minPoint, usePlane, useCylinder, useCone, useSphere, useTorus);
	this->pEntity->recModels->Updata();

	//set infomation scale ---- YiBing, 2014-12-15
	this->pEntity->recModels->SetNormalScale(pPolyData->GetLength() / 20);

	if(this->pPrimitiveActorColletion)
	{
		this->pPrimitiveActorColletion->RemoveAllItems();
		this->pEntity->recModels->DisplayModel(this->pPrimitiveActorColletion);

		//visulize the information ---- YiBing, 2014-12-15
		this->pEntity->recModels->DisplayHighLevelInfo(this->pPrimitiveParaActorColletion);

	}

	this->DisplayRecognizedModel();
	this->UpdateAllViews(NULL);
}

void CPointCloudMeasureDoc::DisplayRecognizedModel()
{
	this->ClearlAllOldActors();

	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(this->pPrimitiveActorColletion->GetNumberOfItems()>0)//��ʾpPrimitiveActorColletionģ������
	{
		this->pPrimitiveActorColletion->InitTraversal();
		for(int i=0;i<this->pPrimitiveActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pPrimitiveActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}
}

//display recognize model infomation ---- YiBing, 2014-12-15
void CPointCloudMeasureDoc::DisplayRecognizedModelInfo()
{
	//	this->ClearlAllOldActors();

	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(this->pPrimitiveParaActorColletion->GetNumberOfItems()>0)//��ʾpPrimitiveActorColletionģ������
	{
		this->pPrimitiveParaActorColletion->InitTraversal();
		for(int i=0;i<this->pPrimitiveParaActorColletion->GetNumberOfItems();i++)
		{
			this->ActorCollectionToDisplay(this->pPrimitiveParaActorColletion,pCPointCloudMeasureView->GetRenderer());
		}
	}
}

//display recognize model infomation of single feature ---- edony 2014/12/16
void CPointCloudMeasureDoc::DisplayRecognizedSingleInfo(int num)
{
	//	this->ClearlAllOldActors();

	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	vtkActor *pCurrentActor = NULL;
	int N = pPrimitiveParaActorColletion->GetNumberOfItems();
	pPrimitiveParaActorColletion->InitTraversal();
	for(int i=0;i< N;i++)//��Ӧ����ǰActor
	{
		pCurrentActor=pPrimitiveParaActorColletion->GetNextActor();

		if(i == num)
		{
			pCPointCloudMeasureView->GetRenderer()->AddActor(pCurrentActor);
			//pCurrentActor->VisibilityOn();
		}
		//else
		//{
		//	cout<<"No Item\n";
		//}
	}

}

//add time observer for motion
void CPointCloudMeasureDoc::AddMotionActionObserver(const int index)
{
	if(this->motCallback)
		this->motCallback->Delete();

	this->motCallback = vtkMotionTimerCallback::New();

	int start = this->pEntity->cadModels[index]->startSurfId;
	int end = this->pEntity->cadModels[index]->endSurfId;

	int id = 0;
	
	this->pSurfacesCADActorColletion->InitTraversal();
	for(int i = 0; i < this->pSurfacesCADActorColletion->GetNumberOfItems(); i++)
	{
		vtkActor *pCurrentActor = this->pSurfacesCADActorColletion->GetNextActor();
		if( i >= start && i <= end)
		{
			motCallback->actors.push_back(pCurrentActor);
			motCallback->index.push_back(id);
			id++;
		}
	}

	this->IntializeMotionActionObserver();
}

//initialize model motion observer
void CPointCloudMeasureDoc::IntializeMotionActionObserver()
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->AddObserver(vtkCommand::TimerEvent, this->motCallback);
	timeID = pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->CreateRepeatingTimer(100);
	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->Start();
}

//remove motion observer
void CPointCloudMeasureDoc::RemoveMotionActionObserver()
{
	POSITION pos = this->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->RemoveObserver(this->motCallback);
	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->DestroyTimer(timeID);
}
void CPointCloudMeasureDoc::OnMenuIgesAll()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE,"igs",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"igs file of CAD model(*.igs)|*.igs||",NULL);//��������igs�ļ����˵��ļ�����ѡ��Ի���
	if(dlg.DoModal()==IDOK)
	{
		RemoveActors();//�Ƴ�VTKACTORS
		CString strName=dlg.GetFolderPath();//�õ��ļ�·��

		PhysicalFileBrowseDir pDir;
		pDir.SetInitDir( (LPCTSTR)strName );
		pDir.BeginBrowse("*.*");

		int size = pDir.fileNameArray.size();

		for(int i = 0; i < size; i++)
		{
			//read iges file
			vtkIGESReader reader;
			reader.SetFileName( pDir.fileNameArray[i].c_str() );
			reader.ReadModel();

			TopoDS_Shape shape = reader.GetTopologyShape();

			if(shape.IsNull())
			{
				return; 
			}

			double r = vtkMath::Random(0,1);
			double g = vtkMath::Random(0,1);
			double b = vtkMath::Random(0,1);

			IgesAndVtkModel* model = new IgesAndVtkModel();
			model->ConvetModel(shape, 0.1, this->pEntity->numTotalSurf);
			model->SetModelName(pDir.fileNameArray[i].c_str());
			model->DisplayModel(pSurfacesCADActorColletion, r, g, b);

			this->pEntity->cadModels.push_back(model);

			//for hightlight initialize
			int size = model->convert.surfaces.size();
			for(int i = 0; i < size; i++)
			{
				this->pEntity->highLightSurface.push_back(false);
			}

			this->pEntity->numTotalSurf += size;
		}

		this->ReorderActors();

		this->UpdateAllViews(NULL);//����������ͼ
	}
}

void CPointCloudMeasureDoc::OnMuneExportRansac()
{
	// TODO: Add your command handler code here
	
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"recognized model files(*.ra)|*.ra||",NULL);//��������ļ��Ի�����������ļ���
	if(dlg.DoModal()==IDOK)
	{ 
		CString strName=dlg.GetPathName();//�õ��ļ�·��
		if(pEntity->recModels != NULL)
		{
			pEntity->recModels->OutPutFile((LPCTSTR)strName);
		}
	}
}
