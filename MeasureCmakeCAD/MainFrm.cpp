
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "PointCloudMeasure.h"
#include "PartTreeView.h"
#include "MainFrm.h"
#include "Entity.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkRenderWindowInteractor.h>
#include "PointCloudMeasureView.h"
#include "vtkInteractorStyleTrackballActorMatch.h"
#include "vtkInteractorStyleTrackballActorQuery.h"
#include "vtkScanPointReader.h"
#include "MeasurementDialog.h"
#include "XPComputeDlg.h"
#include "Xpcompute.h"
#include "XPMotionDlg.h"
#include "RansacPara.h"
#include "ToleranceDlg.h"
#include "cellpicking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_MODE_INTERACT, &CMainFrame::OnUpdateModeInteract)
	ON_UPDATE_COMMAND_UI(ID_MODE_MATCH, &CMainFrame::OnUpdateModeMatch)
	ON_UPDATE_COMMAND_UI(ID_MODE_QUERY, &CMainFrame::OnUpdateModeQuery)
	ON_COMMAND(ID_MODE_INTERACT, &CMainFrame::OnModeInteract)
	ON_COMMAND(ID_MENU_INFORMATION, &CMainFrame::OnMenuInformation)
	ON_COMMAND(ID_MODE_QUERY, &CMainFrame::OnModeQuery)
	ON_WM_INITMENU()
	ON_COMMAND(ID_MODE_MATCH, &CMainFrame::OnModeMatch)
	ON_COMMAND(ID_MATCH_ICP, &CMainFrame::OnMatchIcp)
	ON_COMMAND(ID_MATCH_INTERACT, &CMainFrame::OnMatchInteract)
	ON_COMMAND(ID_MATCH_FEATURE_POINT, &CMainFrame::OnMatchFeaturePoint)
	ON_COMMAND(ID_MATCH_INTERACT_START, &CMainFrame::OnMatchInteractStart)
	ON_COMMAND(ID_MATCH_INTERACT_END, &CMainFrame::OnMatchInteractEnd)
	ON_COMMAND(ID_MATCH_INTERACT_CHANGE, &CMainFrame::OnMatchInteractChange)
	ON_COMMAND(ID_MEASURE_COMPUTE, &CMainFrame::OnMeasureCompute)
	ON_COMMAND(ID_MEASURE_QUERY, &CMainFrame::OnMeasureQuery)
	ON_COMMAND(ID_XIUPEI_COMPUTE, &CMainFrame::OnXiuPeiCompute)
	ON_COMMAND(ID_ZHUANGPEI_MOTION, &CMainFrame::OnZhuangPeiMotion)
	ON_COMMAND(ID_RANSAC_COMPUTE, &CMainFrame::OnRansacCompute)
	ON_COMMAND(ID_TOLERANCE_COMPUTE, &CMainFrame::OnToleranceCompute)
	ON_COMMAND(ID_END_MOTION_SIM, &CMainFrame::OnEndMotionSim)
	ON_COMMAND(ID_32857, &CMainFrame::OnBase)
	ON_COMMAND(ID_ON_PointCloud, &CMainFrame::OnOnPointcloud)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	this->m_bAutoMenuEnable = FALSE;
	 mInteractModeActor = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//创建主工具条
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//创建状态工具条
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable

	//创建视图处理工具条
	if (!m_wndDisplayBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDisplayBar.LoadToolBar(IDR_TOOLBAR_DISPLAY))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//创建多场分析结果导入工具条
	//if (!m_wndImportBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndImportBar.LoadToolBar(IDR_TOOLBAR_IMPORT))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	//设置各工具条动态停靠性
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.SetWindowTextA("Standard Tool");
	m_wndDisplayBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDisplayBar.SetWindowTextA("Display Tool");
	/*m_wndImportBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndImportBar.SetWindowTextA("Import Tool");*/
	EnableDocking(CBRS_ALIGN_ANY);


	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndDisplayBar,AFX_IDW_DOCKBAR_RIGHT);


	//  DockControlBarLeftOf(&m_wndImportBar,&m_wndToolBar);//将多场分析结果导入工具条停靠在主工具条右边


	//-------------------------------------------------
	//创建模型树窗口和输出信息窗口
	m_leftDockBar.SetDockSize(CSize(200,200));
	m_leftDockBar.SetFloatSize(CSize(200,200));
	if(!m_leftDockBar.Create(this,"Work Bar"))
	{
		TRACE0("Failed to create leftDockBar\n");
		return -1;
	}
	m_leftDockBar.m_TabCtrl.AddView("Model Tree",RUNTIME_CLASS(CPartTreeView));
	m_leftDockBar.m_TabCtrl.SetActiveView(0);

	m_OutputDockBar.SetDockSize(CSize(200,100));
	if(!m_OutputDockBar.Create(this,"Output Bar"))
	{
		return -1;
	}

	m_leftDockBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	DockControlBar(&m_leftDockBar,AFX_IDW_DOCKBAR_LEFT);
	m_OutputDockBar.EnableDocking(CBRS_ALIGN_TOP|CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_OutputDockBar,AFX_IDW_DOCKBAR_BOTTOM);

	//才初始化menu设置 
	CMenu* mmenu = GetMenu();
	//mode 
	CMenu* submenu = mmenu->GetSubMenu(4);
	submenu->EnableMenuItem(ID_MODE_INTERACT, MF_BYCOMMAND | MF_GRAYED);
	submenu->EnableMenuItem(ID_MODE_MATCH, MF_BYCOMMAND | MF_ENABLED);
	submenu->EnableMenuItem(ID_MODE_QUERY, MF_BYCOMMAND | MF_GRAYED);

	//match
	CMenu* matchmenu = mmenu->GetSubMenu(5);
	matchmenu->EnableMenuItem(ID_MATCH_INTERACT, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_START, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_END, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_FEATURE_POINT, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_ICP, MF_BYCOMMAND | MF_GRAYED);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序


void CMainFrame::OnUpdateModeInteract(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateModeMatch(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateModeQuery(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

CPointCloudMeasureDoc* CMainFrame::GetDocument()
{
	ASSERT(this->GetActiveDocument()->IsKindOf(RUNTIME_CLASS(CPointCloudMeasureDoc)));
	return (CPointCloudMeasureDoc*)this->GetActiveDocument();
}

void CMainFrame::OnModeInteract()
{
	// TODO: Add your command handler code here
	CMenu* mmenu = GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(4);

	submenu->EnableMenuItem(ID_MODE_INTERACT, MF_BYCOMMAND | MF_GRAYED);
	submenu->EnableMenuItem(ID_MODE_MATCH, MF_BYCOMMAND | MF_ENABLED);
	submenu->EnableMenuItem(ID_MODE_QUERY, MF_BYCOMMAND | MF_GRAYED);

	//match
	CMenu* matchmenu = mmenu->GetSubMenu(5);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_START, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_CHANGE, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_END, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_FEATURE_POINT, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_ICP, MF_BYCOMMAND | MF_GRAYED);

	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->pEntity->setInteractiveStyleMode(Entity::Interact);
}

void CMainFrame::OnMenuInformation()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnModeQuery()
{
	// TODO: Add your command handler code here
	CMenu* mmenu = GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(4);
	submenu->EnableMenuItem(ID_MODE_QUERY, MF_BYCOMMAND | MF_ENABLED);
	submenu->EnableMenuItem(ID_MODE_MATCH, MF_BYCOMMAND | MF_GRAYED);
	submenu->EnableMenuItem(ID_MODE_INTERACT, MF_BYCOMMAND | MF_ENABLED);

	//match
	CMenu* matchmenu = mmenu->GetSubMenu(5);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_START, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_CHANGE, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_END, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_FEATURE_POINT, MF_BYCOMMAND | MF_GRAYED);
	matchmenu->EnableMenuItem(ID_MATCH_ICP, MF_BYCOMMAND | MF_GRAYED);

	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->pEntity->setInteractiveStyleMode( Entity::Query);
}

void CMainFrame::OnInitMenu(CMenu* pMenu)
{
	CFrameWnd::OnInitMenu(pMenu);

	// TODO: Add your message handler code here
}

void CMainFrame::OnModeMatch()
{
	// TODO: Add your command handler code here
	CMenu* mmenu = GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(4);
	submenu->EnableMenuItem(ID_MODE_INTERACT, MF_BYCOMMAND | MF_GRAYED);
	submenu->EnableMenuItem(ID_MODE_MATCH, MF_BYCOMMAND | MF_GRAYED);
	submenu->EnableMenuItem(ID_MODE_QUERY, MF_BYCOMMAND | MF_ENABLED);

	//match
	CMenu* matchmenu = mmenu->GetSubMenu(5);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_START, MF_BYCOMMAND | MF_ENABLED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_CHANGE, MF_BYCOMMAND | MF_ENABLED);
	matchmenu->GetSubMenu(0)->EnableMenuItem(ID_MATCH_INTERACT_END, MF_BYCOMMAND | MF_ENABLED);
	matchmenu->EnableMenuItem(ID_MATCH_FEATURE_POINT, MF_BYCOMMAND | MF_ENABLED);
	matchmenu->EnableMenuItem(ID_MATCH_ICP, MF_BYCOMMAND | MF_ENABLED);

	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->pEntity->setInteractiveStyleMode(Entity::Match);
}


//need to do for match,remove the old actor, add only the current point cloud and stl model 
//in the renderer
void CMainFrame::OnMatchIcp()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->ICPMatchUpdatePolyData();
}

void CMainFrame::OnMatchInteract()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnMatchFeaturePoint()
{
	// TODO: Add your command handler code here

	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CFileDialog dlg(TRUE,"sp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"scan point(*.sp)|*.sp||",NULL);//创建具有sp文件过滤的文件读入选择对话框

	if(dlg.DoModal()==IDOK)
	{
		CString strName=dlg.GetPathName();//得到文件路径
		vtkScanPointReader* featureReader = vtkScanPointReader::New();
		featureReader->SetFileName((LPCTSTR)strName);//通过STLREADER将sp模型读入
		featureReader->Update();
		pDoc->pEntity->pStlDataSetCollection->AddItem(featureReader->GetOutput());//将模型数据加入pEntity的pStlDataSetCollection集合中
		int numForMatch = pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems() - 1;
		pDoc->pEntity->stl_for_match_to_point = numForMatch;
		featureReader->Delete();

		//match
		pDoc->ICPMatchUpdatePolyData();

		pDoc->UpdateAllViews(NULL);
	}
}

void CMainFrame::OnMatchInteractStart()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->DisplayMatchModels();
	pDoc->UpdateAllViews(NULL);

	this->SetInteractiveStyle(Entity::Match);
	mInteractModeActor = TRUE;
}

void CMainFrame::OnMatchInteractEnd()
{
	// TODO: Add your command handler code here
	//need to do, add the modification to the models
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->InteractMatchUpdatePolyData();

	this->SetInteractiveStyle(Entity::Interact);
}


void CMainFrame::SetInteractiveStyle(Entity::ViewMode mode)
{
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)pDoc->GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	if(mode == Entity::Interact)
	{
		//重新设置为交互式操作camera模式
		vtkInteractorStyleTrackballCamera *style =  vtkInteractorStyleTrackballCamera::New();
		pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->SetInteractorStyle(style);

		style->Delete();
	}

	if(mode == Entity::Match)
	{

		//设置为交互式操作actor模式
		vtkInteractorStyleTrackballActorMatch *style =  vtkInteractorStyleTrackballActorMatch::New();
		pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->SetInteractorStyle(style);

		style->Delete();
	}

	//need to do, write a interact style for quering
	if(mode == Entity::Query)
	{
		vtkDataArray * value = pDoc->GetAttributeArrayFromMeasuredData();
		vtkDataArray * dis = pDoc->GetDisplaceArrayFromMeasuredData();
		vtkInteractorStyleTrackballActorQuery *style = vtkInteractorStyleTrackballActorQuery::New();
		style->SetInputMessageBar(&m_OutputDockBar);
		style->SetValueArray(value);
		style->SetDisplaceArray(dis);
		style->SetDocument(pDoc);
		style->SetPickRenderer(pCPointCloudMeasureView->GetRenderer());

		pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->SetInteractorStyle(style);

		/*CString str("ID:");
		double p[3] = {0.5,0.5,0};
		pDoc->AddCaptionWidget(str,p);*/

		style->Delete();
	}
}
void CMainFrame::OnMatchInteractChange()
{
	// TODO: Add your command handler code here
	if(mInteractModeActor)
	{
		this->SetInteractiveStyle(Entity::Interact);
		mInteractModeActor = FALSE;
	}
	else
	{
		this->SetInteractiveStyle(Entity::Match);
		mInteractModeActor = TRUE;
	}
}

void CMainFrame::OnMeasureCompute()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int numStl = pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems();
	int numSurf = pDoc->pEntity->numTotalSurf;

	int numEdge;
	if(pDoc->pEntity->surfEdges != NULL) numEdge = pDoc->pEntity->surfEdges->edges.size();

	if( numStl > 0 || numSurf > 0 || numEdge > 0)
	{
		MeasurementDialog* dlg;
		if(pDoc->pEntity->current_cad_num > 0)
		{
			IgesAndVtkModel* model = pDoc->pEntity->cadModels[pDoc->pEntity->current_cad_num-1];
			dlg = new MeasurementDialog(numStl, model->surfaceNames, pDoc->pEntity->edgeNames, NULL);//创建输出对话框
		}
		else
		{
			dlg = new MeasurementDialog(numStl);
		}

		if(dlg->DoModal()==IDOK)
		{
			bool use_cad_model = dlg->m_use_cad_for_measure;
			bool use_surf_or_edge = dlg->m_use_cad_surf_or_edge;
			int current_stl = dlg->m_stl_out;
			int current_surf = dlg->m_cad_surf_out;
			int current_edge = dlg->m_cad_edge_out;
			
			if(use_cad_model)
			{
				// to do, use feature to compute the distance 
				if(use_surf_or_edge)
				{
					pDoc->ComputeMeasurementWithFeature(current_surf, 1);
				}
				else
				{
					pDoc->ComputeMeasurementWithFeature(current_edge, 0);
				}
			}
			else
			{
				if(numStl > 0)
					pDoc->ComputeMeasurement();
			}
		}

		if(dlg!=NULL)
			delete dlg;
	}
}


CMessageViewDockBar* CMainFrame::GetMessageViewBar()
{
	return &m_OutputDockBar;
}
void CMainFrame::OnMeasureQuery()
{
	// TODO: Add your command handler code here
	this->SetInteractiveStyle(Entity::Query);
}

void CMainFrame::OnXiuPeiCompute()//yuki
{
	// TODO: Add your command handler code here
	XPComputeDlg xpdlg;
	xpdlg.DoModal();

	XP XPcompute;
	XPcompute.L = xpdlg.L;
	XPcompute.vertex_count = xpdlg.count;
	XPcompute.pi = xpdlg.pi;
	XPcompute.temp = xpdlg.temp;
	XPcompute.p.x = xpdlg.px;
	XPcompute.p.y = xpdlg.py;
	XPcompute.cc.cen.x = xpdlg.cx;
	XPcompute.cc.cen.y = xpdlg.cy;
	XPcompute.cc.r = xpdlg.cr;
	XPcompute.Xpcompute();
}

void CMainFrame::OnZhuangPeiMotion()//yuki
{
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//to do, change to the application problem
	pDoc->AddMotionActionObserver(pDoc->pEntity->current_cad_num-1);

}

void CMainFrame::OnRansacCompute()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int numParts = pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();
	
	if(numParts)
	{

		RansacPara dlg;
		dlg.num_parts = numParts;

		if(dlg.DoModal() == IDOK)
		{
			int selParts = dlg.m_selected_part;

			if(selParts >= 0)
			{
				vtkPolyData *pCurrentData;

				pDoc->pEntity->pPointCloudDataSetCollection->InitTraversal();
				for(int i=0;i<=selParts;i++)//得到数据集中相应模型数据的指针
				{
					pCurrentData=pDoc->pEntity->pPointCloudDataSetCollection->GetNextItem();
				}

				pDoc->RANSACBasedShapeRecognition(pCurrentData, dlg.m_distance_ths, dlg.m_projection_ths, dlg.m_probability_ths, dlg.m_normal_ths, dlg.m_minimal_points, dlg.m_set_plane, dlg.m_set_cylinder, dlg.m_set_cone, dlg.m_set_sphere, dlg.m_set_torus);
			}
		}
		cout<<"RANSAC DONE!"<<endl;
	}

	pDoc->DisplayRecognizedModel();
    pDoc->DisplayRecognizedModelInfo();   //display model info ---- YiBing, 2014-12-15

	pDoc->UpdateAllViews(NULL);
}

void CMainFrame::OnToleranceCompute()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();//得到文档类指针
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	ToleranceDlg tolerance;	
	if(pDoc->pEntity->recModels != NULL)
	{
		tolerance.model = pDoc->pEntity->recModels;
		tolerance.DoModal();
	}
	else
	{
		cout<<"No Ransac Model!"<<endl;
	}
}

void CMainFrame::OnEndMotionSim()
{
	// TODO: Add your command handler code here
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//to do, change to the application problem
	pDoc->RemoveMotionActionObserver();
}

void CMainFrame::OnBase()
{
	// TODO: 在此添加命令处理程序代码
	//************************************************
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)pDoc->GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}
	
	vtkInteractorStyleTrackballCamera *style =  vtkInteractorStyleTrackballCamera::New();
	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->SetInteractorStyle(style);

	vtkPolyData *pCurrentData;
	pDoc->pEntity->pStlDataSetCollection->InitTraversal();
	pCurrentData=pDoc->pEntity->pStlDataSetCollection->GetNextItem();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(pCurrentData);
	
	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor(0,1,0); //green
	actor->SetMapper(mapper);
	
	vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
			vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	
	// Set the custom stype to use for interaction.
	vtkSmartPointer<MouseInteractorStyle> interstyle =
			vtkSmartPointer<MouseInteractorStyle>::New();
	interstyle->SetDefaultRenderer(renderer);
	interstyle->Data = pCurrentData;
	
	renderWindowInteractor->SetInteractorStyle(interstyle);
	
	renderer->AddActor(actor);
	renderer->ResetCamera();
	
	renderer->SetBackground(0,0,1); // Blue
	
	renderWindow->Render();
	renderWindowInteractor->Start();

	pDoc->pEntity->getInteractivePoints(interstyle->points);
}

void CMainFrame::OnOnPointcloud()
{
	// TODO: 在此添加命令处理程序代码
	//************************************************
	CPointCloudMeasureDoc* pDoc = this->GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CPointCloudMeasureView *pCPointCloudMeasureView = NULL;

	if (pos)
	{
		pCPointCloudMeasureView = (CPointCloudMeasureView *)pDoc->GetNextView(pos);
	}
	else  // return
	{
		ASSERT(FALSE);
		return;
	}

	vtkInteractorStyleTrackballCamera *style =  vtkInteractorStyleTrackballCamera::New();
	pCPointCloudMeasureView->GetMFCWindow()->GetInteractor()->SetInteractorStyle(style);

	vtkPolyData *pCurrentData;
	pDoc->pEntity->pPointCloudDataSetCollection->InitTraversal();
	pCurrentData=pDoc->pEntity->pPointCloudDataSetCollection->GetNextItem();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(pCurrentData);

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->GetProperty()->SetColor(0,1,0); //green
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	// Set the custom stype to use for interaction.
	vtkSmartPointer<MouseInteractorStyle> interstyle =
		vtkSmartPointer<MouseInteractorStyle>::New();
	interstyle->SetDefaultRenderer(renderer);
	interstyle->Data = pCurrentData;

	renderWindowInteractor->SetInteractorStyle(interstyle);

	renderer->AddActor(actor);
	renderer->ResetCamera();

	renderer->SetBackground(0,0,1); // Blue

	renderWindow->Render();
	renderWindowInteractor->Start();

	pDoc->pEntity->getInteractivePointsPC(interstyle->points);
}
