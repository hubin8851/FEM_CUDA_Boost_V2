#include "BaseVtk.h"
#include <VtkPreDef.h>

bool BaseVtk::bScalarLabel = false;		//是否显示标签栏

vtkIdType  BaseVtk::m_SumNodeNum = 0;	//单元总数
vtkIdType  BaseVtk::m_SumElmtNum = 0;	//单元总数
int		 BaseVtk::m_LabelNum = 0;	//标签分类个数

double BaseVtk::m_scalarRange[2] = { 0, 0 };	//属性范围

vtkSmartPointer<vtkFloatArray> BaseVtk::m_Scalar = nullptr;
vtkSmartPointer<vtkPolyDataAlgorithm> BaseVtk::m_AlgorithmFilter = nullptr;
vtkSmartPointer<vtkRenderWindowInteractor> BaseVtk::m_Interactor = nullptr;//交互器
vtkSmartPointer<vtkScalarBarActor> BaseVtk::m_scalarBar = nullptr;	//标签栏

vtkSmartPointer<vtkPolyDataMapper>   BaseVtk::m_Mapper = nullptr;	//映射器
vtkSmartPointer<vtkRenderer>  BaseVtk::m_Renderer = nullptr;	//绘制类

BaseVtk::BaseVtk() : m_wide(500), m_height(500) 
{
	Initial();
}

BaseVtk::BaseVtk(unsigned int _wide, unsigned int _height): m_wide(_wide), m_height(_height)
{
	Initial();
}

BaseVtk * BaseVtk::New()
{
	static BaseVtk _tmpClass;
	return &_tmpClass;
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

vtkFloatArray * BaseVtk::GetCurData(vtkPolyDataAlgorithm* _dataIn)
{
	return static_cast<vtkFloatArray*>
		(_dataIn->GetOutput()->GetPointData()->GetArray("Gauss_Curvature"));
}

UserStatusError_t BaseVtk::Initial()
{
	
	m_AlgorithmFilter = vtkSmartPointer<vtkPolyDataAlgorithm>::New();

	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();		//标量存储器
	m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

	m_MyCallback = vtkSmartPointer<vtkCallbackCommand>::New();

	m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();	//定义映射器

	m_Actor = vtkSmartPointer<vtkActor>::New();
	m_Actor->SetMapper(m_Mapper);

	vtkCamera *camera = vtkCamera::New();

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	m_Renderer->AddActor(m_Actor);
	m_Renderer->SetActiveCamera(camera);

	m_RenderWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWin->AddRenderer(m_Renderer);

	m_RenderWin->SetSize(m_wide, m_height);	//设置窗口尺寸
	m_RenderWin->SetWindowName("vtk测试");

	m_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_Interactor->SetRenderWindow(m_RenderWin);
	//开始绘制
	m_Interactor->Initialize();
	//设置交互模式
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_Interactor->SetInteractorStyle(style);

	return UserStatusError_t();
}

UserStatusError_t BaseVtk::Instance()
{
	m_Mapper->SetInputData( m_AlgorithmFilter->GetOutput() );
	m_Mapper->SetScalarRange(m_scalarRange);

//	m_RenderWin->Render();		//渲染器

//	SetBackground(m_Renderer, );

	// Make an oblique view
 	m_Renderer->GetActiveCamera()->Azimuth(30);
 	m_Renderer->GetActiveCamera()->Elevation(30);
//	m_Renderer->GetActiveCamera()->tr
	m_Renderer->ResetCamera();	//重置相机

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
	vtkSmartPointer<vtkPolyData> _PolyCube = vtkSmartPointer<vtkPolyData>::New();		//即定义多边形数据集


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
	_PolyCube->SetPoints(points);
	//设置单元类型，多边形
	_PolyCube->SetPolys(_pElmt);	
	
	m_AlgorithmFilter->SetOutput( _PolyCube );
//	m_vMappers[_ii]->SetInputData(PolyCube);
//	m_vMappers[_ii]->SetScalarRange(0, _dm->_vNode.size() - 1);

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::SetData( const char * _FileName, boost::filesystem::path  _path)
{

	vtkSmartPointer<vtkSTLReader> _StlInstance = vtkSmartPointer<vtkSTLReader>::New();
	vtkSmartPointer<vtkPolyData>_PolyCube = vtkSmartPointer<vtkPolyData>::New();		//即定义多边形数据集

	boost::filesystem::path _tmppath(_path);
	_tmppath.append(_FileName);

	_StlInstance->SetOutput(_StlInstance->GetOutput());
	_StlInstance->SetFileName(_tmppath.string().c_str());
	_StlInstance->Update();

	m_SumNodeNum = _StlInstance->GetOutput()->GetPoints()->GetNumberOfPoints();
	m_SumElmtNum = _StlInstance->GetOutput()->GetCellData()->GetNumberOfArrays();
	_PolyCube->GetNumberOfPoints();
	std::cout << "stl模型中共计节点" << m_SumNodeNum << std::endl;

	m_AlgorithmFilter = vtkSTLReader::SafeDownCast(_StlInstance);

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::SetScalar(vtkDataArray * _inArray)
{
	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(_inArray);
}

void BaseVtk::CallbackFunc(vtkObject * _caller, unsigned long _eventId, void* clientdata, void * _callData)
{
	std::cout << "callbackFunc ing..." << std::endl;



}

void BaseVtk::SetColorTable()
{
	//定义颜色映射表
	m_ColorTable = vtkSmartPointer<vtkLookupTable>::New();
	//设置颜色表中的颜色
	m_ColorTable->SetHueRange(0.6, 0.0);        //色调范围从红色到蓝色
	m_ColorTable->SetAlphaRange(1.0, 1.0);
	m_ColorTable->SetValueRange(1.0, 1.0);
	m_ColorTable->SetSaturationRange(1.0, 1.0);
	m_ColorTable->SetNumberOfTableValues(512);	//与setnumberofcolors一样
	m_ColorTable->SetRange(m_scalarRange);
	m_ColorTable->Build();

	m_Mapper->SetLookupTable(m_ColorTable);
}

void BaseVtk::SetScalarBar()
{
	m_scalarBar->SetLookupTable( m_Mapper->GetLookupTable() );
	if (nullptr == m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars())//如果属性向量里没有数据则直接返回
	{
		return;
	}
	if (nullptr != m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars()->GetName())
	{
		m_scalarBar->SetTitle(m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
	}
	m_scalarBar->SetNumberOfLabels(m_LabelNum);
}

void BaseVtk::AutoFreshNodeNum()
{
	m_MyCallback->SetCallback( BaseVtk::SetNodeNum );

	m_Interactor->Initialize();
	m_Interactor->CreateRepeatingTimer(50);//设置50ms刷新一次，也就是一秒20帧
	m_Interactor->AddObserver( vtkCommand::TimerEvent, m_MyCallback );
}

void BaseVtk::SetNodeNum(vtkObject * _caller, unsigned long _eventId, void* clientdata, void * _callData)
{
	for (int i = 0; i < m_SumNodeNum; i++)
		m_Scalar->InsertTuple1(i, i);

	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(m_Scalar);

	m_scalarRange[0] = 0;
	m_scalarRange[1] = m_SumNodeNum;

	Paint();
}

void BaseVtk::SetCellNum()
{
	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();
	for (int i = 0; i < m_SumElmtNum; i++)
		m_Scalar->InsertTuple1(i,i);

	m_AlgorithmFilter->GetOutput()->GetCellData()->SetScalars(m_Scalar);

	m_scalarRange[0] = 0;
	m_scalarRange[1] = m_SumElmtNum;
}

void BaseVtk::SetNormal(HBXDef::NormalType_t _nt)
{
	//https://blog.csdn.net/shenziheng1/article/details/54845071
	vtkSmartPointer<vtkPolyDataNormals> _NormalsFilter = vtkSmartPointer<vtkPolyDataNormals>::New();
	_NormalsFilter->SetInputData(m_AlgorithmFilter->GetOutput());
	_NormalsFilter->SetComputePointNormals(1);//开启点法向量计算  
	_NormalsFilter->SetComputeCellNormals(0); //关闭单元法向量计算
	_NormalsFilter->SetAutoOrientNormals(1);
	_NormalsFilter->SetSplitting(0);
	_NormalsFilter->Update();

	//创建mark点
	vtkSmartPointer<vtkMaskPoints> _mask =
		vtkSmartPointer<vtkMaskPoints>::New();
	_mask->SetInputData(_NormalsFilter->GetOutput());
	_mask->SetMaximumNumberOfPoints(300);
	_mask->RandomModeOn();
	_mask->Update();

	//构建箭头
	vtkSmartPointer<vtkArrowSource> _arrow =
		vtkSmartPointer<vtkArrowSource>::New();
	_arrow->Update(); //一定要更新 否则数据没有添加进来，程序会报错

	vtkSmartPointer<vtkGlyph3D> _glyph =
		vtkSmartPointer<vtkGlyph3D>::New();
	_glyph->SetInputData(_mask->GetOutput());
	_glyph->SetSourceData(_arrow->GetOutput());//每一点用箭头代替  
	_glyph->SetVectorModeToUseNormal();//设置向量显示模式和法向量一致  
	_glyph->SetScaleFactor(0.01); //设置伸缩比例  
	_glyph->Update();


	m_scalarRange[0] = 0;
	m_scalarRange[1] = m_SumNodeNum;

	//未完待续...
}


void BaseVtk::SetCur(HBXDef::CurvatureType_t _t)
{
	//创建曲率属性智能指针
	//https://blog.csdn.net/shenziheng1/article/details/54846277
	vtkSmartPointer<vtkCurvatures> _curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();
	_curvaturesFilter->SetInputData(m_AlgorithmFilter->GetOutput());

	switch (_t)
	{
	case  HBXDef::CurvatureType_t::MAX:
		_curvaturesFilter->SetCurvatureTypeToMaximum();	//最大曲率
		break;
	case HBXDef::CurvatureType_t::MIN:
		_curvaturesFilter->SetCurvatureTypeToMinimum();	//最小曲率
		break;
	case HBXDef::CurvatureType_t::GAUSS:
		_curvaturesFilter->SetCurvatureTypeToGaussian();	//高斯曲率
		break;
	case HBXDef::CurvatureType_t::MEAN:
		_curvaturesFilter->SetCurvatureTypeToMean();		//平均曲率
		break;
	}
	
	_curvaturesFilter->Update();

	_curvaturesFilter->GetOutput()->GetScalarRange(m_scalarRange);
	
	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(this->GetCurData(_curvaturesFilter));

	m_AlgorithmFilter = vtkCurvatures::SafeDownCast(_curvaturesFilter);

//	m_Mapper->SetInputData(m_curvaturesFilter->GetOutput());
	//存储标量值
//	vtkFloatArray *scalars = vtkFloatArray::New();
	//设定每个顶点的标量值
//	for (vtkIdType i = 0; i < m_SumElmtNum; i++)scalars->InsertTuple1(i, i);
//	m_PolyCube->GetPointData()->SetScalars(scalars);
}

UserStatusError_t BaseVtk::Paint()
{
	if (bScalarLabel)
	{
		m_Renderer->AddActor2D(m_scalarBar);
	}

	m_Interactor->Render();

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::Draw()
{
	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::Run()
{
	m_Interactor->Start();
}


void BaseVtk::SetBackground(vtkSmartPointer<vtkRenderer> _render, double * _rgb)
{
	_render->SetBackground(_rgb);
}
