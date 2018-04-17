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




// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int main()
{
	if (1)
	{
		InterpolateTest();
	}


	if (0)
	{
		vtktest();
	}

    int nRetCode = 0;


	HBXFEMDef::BaseReader* xxx = HBXFEMDef::InstanceInpReader();
	if (0)
	{
		using namespace HBXFEMDef;
		xxx->SetSourceFilePath("spaceshuttle_quadmain.inp", "C:\\Users\\hbx\\Desktop\\");
//		xxx->SetSourceFilePath("beam.inp", "D:\\AbaqusTemp"); 
		xxx->SetInputData();
		
//		HBXFEMDef::Engng* em = HBXFEMDef::InstanceProblem(xxx, problemMode_t::_processor, 0);
	}

	BaseVtk g_VTK;
	g_VTK.Initial();

	g_VTK.SetData("first_stage.stl", "C:\\Users\\hbx\\Desktop\\");
//	g_VTK.SetData(xxx->GetInputRecord());
	g_VTK.AutoFreshNodeNum();
	g_VTK.SetColorTable();
	g_VTK.SetScalarBar();

	g_VTK.Instance();
	g_VTK.Run();



#pragma region MFC相关
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }
#pragma endregion
    return nRetCode;
}


