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

	//传入所有的节点坐标数据
	for (size_t i = 0; i != _dm->_vNode.size(); i++)
	{
		vtkIdType _nodeId = points->InsertNextPoint( _dm->_vNode[i]._X,
			_dm->_vNode[i]._Y,
			_dm->_vNode[i]._Z);
	}
#ifdef _DEBUG
	std::cout << "当前块:" << " 共计节点" << points->GetNumberOfPoints() << std::endl;
#endif // _DEBUG

	//绘制所有的cell，当前为单元下围成其所有的节点
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
