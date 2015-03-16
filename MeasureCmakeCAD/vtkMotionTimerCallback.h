//--------------------------------------------------------------
//模型运动对象
//--------------------------------------------------------------
#ifndef _MOTION_CALL_BACK_VTK_DEF_H_
#define _MOTION_CALL_BACK_VTK_DEF_H_

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include "vtkActor.h"
#include <vector>

using namespace std;

class vtkMotionTimerCallback : public vtkCommand
{
public:
	 public:
    static vtkMotionTimerCallback *New()
    {
      vtkMotionTimerCallback *motionCallback = new vtkMotionTimerCallback;
	  motionCallback->rotAngle = 0;
      return motionCallback;
    }

    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void * vtkNotUsed(callData))
    {
		if (vtkCommand::TimerEvent == eventId)
		{
			rotAngle = (rotAngle + 10) % 360;

			int size = index.size();
			for(int i =0; i < size; i++)
			{
				int id = index[i];
			    actors[id]->RotateZ(rotAngle);
			}
			
			vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
			iren->GetRenderWindow()->Render();
		}
    }

  private:
    int rotAngle;
  public:
    vector<vtkActor*> actors;
	vector<int> index;
};

#endif
