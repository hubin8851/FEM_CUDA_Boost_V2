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

		//获取域的名字
		GET_FIELD_FROM_IR(_dr, this->_myName,_IF_Domain_Type );
		//根据名字计算自由度
		GetDomainDofsDefaults(this->_myName.c_str());

		//获取节点数、单元数、材料数、截面数等信息
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

		//读取节点数据
		_vNode.clear();
		_vNode.resize( nnode );
		std::map< std::string, std::vector<Node>>* _NodeMap = nullptr;
		_dr->GiveField((void*)_NodeMap, InputRecord::DataType::ALL_NODE );
		auto _iterN = _NodeMap->begin();
		while (_iterN != _NodeMap->end())
		{

		}
		delete _NodeMap;
		//关于单元在全局刚度阵、节点在全局刚度阵中的位置等映射关系待定


		//读取单元数据
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
				std::cerr << "创建" << _iterE->first.c_str() << "单元失败" << std::endl;
			}
			result = _tElemt->InitFrom(_dr);
			_vElmt[sortNum] = std::move(_tElemt);
			sortNum++;
		}

		//生成材料解算器，材料随温度、湿度等外部环境影响性质不一
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
				std::cerr << "创建" << _iterMat->first.c_str() << "单元失败" << std::endl;
			}
			result = _tMat->InitFrom(_dr);
			_vMat[sortNum] = std::move(_tMat);
			sortNum++;
		}

		//截面解算器便于解算时变参数
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
				std::cerr << "创建" << _iterSec->first.c_str() << "单元失败" << std::endl;
			}
			result = _tSec->InitFrom(_dr);
			_vSection[sortNum] = std::move(_tSec);
			sortNum++;
		}

		//时变函数等尚未完成

		return result;
	}





	BaseBoundary* Domain::GetBoundry(int _n)
	{
#ifdef _DEBUG
		if (_n<0 || _n>=_vLoad.size())
		{
			std::cerr << "访问越界..." << std::endl;
		}
#endif
		return &_vLoad[_n];
	}
	std::vector<BaseBoundary>& Domain::GetBoundry()
	{
		// TODO: 在此处插入 return 语句
		return this->_vLoad;
	}
}

