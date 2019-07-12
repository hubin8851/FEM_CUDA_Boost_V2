#include "stdafx.h"
#include <..\src\libReader\InpDataReader.h>
#include "vtktest.h"


#include <Vtk8.0/vtkSmartPointer.h>

#include <Vtk8.0/vtkActor.h>
#include <Vtk8.0/vtkDelaunay2D.h>
#include <Vtk8.0/vtkLookupTable.h>
#include <Vtk8.0/vtkMath.h>
#include <Vtk8.0/vtkPointData.h>
#include <Vtk8.0/vtkPoints.h>
#include <Vtk8.0/vtkPolyData.h>
#include <Vtk8.0/vtkPolyDataMapper.h>
#include <Vtk8.0/vtkProperty.h>
#include <Vtk8.0/vtkRenderWindow.h>
#include <Vtk8.0/vtkRenderWindowInteractor.h>
#include <Vtk8.0/vtkRenderer.h>
#include <Vtk8.0/vtkVertexGlyphFilter.h>
#include <Vtk8.0/vtkXMLPolyDataWriter.h>

#include <Vtk8.0/vtkPlaneSource.h>
#include <Vtk8.0/vtkNamedColors.h>

void vtktest(void * void_arg)
{
	if (0)
	{
		// Provide some geometry
		int resolution = 3;
		vtkSmartPointer<vtkPlaneSource> aPlane =
			vtkSmartPointer<vtkPlaneSource>::New();
		aPlane->SetXResolution(resolution);
		aPlane->SetYResolution(resolution);

		// Create cell data
		vtkSmartPointer<vtkFloatArray> cellData =
			vtkSmartPointer<vtkFloatArray>::New();
		for (int i = 0; i < resolution * resolution; i++)
		{
			cellData->InsertNextValue(i + 1);
		}

		// Create a lookup table to map cell data to colors
		vtkSmartPointer<vtkLookupTable> lut =
			vtkSmartPointer<vtkLookupTable>::New();
		int tableSize = std::max(resolution*resolution + 1, 10);
		lut->SetNumberOfTableValues(tableSize);
		lut->Build();

		// Fill in a few known colors, the rest will be generated if needed
		vtkSmartPointer<vtkNamedColors> colors =
			vtkSmartPointer<vtkNamedColors>::New();
		lut->SetTableValue(0, colors->GetColor4d("Black").GetData());
		lut->SetTableValue(1, colors->GetColor4d("Banana").GetData());
		lut->SetTableValue(2, colors->GetColor4d("Tomato").GetData());
		lut->SetTableValue(3, colors->GetColor4d("Wheat").GetData());
		lut->SetTableValue(4, colors->GetColor4d("Lavender").GetData());
		lut->SetTableValue(5, colors->GetColor4d("Flesh").GetData());
		lut->SetTableValue(6, colors->GetColor4d("Raspberry").GetData());
		lut->SetTableValue(7, colors->GetColor4d("Salmon").GetData());
		lut->SetTableValue(8, colors->GetColor4d("Mint").GetData());
		lut->SetTableValue(9, colors->GetColor4d("Peacock").GetData());

		aPlane->Update(); // Force an update so we can set cell data
		aPlane->GetOutput()->GetCellData()->SetScalars(cellData);

		// Setup actor and mapper
		vtkSmartPointer<vtkPolyDataMapper> mapper =
			vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(aPlane->GetOutputPort());
		mapper->SetScalarRange(0, tableSize - 1);
		mapper->SetLookupTable(lut);

		vtkSmartPointer<vtkActor> actor =
			vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		// Setup render window, renderer, and interactor
		vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();
		vtkSmartPointer<vtkRenderWindow> renderWindow =
			vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow->AddRenderer(renderer);
		vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		renderWindowInteractor->SetRenderWindow(renderWindow);
		renderer->AddActor(actor);
		renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
		renderWindow->Render();
		renderWindowInteractor->Start();
	}

	if (1)
	{
		using namespace HBXFEMDef;
		InpDataReader g_InpDataReader("2D4NShell.inp", "F:\\data_from_HBX_phd\\database\\2D4NShell\\");
		g_InpDataReader.SetEltPropFilePath("EltProperty.xml", "F:\\data_from_HBX_phd\\vs2015\\FEM_CUDA_Boost_V2\\");
		g_InpDataReader.SetInputData();
		InputRecord* g_Record = g_InpDataReader.GetInputRecord();


		BaseVtk g_VTK;
		g_VTK.Initial();

		int scalar[50] = {	1,2,3,4,5,1,2,3,4,5,
							2,3,4,5,1,2,3,4,5,1,
							3,4,5,1,2,3,4,5,1,2,
							4,5,1,2,3,4,5,1,2,3,
							5,1,2,3,4,5,1,2,3,4 };

		//	g_VTK.SetData("first_stage.stl", "C:\\Users\\hbx\\Desktop\\");
		g_VTK.SetData(g_Record);
		g_VTK.SetScalar(scalar, 50, 5);
		//	g_VTK.AutoFreshNodeNum();
		g_VTK.SetColorTable(HBXDef::ColorType_t::CUSTOM, 5);
		g_VTK.SetScalarBar();
		//
		g_VTK.Instance();
		g_VTK.Run();
	}
	
}
