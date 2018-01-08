#pragma once
#include <map>
#include <BoostPreDef.h>
#include <ExportDef.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	class AttriRecord;

	//解算器相关记录。包括解算类型，元步信息，积分方法等。
	class BOOST_SYMBOL_EXPORT BaseSlnRecord
	{
	private:
	protected:
	public:
		BaseSlnRecord();
		virtual ~BaseSlnRecord();

		//计算类型
		std::string keyword;

		std::vector< AttriRecord > m_vAtrriRecord;

	};

	struct RecordLess
	{
		bool operator()(const BaseSlnRecord& _lhs, const BaseSlnRecord& _rhs)
		{
			return _lhs.keyword < _rhs.keyword || (_lhs.keyword == _rhs.keyword && _lhs.keyword < _rhs.keyword);
		}

	};

	typedef std::pair< BaseSlnRecord, _DomainInMap >	_SlnPropInpair;
	typedef std::map< BaseSlnRecord, _DomainInMap, RecordLess >	_SlnPropInMap;//采用该架构是因为一个解算器对应多个part（即domain），若某个部件发生损坏可在map中快速替换完成下一步的解算。
	typedef std::map< BaseSlnRecord, _DomainInMap >::iterator	_SlnPropIter;
}