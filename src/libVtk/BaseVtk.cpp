#include "BaseVtk.h"
#include <VtkPreDef.h>

bool BaseVtk::bScalarLabel = false;		//�Ƿ���ʾ��ǩ��

vtkIdType  BaseVtk::m_SumNodeNum = 0;	//��Ԫ����
vtkIdType  BaseVtk::m_SumElmtNum = 0;	//��Ԫ����
int		 BaseVtk::m_LabelNum = 0;	//��ǩ�������

double BaseVtk::m_scalarRange[2] = { 0, 0 };	//���Է�Χ

vtkSmartPointer<vtkFloatArray> BaseVtk::m_Scalar = nullptr;
vtkSmartPointer<vtkPolyDataAlgorithm> BaseVtk::m_AlgorithmFilter = nullptr;
vtkSmartPointer<vtkRenderWindowInteractor> BaseVtk::m_Interactor = nullptr;//������
vtkSmartPointer<vtkScalarBarActor> BaseVtk::m_scalarBar = nullptr;	//��ǩ��

vtkSmartPointer<vtkPolyDataMapper>   BaseVtk::m_Mapper = nullptr;	//ӳ����
vtkSmartPointer<vtkRenderer>  BaseVtk::m_Renderer = nullptr;	//������

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
	m_SumNodeNum += _NIter->second->size();	//��ǰ�ڵ�

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
	vtkIdType _dim = _EIter->second->GetDim();	//��ǰ���൥Ԫ�ں��Ľڵ���
	HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>* _pMat = _EIter->second.get();
	vtkIdType* _tmpVec = new vtkIdType[_dim];		//�����ڴ�
	m_SumElmtNum += _pMat->GetRowNum();	//�������ۼ�
	for (size_t i = 0; i < _pMat->GetRowNum(); i++)
	{
		for (size_t j = 0; j < _dim; j++)
		{
			_tmpVec[j] = (*_pMat)(i, j);
		}
		vtkIdType _id = _Elmt->InsertNextCell(_dim, _tmpVec);//����һ����Ԫ
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

	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();		//�����洢��
	m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

	m_MyCallback = vtkSmartPointer<vtkCallbackCommand>::New();

	m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();	//����ӳ����

	m_Actor = vtkSmartPointer<vtkActor>::New();
	m_Actor->SetMapper(m_Mapper);

	vtkCamera *camera = vtkCamera::New();

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	m_Renderer->AddActor(m_Actor);
	m_Renderer->SetActiveCamera(camera);

	m_RenderWin = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWin->AddRenderer(m_Renderer);

	m_RenderWin->SetSize(m_wide, m_height);	//���ô��ڳߴ�
	m_RenderWin->SetWindowName("vtk����");

	m_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_Interactor->SetRenderWindow(m_RenderWin);
	//��ʼ����
	m_Interactor->Initialize();
	//���ý���ģʽ
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	m_Interactor->SetInteractorStyle(style);

	return UserStatusError_t();
}

UserStatusError_t BaseVtk::Instance()
{
	m_Mapper->SetInputData( m_AlgorithmFilter->GetOutput() );
	m_Mapper->SetScalarRange(m_scalarRange);

//	m_RenderWin->Render();		//��Ⱦ��

//	SetBackground(m_Renderer, );

	// Make an oblique view
 	m_Renderer->GetActiveCamera()->Azimuth(30);
 	m_Renderer->GetActiveCamera()->Elevation(30);
//	m_Renderer->GetActiveCamera()->tr
	m_Renderer->ResetCamera();	//�������

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
	vtkSmartPointer<vtkCellArray> _pElmt = vtkSmartPointer<vtkCellArray>::New();	//Ϊ��Ԫ���������ڴ�;
	vtkSmartPointer<vtkPolyData> _PolyCube = vtkSmartPointer<vtkPolyData>::New();		//�������������ݼ�


	//�������еĽڵ���������
	_NodeIter _Niter;
	for ( _Niter = _dynRecord->GetNodeArray()->begin(); _Niter != _dynRecord->GetNodeArray()->end(); _Niter++)
	{
		std::string _PartName = _Niter->first;		//��ȡ��ǰ�������
		this->InstanceNodes(points, _Niter);
#ifdef _DEBUG
	std::cout << "��ǰ��:" << " ���ƽڵ�" << points->GetNumberOfPoints() << std::endl;
#endif // _DEBUG
	}
	
	_ElmtIter _Eiter;
	//�������е�cell����ǰΪ��Ԫ��Χ�������еĽڵ�
	for ( _Eiter = _dynRecord->GetNSortMat()->begin(); _Eiter != _dynRecord->GetNSortMat()->end(); _Eiter++)
	{
		std::string _ElmtName = _Eiter->first;		//��ȡ��ǰ�ڵ�����
		this->InstanceElmts(_pElmt, _Eiter);
		m_MElmt.insert(std::make_pair(_ElmtName, _pElmt));//��������-��Ԫӳ��������պ��ѡȡ
#ifdef _DEBUG
		std::cout << " ��Ԫ " << _ElmtName << "����" << _pElmt->GetNumberOfCells() << std::endl;
#endif // _DEBUG

	}

	//���ù��ɶ���εļ�������
	_PolyCube->SetPoints(points);
	//���õ�Ԫ���ͣ������
	_PolyCube->SetPolys(_pElmt);	
	
	m_AlgorithmFilter->SetOutput( _PolyCube );
//	m_vMappers[_ii]->SetInputData(PolyCube);
//	m_vMappers[_ii]->SetScalarRange(0, _dm->_vNode.size() - 1);

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::SetData( const char * _FileName, boost::filesystem::path  _path)
{

	vtkSmartPointer<vtkSTLReader> _StlInstance = vtkSmartPointer<vtkSTLReader>::New();
	vtkSmartPointer<vtkPolyData>_PolyCube = vtkSmartPointer<vtkPolyData>::New();		//�������������ݼ�

	boost::filesystem::path _tmppath(_path);
	_tmppath.append(_FileName);

	_StlInstance->SetOutput(_StlInstance->GetOutput());
	_StlInstance->SetFileName(_tmppath.string().c_str());
	_StlInstance->Update();

	m_SumNodeNum = _StlInstance->GetOutput()->GetPoints()->GetNumberOfPoints();
	m_SumElmtNum = _StlInstance->GetOutput()->GetCellData()->GetNumberOfArrays();
	_PolyCube->GetNumberOfPoints();
	std::cout << "stlģ���й��ƽڵ�" << m_SumNodeNum << std::endl;

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
	//������ɫӳ���
	m_ColorTable = vtkSmartPointer<vtkLookupTable>::New();
	//������ɫ���е���ɫ
	m_ColorTable->SetHueRange(0.6, 0.0);        //ɫ����Χ�Ӻ�ɫ����ɫ
	m_ColorTable->SetAlphaRange(1.0, 1.0);
	m_ColorTable->SetValueRange(1.0, 1.0);
	m_ColorTable->SetSaturationRange(1.0, 1.0);
	m_ColorTable->SetNumberOfTableValues(512);	//��setnumberofcolorsһ��
	m_ColorTable->SetRange(m_scalarRange);
	m_ColorTable->Build();

	m_Mapper->SetLookupTable(m_ColorTable);
}

void BaseVtk::SetScalarBar()
{
	m_scalarBar->SetLookupTable( m_Mapper->GetLookupTable() );
	if (nullptr == m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars())//�������������û��������ֱ�ӷ���
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
	m_Interactor->CreateRepeatingTimer(50);//����50msˢ��һ�Σ�Ҳ����һ��20֡
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
	_NormalsFilter->SetComputePointNormals(1);//�����㷨��������  
	_NormalsFilter->SetComputeCellNormals(0); //�رյ�Ԫ����������
	_NormalsFilter->SetAutoOrientNormals(1);
	_NormalsFilter->SetSplitting(0);
	_NormalsFilter->Update();

	//����mark��
	vtkSmartPointer<vtkMaskPoints> _mask =
		vtkSmartPointer<vtkMaskPoints>::New();
	_mask->SetInputData(_NormalsFilter->GetOutput());
	_mask->SetMaximumNumberOfPoints(300);
	_mask->RandomModeOn();
	_mask->Update();

	//������ͷ
	vtkSmartPointer<vtkArrowSource> _arrow =
		vtkSmartPointer<vtkArrowSource>::New();
	_arrow->Update(); //һ��Ҫ���� ��������û����ӽ���������ᱨ��

	vtkSmartPointer<vtkGlyph3D> _glyph =
		vtkSmartPointer<vtkGlyph3D>::New();
	_glyph->SetInputData(_mask->GetOutput());
	_glyph->SetSourceData(_arrow->GetOutput());//ÿһ���ü�ͷ����  
	_glyph->SetVectorModeToUseNormal();//����������ʾģʽ�ͷ�����һ��  
	_glyph->SetScaleFactor(0.01); //������������  
	_glyph->Update();


	m_scalarRange[0] = 0;
	m_scalarRange[1] = m_SumNodeNum;

	//δ�����...
}


void BaseVtk::SetCur(HBXDef::CurvatureType_t _t)
{
	//����������������ָ��
	//https://blog.csdn.net/shenziheng1/article/details/54846277
	vtkSmartPointer<vtkCurvatures> _curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();
	_curvaturesFilter->SetInputData(m_AlgorithmFilter->GetOutput());

	switch (_t)
	{
	case  HBXDef::CurvatureType_t::MAX:
		_curvaturesFilter->SetCurvatureTypeToMaximum();	//�������
		break;
	case HBXDef::CurvatureType_t::MIN:
		_curvaturesFilter->SetCurvatureTypeToMinimum();	//��С����
		break;
	case HBXDef::CurvatureType_t::GAUSS:
		_curvaturesFilter->SetCurvatureTypeToGaussian();	//��˹����
		break;
	case HBXDef::CurvatureType_t::MEAN:
		_curvaturesFilter->SetCurvatureTypeToMean();		//ƽ������
		break;
	}
	
	_curvaturesFilter->Update();

	_curvaturesFilter->GetOutput()->GetScalarRange(m_scalarRange);
	
	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(this->GetCurData(_curvaturesFilter));

	m_AlgorithmFilter = vtkCurvatures::SafeDownCast(_curvaturesFilter);

//	m_Mapper->SetInputData(m_curvaturesFilter->GetOutput());
	//�洢����ֵ
//	vtkFloatArray *scalars = vtkFloatArray::New();
	//�趨ÿ������ı���ֵ
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
