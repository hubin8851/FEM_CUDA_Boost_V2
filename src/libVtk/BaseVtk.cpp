#include "BaseVtk.h"
#include <VtkPreDef.h>

BaseVtk::BaseVtk() : m_wide(500), m_height(500), 
					m_SumNodeNum(0), m_SumElmtNum(0), m_LabelNum(5),
					bScalarLabel(true)
{
	
}

BaseVtk::BaseVtk(unsigned int _wide, unsigned int _height): m_wide(_wide), m_height(_height),
			m_SumNodeNum(0), m_SumElmtNum(0), m_LabelNum(5),bScalarLabel(true)
{

}

BaseVtk::~BaseVtk()
{
}

UserStatusError_t BaseVtk::InstanceNodes(vtkSmartPointer<vtkPoints> _Node, _NodeIter _NIter)
{
	m_SumNodeNum += _NIter->second->size();	//当前节点

	for (size_t i = 0; i < m_SumNodeNum; i++)
	{
		vtkIdType _nodeId = _Node->InsertNextPoint(_NIter->second->at(i)._X,
			_NIter->second->at(i)._Y,
			_NIter->second->at(i)._Z);
	}

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::InstanceElmts(vtkSmartPointer<vtkCellArray> _Elmt, _ElmtIter _EIter)
{
	vtkIdType _dim = _EIter->second->GetDim();	//当前种类单元内含的节点数
	HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>* _pMat = _EIter->second.get();
	vtkIdType* _tmpVec = new vtkIdType[_dim];		//分配内存
	m_SumElmtNum += _pMat->GetRowNum();	//计数器累加
	for (size_t i = 0; i < _pMat->GetRowNum(); i++)
	{
		for (size_t j = 0; j < _dim; j++)
		{
			_tmpVec[j] = (*_pMat)(i, j);
		}
		vtkIdType _id = _Elmt->InsertNextCell(_dim, _tmpVec);//插入一个单元
	}
	delete[] _tmpVec;

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

vtkFloatArray * BaseVtk::GetCurData()
{
	return static_cast<vtkFloatArray*>
		(m_curvaturesFilter->GetOutput()->GetPointData()->GetArray("Gauss_Curvature"));
}

UserStatusError_t BaseVtk::Instance()
{

	m_Actor = vtkSmartPointer<vtkActor>::New();
	m_Actor->SetMapper(m_Mapper);

	vtkCamera *camera = vtkCamera::New();
	camera->SetPosition(1, 1, 1);
	camera->SetFocalPoint(0, 0, 0);

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	m_Renderer->AddActor(m_Actor);
	if (bScalarLabel)
	{
		m_Renderer->AddActor2D(m_scalarBar);
	}
	m_Renderer->SetActiveCamera(camera);

	m_RenderWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWin->AddRenderer(m_Renderer);

	m_RenderWin->SetSize(m_wide, m_height);	//设置窗口尺寸
	m_RenderWin->SetWindowName("vtk测试");
//	m_RenderWin->Render();		//渲染器

//	SetBackground(m_Renderer, );

	// Make an oblique view
// 	m_Renderer->GetActiveCamera()->Azimuth(30);
// 	m_Renderer->GetActiveCamera()->Elevation(30);
//	m_Renderer->ResetCamera();	//重置相机

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::Reset()
{
	m_SumNodeNum = 0;
	m_SumElmtNum = 0;
}

void BaseVtk::ResizeWin(unsigned int _wide, unsigned int _height)
{
	this->m_wide = _wide;
	this->m_height = _height;
}

UserStatusError_t BaseVtk::SetData(HBXFEMDef::InputRecord* _ir)
{
	this->Reset();
	HBXFEMDef::DynamicInputRecord* _dynRecord = static_cast<HBXFEMDef::DynamicInputRecord*>(_ir);

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> _pElmt = vtkSmartPointer<vtkCellArray>::New();	//为单元容器分配内存;

	m_PolyCube = vtkSmartPointer<vtkPolyData>::New();		//即定义多边形数据集
	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();		//标量存储器
	m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();	//定义映射器

	//传入所有的节点坐标数据
	_NodeIter _Niter;
	for ( _Niter = _dynRecord->GetNodeArray()->begin(); _Niter != _dynRecord->GetNodeArray()->end(); _Niter++)
	{
		std::string _PartName = _Niter->first;		//获取当前零件名称
		this->InstanceNodes(points, _Niter);
#ifdef _DEBUG
	std::cout << "当前块:" << " 共计节点" << points->GetNumberOfPoints() << std::endl;
#endif // _DEBUG
	}
	
	_ElmtIter _Eiter;
	//绘制所有的cell，当前为单元下围成其所有的节点
	for ( _Eiter = _dynRecord->GetNSortMat()->begin(); _Eiter != _dynRecord->GetNSortMat()->end(); _Eiter++)
	{
		std::string _ElmtName = _Eiter->first;		//获取当前节点名称
		this->InstanceElmts(_pElmt, _Eiter);
		m_MElmt.insert(std::make_pair(_ElmtName, _pElmt));//放入名称-单元映射表，便于日后的选取
#ifdef _DEBUG
		std::cout << " 单元 " << _ElmtName << "共计" << _pElmt->GetNumberOfCells() << std::endl;
#endif // _DEBUG

	}

	//设置构成多边形的几何数据
	m_PolyCube->SetPoints(points);
	//设置单元类型，多边形
	m_PolyCube->SetPolys(_pElmt);	
	
//	m_vMappers[_ii]->SetInputData(PolyCube);
//	m_vMappers[_ii]->SetScalarRange(0, _dm->_vNode.size() - 1);

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::SetScalar(vtkDataArray * _inArray)
{
	m_PolyCube->GetPointData()->SetScalars(_inArray);
}

void BaseVtk::SetColorTable()
{
	//定义颜色映射表
	m_ColorTable = vtkSmartPointer<vtkLookupTable>::New();
	//设置颜色表中的颜色
	m_ColorTable->SetHueRange(0.0, 0.6);        //色调范围从红色到蓝色
	m_ColorTable->SetAlphaRange(1.0, 1.0);
	m_ColorTable->SetValueRange(1.0, 1.0);
	m_ColorTable->SetSaturationRange(1.0, 1.0);
	m_ColorTable->SetNumberOfTableValues(256);	//与setnumberofcolors一样
	m_ColorTable->SetRange(m_scalarRange);
	m_ColorTable->Build();

	m_Mapper->SetLookupTable(m_ColorTable);
}

void BaseVtk::SetScalarBar()
{
	m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	m_scalarBar->SetLookupTable( m_Mapper->GetLookupTable() );
//	m_scalarBar->SetTitle("max");
	m_scalarBar->SetNumberOfLabels(m_LabelNum);
}

void BaseVtk::FreshWithNum()
{
	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();
	for (int i = 0; i < m_SumElmtNum; i++)
		m_Scalar->InsertTuple1(i,i);
	m_PolyCube->GetCellData()->SetScalars(m_Scalar);

	m_Mapper->SetInputData(m_PolyCube);
	m_scalarRange[0] = 0;
	m_scalarRange[1] = m_SumElmtNum;
	m_Mapper->SetScalarRange(m_scalarRange);
}


void BaseVtk::FreshWithCur()
{
	//创建曲率属性智能指针
	//https://blog.csdn.net/shenziheng1/article/details/54846277
	m_curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();
	m_curvaturesFilter->SetInputData(m_PolyCube);
//	m_curvaturesFilter->SetCurvatureTypeToMaximum();	//最大曲率
//	m_curvaturesFilter->SetCurvatureTypeToMinimum();	//最小曲率
//	m_curvaturesFilter->SetCurvatureTypeToGaussian();	//高斯曲率
	m_curvaturesFilter->SetCurvatureTypeToMean();		//平均曲率
	m_curvaturesFilter->Update();

	m_curvaturesFilter->GetOutput()->GetScalarRange(m_scalarRange);
	
	m_PolyCube->GetPointData()->SetScalars(this->GetCurData());

//	m_Mapper->SetInputData(m_curvaturesFilter->GetOutput());
	m_Mapper->SetInputData(m_PolyCube);
	m_Mapper->SetScalarRange(m_scalarRange);
	//存储标量值
//	vtkFloatArray *scalars = vtkFloatArray::New();
	//设定每个顶点的标量值
//	for (vtkIdType i = 0; i < m_SumElmtNum; i++)scalars->InsertTuple1(i, i);
//	m_PolyCube->GetPointData()->SetScalars(scalars);
}

UserStatusError_t BaseVtk::Paint()
{
	m_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_Interactor->SetRenderWindow(m_RenderWin);
	//开始绘制
	m_Interactor->Initialize();
	//设置交互模式
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_Interactor->SetInteractorStyle(style);
	m_Interactor->Start();

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::Draw()
{
	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::SetBackground(vtkSmartPointer<vtkRenderer> _render, double * _rgb)
{
	_render->SetBackground(_rgb);
}
