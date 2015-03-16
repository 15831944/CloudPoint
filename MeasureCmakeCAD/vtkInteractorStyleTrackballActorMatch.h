/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInteractorStyleTrackballActorMatchMatch.h

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

#ifndef __vtkInteractorStyleTrackballActorMatch_h
#define __vtkInteractorStyleTrackballActorMatch_h

#include "vtkInteractorStyle.h"

class vtkCellPicker;

class vtkInteractorStyleTrackballActorMatch : public vtkInteractorStyle
{
public:
  static vtkInteractorStyleTrackballActorMatch *New();
  vtkTypeMacro(vtkInteractorStyleTrackballActorMatch,vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Event bindings controlling the effects of pressing mouse buttons
  // or moving the mouse.
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();

  // These methods for the different interactions in different modes
  // are overridden in subclasses to perform the correct motion. Since
  // they might be called from OnTimer, they do not have mouse coord parameters
  // (use interactor's GetEventPosition and GetLastEventPosition)
  virtual void Rotate();
  virtual void Spin();
  virtual void Pan();
  virtual void Dolly();
  virtual void UniformScale();

protected:
  vtkInteractorStyleTrackballActorMatch();
  ~vtkInteractorStyleTrackballActorMatch();

  void FindPickedActor(int x, int y);

  void Prop3DTransform(vtkProp3D *prop3D,
                       double *boxCenter,
                       int NumRotation,
                       double **rotate,
                       double *scale);
  
  double MotionFactor;

  vtkProp3D *InteractionProp;
  vtkCellPicker *InteractionPicker;

private:
  vtkInteractorStyleTrackballActorMatch(const vtkInteractorStyleTrackballActorMatch&);  // Not implemented.
  void operator=(const vtkInteractorStyleTrackballActorMatch&);  // Not implemented.
};

#endif
