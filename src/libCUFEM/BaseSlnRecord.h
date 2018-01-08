#pragma once
#include <map>
#include <BoostPreDef.h>
#include <ExportDef.h>
#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	class AttriRecord;

	//��������ؼ�¼�������������ͣ�Ԫ����Ϣ�����ַ����ȡ�
	class BOOST_SYMBOL_EXPORT BaseSlnRecord
	{
	private:
	protected:
	public:
		BaseSlnRecord();
		virtual ~BaseSlnRecord();

		//��������
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
	typedef std::map< BaseSlnRecord, _DomainInMap, RecordLess >	_SlnPropInMap;//���øüܹ�����Ϊһ����������Ӧ���part����domain������ĳ�����������𻵿���map�п����滻�����һ���Ľ��㡣
	typedef std::map< BaseSlnRecord, _DomainInMap >::iterator	_SlnPropIter;
}