#pragma once

#include <libVtk\BaseVtk.h>
void vtktest()
{
	int i;
	//梯形的顶点坐标  
	static float x[8][3] = { { 0,0,0 },{ 4,0,0 },{ 4,4,0 },{ 0,4,0 },
	{ 1,1,1 },{ 3,1,1 },{ 3,3,1 },{ 1,3,1 } };
	//4个顶点构成一个单元，一共6个单元  
	static vtkIdType y[6][4] = { { 0,1,2,3 },{ 4,5,6,7 },{ 0,1,5,4 },
	{ 1,2,6,5 },{ 2,3,7,6 },{ 3,0,4,7 } };

	vtkPoints *points = vtkPoints::New();
	for (i = 0; i < 8; i++)
		points->InsertPoint(i, x[i]);

	vtkCellArray *polys = vtkCellArray::New();
	for (i = 0; i < 6; i++)
		polys->InsertNextCell(4, y[i]);
	//存储标量值  
	vtkFloatArray *scalars = vtkFloatArray::New();
	for (i = 0; i < 8; i++)
		scalars->InsertTuple1(i, i);
	//构建多边形数据  
	vtkPolyData *cube = vtkPolyData::New();
	cube->SetPoints(points);
	//设定单元的组成方式  
	cube->SetPolys(polys);
	cube->GetPointData()->SetScalars(scalars);

	//定义颜色映射表  
	vtkLookupTable *pColorTable = vtkLookupTable::New();
	pColorTable->SetNumberOfColors(6);
	pColorTable->SetTableValue(0, 1.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(1, 0.0, 1.0, 1.0, 1.0);
	pColorTable->SetTableValue(2, 1.0, 1.0, 1.0, 1.0);
	pColorTable->SetTableValue(3, 1.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(4, 0.0, 0.0, 1.0, 1.0);
	pColorTable->SetTableValue(5, 1.0, 1.0, 0.0, 1.0);
	pColorTable->Build();

	//数据映射  
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
	//删除  
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