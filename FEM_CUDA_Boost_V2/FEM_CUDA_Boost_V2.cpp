// FEM_CUDA_Boost_V2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "FEM_CUDA_Boost_V2.h"
#include <HBXPreDef.h>
#include <libCUFEM\util.h>
#include <libReader\PWDataReader.h>
#include <libReader\InpDataReader.h>
#include <libEngng\LinearStaticEngng.h>
#include "vtktest.h"
#include "regextest.h"
#include "IntervalTest.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern "C" CUT_THREADPROC InterpolateTest(void* void_arg);
CUT_THREADPROC AsynInterpolateTest(void* void_arg);
extern "C" float MultiInterpolateTest(void* void_arg);

// 唯一的应用程序对象


using namespace std;

int main()
{
	if (1)
	{
//		InterpolateTest();	
		MultiInterpolateTest();
	}

	if (0)
	{
		vtktest();
	}

//
//	HBXFEMDef::BaseReader* xxx = HBXFEMDef::InstanceInpReader();
//	if (0)
//	{
//		using namespace HBXFEMDef;
//		xxx->SetSourceFilePath("spaceshuttle_quadmain.inp", "C:\\Users\\hbx\\Desktop\\");
////		xxx->SetSourceFilePath("beam.inp", "D:\\AbaqusTemp"); 
//		xxx->SetInputData();
//		
////		HBXFEMDef::Engng* em = HBXFEMDef::InstanceProblem(xxx, problemMode_t::_processor, 0);
//	}
//
//	BaseVtk g_VTK;
//	g_VTK.Initial();
//
//	g_VTK.SetData("first_stage.stl", "C:\\Users\\hbx\\Desktop\\");
////	g_VTK.SetData(xxx->GetInputRecord());
//	g_VTK.AutoFreshNodeNum();
//	g_VTK.SetColorTable();
//	g_VTK.SetScalarBar();
//
//	g_VTK.Instance();
//	g_VTK.Run();


    return 0;
}


