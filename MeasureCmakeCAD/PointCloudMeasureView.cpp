
// PointCloudMeasureView.cpp : CPointCloudMeasureView 类的实现
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
	// 标准打印命令
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

// CPointCloudMeasureView 构造/析构

CPointCloudMeasureView::CPointCloudMeasureView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPointCloudMeasureView 绘制

void CPointCloudMeasureView::OnDraw(CDC* pDC)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (this->pvtkMFCWindow)
	{
		if (pDC->IsPrinting())
			this->pvtkMFCWindow->DrawDC(pDC);
	}

	pDoc->DisplayWidget();//视图重绘时，显示物理属性bar
}


// CPointCloudMeasureView 打印


void CPointCloudMeasureView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPointCloudMeasureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPointCloudMeasureView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPointCloudMeasureView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CPointCloudMeasureView 诊断

#ifdef _DEBUG
void CPointCloudMeasureView::AssertValid() const
{
	CView::AssertValid();
}

void CPointCloudMeasureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPointCloudMeasureDoc* CPointCloudMeasureView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPointCloudMeasureDoc)));
	return (CPointCloudMeasureDoc*)m_pDocument;
}
#endif //_DEBUG


// CPointCloudMeasureView 消息处理程序

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

    CreateWidget();;//在视图类创建完成之后就创建交互式坐标轴

	CreateAxes();//创建世界坐标系原点位置的坐标轴点
	// TODO: Add your specialized code here and/or call the base class
}

void CPointCloudMeasureView::CreateAxes() //创建世界坐标系原点位置的坐标轴点
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
	vtkLineSource *pLine=vtkLineSource::New();//创建线资源
    pLine->SetPoint1(0,0,0);
    pLine->SetPoint2(0,1,0);

    vtkConeSource *pCone=vtkConeSource::New();//创建圆锥资源
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


    vtkAssembly *pAssem=vtkAssembly::New();//建立坐标系
    pAssem->AddPart(pXAxis);
    pAssem->AddPart(pXAxiStip);
    pAssem->AddPart(pYAxis);
    pAssem->AddPart(pYAxiStip);
    pAssem->AddPart(pZAxis);
    pAssem->AddPart(pZAxiStip);
    
	this->pWidget->SetOutlineColor( 0.2, 0.2, 0.2 );//建立交互式坐标系
 
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
    cam->SetViewUp(0,1,0);//设置相机的上方向
	cam->Azimuth(180);//旋转相机180度
    this->pvtkRenderer->SetActiveCamera(cam);//设置当前相机
    pvtkRenderer->ResetCamera();

	cam->Delete();
    this->Invalidate(TRUE);//重绘
}

void CPointCloudMeasureView::OnViewBottom()
{
	// TODO: Add your command handler code here
	 vtkCamera *cam=vtkCamera::New();
	
	cam->Elevation(-90);
	cam->SetViewUp(-0,0,1);//设置显示物体下视图相机的位置与上方向
    this->pvtkRenderer->SetActiveCamera(cam);
    pvtkRenderer->ResetCamera();
    
	cam->Delete();
    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewFront()
{
	// TODO: Add your command handler code here
	vtkCamera *cam=vtkCamera::New();
	cam->SetViewUp(0,1,0);//设置显示物体前视图相机的上方向
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
	cam->Azimuth(-90);//设置显示物体左视图相机的位置与上方向
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
	cam->Azimuth(90);//设置显示物体右视图相机的位置与上方向
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
	cam->SetViewUp(-0,0,-1);//设置显示物体上视图相机的位置与上方向
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

	cam1->Zoom(0.8);//按设置的比例缩小物体

	pvtkRenderer->SetActiveCamera(cam1);

    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewZoomin()
{
	// TODO: Add your command handler code here
	vtkCamera *cam1;

	cam1=pvtkRenderer->GetActiveCamera();

	cam1->Zoom(1.2);//按设置的比例放大物体

	pvtkRenderer->SetActiveCamera(cam1);

	this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewZoomall()
{
	// TODO: Add your command handler code here
	pvtkRenderer->ResetCamera();//重新设置相机的参数，使屏幕中的所有物体都能显示

    this->Invalidate(TRUE);
}

void CPointCloudMeasureView::OnViewShade()
{
	// TODO: Add your command handler code here
	
	vtkActorCollection *pActorCollection=pvtkRenderer->GetActors();//获取屏幕上显示的所有Actor
    pActorCollection->InitTraversal();//链表指针指向初始位置
    vtkActor *pvtkActorNew;
    int set;
    while (pvtkActorNew=pActorCollection->GetNextActor())//建立循环，一个一个获取Actor
    {
	   set=pvtkActorNew->GetProperty()->GetRepresentation();//获取物体的属性信息，如果着色则返回set=2.如果时线框显示，则返回set=1
       switch(set)
	   {
	   case 0:
			break;
	   case 1:
		    pvtkActorNew->GetProperty()->SetRepresentationToSurface();//设置物体属性为面显示
			break;
	   case 2:
            pvtkActorNew->GetProperty()->SetRepresentationToWireframe();//设置物体属性为线框显示
			break;
	   default:
		    break;
	   }
	}//循环判断，改变所有Actor的着色与消隐，如果线框显示，则改变为面着色显示，反之亦然
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

	vtkCamera *pCam;// 获取当前相机的焦点，上方向，相机的位置等参数
	pCam= pvtkRenderer->GetActiveCamera();
	m_viewAngle=pCam->GetViewAngle();
	m_dis=pCam->GetDistance();
	pCam->GetWindowCenter(center);
	pCam->GetFocalPoint(focal);
	pCam->GetPosition(pos);
	pCam->GetViewUp(viewup);
	pCam->GetViewPlaneNormal(nor);//获取面法线参数

	pvtkRenderer->GetBackground(rgbBag);
	
	//将获取的变量数值赋值给对话框中的变量，使对话框显示上一次设置的值 
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

	 if(m_Mydlg.DoModal()==IDOK)//当按下对话框的OK按钮，将用户设置的值赋值给设置的变量
	 {
		 m_zoomIn=m_Mydlg.m_zoomIn;//传递放大系数
		 m_zoomOut=m_Mydlg.m_zoomOut;//传递缩小系数
		 
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

		pvtkRenderer->SetBackground(m_rBag,m_gBag,m_bBag);//设置背景的颜色


        if(m_azimuthAngle!=0 || m_elevation!=0)//由于旋转相机与改变相机的位置不能同时进行，对角度变量判断
		{
		 pCam->Azimuth(m_azimuthAngle);//按照某一角度旋转相机
		 pCam->Elevation(m_elevation);
		 
		}
		else
		 {
         pCam->SetPosition(pos[0],pos[1],pos[2]);//设置相机的位置
		 }

		
		pCam->SetViewAngle(m_viewAngle);//设置视角
		pCam->SetViewUp(viewup[0],viewup[1],viewup[2]);//设置上方向
		pCam->SetFocalPoint(focal[0],focal[1],focal[2]);//设置焦点位置
		pCam->SetWindowCenter(center[0],center[1]);//设置屏幕中心

		pCam->GetViewPlaneNormal(nor);//获取面法线

		if ( fabs(vtkMath::Dot(viewup,nor)) > 0.999 )//判断上方向与面法线的方向，平行则改变上方向的值
        {
         pCam->SetViewUp(-viewup[2], viewup[0], viewup[1]);
        }

		 pvtkRenderer->SetActiveCamera(pCam);//设置相机

         this->Invalidate(TRUE);
	 }
}
