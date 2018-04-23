#pragma once

//#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <ExportDef.h>

namespace HBXFEMDef
{
	class Domain;
	class AttriRecord;

	//解算器相关记录。包括解算类型，元步信息，积分方法等。
	//也包含待计算的数据
	class CUFEM_API BaseSlnRecord
	{
	public:
		BaseSlnRecord();
		virtual ~BaseSlnRecord();

		//计算类型
		std::string keyword;

		bool PlaneStressOrStrain;

		std::vector< AttriRecord > m_vAtrriRecord;

		std::vector<Domain*>	m_DomainList;

	public:
		void InsertNode(Domain* _dm, int _id);
	};

	struct RecordLess
	{
		bool operator()(const BaseSlnRecord& _lhs, const BaseSlnRecord& _rhs)
		{
			return _lhs.keyword < _rhs.keyword || (_lhs.keyword == _rhs.keyword && _lhs.keyword < _rhs.keyword);
		}

	};

//	struct __Record {};
//	struct __Domain {};
//	using namespace boost::bimaps;
//	typedef bimap<multiset_of<tagged<BaseSlnRecord, __Record> >, multiset_of<tagged<Domain*, __Domain> >> _SlnPropInMap;
//	typedef std::pair<BaseSlnRecord, std::list<Domain*> > _SlnPropInpair;
//	typedef std::pair< BaseSlnRecord, HBXFEMDef::_DomainInMap >	_SlnPropInpair;
//	typedef std::map< BaseSlnRecord, std::pair< std::string, Domain* >, RecordLess >	_SlnPropInMap;//采用该架构是因为一个解算器对应多个part（即domain），若某个部件发生损坏可在map中快速替换完成下一步的解算。
//	typedef std::map< BaseSlnRecord, _DomainInMap >::iterator	_SlnPropIter;
}