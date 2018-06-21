#include "domain.h"
#include <libCUFEM\ClassFactory.h>
#include <libCUFEM\NSortMat.h>

namespace HBXFEMDef
{
	void Domain::GetDomainDofsDefaults(const char* typeName)
	{
		
		if (!strncmp(typeName, "3dshell", 7)) {
			_myType = _3dShellMode;
		}
		else if (!strncmp(typeName, "2dtruss", 7)) {
			_myType = _2dTrussMode;
		}
		else if (!strncmp(typeName, "1dtruss", 7)) {
			_myType = _1dTrussMode;
		}
		else if (!strncmp(typeName, "2dbeam", 6)) {
			_myType = _2dBeamMode;
		}
		else if (!strncmp(typeName, "3d", 2)) {
			_myType = _3dMode;
		}
		else if (!strncmp(typeName, "3dAxisymm", 9)) {
			_myType = _3dAxisymmMode;
		}
		else {
			std::cerr << "unknown domainType (%s)" << std::endl;
			return;
		}

	}

	InputFileResult_t Domain::InstanceSelf(InputRecord * _dr)
	{
		InputFileResult_t result;

		int nnode, nelemt, nmat, nsection, nload, nset;

		int sortNum;

		//��ȡ�������
		GET_FIELD_FROM_IR(_dr, this->_myName,_IF_Domain_Type );
		//�������ּ������ɶ�
		GetDomainDofsDefaults(this->_myName.c_str());

		//��ȡ�ڵ�������Ԫ����������������������Ϣ
//		GET_FIELD_FROM_IR(_dr, nnode, _IF_Domain_nnode);
//		GET_FIELD_FROM_IR(_dr, nelemt, _IF_Domain_nelt);
//		GET_FIELD_FROM_IR(_dr, nsection, _IF_Domain_nsect);
//		GET_FIELD_FROM_IR(_dr, nmat, _IF_Domain_nmat);
//		GET_FIELD_FROM_IR(_dr, nset, _IF_Domain_nset);
		if (_myType == _1dTrussMode) {
			_nsd = 1;
		}
		else if ( _myType == _2dBeamMode || _myType == _2dTrussMode ) {
			_nsd = 2;
		}
		else if ( _myType == _3dShellMode || _myType == _3dMode ) {
			_nsd = 3;
		}
		else if (_myType == _3dAxisymmMode) {
			_nsd = 2;
			_Axisymm = true;
		}

		//��ȡ�ڵ�����
		_vNode.clear();
		_vNode.resize( nnode );
		std::map< std::string, std::vector<Node>>* _NodeMap = nullptr;
		_dr->GiveField((void*)_NodeMap, InputRecord::DataType::ALL_NODE );
		auto _iterN = _NodeMap->begin();
		while (_iterN != _NodeMap->end())
		{

		}
		delete _NodeMap;
		//���ڵ�Ԫ��ȫ�ָն��󡢽ڵ���ȫ�ָն����е�λ�õ�ӳ���ϵ����


		//��ȡ��Ԫ����
		_vNSortArray.clear();
		_vNSortArray.resize(nelemt);
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> >* _elemtMap = nullptr;
		_dr->GiveField((void*)_elemtMap, InputRecord::DataType::ALL_ELEMENT);
		auto _iterE = _elemtMap->begin();
		sortNum = 0;
		while ( _iterE != _elemtMap->end() )
		{
			std::unique_ptr< BaseElem > _tElemt( classFactory.CreateElem(_iterE->first.c_str(), this->_myEngng, this, sortNum) );
			if (!_tElemt)
			{
				std::cerr << "����" << _iterE->first.c_str() << "��Ԫʧ��" << std::endl;
			}
			result = _tElemt->InitFrom(_dr);
			_vElmt[sortNum] = std::move(_tElemt);
			sortNum++;
		}

		//���ɲ��Ͻ��������������¶ȡ�ʪ�ȵ��ⲿ����Ӱ�����ʲ�һ
		_vMat.clear();
		_vMat.resize(nmat);
		std::map< std::string, std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>> >* _matMap = nullptr;
		_dr->GiveField((void*)_matMap, InputRecord::DataType::ALL_MATERIAL);
		auto _iterMat = _matMap->begin();
		sortNum = 0;
		while (_iterMat != _matMap->end())
		{
			std::unique_ptr< BaseMaterial > _tMat(classFactory.CreateMaterial(_iterMat->first.c_str(), this, sortNum ));
			if (!_tMat)
			{
				std::cerr << "����" << _iterMat->first.c_str() << "��Ԫʧ��" << std::endl;
			}
			result = _tMat->InitFrom(_dr);
			_vMat[sortNum] = std::move(_tMat);
			sortNum++;
		}

		//������������ڽ���ʱ�����
		_vSection.clear();
		_vSection.resize(nmat);
		std::map< std::string, std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec>> >* _secMap = nullptr;
		_dr->GiveField((void*)_secMap, InputRecord::DataType::ALL_SECTION);
		auto _iterSec = _secMap->begin();
		sortNum = 0;
		while (_iterSec != _secMap->end())
		{
			std::unique_ptr< BaseSection > _tSec(classFactory.CreateSection(_iterSec->first.c_str(), this, sortNum));
			if (!_tSec)
			{
				std::cerr << "����" << _iterSec->first.c_str() << "��Ԫʧ��" << std::endl;
			}
			result = _tSec->InitFrom(_dr);
			_vSection[sortNum] = std::move(_tSec);
			sortNum++;
		}

		//ʱ�亯������δ���

		return result;
	}





	BaseBoundary* Domain::GetBoundry(int _n)
	{
#ifdef _DEBUG
		if (_n<0 || _n>=_vLoad.size())
		{
			std::cerr << "����Խ��..." << std::endl;
		}
#endif
		return &_vLoad[_n];
	}
	std::vector<BaseBoundary>& Domain::GetBoundry()
	{
		// TODO: �ڴ˴����� return ���
		return this->_vLoad;
	}
}

