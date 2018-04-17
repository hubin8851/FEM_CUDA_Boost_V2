// FEM_CUDA_Boost_V2.cpp : �������̨Ӧ�ó������ڵ㡣
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




// Ψһ��Ӧ�ó������

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



#pragma region MFC���
    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
        }
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }
#pragma endregion
    return nRetCode;
}


