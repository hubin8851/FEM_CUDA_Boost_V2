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

	m_RenderWin->SetSize(m_wide, m_height);	//���ô��ڳߴ�
	m_RenderWin->SetWindowName("vtk����");
//	m_RenderWin->Render();		//��Ⱦ��

//	SetBackground(m_Renderer, );

	// Make an oblique view
// 	m_Renderer->GetActiveCamera()->Azimuth(30);
// 	m_Renderer->GetActiveCamera()->Elevation(30);
//	m_Renderer->ResetCamera();	//�������

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

	m_PolyCube = vtkSmartPointer<vtkPolyData>::New();		//�������������ݼ�
	m_Scalar = vtkSmartPointer<vtkFloatArray>::New();		//�����洢��
	m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();	//����ӳ����

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
	m_PolyCube->SetPoints(points);
	//���õ�Ԫ���ͣ������
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
	//������ɫӳ���
	m_ColorTable = vtkSmartPointer<vtkLookupTable>::New();
	//������ɫ���е���ɫ
	m_ColorTable->SetHueRange(0.0, 0.6);        //ɫ����Χ�Ӻ�ɫ����ɫ
	m_ColorTable->SetAlphaRange(1.0, 1.0);
	m_ColorTable->SetValueRange(1.0, 1.0);
	m_ColorTable->SetSaturationRange(1.0, 1.0);
	m_ColorTable->SetNumberOfTableValues(256);	//��setnumberofcolorsһ��
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
	//����������������ָ��
	//https://blog.csdn.net/shenziheng1/article/details/54846277
	m_curvaturesFilter = vtkSmartPointer<vtkCurvatures>::New();
	m_curvaturesFilter->SetInputData(m_PolyCube);
//	m_curvaturesFilter->SetCurvatureTypeToMaximum();	//�������
//	m_curvaturesFilter->SetCurvatureTypeToMinimum();	//��С����
//	m_curvaturesFilter->SetCurvatureTypeToGaussian();	//��˹����
	m_curvaturesFilter->SetCurvatureTypeToMean();		//ƽ������
	m_curvaturesFilter->Update();

	m_curvaturesFilter->GetOutput()->GetScalarRange(m_scalarRange);
	
	m_PolyCube->GetPointData()->SetScalars(this->GetCurData());

//	m_Mapper->SetInputData(m_curvaturesFilter->GetOutput());
	m_Mapper->SetInputData(m_PolyCube);
	m_Mapper->SetScalarRange(m_scalarRange);
	//�洢����ֵ
//	vtkFloatArray *scalars = vtkFloatArray::New();
	//�趨ÿ������ı���ֵ
//	for (vtkIdType i = 0; i < m_SumElmtNum; i++)scalars->InsertTuple1(i, i);
//	m_PolyCube->GetPointData()->SetScalars(scalars);
}

UserStatusError_t BaseVtk::Paint()
{
	m_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	m_Interactor->SetRenderWindow(m_RenderWin);
	//��ʼ����
	m_Interactor->Initialize();
	//���ý���ģʽ
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
