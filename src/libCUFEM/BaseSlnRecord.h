#pragma once

//#include <libCUFEM\domain.h>
#include <libCUFEM\AttriRecord.h>
#include <ExportDef.h>

namespace HBXFEMDef
{
	class Domain;
	class AttriRecord;

	//��������ؼ�¼�������������ͣ�Ԫ����Ϣ�����ַ����ȡ�
	//Ҳ���������������
	class CUFEM_API BaseSlnRecord
	{
	public:
		BaseSlnRecord();
		virtual ~BaseSlnRecord();

		//��������
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
//	typedef std::map< BaseSlnRecord, std::pair< std::string, Domain* >, RecordLess >	_SlnPropInMap;//���øüܹ�����Ϊһ����������Ӧ���part����domain������ĳ�����������𻵿���map�п����滻�����һ���Ľ��㡣
//	typedef std::map< BaseSlnRecord, _DomainInMap >::iterator	_SlnPropIter;
}