
// PointCloudMeasureView.cpp : CPointCloudMeasureView ���ʵ��
//

#include "stdafx.h"
#include "PointCloudMeasure.h"

#include "PointCloudMeasureView.h"
#include "MyCameraDlg.h"
#include <vtkCallbackCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxes.h>
#include <vtkAssembly.h>
#include <vtkLineSource.h>
#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkProperty.h> 
#include <vtkMath.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActorCollection.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPointCloudMeasureView

IMPLEMENT_DYNCREATE(CPointCloudMeasureView, CView)

BEGIN_MESSAGE_MAP(CPointCloudMeasureView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPointCloudMeasureView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_BACK, &CPointCloudMeasureView::OnViewBack)
	ON_COMMAND(ID_VIEW_BOTTOM, &CPointCloudMeasureView::OnViewBottom)
	ON_COMMAND(ID_VIEW_FRONT, &CPointCloudMeasureView::OnViewFront)
	ON_COMMAND(ID_VIEW_LEFT, &CPointCloudMeasureView::OnViewLeft)
	ON_COMMAND(ID_VIEW_RIGHT, &CPointCloudMeasureView::OnViewRight)
	ON_COMMAND(ID_VIEW_TOP, &CPointCloudMeasureView::OnViewTop)
	ON_COMMAND(ID_VIEW_NE_ISOMETRIC, &CPointCloudMeasureView::OnViewNeIsometric)
	ON_COMMAND(ID_VIEW_NW_ISOMETRIC, &CPointCloudMeasureView::OnViewNwIsometric)
	ON_COMMAND(ID_VIEW_SE_ISOMETRIC, &CPointCloudMeasureView::OnViewSeIsometric)
	ON_COMMAND(ID_VIEW_SW_ISOMETRIC, &CPointCloudMeasureView::OnViewSwIsometric)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CPointCloudMeasureView::OnViewZoomout)
	ON_COMMAND(ID_VIEW_ZOOMIN, &CPointCloudMeasureView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMALL, &CPointCloudMeasureView::OnViewZoomall)
	ON_COMMAND(ID_VIEW_SHADE, &CPointCloudMeasureView::OnViewShade)
	ON_COMMAND(ID_CAMERA_OPERATION, &CPointCloudMeasureView::OnCameraOperation)
END_MESSAGE_MAP()

// CPointCloudMeasureView ����/����

CPointCloudMeasureView::CPointCloudMeasureView()
{
	// TODO: �ڴ˴���ӹ������
	// TODO: add construction code here
	this->pvtkMFCWindow     = NULL;

    // Create the the renderer, window and interactor objects.
    this->pvtkRenderer    = vtkRenderer::New();

	this->pAxesActor=vtkActor::New();
	this->pWidget=vtkOrientationMarkerWidget::New();

	m_zoomIn=1.2;
    m_zoomOut=0.8;
	m_azimuthAngle=0;
	m_elevation=0;

}

CPointCloudMeasureView::~CPointCloudMeasureView()
{
	if (this->pvtkMFCWindow) delete this->pvtkMFCWindow;

	if(this->pAxesActor)
	{
		this->pAxesActor->Delete();
	}
	if(this->pWidget)
	{
		this->pWidget->Delete();
	}
}

BOOL CPointCloudMeasureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPointCloudMeasureView ����

void CPointCloudMeasureView::OnDraw(CDC* pDC)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (this->pvtkMFCWindow)
	{
		if (pDC->IsPrinting())
			this->pvtkMFCWindow->DrawDC(pDC);
	}

	pDoc->DisplayWidget();//��ͼ�ػ�ʱ����ʾ��������bar
}


// CPointCloudMeasureView ��ӡ


void CPointCloudMeasureView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPointCloudMeasureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPointCloudMeasureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPointCloudMeasureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPointCloudMeasureView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPointCloudMeasureView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CPointCloudMeasureView ���

#ifdef _DEBUG
void CPointCloudMeasureView::AssertValid() const
{
	CView::AssertValid();
}

void CPointCloudMeasureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPointCloudMeasureDoc* CPointCloudMeasureView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPointCloudMeasureDoc)));
	return (CPointCloudMeasureDoc*)m_pDocument;
}
#endif //_DEBUG


// CPointCloudMeasureView ��Ϣ�������

void CPointCloudMeasureView::OnDestroy()
{
	CView::OnDestroy();
	
    // Delete the the renderer, window and interactor objects.
	if (this->pvtkRenderer)      this->pvtkRenderer->Delete();

	// TODO: Add your message handler code here
}

void CPointCloudMeasureView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (this->pvtkMFCWindow)
    this->pvtkMFCWindow->MoveWindow(0, 0, cx, cy);
	// TODO: Add your message handler code here
}

void CPointCloudMeasureView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	if (this->pvtkMFCWindow) delete this->pvtkMFCWindow;
   
	this->pvtkMFCWindow = new vtkMFCWindow(this);

    this->pvtkMFCWindow->GetRenderWindow()->AddRenderer(this->pvtkRenderer);

	vtkInteractorStyleTrackballCamera *style =  vtkInteractorStyleTrackballCamera::New();

	this->pvtkMFCWindow->GetInteractor()->SetInteractorStyle(style);

	style->Delete();

    CreateWidget();;//����ͼ�ഴ�����֮��ʹ�������ʽ������

	CreateAxes();//������������ϵԭ��λ�õ��������
	// TODO: Add your specialized code here and/or call the base class
}

void CPointCloudMeasureView::CreateAxes() //������������ϵԭ��λ�õ��������
{
	vtkAxes *pAxes=vtkAxes::New();
    pAxes->SetScaleFactor(0.15);
    pAxes->SetComputeNormals(0.4);


    vtkPolyDataMapper *pAxesMapper=vtkPolyDataMapper::New();
    pAxesMapper->SetInputConnection(pAxes->GetOutputPort());

	this->pAxesActor->SetMapper(pAxesMapper);
	this->pAxesActor->PickableOff();
	this->pAxesActor->DebugOff();

	pvtkRenderer->AddActor(this->pAxesActor);
	
	pAxes->Delete();
	pAxesMapper->Delete();
}

void CPointCloudMeasureView::CreateWidget()
{
	vtkLineSource *pLine=vtkLineSource::New();//��������Դ
    pLine->SetPoint1(0,0,0);
    pLine->SetPoint2(0,1,0);

    vtkConeSource *pCone=vtkConeSource::New();//����Բ׶��Դ
    pCone->SetDirection(0,1,0);
    pCone->SetHeight(0.25);
    pCone->SetRadius(0.05);
    pCone->SetResolution(16);

    vtkPolyDataMapper *pLineMapper=vtkPolyDataMapper::New();
    pLineMapper->SetInputConnection(pLine->GetOutputPort());

    vtkPolyDataMapper *pConeMapper=vtkPolyDataMapper::New();
    pConeMapper->SetInputConnection(pCone->GetOutputPort());

    vtkTransform *pXtrans=vtkTransform::New();
    pXtrans->RotateZ(-90);

    vtkTransform *pXtransTip=vtkTransform::New();
    pXtransTip->RotateZ(-90);
    pXtransTip->Translate(0,1,0);


    vtkTransform *pYtrans=vtkTransform::New();
    pYtrans->Translate(0,1,0);

    vtkTransform *pZtrans=vtkTransform::New();
    pZtrans->RotateX(90);

    vtkTransform *pZtransTip=vtkTransform::New();
    pZtransTip->RotateX(90);
    pZtransTip->Translate(0,1,0);



    vtkActor *pXAxis=vtkActor::New();
    pXAxis->SetMapper(pLineMapper);
    pXAxis->GetProperty()->SetColor(1,0,0);
    pXAxis->SetUserTransform(pXtrans);
    vtkActor *pYAxis=vtkActor::New();
    pYAxis->SetMapper(pLineMapper);
    pYAxis->GetProperty()->SetColor(1,1,0);
    vtkActor *pZAxis=vtkActor::New();
    pZAxis->SetMapper(pLineMapper);
    pZAxis->GetProperty()->SetColor(0,1,0);
    pZAxis->SetUserTransform(pZtrans);

    vtkActor *pXAxiStip=vtkActor::New();
    pXAxiStip->SetMapper(pConeMapper);
    pXAxiStip->GetProperty()->SetColor(1,0,0);
    pXAxiStip->SetUserTransform(pXtransTip);
    vtkActor *pYAxiStip=vtkActor::New();
    pYAxiStip->SetMapper(pConeMapper);
    pYAxiStip->GetProperty()->SetColor(1,1,0);
    pYAxiStip->SetUserTransform(pYtrans);
    vtkActor *pZAxiStip=vtkActor::New();
    pZAxiStip->SetMapper(pConeMapper);
    pZAxiStip->GetProperty()->SetColor(0,1,0);
    pZAxiStip->SetUserTransform(pZtransTip);


    vtkAssembly *pAssem=vtkAssembly::New();//��������ϵ
    pAssem->AddPart(pXAxis);
    pAssem->AddPart(pXAxiStip);
    pAssem->AddPart(pYAxis);
    pAssem->AddPart(pYAxiStip);
    pAssem->AddPart(pZAxis);
    pAssem->AddPart(pZAxiStip);
    
	this->pWidget->SetOutlineColor( 0.2, 0.2, 0.2 );//��������ʽ����ϵ
 
    this->pWidget->SetOrientationMarker(pAssem);
    this->pWidget->SetInteractor(pvtkMFCWindow->GetInteractor() );
    this->pWidget->SetViewport( 0.0, 0.0, 0.2, 0.2 );
    this->pWidget->SetEnabled( 1 );
    this->pWidget->InteractiveOn();

	pLine->Delete();
	pCone->Delete();
	pLineMapper->Delete();
	pConeMapper->Delete();
	pXtrans->Delete();
	pXtransTip->Delete();
	pYtrans->Delete();
	pZtrans->Delete();
	pZtransTip->Delete();
	pXAxis->Delete();
	pYAxis->Delete();
	pZAxis->Delete();
	pXAxiStip->Delete();
	pYAxiStip->Delete();
	pZAxiStip->Delete();
	pAssem->Delete();
}

int CPointCloudMeasureView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CPointCloudMeasureView::OnViewBack()
{
	// TODO: Add your command handler code here
	vtkCamera *cam=vtkCamera::New();
    cam->SetViewUp(0,1,0);//����������Ϸ���
	cam->Azimuth(180);//��ת���180��
    this->pvtkRenderer->SetActiveCamera(cam);//���õ�ǰ���
    pvtkRenderer->ResetCamera();

	cam->Delete();
    this->Invalidate(TRUE);//�ػ�
}

void CPointCloudMeasureView::OnViewBottom()
{
	// TODO: Add your command handler code here
	 vtkCamera *cam=vtkCamera::New();
	
	cam->Elevation(-90);
	cam->SetViewUp(-0,0,1);//������ʾ��������ͼ�����λ�����Ϸ���
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewFront()
{
	// TODO: Add your command handler code here
	vtkCamera *cam=vtkCamera::New();
	cam->SetViewUp(0,1,0);//������ʾ����ǰ��ͼ������Ϸ���
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewLeft()
{
	// TODO: Add your command handler code here
	vtkCamera *cam=vtkCamera::New();
	cam->SetViewUp(0,1,0);
	cam->Azimuth(-90);//������ʾ��������ͼ�����λ�����Ϸ���
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewRight()
{
	// TODO: Add your command handler code here
	vtkCamera *cam=vtkCamera::New();
	cam->SetViewUp(0,1,0);
	cam->Azimuth(90);//������ʾ��������ͼ�����λ�����Ϸ���
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewTop()
{
	// TODO: Add your command handler code here
	 vtkCamera *cam=vtkCamera::New();
	
	cam->Elevation(90);
	cam->SetViewUp(-0,0,-1);//������ʾ��������ͼ�����λ�����Ϸ���
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewNeIsometric()
{
	// TODO: Add your command handler code here
}

void CPointCloudMeasureView::OnViewNwIsometric()
{
	// TODO: Add your command handler code here
}

void CPointCloudMeasureView::OnViewSeIsometric()
{
	// TODO: Add your command handler code here
}

void CPointCloudMeasureView::OnViewSwIsometric()
{
	// TODO: Add your command handler code here
}

void CPointCloudMeasureView::OnViewZoomout()
{
	// TODO: Add your command handler code here
	vtkCamera *cam1;

	cam1=pvtkRenderer->GetActiveCamera();

	cam1->Zoom(0.8);//�����õı�����С����

	pvtkRenderer->SetActiveCamera(cam1);

    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewZoomin()
{
	// TODO: Add your command handler code here
	vtkCamera *cam1;

	cam1=pvtkRenderer->GetActiveCamera();

	cam1->Zoom(1.2);//�����õı����Ŵ�����

	pvtkRenderer->SetActiveCamera(cam1);

	this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewZoomall()
{
	// TODO: Add your command handler code here
	pvtkRenderer->ResetCamera();//������������Ĳ�����ʹ��Ļ�е��������嶼����ʾ

    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewShade()
{
	// TODO: Add your command handler code here
	
	vtkActorCollection *pActorCollection=pvtkRenderer->GetActors();//��ȡ��Ļ����ʾ������Actor
    pActorCollection->InitTraversal();//����ָ��ָ���ʼλ��
    vtkActor *pvtkActorNew;
    int set;
    while (pvtkActorNew=pActorCollection->GetNextActor())//����ѭ����һ��һ����ȡActor
    {
	   set=pvtkActorNew->GetProperty()->GetRepresentation();//��ȡ�����������Ϣ�������ɫ�򷵻�set=2.���ʱ�߿���ʾ���򷵻�set=1
       switch(set)
	   {
	   case 0:
			break;
	   case 1:
		    pvtkActorNew->GetProperty()->SetRepresentationToSurface();//������������Ϊ����ʾ
			break;
	   case 2:
            pvtkActorNew->GetProperty()->SetRepresentationToWireframe();//������������Ϊ�߿���ʾ
			break;
	   default:
		    break;
	   }
	}//ѭ���жϣ��ı�����Actor����ɫ������������߿���ʾ����ı�Ϊ����ɫ��ʾ����֮��Ȼ
	this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnCameraOperation()
{
	// TODO: Add your command handler code here
	double center[2];
	double focal[3];
	double pos[3];
	double viewup[3];
	double nor[3];
	double rgbBag[3];
	
	CMyCameraDlg m_Mydlg;

	vtkCamera *pCam;// ��ȡ��ǰ����Ľ��㣬�Ϸ��������λ�õȲ���
	pCam= pvtkRenderer->GetActiveCamera();
	m_viewAngle=pCam->GetViewAngle();
	m_dis=pCam->GetDistance();
	pCam->GetWindowCenter(center);
	pCam->GetFocalPoint(focal);
	pCam->GetPosition(pos);
	pCam->GetViewUp(viewup);
	pCam->GetViewPlaneNormal(nor);//��ȡ�淨�߲���

	pvtkRenderer->GetBackground(rgbBag);
	
	//����ȡ�ı�����ֵ��ֵ���Ի����еı�����ʹ�Ի�����ʾ��һ�����õ�ֵ 
	 m_Mydlg.m_viewangle=m_viewAngle;
     m_Mydlg.m_azimuthangle=m_azimuthAngle;
     m_Mydlg.m_elevation=m_elevation;
     m_Mydlg.m_dis=m_dis;

	 m_Mydlg.m_xcenter=center[0];
	 m_Mydlg.m_ycenter=center[1];
	
	 m_Mydlg.m_xpoisition=pos[0];
	 m_Mydlg.m_ypoisition=pos[1];
	 m_Mydlg.m_zpoisition=pos[2];

	 m_Mydlg.m_VX=viewup[0];
	 m_Mydlg.m_VY=viewup[1];	
	 m_Mydlg.m_VZ=viewup[2];
     
	 m_Mydlg.m_rBag=rgbBag[0];
	 m_Mydlg.m_gBag=rgbBag[1];
	 m_Mydlg.m_bBag=rgbBag[2];

	 m_Mydlg.m_xfocal=focal[0];
	 m_Mydlg.m_yfocal=focal[1];
	 m_Mydlg.m_zfocal=focal[2];

	 m_Mydlg.m_xnormal=nor[0];
	 m_Mydlg.m_ynormal=nor[1];
	 m_Mydlg.m_znormal=nor[2];

	 m_Mydlg.m_zoomIn=m_zoomIn;
	 m_Mydlg.m_zoomOut=m_zoomOut;

	 if(m_Mydlg.DoModal()==IDOK)//�����¶Ի����OK��ť�����û����õ�ֵ��ֵ�����õı���
	 {
		 m_zoomIn=m_Mydlg.m_zoomIn;//���ݷŴ�ϵ��
		 m_zoomOut=m_Mydlg.m_zoomOut;//������Сϵ��
		 
		 m_viewAngle=m_Mydlg.m_viewangle;

		 m_dis=m_Mydlg.m_dis;

		 center[0]=m_Mydlg.m_xcenter;
		 center[1]=m_Mydlg.m_ycenter;

		 pos[0]=m_Mydlg.m_xpoisition;
		 pos[1]=m_Mydlg.m_ypoisition;
		 pos[2]=m_Mydlg.m_zpoisition;

		 viewup[0]=m_Mydlg.m_VX;
		 viewup[1]=m_Mydlg.m_VY;	
		 viewup[2]=m_Mydlg.m_VZ;

		 focal[0]=m_Mydlg.m_xfocal;
		 focal[1]=m_Mydlg.m_yfocal;
		 focal[2]=m_Mydlg.m_zfocal;

		 nor[0]=m_Mydlg.m_xnormal;
		 nor[1]=m_Mydlg.m_ynormal;
		 nor[2]=m_Mydlg.m_znormal;

		 m_rBag=m_Mydlg.m_rBag;
		 m_gBag=m_Mydlg.m_gBag;
		 m_bBag=m_Mydlg.m_bBag;

		 m_azimuthAngle=m_Mydlg.m_azimuthangle;
		 m_elevation=m_Mydlg.m_elevation;

		pvtkRenderer->SetBackground(m_rBag,m_gBag,m_bBag);//���ñ�������ɫ


        if(m_azimuthAngle!=0 || m_elevation!=0)//������ת�����ı������λ�ò���ͬʱ���У��ԽǶȱ����ж�
		{
		 pCam->Azimuth(m_azimuthAngle);//����ĳһ�Ƕ���ת���
		 pCam->Elevation(m_elevation);
		 
		}
		else
		 {
         pCam->SetPosition(pos[0],pos[1],pos[2]);//���������λ��
		 }

		
		pCam->SetViewAngle(m_viewAngle);//�����ӽ�
		pCam->SetViewUp(viewup[0],viewup[1],viewup[2]);//�����Ϸ���
		pCam->SetFocalPoint(focal[0],focal[1],focal[2]);//���ý���λ��
		pCam->SetWindowCenter(center[0],center[1]);//������Ļ����

		pCam->GetViewPlaneNormal(nor);//��ȡ�淨��

		if ( fabs(vtkMath::Dot(viewup,nor)) > 0.999 )//�ж��Ϸ������淨�ߵķ���ƽ����ı��Ϸ����ֵ
        {
         pCam->SetViewUp(-viewup[2], viewup[0], viewup[1]);
        }

		 pvtkRenderer->SetActiveCamera(pCam);//�������

         this->Invalidate(TRUE);
	 }
}
