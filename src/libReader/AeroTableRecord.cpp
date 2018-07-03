#ifdef USESTDAFX
#include "stdafx.h"
#endif

#include "AeroTableRecord.h"

namespace HBXFEMDef
{

	AeroTableRecord::AeroTableRecord(std::string _keyword, int _id) :InputRecord(),
		RecordKeyWord(_keyword), RecordNum(_id)
	{

	}

	AeroTableRecord::AeroTableRecord(const AeroTableRecord &_src) : InputRecord(_src),
		RecordKeyWord(_src.RecordKeyWord),
		RecordNum(_src.RecordNum),
		m_AeroTableInMap(_src.m_AeroTableInMap)
	{

	}

	AeroTableRecord & AeroTableRecord::operator=(const AeroTableRecord &)
	{
		// TODO: �ڴ˴����� return ���
		return *this;
	}

	AeroTableRecord::AeroTableRecord(BaseComponent & _component)
	{
	}

	bool AeroTableRecord::SetField(int _item, InputFieldType _id)
	{
		std::cout << "�������ݱ�����int���ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(float _item, InputFieldType _id)
	{
		std::cout << "�������ݱ�����float���ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(double & _input, InputFieldType _id)
	{
		std::cout << "�������ݱ�����double���ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::vector<int> _item, InputFieldType _id)
	{
		std::cout << "�������ݱ�����vec<int>���ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(HBXFEMDef::Node _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޽ڵ㼯��" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>> _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޽ڵ��������ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserReadPrec> > _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserReadPrec> > _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return false;
	}

	bool AeroTableRecord::SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id)
	{
		this->m_AeroTableInMap[_id] = std::move(_item);
		return false;
	}

	InputFileResult_t AeroTableRecord::GiveField(void * answer, DataType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return InputFileResult_t::IRRT_NOTFOUND;
	}

	InputFileResult_t AeroTableRecord::GiveField(int & answer, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return InputFileResult_t::IRRT_NOTFOUND;
	}
	InputFileResult_t AeroTableRecord::GiveField(float & answer, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return InputFileResult_t::IRRT_NOTFOUND;
	}
	InputFileResult_t AeroTableRecord::GiveField(double & answer, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return InputFileResult_t::IRRT_NOTFOUND;
	}
	InputFileResult_t AeroTableRecord::GiveField(std::string & answer, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return InputFileResult_t::IRRT_NOTFOUND;
	}


	HBXDef::_AEROTABLE* AeroTableRecord::GiveField( InputFieldType _id)
	{
		_AeroTableMapIter _it = this->m_AeroTableInMap.find(_id);
		if (_it == this->m_AeroTableInMap.end())
		{
			return nullptr;
		}
		return _it->second.get();
	}


	bool AeroTableRecord::SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserReadPrec> > _item, InputFieldType _id)
	{
		std::cout << "�������ݱ����޸����ͼ���" << std::endl;
		return false;
	}

}


