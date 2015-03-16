/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInteractorStyleTrackballActorQuery.h

  Copyright (c) Bing Yi
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// this class inherit from interactorstlye for actor model match,
// we disable the style of ivtkInteractorStyleTrackballActorMatch of scaling
// for that in model match, we should scale the model

// .SECTION See Also
// vtkInteractorStyleTrackballCamera vtkInteractorStyleJoystickActor
// vtkInteractorStyleJoystickCamera

#ifndef __vtkInteractorStyleTrackballActorQuery_h
#define __vtkInteractorStyleTrackballActorQuery_h

#include "stdafx.h"
#include "..\inc\DockTool.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkFloatArray.h>
#include <vtkPointPicker.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>
#include "PointCloudMeasureDoc.h"

class vtkInteractorStyleTrackballActorQuery : public vtkInteractorStyleTrackballCamera
{
	//属性
private:
	vtkFloatArray *value;
	vtkFloatArray *displace;
    CMessageViewDockBar *bar;
	vtkRenderer *pickRender;
	CPointCloudMeasureDoc *pDoc;
public:
    static vtkInteractorStyleTrackballActorQuery* New();
    vtkTypeMacro(vtkInteractorStyleTrackballActorQuery, vtkInteractorStyleTrackballCamera);

	vtkInteractorStyleTrackballActorQuery()
	{
		this->value = vtkFloatArray::New();
		this->displace = vtkFloatArray::New();
		bar = NULL;
		pickRender = NULL;
		pDoc = NULL;
	}

	~vtkInteractorStyleTrackballActorQuery()
	{
		if(this->value)
			this->value->Delete();

		if(this->displace)
			this->displace->Delete();

		bar = NULL;
		pickRender = NULL;
		pDoc = NULL;
	}

	//设置valueArray，从而可以查询，该函数可以通过重载，支持向量等数组的查询
	void SetValueArray(vtkDataArray *val)
	{
		int num = val->GetNumberOfTuples();

		this->value->Allocate(num);

		//copy属性值
		for(int i = 0; i < num; i++)
		{
			this->value->InsertNextTuple1(val->GetTuple1(i));
		}
	}


	//设置valueArray，从而可以查询，该函数可以通过重载，支持向量等数组的查询
	void SetDisplaceArray(vtkDataArray *val)
	{
		int num = val->GetNumberOfTuples();
		int nComp = val->GetNumberOfComponents();
		this->displace->SetNumberOfComponents(nComp);
		this->displace->Allocate(num * nComp);

		//copy属性值
		for(int i = 0; i < num; i++)
		{
			double *p = val->GetTuple3(i);
			this->displace->InsertNextTuple3(*p, *(p+1), *(p+2));
		}
	}

	//输入当前的Mainframe，从而可以显示属性值
	void SetInputMessageBar(CMessageViewDockBar *messageBar)
	{
		this->bar = messageBar;
	}

	void SetPickRenderer(vtkRenderer *ren)
	{
		pickRender = ren;
	}

	void SetDocument(CPointCloudMeasureDoc *doc)
	{
		pDoc = doc;
	}
 
    virtual void OnLeftButtonDown()
    {
      int* clickPos = this->GetInteractor()->GetEventPosition();
 
      // Pick from this location.
	  vtkPointPicker *picker = vtkPointPicker::New();
      picker->Pick(clickPos[0], clickPos[1], 0, pickRender);
 
	  //当前属性值输出
      if(picker->GetActor() && picker->GetActor()->GetVisibility())
	  {
		  int id = picker->GetPointId();
		  double point[3];
		  picker->GetPickPosition(point);

		  if(id != -1 && value && id < value->GetNumberOfTuples())
		  {
			  CString str;
			  str.Format("Point id: %d,    value: %f", id, value->GetTuple1(id));
			  bar->AddMessageLine(str);

			  double *cDis = displace->GetTuple3(id);
			  CString strDis;
			  strDis.Format("%f   %f   %f", *cDis, *(cDis+1), *(cDis+2));

			  pDoc->AddCaptionWidget(strDis, point);
		  }
	  }

	  picker->Delete();
     
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
 
};

vtkStandardNewMacro(vtkInteractorStyleTrackballActorQuery);

#endif