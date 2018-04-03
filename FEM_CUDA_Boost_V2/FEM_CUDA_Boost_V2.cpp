// FEM_CUDA_Boost_V2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FEM_CUDA_Boost_V2.h"
#include <HBXPreDef.h>
#include <libCUFEM\util.h>
#include <libReader\PWDataReader.h>
#include <libReader\InpDataReader.h>
#include <libEngng\LinearStaticEngng.h>
#include <libVtk\BaseVtk.h>
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void vtktest()
{
	int i;
	//���εĶ�������  
	static float x[8][3] = { { 0,0,0 },{ 4,0,0 },{ 4,4,0 },{ 0,4,0 },
	{ 1,1,1 },{ 3,1,1 },{ 3,3,1 },{ 1,3,1 } };
	//4�����㹹��һ����Ԫ��һ��6����Ԫ  
	static vtkIdType y[6][4] = { { 0,1,2,3 },{ 4,5,6,7 },{ 0,1,5,4 },
	{ 1,2,6,5 },{ 2,3,7,6 },{ 3,0,4,7 } };

	vtkPoints *points = vtkPoints::New();
	for (i = 0; i < 8; i++)
		points->InsertPoint(i, x[i]);

	vtkCellArray *polys = vtkCellArray::New();
	for (i = 0; i < 6; i++)
		polys->InsertNextCell(4, y[i]);
	//�洢����ֵ  
	vtkFloatArray *scalars = vtkFloatArray::New();
	for (i = 0; i < 8; i++)
		scalars->InsertTuple1(i, i);
	//�������������  
	vtkPolyData *cube = vtkPolyData::New();
	cube->SetPoints(points);
	//�趨��Ԫ����ɷ�ʽ  
	cube->SetPolys(polys);
	cube->GetPointData()->SetScalars(scalars);

	//������ɫӳ���  
	vtkLookupTable *pColorTable = vtkLookupTable::New();
	pColorTable->SetNumberOfColors(6);
	pColorTable->SetTableValue(0, 1.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(1, 0.0, 1.0, 1.0, 1.0);
	pColorTable->SetTableValue(2, 1.0, 1.0, 1.0, 1.0);
	pColorTable->SetTableValue(3, 1.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(4, 0.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(5, 1.0, 1.0, 0.0, 1.0);
	pColorTable->Build();

	//����ӳ��  
	vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
	cubeMapper->SetInputData(cube);
	cubeMapper->SetScalarRange(0, 7);
	cubeMapper->SetLookupTable(pColorTable);
	vtkActor *cubeActor = vtkActor::New();
	cubeActor->SetMapper(cubeMapper);

	vtkCamera *camera = vtkCamera::New();
	camera->SetPosition(1, 1, 1);
	camera->SetFocalPoint(0, 0, 0);

	vtkRenderer *renderer = vtkRenderer::New();
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	renderer->AddActor(cubeActor);
	renderer->SetActiveCamera(camera);
	renderer->ResetCamera();
	renderer->SetBackground(1, 1, 1);
	renWin->SetSize(400, 400);
	renWin->Render();
	iren->Start();
	//ɾ��  
	points->Delete();
	polys->Delete();
	scalars->Delete();
	cube->Delete();
	cubeMapper->Delete();
	cubeActor->Delete();
	camera->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	pColorTable->Delete();
	return;
}

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int main()
{
	if (0)
	{
		vtktest();
	}

    int nRetCode = 0;

	if (0)
	{
		std::string  stringLine = "      1,   31054.8535,  -2140.90234,   8901.31738";
		std::string  stringLine2 = "*Part, name=spaceshuttle2005";
		std::regex	IfNumRule("((\\s+[-]\\d+(\\.\\d+)?)[\,])+");	//�ڶ�ȡpw�ļ�ʱ�������������ʽ
		std::regex  IfNotNum("\\*+.*");	//������ʽ����*��ʼ���ַ���
		std::regex	NumRule("\\s*\\d+((\.\\d*)?)");
		std::regex  Rule3("\\s*\\d+(\\.\\d+)?(,|\\s)?");//ƥ�両����
		std::smatch  o;

	//	bool _rlt2 = std::regex_match(stringLine2, Rule3);
		int i = 0;
		std::vector<float> vFloat;
		std::sregex_iterator end_iter;

		bool _rlt = std::regex_match(stringLine2, IfNotNum);

		if (!std::regex_match(stringLine, IfNotNum))
		{
			auto begin_iter = std::sregex_iterator(stringLine.begin(), stringLine.end(), Rule3);
			for (auto it = begin_iter; it != end_iter; it++)
			{
				vFloat.emplace_back(std::move(std::stof(it->str())));
			}
		}

	}

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

	g_VTK.SetData("first_stage.stl", "C:\\Users\\hbx\\Desktop\\");
//	g_VTK.SetData(xxx->GetInputRecord());
	g_VTK.SetNodeNum();
	g_VTK.SetColorTable();
	g_VTK.SetScalarBar();

	g_VTK.Instance();
	g_VTK.Paint();



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


