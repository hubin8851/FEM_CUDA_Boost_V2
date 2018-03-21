#include "BaseVtk.h"

BaseVtk::BaseVtk()
{
}

BaseVtk::~BaseVtk()
{
}

UserStatusError_t BaseVtk::Initial()
{
	return UserStatusError_t::USER_STATUS_SUCCESS;
}

UserStatusError_t BaseVtk::SetData(HBXFEMDef::Domain* _dm )
{
	vtkSmartPointer<vtkPolyData> PolyCube = vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();

	//�������еĽڵ���������
	for (size_t i = 0; i != _dm->_vNode.size(); i++)
	{
		vtkIdType _nodeId = points->InsertNextPoint( _dm->_vNode[i]._X,
			_dm->_vNode[i]._Y,
			_dm->_vNode[i]._Z);
	}
#ifdef _DEBUG
	std::cout << "��ǰ��:" << " ���ƽڵ�" << points->GetNumberOfPoints() << std::endl;
#endif // _DEBUG

	//�������е�cell����ǰΪ��Ԫ��Χ�������еĽڵ�
	for (size_t i = 0; i != _dm->_vElmt.size(); i++)
	{
		vtkIdType* _tmpVec = new vtkIdType[_dm->_vElmt[i]._myNode.size()];
		for (size_t j = 0; j < _dm->_vElmt[i]._myNode.size(); j++)
		{
			_tmpVec[j] = _Iter->second->_vElmt[i]->_myNode[j];
		}

		polys->InsertNextCell(_Iter->second->_vElmt[i]->_myNode.size(), _tmpVec);
		delete[] _tmpVec;
	}

	return UserStatusError_t::USER_STATUS_SUCCESS;
}
