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



//����ģ���жೡ�ļ��������
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

//ģ������ͼ�õ��ĵ����ָ��
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

//ģ������ͼ���º���
void CPartTreeView::OnUpdate(CView *pSender, LPARAM iHint, CObject *pHint)
{
	CPointCloudMeasureDoc* pDoc = GetDocument();//�õ��ĵ���ָ��
	ASSERT(pDoc);
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	treeCtrl.DeleteAllItems();//���ģ������

	//yuki HTREEITEM hti,P_hti0,P_hti1,S_hti0,S_hti1, S_cad_P_hti0, S_cad_P_hti1, E_cad_P_hti0, E_cad_P_hti1;
	HTREEITEM hti,P_hti0,P_hti1,S_hti0,S_hti1, S_cad_P_hti0, S_cad_P_hti1, S_cad_P_hti2, E_cad_P_hti0, E_cad_P_hti1; //yuki

	treeCtrl.SetItemHeight(20);//����ģ������߶�

	CString str;
	str.Format("%s","Models:");
	hti = treeCtrl.InsertItem(str,2,2,NULL,TVI_LAST);//���ö���ģ����ָ��

	//�����ĵ�����pEntity���µ�pCloudDataSetCollection�д洢��point cloud�ļ���Ŀ������Ӧ��ģ������
	if(pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()!=0)
	{
		CString str;
		str.Format("%s","Cloud:");
		P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//����cloudģ��һ��Ŀ¼
		treeCtrl.SetItemData(P_hti0,ITEM_POINTCLOUD);

		int count=0;

		for(int i=0;i<pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems();i++)
		{
			str.Format("Part:%d",i+1);
			P_hti1 = treeCtrl.InsertItem(str,1,1,P_hti0,TVI_LAST);//����cloudģ�Ͷ���Ŀ¼
			part_num=count;
			treeCtrl.SetItemData(P_hti1,(DWORD)part_num);
			count++;
		}
	}

	//�����ĵ�����pEntity���µ�pStlDataSetCollection�д洢��Stl�ļ���Ŀ������Ӧ��ģ������
	if(pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems()!=0)
	{
		CString str;
		str.Format("%s","Stl:");
		S_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//����Stlģ��һ��Ŀ¼
		treeCtrl.SetItemData(S_hti0,ITEM_STL);

		int count=0;

		for(int i=0;i<pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems();i++)
		{
			str.Format("Part:%d",i+1);
			S_hti1 = treeCtrl.InsertItem(str,1,1,S_hti0,TVI_LAST);//����Stlģ�Ͷ���Ŀ¼
			part_num=count;
			treeCtrl.SetItemData(S_hti1,(DWORD)part_num);
			count++;
		}
	}

	//�����ĵ�����pEntity���µ�CADģ�ʹ�����Ӧ��ģ����
	if(pDoc->pEntity->cadModels.size())
	{
		CString str;
		str.Format("%s","CAD Surface:");
		S_cad_P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//����cadģ��һ��Ŀ¼
		treeCtrl.SetItemData(S_cad_P_hti0,ITEM_SURFACE);

		int count = 0;

		vector<IgesAndVtkModel*>::iterator iterCAD;

		for(iterCAD = pDoc->pEntity->cadModels.begin(); iterCAD != pDoc->pEntity->cadModels.end(); iterCAD++)
		{
			CString modelName;
			modelName.Format("%s", (*iterCAD)->modelName.c_str());
			S_cad_P_hti1 = treeCtrl.InsertItem(modelName,0,0,S_cad_P_hti0,TVI_LAST);//����cadģ�Ͷ���Ŀ¼
			treeCtrl.SetItemData(S_cad_P_hti1,ITEM_SURFACE);

			list<SurfaceConvertion*>::iterator iterSurf;
			
			for(iterSurf = (*iterCAD)->convert.surfaces.begin(); iterSurf != (*iterCAD)->convert.surfaces.end(); iterSurf++)
			{
				string name;
				(*iterSurf)->GetName(name);
			    S_cad_P_hti2 = treeCtrl.InsertItem(name.c_str(),1,1,S_cad_P_hti1,TVI_LAST);//����Stlģ������Ŀ¼
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
		E_cad_P_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//����Stlģ��һ��Ŀ¼
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
			E_cad_P_hti1 = treeCtrl.InsertItem(name.c_str(),1,1,E_cad_P_hti0,TVI_LAST);//����Stlģ�Ͷ���Ŀ¼
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

	//�����ĵ�����pEntity���µ�RansacModel�д洢��ʶ��ģ�ʹ�����Ӧ��ģ������
	if(pDoc->pEntity->recModels != NULL)
	{
		CString str;
		str.Format("%s","Recognized:");
		S_hti0 = treeCtrl.InsertItem(str,0,0,hti,TVI_LAST);//����recģ��һ��Ŀ¼
		treeCtrl.SetItemData(S_hti0,ITEM_RANSAC);

		int count=0;

		for(int i=0;i<pDoc->pEntity->recModels->primitives.size();i++)
		{
			string name = pDoc->pEntity->recModels->primitives[i]->shapeType;
			str.Format("%s:%d",name.c_str(),i+1);
			S_hti1 = treeCtrl.InsertItem(str,1,1,S_hti0,TVI_LAST);//����recģ�Ͷ���Ŀ¼
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
	m_ImageList.Create(IDB_PART_TREE,16,1,RGB(0,255,0));//��ģ�����е�ͼ�굼��
	GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);
	GetTreeCtrl().CancelToolTips(TRUE);
	CancelToolTips(TRUE);

	part_num=0;
	return 0;
}

//ģ��������Ӧ���˫������
void CPartTreeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags = 0;
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	HTREEITEM hItem = treeCtrl.HitTest(point,&uFlags);//�õ����˫����

	CPointCloudMeasureDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	//��Ӧģ�������м���������õ���Ӧ���ֵ
	if(	hItem && 
		hItem != treeCtrl.GetRootItem() &&
		!treeCtrl.ItemHasChildren(hItem)){
			DWORD itemData = treeCtrl.GetItemData(hItem);//ȡ����ǰģ�������ֵ

			HTREEITEM hParentItem = treeCtrl.GetParentItem(hItem);
			ASSERT(hParentItem);
			DWORD parentData = treeCtrl.GetItemData(hParentItem);//ȡ����ǰģ��������ֵ

			//HTREEITEM hNameItem = treeCtrl.GetParentItem(hItem);
			//ASSERT(hNameItem);
			//DWORD nameData = treeCtrl.GetItemData(hNameItem);//ȡ����ǰģ�������ļ������ֵ/yuki
			//std::cout<<nameData;

			//HTREEITEM hParentItem = treeCtrl.GetParentItem(hNameItem);
			//ASSERT(hParentItem);
			//DWORD parentData = treeCtrl.GetItemData(hParentItem);//ȡ����ǰģ��������ֵ/yuki


			//���ݸ����ֵ����Ӧ�ĵ����е����ݼ��ϣ��Ӷ����д������

			//��Ӧ��Ϊstl��ʱ,��pCloudDataSetCollection����Ӧ���ݼ����в���
			if(parentData==ITEM_POINTCLOUD)
			{
				int mode = (int) pDoc->pEntity->getInteractiveStyleMode();

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->pPointCloudDataSetCollection->GetNumberOfItems()-1));//ȷ����Ӧ���Ƿ������ݼ���
				pDoc->pEntity->current_point_cloud_part_num=itemData;

				vtkPolyData *pCurrentData;
				vtkStringArray *pScalarArray=vtkStringArray::New();//�����ڵ��������������
				vtkStringArray *pVectorArray=vtkStringArray::New();//�����ڵ�ʸ������������

				pDoc->pEntity->pPointCloudDataSetCollection->InitTraversal();
				for(int i=0;i<=itemData;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
				{
					pCurrentData=pDoc->pEntity->pPointCloudDataSetCollection->GetNextItem();
				}

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//�õ�ģ�������нڵ���
				int num_verts=pCurrentData->GetNumberOfPoints();//�õ�ģ�������е�Ԫ��

				PartProcess *dlg;//�������ݲ����Ի���

				if(mode == 0)
				{
					dlg = new PartProcess();
				}

				if(mode == 1)
				{
					dlg = new PartProcess(mode);
					//����stlģ����Ŀ���Ӷ�����ѡ��ƥ��
					int numStlModel = pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems();
					dlg->SetNumStlModel( numStlModel);
				}

				if(mode == 2)
				{
					int m_array_num=pCurrentData->GetPointData()->GetNumberOfArrays();//�õ����ݼ�����Ӧģ�������нڵ������������Ŀ
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
								pScalarArray->InsertNextValue(str);//���ڵ������������������

							}
							if(num_component==3)
							{
								str=pCurrentData->GetPointData()->GetArray(i)->GetName();
								pVectorArray->InsertNextValue(str);//���ڵ�ʸ����������������
							}
						}
					}

					dlg = new PartProcess(pScalarArray,pVectorArray,mode);//�������ݲ����Ի���
					dlg->m_part_legend = pDoc->m_widget_on;
				}



				dlg->m_model="vtkPolyData";
				dlg->m_type="Cloud";


				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pCloudActorColletion,itemData);//����ģ���ڴ��ڵĿɼ������öԻ�����ģ�Ϳɼ��Ե�ֵ
				dlg->m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg->m_num_cells=buffer;//�ڶԻ�����ʾ�����нڵ���
				itoa(num_verts,buffer,10);
				dlg->m_num_verts=buffer;//�ڶԻ�����ʾ�����е�Ԫ��

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
						if(dlg->m_set_attribute!=LB_ERR&&dlg->m_set_attribute<pScalarArray->GetNumberOfTuples())//�õ�ģ����������ӳ����ʾ��Ϣ�����ҽ��п��ӻ�
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
						if(dlg->m_part_legend)//������������bar�Ŀɼ���
						{
							pDoc->m_widget_on=TRUE;
						}
						else
						{
							pDoc->m_widget_on=FALSE;
						}
					}


					m_change_visible=dlg->m_part_visible;
					if(m_change_visible!=m_origin_visible)//����ģ�͵Ŀɼ���
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

			//��Ӧ��Ϊstl��ʱ,��pStlDataSetCollection����Ӧ���ݼ����в���
			if(parentData==ITEM_STL)
			{
				PartProcess dlg;//�������ݲ����Ի���

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->pStlDataSetCollection->GetNumberOfItems()-1));//ȷ����Ӧ���Ƿ������ݼ���
				pDoc->pEntity->current_stl_part_num=itemData;

				vtkPolyData *pCurrentData;

				pDoc->pEntity->pStlDataSetCollection->InitTraversal();
				for(int i=0;i<=itemData;i++)//�õ����ݼ�����Ӧģ�����ݵ�ָ��
				{
					pCurrentData=pDoc->pEntity->pStlDataSetCollection->GetNextItem();
				}
				dlg.m_model="vtkPolyData";
				dlg.m_type="STL";

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//�õ�ģ�������нڵ���
				int num_verts=pCurrentData->GetNumberOfPoints();//�õ�ģ�������е�Ԫ��
				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pStlActorColletion,itemData);//����ģ���ڴ��ڵĿɼ������öԻ�����ģ�Ϳɼ��Ե�ֵ
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//�ڶԻ�����ʾ�����нڵ���
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//�ڶԻ�����ʾ�����е�Ԫ��

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					if(m_change_visible!=m_origin_visible)//����ģ�͵Ŀɼ���
					{
						pDoc->ChangeToActorVisibility(pDoc->pStlActorColletion,itemData,m_change_visible);
					}
					//				pDoc->m_widget_on=FALSE;//����ʾ��������bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//��Ӧ��Ϊcad��ʱ,��surface features����Ӧ���ݼ����в���
			if(parentData==ITEM_SURFACE)
			{
				PartProcess dlg;//�������ݲ����Ի���

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->numTotalSurf - 1));//ȷ����Ӧ���Ƿ������ݼ���
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
					iter++;                          //�õ����ݼ�����Ӧģ�����ݵ�ָ��
				
				dlg.m_model="CAD";
				dlg.m_type="Surface";
				dlg.m_cad_surface = TRUE;
				dlg.m_recognize_model = FALSE;
				dlg.m_edge_selection = TRUE;

				(*iter)->GetPolyData(pCurrentData);
				//(*iter)->

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//�õ�ģ�������нڵ���
				int num_verts=pCurrentData->GetNumberOfPoints();//�õ�ģ�������е�Ԫ��
				bool m_origin_visible, m_origin_highlight;

				//pCurrentData->Delete();

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pSurfacesCADActorColletion,itemData);//����ģ���ڴ��ڵĿɼ������öԻ�����ģ�Ϳɼ��Ե�ֵ
				dlg.m_part_visible=m_origin_visible;
				m_origin_highlight = pDoc->pEntity->highLightSurface[itemData];
				dlg.m_part_highlight = m_origin_highlight;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//�ڶԻ�����ʾ�����нڵ���
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//�ڶԻ�����ʾ�����е�Ԫ��

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					bool m_chage_hightlight = dlg.m_part_highlight;

					//handle edges
					if(dlg.m_set_edge_selection)
					{
						ProcessSurfaceEdge( (*iterCAD),itemData);
					}

					if(m_change_visible!=m_origin_visible)//����ģ�͵Ŀɼ���
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
					//				pDoc->m_widget_on=FALSE;//����ʾ��������bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//��Ӧ��Ϊcad��ʱ,��edge feature����Ӧ���ݼ����в���
			if(parentData==ITEM_EDGE)
			{
				PartProcess dlg;//�������ݲ����Ի���

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->surfEdges->edges.size() - 1));//ȷ����Ӧ���Ƿ������ݼ���
				pDoc->pEntity->current_edge_num = itemData;

				vtkPolyData *pCurrentData = vtkPolyData::New();

				list<EdgeConvertion*>::iterator iter;
				iter = pDoc->pEntity->surfEdges->edges.begin();
				for(int i=0;i<itemData;i++)
					iter++;                          //�õ����ݼ�����Ӧģ�����ݵ�ָ��
				
				dlg.m_model="CAD";
				dlg.m_type="Edge";

				(*iter)->GetPolyData(pCurrentData);

				char buffer[20];
				int num_cells=pCurrentData->GetNumberOfCells();//�õ�ģ�������нڵ���
				int num_verts=pCurrentData->GetNumberOfPoints();//�õ�ģ�������е�Ԫ��
				bool m_origin_visible, m_origin_highlight;
				m_origin_highlight = pDoc->pEntity->highLightEdge[itemData];
				dlg.m_part_highlight = m_origin_highlight;

				pCurrentData->Delete();

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pEdgesCADActorColletion,itemData);//����ģ���ڴ��ڵĿɼ������öԻ�����ģ�Ϳɼ��Ե�ֵ
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//�ڶԻ�����ʾ�����нڵ���
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//�ڶԻ�����ʾ�����е�Ԫ��

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					bool m_chage_hightlight = dlg.m_part_highlight;
					if(m_change_visible!=m_origin_visible)//����ģ�͵Ŀɼ���
					{
						pDoc->ChangeToActorVisibility(pDoc->pEdgesCADActorColletion,itemData,m_change_visible);
					}
					if(m_origin_highlight != m_chage_hightlight)
					{
						pDoc->ChangToActorHighlight(pDoc->pEdgesCADActorColletion,itemData,m_chage_hightlight, 1);
					}
					pDoc->pEntity->highLightEdge[itemData] = m_chage_hightlight;
					//				pDoc->m_widget_on=FALSE;//����ʾ��������bar
					pDoc->ReorderActors();
					pDoc->UpdateAllViews(NULL);
				}
			}

			//��Ӧ��Ϊrecognized��ʱ,��recModel����Ӧ���ݼ����в���
			if(parentData==ITEM_RANSAC)
			{
				PartProcess dlg;//�������ݲ����Ի���

				ASSERT(0<=itemData&&itemData<=(pDoc->pEntity->recModels->primitives.size()-1));//ȷ����Ӧ���Ƿ������ݼ���

				dlg.m_model="vtkPolyData";
				dlg.m_type="Recognized Points Model";

				char buffer[20];
				int num_verts=pDoc->pEntity->recModels->primitives[itemData]->numPoints;//�õ�ģ�������нڵ���
				int num_cells = num_verts;//����ģ�� ����ͬcell��Ŀ��ͬ
				bool m_origin_visible;

				m_origin_visible=pDoc->GetActorVisibility(pDoc->pPrimitiveActorColletion,itemData);//����ģ���ڴ��ڵĿɼ������öԻ�����ģ�Ϳɼ��Ե�ֵ
				dlg.m_part_visible=m_origin_visible;

				itoa(num_cells,buffer,10);
				dlg.m_num_cells=buffer;//�ڶԻ�����ʾ�����нڵ���
				itoa(num_verts,buffer,10);
				dlg.m_num_verts=buffer;//�ڶԻ�����ʾ�����е�Ԫ��

				//enable the checkbox for axis and normal view ---- edony 2014/12/16
				dlg.m_cad_surface = FALSE;
				dlg.m_recognize_model = TRUE;

				if(dlg.DoModal()==IDOK)
				{
					bool m_change_visible=dlg.m_part_visible;
					if(m_change_visible!=m_origin_visible)//����ģ�͵Ŀɼ���
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
					
					//pDoc->m_widget_on=FALSE;//����ʾ��������bar

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
					iter++;                          //�õ����ݼ�����Ӧģ�����ݵ�ָ��

	//ת����ģ��
	if( pDoc->pEntity->surfEdges != NULL)
	{
		delete pDoc->pEntity->surfEdges;
		pDoc->pEntity->surfEdges = NULL;
	}

	pDoc->pEntity->surfEdges = new EdgeFromSurface();
	pDoc->pEntity->surfEdges->SetTopologyModel((*iter)->GetTopologyShape());
	pDoc->pEntity->surfEdges->SetEdgePrecision(0.01);
	pDoc->pEntity->surfEdges->ConvertModel();

	//���֮ǰ����
	pDoc->pEdgesCADActorColletion->RemoveAllItems();
	pDoc->pEntity->edgeNames.clear();
	pDoc->pEntity->highLightEdge.clear();

    //��ʾ��ģ��
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