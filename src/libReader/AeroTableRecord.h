#pragma once

#include <vector>
#include <string>
#include <map>
#include <BoostPreDef.h>
#include <HbxDefMacro.h>
#include <HBXFEMDefStruct.h>
#include <HbxXmlDef\AeroTable.h>
#include <libCUFEM\BaseComponent.h>
#include <libCUFEM\InputRecord.h>
#include <libCUFEM\NSortMat.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>

namespace HBXFEMDef
{
	using namespace std;

	//�������Ҫ����������µļ�¼���������¼���Ծ������ⷽ��ʵ�֣�
	//�������ɸ���ͳһ��ʽ��
	class CUFEM_API AeroTableRecord :public InputRecord
	{
	protected:
		std::string RecordKeyWord;
		int RecordNum;

		//Ϊ�˳������չ�ԣ��ɴ�XML���ȡÿ�ֵ�Ԫ��Ӧ������
		typedef std::map< std::string, std::shared_ptr<HBXDef::_AEROTABLE> > _AeroTableInMap;
		typedef _AeroTableInMap::iterator _AeroTableMapIter;
		//�����ĳ�Ա
		_AeroTableInMap	m_AeroTableInMap;

	public:
		//Ĭ�Ϲ��캯��
		AeroTableRecord(std::string _keyword = "", int _id = 0);
		//�������캯��
		AeroTableRecord(const AeroTableRecord& _src);
		AeroTableRecord& operator = (const AeroTableRecord&);
		//�Ӹ������齨����ȡ��¼
		AeroTableRecord(BaseComponent& _component);

		//
		virtual ~AeroTableRecord() {};

		//
		virtual AeroTableRecord* GiveCopy() { return new AeroTableRecord(*this); };

		virtual bool SetField(int _item, InputFieldType _id);

		virtual bool SetField(double &_input, InputFieldType _id);

		virtual bool SetField(std::vector<int> _item, InputFieldType _id);

		virtual bool SetField(HBXFEMDef::Node  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>>  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id);

		virtual InputFileResult_t GiveField(double &answer, InputFieldType _id);

		virtual HBXDef::_AEROTABLE* GiveField( InputFieldType _id);

	};



}
