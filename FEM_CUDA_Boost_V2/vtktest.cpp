#include "stdafx.h"
#include <..\src\libReader\InpDataReader.h>
#include "vtktest.h"


void vtktest(void * void_arg)
{
	using namespace HBXFEMDef;
	InpDataReader g_InpDataReader("B31BEAMTEST.inp", "F:\\data_from_HBX_phd\\database\\B31Test\\");
	g_InpDataReader.SetEltPropFilePath("EltProperty.xml", "F:\\data_from_HBX_phd\\vs2015\\FEM_CUDA_Boost_V2\\");
	g_InpDataReader.SetInputData();
	InputRecord* g_Record = g_InpDataReader.GetInputRecord();

	
	BaseVtk g_VTK;
	g_VTK.Initial();

//	g_VTK.SetData("first_stage.stl", "C:\\Users\\hbx\\Desktop\\");
	g_VTK.SetData(g_Record);
//	g_VTK.AutoFreshNodeNum();
	g_VTK.SetColorTable();
	g_VTK.SetScalarBar();
//
	g_VTK.Instance();
	g_VTK.Run();
}
