// PartTreeView.cpp : implementation file
//

#include "stdafx.h"
//#include "STL.h"
#include "PartTreeView.h"
#include "Entity.h"

//#include "EntityNameDlg.h"

#include "PointCloudMeasureDoc.h"
#include "resource.h"
#include "PartProcess.h"
#include "vtkPolyData.h"
#include "vtkStringArray.h"
#include "vtkPointData.h"
#include "vtkDataArray.h" 
#include "vtkStdString.h" 
#include "vtkActor.h"
#include "vtkMapper.h" 
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkLineSource.h"

#include <sstream>
#include <iostream>



//定义模型中多场文件结果类型
#define ITEM_POINTCLOUD	1
#define ITEM_STL		2
#define ITEM_SURFACE	3
#define ITEM_EDGE		4
#define ITEM_RANSAC	    5
/////////////////////////////////////////////////////////////////////////////
// CPartTreeView

IMPLEMENT_DYNCREATE(CPartTreeView, CTreeView)

CPartTreeView::CPartTreeView()
{
}

CPartTreeView::~CPartTreeView()
{
}

BEGIN_MESSAGE_MAP(CPartTreeView, CTreeView)
	//{{AFX_MSG_MAP(CPartTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView drawing

void CPartTreeView::OnDraw(CDC* pDC)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView diagnostics

#ifdef _DEBUG
void CPartTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CPartTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPartTreeView message handlers

//模型树视图得到文档类的指针
CPointCloudMeasureDoc * CPartTreeView::GetDocument()
{
	if(m_pDocument==NULL)
	{
		CFrameWnd *frm=(CFrameWnd *)::AfxGetMainWnd();
		ASSERT(frm);
		CDocument *pDoc=frm->GetActiveDocument();
		ASSERT(pDoc);
		ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(CPointCloudMeasureDoc)));
		pDoc->AddView(this);
	}
	return (CPointCloudMeasureDoc*)m_pDocument; 
}

//模型树视图更新函数
void CPartTreeView::OnUpdate(CView *pSender, LPARAM iHint, CObject *pHint)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();//得到文档类指针
	ASSERT(pDoc);
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	treeCtrl.DeleteAllItems();//清除模型树项

	//yuki HTREEITEM hti,P_hti0,P_hti1,S_hti0,S_hti1, S_cad_P_hti0, S_cad_P_hti1, E_cad_P_hti0, E_cad_P_hti1;
	HTREEITEM hti,P_hti0,P_hti1,S_hti0,S_hti1, S_cad_P_hti0, S_cad_P_hti1, S_cad_P_hti2, E_cad_P_hti0, E_cad_P_hti1; //yuki

	treeCtrl.SetItemHeight(20);//设置模型树项高度

	CString str;
	str.Format("%s","Models:");
	hti = treeCtrl.InsertItem(str,2,2,NULL,TVI_LAST);//设置顶层模型树指针

	//根据文档类中pEntity项下的pCloudDataSetCollection中存储的point cloud文件数目创建对应的模型数项
	if(pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()!=0)
	{
		CString str;
		str.Format("%s","Cloud:");
		P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//建立cloud模型一级目录
		treeCtrl.SetItemData(P_hti0,ITEM_POINTCLOUD);

		int count=0;

		for(int i=0;i<pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();i++)
		{
			str.Format("Part:%d",i+1);
			P_hti1 = treeCtrl.InsertItem(str,1,1,P_hti0,TVI_LAST);//建立cloud模型二级目录
			part_num=count;
			treeCtrl.SetItemData(P_hti1,(DWORD)part_num);
			count++;
		}
	}

	//根据文档类中pEntity项下的pStlDataSetCollection中存储的Stl文件数目创建对应的模型数项
	if(pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems()!=0)
	{
		CString str;
		str.Format("%s","Stl:");
		S_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//建立Stl模型一级目录
		treeCtrl.SetItemData(S_hti0,ITEM_STL);

		int count=0;

		for(int i=0;i<pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems();i++)
		{
			str.Format("Part:%d",i+1);
			S_hti1 = treeCtrl.InsertItem(str,1,1,S_hti0,TVI_LAST);//建立Stl模型二级目录
			part_num=count;
			treeCtrl.SetItemData(S_hti1,(DWORD)part_num);
			count++;
		}
	}

	//根据文档类中pEntity项下的CAD模型创建相应的模型树
	if(pDoc->pEntity->cadModels.size())
	{
		CString str;
		str.Format("%s","CAD Surface:");
		S_cad_P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//建立cad模型一级目录
		treeCtrl.SetItemData(S_cad_P_hti0,ITEM_SURFACE);

		int count = 0;

		vector<IgesAndVtkModel*>::iterator iterCAD;

		for(iterCAD = pDoc->pEntity->cadModels.begin(); iterCAD != pDoc->pEntity->cadModels.end(); iterCAD++)
		{
			CString modelName;
			modelName.Format("%s", (*iterCAD)->modelName.c_str());
			S_cad_P_hti1 = treeCtrl.InsertItem(modelName,0,0,S_cad_P_hti0,TVI_LAST);//建立cad模型二级目录
			treeCtrl.SetItemData(S_cad_P_hti1,ITEM_SURFACE);

			list<SurfaceConvertion*>::iterator iterSurf;
			
			for(iterSurf = (*iterCAD)->convert.surfaces.begin(); iterSurf != (*iterCAD)->convert.surfaces.end(); iterSurf++)
			{
				string name;
				(*iterSurf)->GetName(name);
			    S_cad_P_hti2 = treeCtrl.InsertItem(name.c_str(),1,1,S_cad_P_hti1,TVI_LAST);//建立Stl模型三级目录
				part_num=count;
				treeCtrl.SetItemData(S_cad_P_hti2,(DWORD)part_num);
				count++;
			}
		}

		part_num = count;
		
	}

	if(pDoc->pEntity->surfEdges != NULL && pDoc->pEntity->surfEdges->edges.size() > 0)
	{
		CString str;
		str.Format("%s","CAD edge:");
		E_cad_P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//建立Stl模型一级目录
		treeCtrl.SetItemData(E_cad_P_hti0,ITEM_EDGE);

		int count=0;

		////debug
		//std::ofstream out("tree.txt",std::ios::app | std::ios::out);
		//out << pDoc->pEntity->convert.edges.size() << std::endl;

		list<EdgeConvertion*>::iterator iter;
		for(iter = pDoc->pEntity->surfEdges->edges.begin(); iter != pDoc->pEntity->surfEdges->edges.end(); iter++)
		{
			string name;
			(*iter)->GetName(name);
			E_cad_P_hti1 = treeCtrl.InsertItem(name.c_str(),1,1,E_cad_P_hti0,TVI_LAST);//建立Stl模型二级目录
			part_num=count;
			treeCtrl.SetItemData(E_cad_P_hti1,(DWORD)part_num);
			count++;

			////debug 
			//out << count << std::endl;
			//out << name << std::endl;
		}

		////debug
		//out.close();
	}

	//根据文档类中pEntity项下的RansacModel中存储的识别模型创建对应的模型数项
	if(pDoc->pEntity->recModels != NULL)
	{
		CString str;
		str.Format("%s","Recognized:");
		S_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//建立rec模型一级目录
		treeCtrl.SetItemData(S_hti0,ITEM_RANSAC);

		int count=0;

		for(int i=0;i<pDoc->pEntity->recModels->primitives.size();i++)
		{
			string name = pDoc->pEntity->recModels->primitives[i]->shapeType;
			str.Format("%s:%d",name.c_str(),i+1);
			S_hti1 = treeCtrl.InsertItem(str,1,1,S_hti0,TVI_LAST);//建立rec模型二级目录
			part_num=count;
			treeCtrl.SetItemData(S_hti1,(DWORD)part_num);
			count++;
		}
	}

	GetTreeCtrl().Expand(hti,TVE_EXPAND);

}

int CPartTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	m_ImageList.Create(IDB_PART_TREE,16,1,RGB(0,255,0));//将模型树中的图标导入
	GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);
	GetTreeCtrl().CancelToolTips(TRUE);
	CancelToolTips(TRUE);

	part_num=0;
	return 0;
}

//模型树项响应鼠标双击函数
void CPartTreeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags = 0;
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	HTREEITEM hItem = treeCtrl.HitTest(point,&uFlags);//得到鼠标双击项

	CPointCloudMeasureDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	//响应模型树中中间项操作，得到相应项的值
	if(	hItem && 
		hItem != treeCtrl.GetRootItem() &&
		!treeCtrl.ItemHasChildren(hItem)){
			DWORD itemData = treeCtrl.GetItemData(hItem);//取出当前模型树项的值

			HTREEITEM hParentItem = treeCtrl.GetParentItem(hItem);
			ASSERT(hParentItem);
			DWORD parentData = treeCtrl.GetItemData(hParentItem);//取出当前模型树项父项的值

			//HTREEITEM hNameItem = treeCtrl.GetParentItem(hItem);
			//ASSERT(hNameItem);
			//DWORD nameData = treeCtrl.GetItemData(hNameItem);//取出当前模型树项文件名项的值/yuki
			//std::cout<<nameData;

			//HTREEITEM hParentItem = treeCtrl.GetParentItem(hNameItem);
			//ASSERT(hParentItem);
			//DWORD parentData = treeCtrl.GetItemData(hParentItem);//取出当前模型树项父项的值/yuki


			//根据父项的值，对应文档类中的数据集合，从而进行处理操作

			//对应项为stl项时,对pCloudDataSetCollection的相应数据集进行操作
			if(parentData==ITEM_POINTCLOUD)
			{
				int mode = (int) pDoc->pEntity->getInteractiveStyleMode();

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()-1));//确定对应项是否在数据集中
				pDoc->pEntity->current_point_cloud_part_num=itemData;

				vtkPolyData *pCurrentData;
				vtkStringArray *pScalarArray=vtkStringArray::New();//建立节点标量属性名数组
				vtkStringArray *pVectorArray=vtkStringArray::New();//建立节点矢量属性名数组

				pDoc->pEntity->pPointCloudDataSetCollection->InitTraversal();
				for(int i=0;i<=itemData;i++)//得到数据集中相应模型数据的指针
				{
					pCurrentData=pDoc->pEntity->pPointCloudDataSetCollection->GetNextItem();
				}

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//得到模型数据中节点数
				int num_verts=pCurrentData->GetNumberOfPoints();//得到模型数据中单元数

				PartProcess *dlg;//创建数据操作对话框

				if(mode == 0)
				{
					dlg = new PartProcess();
				}

				if(mode == 1)
				{
					dlg = new PartProcess(mode);
					//设置stl模型数目，从而可以选择匹配
					int numStlModel = pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems();
					dlg->SetNumStlModel( numStlModel);
				}

				if(mode == 2)
				{
					int m_array_num=pCurrentData->GetPointData()->GetNumberOfArrays();//得到数据集中相应模型数据中节点属性数组的数目
					pScalarArray->Allocate(10,10);
					pVectorArray->Allocate(10,10);

					if(m_array_num)
					{
						CString str;
						for(int i=0;i<m_array_num;i++)
						{
							int num_component=pCurrentData->GetPointData()->GetArray(i)->GetNumberOfComponents();
							if(num_component==1)
							{
								str=pCurrentData->GetPointData()->GetArray(i)->GetName();
								pScalarArray->InsertNextValue(str);//将节点标量属性名加入数组

							}
							if(num_component==3)
							{
								str=pCurrentData->GetPointData()->GetArray(i)->GetName();
								pVectorArray->InsertNextValue(str);//将节点矢量属性名加入数组
							}
						}
					}

					dlg = new PartProcess(pScalarArray,pVectorArray,mode);//创建数据操作对话框
					dlg->m_part_legend = pDoc->m_widget_on;
				}



				dlg->m_model="vtkPolyData";
				dlg->m_type="Cloud";


				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pCloudActorColletion,itemData);//根据模型在窗口的可见性设置对话框中模型可见性的值
				dlg->m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg->m_num_cells=buffer;//在对话框显示数据中节点数
				itoa(num_verts,buffer,10);
				dlg->m_num_verts=buffer;//在对话框显示数据中单元数

				if(dlg->DoModal()==IDOK)
				{
					double m_range[2];
					CString m_set_str;
					int m_component;

					bool m_change_visible=dlg->m_part_visible;

					if(mode == 0)
					{
						pDoc->m_widget_on = FALSE;
					}

					if(mode == 1)
					{
						pDoc->pEntity->stl_for_match_to_point = dlg->m_set_match;
						pDoc->m_widget_on = FALSE;

					}

					if(mode == 2)
					{
						if(dlg->m_set_attribute!=LB_ERR&&dlg->m_set_attribute<pScalarArray->GetNumberOfTuples())//得到模型物理属性映射显示信息，并且进行可视化
						{
							m_set_str=pScalarArray->GetValue(dlg->m_set_attribute);
							pCurrentData->GetPointData()->GetArray(m_set_str)->GetRange(m_range);

							pDoc->MapperChangeToDisplay(pDoc->pCloudActorColletion,pDoc->pEntity->pPointCloudDataSetCollection,pCurrentData,m_set_str,m_range,itemData);
						}
						else if(dlg->m_set_attribute>=pScalarArray->GetNumberOfTuples())
						{
							m_set_str=pVectorArray->GetValue(dlg->m_set_attribute-pScalarArray->GetNumberOfTuples());
							pCurrentData->GetPointData()->GetArray(m_set_str)->GetRange(m_range);
							if(dlg->m_has_component&&dlg->m_set_component!=LB_ERR)
							{
								m_component=dlg->m_set_component;
								pCurrentData->GetPointData()->GetArray(m_set_str)->GetRange(m_range,m_component);
							}
							pDoc->MapperChangeToDisplay(pDoc->pCloudActorColletion,pDoc->pEntity->pPointCloudDataSetCollection,pCurrentData,m_set_str,m_range,itemData,m_component);
						}
						if(dlg->m_part_legend)//设置物理属性bar的可见性
						{
							pDoc->m_widget_on=TRUE;
						}
						else
						{
							pDoc->m_widget_on=FALSE;
						}
					}


					m_change_visible=dlg->m_part_visible;
					if(m_change_visible!=m_origin_visible)//设置模型的可见性
					{
						pDoc->ChangeToActorVisibility(pDoc->pCloudActorColletion,itemData,m_change_visible);
					}

					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}

				pScalarArray->Delete();
				pVectorArray->Delete();
				delete dlg;

			}

			//对应项为stl项时,对pStlDataSetCollection的相应数据集进行操作
			if(parentData==ITEM_STL)
			{
				PartProcess dlg;//创建数据操作对话框

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems()-1));//确定对应项是否在数据集中
				pDoc->pEntity->current_stl_part_num=itemData;

				vtkPolyData *pCurrentData;

				pDoc->pEntity->pStlDataSetCollection->InitTraversal();
				for(int i=0;i<=itemData;i++)//得到数据集中相应模型数据的指针
				{
					pCurrentData=pDoc->pEntity->pStlDataSetCollection->GetNextItem();
				}
				dlg.m_model="vtkPolyData";
				dlg.m_type="STL";

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//得到模型数据中节点数
				int num_verts=pCurrentData->GetNumberOfPoints();//得到模型数据中单元数
				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pStlActorColletion,itemData);//根据模型在窗口的可见性设置对话框中模型可见性的值
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//在对话框显示数据中节点数
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//在对话框显示数据中单元数

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					if(m_change_visible!=m_origin_visible)//设置模型的可见性
					{
						pDoc->ChangeToActorVisibility(pDoc->pStlActorColletion,itemData,m_change_visible);
					}
					//				pDoc->m_widget_on=FALSE;//不显示物理属性bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//对应项为cad项时,对surface features的相应数据集进行操作
			if(parentData==ITEM_SURFACE)
			{
				PartProcess dlg;//创建数据操作对话框

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->numTotalSurf - 1));//确定对应项是否在数据集中
				pDoc->pEntity->current_surface_num = itemData;

				vtkPolyData *pCurrentData = vtkPolyData::New();

				//get cadmodel index
				vector<IgesAndVtkModel*>::iterator iterCAD;
				int surfIndx = 0;
		        for(iterCAD = pDoc->pEntity->cadModels.begin(); iterCAD != pDoc->pEntity->cadModels.end(); iterCAD++)
				{
					surfIndx++;
					if((*iterCAD)->startSurfId <= itemData && (*iterCAD)->endSurfId >= itemData)
						break;
				}

				list<SurfaceConvertion*>::iterator iter;
		        iter = (*iterCAD)->convert.surfaces.begin();
				pDoc->pEntity->current_cad_num = surfIndx;
				
				//vector<int>::iterator itersize;//yuki
				//itersize = pDoc->pEntity->surfacesize.begin();//yuki
				//if(nameData == 0)
				//{
				//	itemData = itemData;
				//}
				//else
				//{
				//	itemData = itemData + *(itersize + nameData - 1);
				//}
		

				for(int i = (*iterCAD)->startSurfId; i < itemData; i++)
					iter++;                          //得到数据集中相应模型数据的指针
				
				dlg.m_model="CAD";
				dlg.m_type="Surface";
				dlg.m_cad_surface = TRUE;
				dlg.m_recognize_model = FALSE;
				dlg.m_edge_selection = TRUE;

				(*iter)->GetPolyData(pCurrentData);
				//(*iter)->

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//得到模型数据中节点数
				int num_verts=pCurrentData->GetNumberOfPoints();//得到模型数据中单元数
				bool m_origin_visible, m_origin_highlight;

				//pCurrentData->Delete();

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pSurfacesCADActorColletion,itemData);//根据模型在窗口的可见性设置对话框中模型可见性的值
				dlg.m_part_visible=m_origin_visible;
				m_origin_highlight = pDoc->pEntity->highLightSurface[itemData];
				dlg.m_part_highlight = m_origin_highlight;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//在对话框显示数据中节点数
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//在对话框显示数据中单元数

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					bool m_chage_hightlight = dlg.m_part_highlight;

					//handle edges
					if(dlg.m_set_edge_selection)
					{
						ProcessSurfaceEdge( (*iterCAD),itemData);
					}

					if(m_change_visible!=m_origin_visible)//设置模型的可见性
					{
						pDoc->ChangeToActorVisibility(pDoc->pSurfacesCADActorColletion,itemData,m_change_visible);
					}

					if(m_origin_highlight != m_chage_hightlight)
					{
						pDoc->ChangToActorHighlight(pDoc->pSurfacesCADActorColletion,itemData,m_chage_hightlight, 0);
					}

					if(dlg.m_set_visible_all)
					{
						pDoc->ActorVisibleAll(pDoc->pSurfacesCADActorColletion);
					}

					if(dlg.m_set_visible_only)
					{
						pDoc->ActorVisibleOnly(pDoc->pSurfacesCADActorColletion,itemData);
					}

					//if(dlg.m_set_2d)//yuki
					//{
					//	vtkPolyData *poly2d = vtkPolyData::New();

					//	vtkTransform *translation = vtkTransform::New();

					//	double a =-1, b = -1, c= -1;
					//	double ag1, ag2;
					//	ag1=(180*acos(c/sqrt(b*b+c*c)))/3.1415926;
					//	ag2=-(180 * acos((sqrt(b*b + c*c))/(sqrt(a*a+ b*b+c*c))))/3.1415926;
					//	translation->PostMultiply();
					//	translation->RotateX(ag1);
					//	translation->RotateY(ag2);

					//	vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
					//	transformFilter->SetInput(pCurrentData);
					//	transformFilter->SetTransform(translation);
					//	transformFilter->Update();

					//	vtkPolyDataMapper *mapper2d = vtkPolyDataMapper::New();
					//	mapper2d->SetInputConnection( transformFilter->GetOutputPort() );

					//	vtkActor *actor2d = vtkActor::New();
					//	actor2d->SetMapper (mapper2d);
					//	actor2d->GetProperty()->SetColor(0,0,1);

					//	pDoc->pModelCADActorColletion->AddItem(actor2d);

					//	mapper2d->Delete();
					//	actor2d->Delete();
					//	poly2d->Delete();

					//}

					//if(dlg.m_set_2d_all)//yuki
					//{
					//	list<SurfaceConvertion*>::iterator miter; 

					//	for(miter = pDoc->pEntity->convert.surfaces.begin();miter != pDoc->pEntity->convert.surfaces.end(); miter++)
					//	{                  					
					//		vtkPolyData *mCurrentData = vtkPolyData::New();
					//		
					//		(*miter)->GetPolyData(mCurrentData);

					//		vtkPolyData *poly2d = vtkPolyData::New();

					//		vtkTransform *translation = vtkTransform::New();

					//		double a =-1, b = -1, c= -1;
					//		double ag1, ag2;
					//		ag1=(180*acos(c/sqrt(b*b+c*c)))/3.1415926;
					//		ag2=-(180 * acos((sqrt(b*b + c*c))/(sqrt(a*a+ b*b+c*c))))/3.1415926;
					//		translation->PostMultiply();
					//		translation->RotateX(ag1);
					//		translation->RotateY(ag2);

					//		vtkTransformPolyDataFilter *transformFilter = vtkTransformPolyDataFilter::New();
					//		transformFilter->SetInput(mCurrentData);
					//		transformFilter->SetTransform(translation);
					//		transformFilter->Update();

					//		vtkPolyDataMapper *mapper2d = vtkPolyDataMapper::New();
					//		mapper2d->SetInputConnection( transformFilter->GetOutputPort() );

					//		vtkActor *actor2d = vtkActor::New();
					//		actor2d->SetMapper (mapper2d);
					//		actor2d->GetProperty()->SetColor(0,0,1);

					//		pDoc->pModelCADActorColletion->AddItem(actor2d);

					//		mapper2d->Delete();
					//		actor2d->Delete();
					//		poly2d->Delete();
					//	}
					//}

					//if(dlg.m_set_axis)//yuki
					//{
					//	gp_Ax1 raxis;
					//	(*iter)->GetRAxis(raxis);
					//	gp_Dir r_dir;
					//	gp_Pnt r_pnt;
					//	r_dir = raxis.Direction();
					//	r_pnt = raxis.Location();

					//	vtkLineSource *axisline = vtkLineSource::New();  
					//	axisline->SetPoint1(r_pnt.X(),r_pnt.Y(),r_pnt.Z());  
					//	axisline->SetPoint2(r_pnt.X()-50*r_dir.X(),r_pnt.Y()-50*r_dir.Y(),r_pnt.Z()-50*r_dir.Z());  

					//	vtkPolyDataMapper *axismapper = vtkPolyDataMapper::New();
					//	axismapper->SetInputConnection( axisline->GetOutputPort() );

					//	vtkActor *axisactor = vtkActor::New();
					//	axisactor->SetMapper (axismapper);
					//	axisactor->GetProperty()->SetColor(0,1,1);

					//	pDoc->pSurfacesCADActorColletion->AddItem(axisactor);

					//}

					pDoc->pEntity->highLightSurface[itemData] = m_chage_hightlight;
					//				pDoc->m_widget_on=FALSE;//不显示物理属性bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//对应项为cad项时,对edge feature的相应数据集进行操作
			if(parentData==ITEM_EDGE)
			{
				PartProcess dlg;//创建数据操作对话框

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->surfEdges->edges.size() - 1));//确定对应项是否在数据集中
				pDoc->pEntity->current_edge_num = itemData;

				vtkPolyData *pCurrentData = vtkPolyData::New();

				list<EdgeConvertion*>::iterator iter;
				iter = pDoc->pEntity->surfEdges->edges.begin();
				for(int i=0;i<itemData;i++)
					iter++;                          //得到数据集中相应模型数据的指针
				
				dlg.m_model="CAD";
				dlg.m_type="Edge";

				(*iter)->GetPolyData(pCurrentData);

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//得到模型数据中节点数
				int num_verts=pCurrentData->GetNumberOfPoints();//得到模型数据中单元数
				bool m_origin_visible, m_origin_highlight;
				m_origin_highlight = pDoc->pEntity->highLightEdge[itemData];
				dlg.m_part_highlight = m_origin_highlight;

				pCurrentData->Delete();

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pEdgesCADActorColletion,itemData);//根据模型在窗口的可见性设置对话框中模型可见性的值
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//在对话框显示数据中节点数
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//在对话框显示数据中单元数

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					bool m_chage_hightlight = dlg.m_part_highlight;
					if(m_change_visible!=m_origin_visible)//设置模型的可见性
					{
						pDoc->ChangeToActorVisibility(pDoc->pEdgesCADActorColletion,itemData,m_change_visible);
					}
					if(m_origin_highlight != m_chage_hightlight)
					{
						pDoc->ChangToActorHighlight(pDoc->pEdgesCADActorColletion,itemData,m_chage_hightlight, 1);
					}
					pDoc->pEntity->highLightEdge[itemData] = m_chage_hightlight;
					//				pDoc->m_widget_on=FALSE;//不显示物理属性bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//对应项为recognized项时,对recModel的相应数据集进行操作
			if(parentData==ITEM_RANSAC)
			{
				PartProcess dlg;//创建数据操作对话框

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->recModels->primitives.size()-1));//确定对应项是否在数据集中

				dlg.m_model="vtkPolyData";
				dlg.m_type="Recognized Points Model";

				char buffer[20];
				int num_verts=pDoc->pEntity->recModels->primitives[itemData]->numPoints;//得到模型数据中节点数
				int num_cells = num_verts;//点云模型 点数同cell数目相同
				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pPrimitiveActorColletion,itemData);//根据模型在窗口的可见性设置对话框中模型可见性的值
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//在对话框显示数据中节点数
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//在对话框显示数据中单元数

				//enable the checkbox for axis and normal view ---- edony 2014/12/16
				dlg.m_cad_surface = FALSE;
				dlg.m_recognize_model = TRUE;

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					if(m_change_visible!=m_origin_visible)//设置模型的可见性
					{
						pDoc->ChangeToActorVisibility(pDoc->pPrimitiveActorColletion,itemData,m_change_visible);

					/*	if(pDoc->pEntity->recModels->Check[itemData]==1)
						{
							pDoc->pEntity->recModels->Check[itemData]=0;
						}
						else 
						{
							pDoc->pEntity->recModels->Check[itemData]=1;
						}*/
						if(m_origin_visible== true && m_change_visible == false)
						{
							pDoc->pEntity->recModels->outputMarks[itemData] = false;
						}

						if(m_origin_visible== false && m_change_visible == true)
						{
							pDoc->pEntity->recModels->outputMarks[itemData] = true;
						}
					}
					
					//pDoc->m_widget_on=FALSE;//不显示物理属性bar

					//display the axis and normal ---- edony 2014/12/16
					if(dlg.m_inform_all == TRUE)
					{
						pDoc->ActorVisibleAll(pDoc->pPrimitiveParaActorColletion);
						
					}
					if(dlg.m_inform_only == TRUE)
					{
						pDoc->ActorVisibleOnly(pDoc->pPrimitiveParaActorColletion, itemData);
					}
					pDoc->UpdateAllViews(NULL);
				}
			}
	}
	CTreeView::OnLButtonDblClk(nFlags, point);
}


void CPartTreeView::ProcessSurfaceEdge(IgesAndVtkModel* model,int item)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	list<SurfaceConvertion*>::iterator iter;
	iter = model->convert.surfaces.begin();
	for(int i = model->startSurfId; i < item; i++)
					iter++;                          //得到数据集中相应模型数据的指针

	//转化边模型
	if( pDoc->pEntity->surfEdges != NULL)
	{
		delete pDoc->pEntity->surfEdges;
		pDoc->pEntity->surfEdges = NULL;
	}

	pDoc->pEntity->surfEdges = new EdgeFromSurface();
	pDoc->pEntity->surfEdges->SetTopologyModel((*iter)->GetTopologyShape());
	pDoc->pEntity->surfEdges->SetEdgePrecision(0.01);
	pDoc->pEntity->surfEdges->ConvertModel();

	//清除之前数据
	pDoc->pEdgesCADActorColletion->RemoveAllItems();
	pDoc->pEntity->edgeNames.clear();
	pDoc->pEntity->highLightEdge.clear();

    //显示边模型
	list<EdgeConvertion*>::iterator iter1;

	for(iter1 = pDoc->pEntity->surfEdges->edges.begin(); iter1 != pDoc->pEntity->surfEdges->edges.end(); iter1++)
	{
		vtkPolyData *poly = vtkPolyData::New();
		//	vtkPolyData *poly;
		(*iter1)->GetPolyData(poly);

		if(poly != NULL)
		{
			vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
			mapper->SetInput( poly );

			vtkActor *actor = vtkActor::New();
			actor->SetMapper (mapper);
			actor->GetProperty()->SetColor(0,1,0);
			actor->GetProperty()->SetLineWidth(2);

			pDoc->pEdgesCADActorColletion->AddItem(actor);
			pDoc->pEntity->highLightEdge.push_back(false);

			string name;
			(*iter1)->GetName(name);
			pDoc->pEntity->edgeNames.push_back(name);

			poly->Delete();
			mapper->Delete();
			actor->Delete();
		}
	}
}