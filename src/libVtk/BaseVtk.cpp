#include "BaseVtk.h"
#include <string>
#include <algorithm>
#include <Vtk8.0\vtkNamedColors.h>
#include <VtkPreDef.h>

//bool BaseVtk::bScalarLabel = false;		//�Ƿ���ʾ��ǩ��

//vtkIdType  BaseVtk::m_SumNodeNum = 0;	//��Ԫ����
//vtkIdType  BaseVtk::m_SumElmtNum = 0;	//��Ԫ����
//int		 BaseVtk::m_LabelNum = 0;	//��ǩ�������

//double BaseVtk::m_scalarRange[2] = { 0, 0 };	//���Է�Χ

//vtkSmartPointer<vtkFloatArray> BaseVtk::m_Scalar = nullptr;
//vtkSmartPointer<vtkPolyDataAlgorithm> BaseVtk::m_AlgorithmFilter = nullptr;
//vtkSmartPointer<vtkRenderWindowInteractor> BaseVtk::m_Interactor = nullptr;//������
//vtkSmartPointer<vtkScalarBarActor> BaseVtk::m_scalarBar = nullptr;	//��ǩ��


BaseVtk::BaseVtk() : m_wide(500), m_height(500) ,//���ڲ���
					m_LabelNum(5),//��ͼ���Ӳ���
					m_PartCount(0)
{
	Initial();
}

BaseVtk::BaseVtk(unsigned int _wide, unsigned int _height): m_wide(_wide), m_height(_height),
															m_LabelNum(0),//��ͼ���Ӳ���
															m_PartCount(0)
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
	m_scalarRange[0] = 0;
	m_scalarRange[1] = 5;

	m_AlgorithmFilter = vtkSmartPointer<vtkPolyDataAlgorithm>::New();

	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();		//�����洢��
	m_scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

	m_MyCallback = vtkSmartPointer<vtkCallbackCommand>::New();

	m_vMapper.clear();

	m_vActor.clear();

	m_Camera = vtkInteractorStyleTrackballCamera::New();

	m_Renderer = vtkSmartPointer< vtkRenderer >::New();
	m_RenderWin = vtkSmartPointer<vtkRenderWindow>::New();

	m_RenderWin->SetSize(m_wide, m_height);	//���ô��ڳߴ�
	m_RenderWin->SetWindowName("vtk����");

	//������Ⱦ������
	m_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_Interactor->SetRenderWindow(m_RenderWin);	
	//���ý���ģʽ
	m_Interactor->SetInteractorStyle(m_Camera);
	

//	m_BoxWidget = vtkSmartPointer<vtkBoxWidget>::New();
//	m_BoxWidget->SetInteractor(m_Interactor);
//	m_BoxWidget->SetPlaceFactor(1.25);



	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::Instance()
{
	//���Զ�ˢ�µĺ���
	m_vMapper[0]->SetScalarRange(m_scalarRange[0], m_scalarRange[1]);

	//�˴���Ҫ�޸�
	m_Renderer->AddActor(m_vActor[0]);
	m_Renderer->AddActor2D(m_scalarBar);
	m_Renderer->ResetCamera();
	m_Renderer->SetBackground(0.1, 0.2, 0.4);

	m_RenderWin->AddRenderer(m_Renderer);

//	m_BoxWidget->SetProp3D(m_vActor[0]);
//	m_BoxWidget->PlaceWidget();
//	m_BoxWidget->AddObserver(vtkCommand::InteractionEvent, m_MyCallback);
//	m_BoxWidget->On();

	//m_Mapper->SetInputData( m_AlgorithmFilter->GetOutput() );
	//m_Mapper->ScalarVisibilityOn();
	//m_Mapper->SetScalarRange(m_scalarRange);


//	SetBackground(m_Renderer, );

	// Make an oblique view
//	m_Renderer->GetActiveCamera()->Azimuth(30);
// 	m_Renderer->GetActiveCamera()->Elevation(30);
//	m_Renderer->GetActiveCamera()->tr
//	m_Renderer->ResetCamera();	//�������

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::Reset()
{
	m_vMapper.clear();
	m_vActor.clear();

	m_Renderer->Clear();

	m_PartCount = 0;

	m_SumNodeNum = 0;
	m_SumElmtNum = 0;

	m_LabelNum = 0;
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

	vtkSmartPointer<vtkPoints> _coords = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> _pElmt = vtkSmartPointer<vtkCellArray>::New();	//Ϊ��Ԫ���������ڴ�;
	vtkSmartPointer<vtkPolyData> _PolyCube = vtkSmartPointer<vtkPolyData>::New();		//�������������ݼ�


	//�������еĽڵ���������
	_NodeIter _Niter;
	for ( _Niter = _dynRecord->GetNodeArray()->begin(); _Niter != _dynRecord->GetNodeArray()->end(); _Niter++)
	{
		std::string _PartName = _Niter->first;		//��ȡ��ǰ�������
		this->InstanceNodes(_coords, _Niter);
#ifdef _DEBUG
	std::cout << "��ǰ��:" << " ���ƽڵ�" << _coords->GetNumberOfPoints() << std::endl;
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
	_PolyCube->SetPoints(_coords);
	//���õ�Ԫ���ͣ������
	_PolyCube->SetPolys(_pElmt);	
	m_PartMap.insert(std::make_pair( ("part"+ std::to_string(m_PartCount) ), _PolyCube));

	m_AlgorithmFilter->SetInputData( _PolyCube );

	m_vMapper.push_back(vtkSmartPointer<vtkPolyDataMapper>::New());
	m_vMapper[m_PartCount]->SetInputData(_PolyCube);

	m_vActor.push_back(vtkSmartPointer<vtkActor>::New());
	m_vActor[m_PartCount]->SetMapper(m_vMapper[m_PartCount]);

	m_PartCount++;//���������1

//	m_AlgorithmFilter->SetOutput( _PolyCube );

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::SetData( const char * _FileName, std::string  _path)
{
	std::string _file(_FileName);
	std::string suffixStr = _file.substr(_file.find_last_of('.') + 1);

	//�ж��Ƿ�Ϊstl�ļ�
	if ( strcmpi( suffixStr.c_str(), "stl"))
	{
		vtkSmartPointer<vtkSTLReader> _StlInstance = vtkSmartPointer<vtkSTLReader>::New();
		vtkSmartPointer<vtkPolyData>_PolyCube = vtkSmartPointer<vtkPolyData>::New();		//�������������ݼ�

		std::string _tmppath(_path);
		_tmppath.append(_FileName);

		_StlInstance->SetOutput(_StlInstance->GetOutput());
		_StlInstance->SetFileName(_tmppath.c_str());
		_StlInstance->Update();

		m_SumNodeNum = _StlInstance->GetOutput()->GetPoints()->GetNumberOfPoints();
		m_SumElmtNum = _StlInstance->GetOutput()->GetCellData()->GetNumberOfArrays();
		_PolyCube->GetNumberOfPoints();
		std::cout << "stlģ���й��ƽڵ�" << m_SumNodeNum << std::endl;

		m_AlgorithmFilter = vtkSTLReader::SafeDownCast(_StlInstance);

		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	

	return UserStatusError_t::USER_EMPTY_POINT;
}

void BaseVtk::SetScalar(int * _intArray, int _lgth, int _colorCount, const char* _name)
{
	vtkSmartPointer<vtkIntArray> _ColorDepart =
		vtkSmartPointer<vtkIntArray>::New();
	_ColorDepart->SetName("DepartSort");
	_ColorDepart->SetNumberOfTuples(_lgth);
	for (int i = 0; i < _lgth; i++)
	{
		_ColorDepart->SetTuple1(i,_intArray[i]);
	}
	_PartIter _iter = m_PartMap.find(_name);
	if (_iter != m_PartMap.end())
	{
		//_iter->second->GetPointData()->AddArray(_ColorDepart);
		_iter->second->GetCellData()->SetScalars(_ColorDepart);
//		_iter->second->GetPointData()->SetActiveScalars("DepartSort");
	}
	
	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetName("Colors");
	colors->SetNumberOfComponents(3);
	colors->SetNumberOfTuples(_colorCount);
	for (int i = 0; i < _colorCount; i++)
	{
		colors->InsertTuple3(i,
			int(255 * i / (_colorCount - 1)),
			0,
			int(255 * (_colorCount - 1 - i) / (_colorCount - 1)));
	}


//	_iter->second->GetPointData()->AddArray(colors);
//
//	m_vMapper[0]->ScalarVisibilityOn();
//	m_vMapper[0]->SetScalarModeToUsePointFieldData();
//	m_vMapper[0]->SelectColorArray("Colors");

//	using namespace std;
//	vector<int> _v(_intArray, _intArray+_num);//���ã���Ϊscalars�������Ƹ�������������������ʧ��scalarҲ����
//	int _max = (*max_element(_intArray, _intArray + _num) );
//	int _min = (*min_element(_intArray, _intArray + _num));
//
//	vtkSmartPointer<vtkIntArray> scalars = vtkSmartPointer<vtkIntArray>::New();
//	//�趨ÿ������ı���ֵ
//	scalars->SetArray(_intArray, _num, 1);
//
//	_PartIter _iter = m_PartMap.find(_name);
//	if (_iter != m_PartMap.end() )
//	{
//		_iter->second->GetPointData()->SetScalars(scalars);
//	}
//
//	m_scalarRange[0] = _min;
//	m_scalarRange[1] = _max;
}

void BaseVtk::SetScalar(float * _fArray, int _num, const char* _name)
{
	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
	//�趨ÿ������ı���ֵ
	scalars->SetArray(_fArray, _num, 1);
	_PartIter _iter = m_PartMap.find(_name);
	if (_iter != m_PartMap.end())
	{
		_iter->second->GetPointData()->SetScalars(scalars);
	}

//	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(_inArray);
}

void BaseVtk::SetScalar(double * _dArray, int _num, const char* _name)
{
	vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
	//�趨ÿ������ı���ֵ
	scalars->SetArray(_dArray, _num, 1);
	_PartIter _iter = m_PartMap.find(_name);
	if (_iter != m_PartMap.end())
	{
		_iter->second->GetPointData()->SetScalars(scalars);
	}

	//	m_AlgorithmFilter->GetOutput()->GetPointData()->SetScalars(_inArray);
}


void BaseVtk::SetColorTable(HBXDef::ColorType_t _t, int _ColorCount)
{
	using namespace HBXDef;

	//������ɫӳ���
	m_ColorTable = vtkSmartPointer<vtkLookupTable>::New();
	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();

	switch (_t)
	{
	case RGB:default:
		break;

	case CUSTOM:
		m_ColorTable->SetNumberOfTableValues(_ColorCount);
		for (int i = 0; i < _ColorCount; i++)
		{
			m_ColorTable->SetTableValue(i,
				int(255 * i / (_ColorCount - 1)),
				0,
				int(155 * (_ColorCount - 1 - i) / (_ColorCount - 1)));
		}

		m_ColorTable->Build();
		// Create a lookup table to map cell data to colors
//		m_ColorTable->SetTableValue(0, colors->GetColor4d("Black").GetData());
//		m_ColorTable->SetTableValue(1, colors->GetColor4d("Banana").GetData());
//		m_ColorTable->SetTableValue(2, colors->GetColor4d("Tomato").GetData());
//		m_ColorTable->SetTableValue(3, colors->GetColor4d("Wheat").GetData());
//		m_ColorTable->SetTableValue(4, colors->GetColor4d("Lavender").GetData());
//		m_ColorTable->SetTableValue(5, colors->GetColor4d("Flesh").GetData());
//		m_ColorTable->SetTableValue(6, colors->GetColor4d("Raspberry").GetData());
//		m_ColorTable->SetTableValue(7, colors->GetColor4d("Salmon").GetData());
//		m_ColorTable->SetTableValue(8, colors->GetColor4d("Mint").GetData());
//		m_ColorTable->SetTableValue(9, colors->GetColor4d("Peacock").GetData());

		break;
	case HUE:
		//������ɫ���е���ɫ
		m_ColorTable->SetHueRange(0.67, 0.0);        //ɫ����Χ�Ӻ�ɫ����ɫ
		m_ColorTable->SetAlphaRange(1.0, 1.0);
		m_ColorTable->SetValueRange(1.0, 1.0);
		m_ColorTable->SetSaturationRange(1.0, 1.0);
		m_ColorTable->SetNumberOfTableValues(5);	//��setnumberofcolorsһ��
//		m_ColorTable->SetRange(m_scalarRange);
		m_ColorTable->Build();

		break;
	}
	m_vMapper[0]->SetLookupTable(m_ColorTable);


}

void BaseVtk::SetScalarBar()
{
	m_scalarBar->SetLookupTable( m_vMapper[0]->GetLookupTable() );
#ifndef _DEBUG
	if (nullptr == m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars())//�������������û��������ֱ�ӷ���
	{
		return;
	}
	else{
#endif 
		_PartIter _iter = m_PartMap.begin();
		while (_iter != m_PartMap.end())
		{
			//_iter->second->GetPointData()->AddArray(_ColorDepart);
			m_scalarBar->SetTitle(
					_iter->second->GetCellData()->GetScalars()->GetName() );
			break;
		}
#ifndef _DEBUG
//		m_scalarBar->SetTitle(m_AlgorithmFilter->GetOutput()->GetPointData()->GetScalars()->GetName());
	}
#endif 
	m_scalarBar->SetNumberOfLabels(m_LabelNum);
	m_scalarBar->SetTextPad(4);
}

void BaseVtk::AutoFreshNodeNum()
{
//	m_MyCallback->SetCallback( BaseVtk::SetNodeNum );

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

//	Paint();
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
	//	m_Renderer->AddActor2D(m_scalarBar);
	}

//	m_Interactor->Render();

	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::Draw()
{
	return UserStatusError_t::USER_STATUS_SUCCESS;
}

void BaseVtk::Run()
{
	//��Ⱦ
	m_RenderWin->Render();	

	//��ʼ����
	m_Interactor->Initialize();
	m_Interactor->Start();

}


void BaseVtk::SetBackground(vtkSmartPointer<vtkRenderer> _render, double * _rgb)
{
	_render->SetBackground(_rgb);
}


//void BaseVtk::CallbackFunc(vtkObject * _caller, unsigned long _eventId, void* clientdata, void * _callData)
//{
//	std::cout << "callbackFunc ing..." << std::endl;
//}