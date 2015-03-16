# This VTK_DIR usage only works in the VTK build tree...
# Make it work in the "make install" tree, too.
#

PROJECT(PointMeasurement)

FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF(NOT VTK_DIR)
INCLUDE(${VTK_USE_FILE})

INCLUDE("${VTK_DIR}/GUISupport/MFC/VTKMFCSettings.cmake")
IF(VTK_MFC_DELAYLOAD_VTK_DLLS)
  VTK_MFC_ADD_DELAYLOAD_FLAGS(CMAKE_EXE_LINKER_FLAGS
    vtkMFC.dll
    vtkRendering.dll
    vtkIO.dll
    vtkFiltering.dll
    vtkCommon.dll
    )
ENDIF(VTK_MFC_DELAYLOAD_VTK_DLLS)

SET( MEASUREMENT_SRCS 
 MainFrm
 stdafx
 Entity.cpp
 PointCloudMeasure.rc
 MyCameraDlg
 OutPutDlg
 PartProcess
 PartTreeView
 PointCloudMeasure
 PointCloudMeasureDoc
 PointCloudMeasureView
 vtkHausdorffDistancePointSetFilter
 vtkInteractorStyleTrackballActorMatch
 vtkScanPointReader
 vtkScanPointWriter
 BasicGeometryConvertion
 EdgeConvertion
 EdgeFromSurface
 IGESToVTKConvertion
 MeasurementDialog
 MeasureWithBasicGeometry
 MeasureWithEdge
 MeasureWithSurface
 SurfaceConvertion
 VertexConvertion
 res/PointCloudMeasure.rc2
) 

ADD_EXECUTABLE(PointCloudMeasure WIN32 ${MEASUREMENT_SRCS})

IF(VTK_MFC_EXTRA_LIBS)
  TARGET_LINK_LIBRARIES(PointCloudMeasure ${VTK_MFC_EXTRA_LIBS})
ENDIF(VTK_MFC_EXTRA_LIBS)

TARGET_LINK_LIBRARIES(PointCloudMeasure vtkMFC)
