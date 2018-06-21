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

	//该类的主要作用是添加新的记录集。输入记录可以经由任意方法实现，
	//但最终由该类统一格式。
	class CUFEM_API AeroTableRecord :public InputRecord
	{
	protected:
		std::string RecordKeyWord;
		int RecordNum;

		//为了程序的拓展性，可从XML里读取每种单元对应的属性
		typedef std::map< std::string, std::shared_ptr<HBXDef::_AEROTABLE> > _AeroTableInMap;
		typedef _AeroTableInMap::iterator _AeroTableMapIter;
		//真正的成员
		_AeroTableInMap	m_AeroTableInMap;

	public:
		//默认构造函数
		AeroTableRecord(std::string _keyword = "", int _id = 0);
		//拷贝构造函数
		AeroTableRecord(const AeroTableRecord& _src);
		AeroTableRecord& operator = (const AeroTableRecord&);
		//从给定的组建中提取记录
		AeroTableRecord(BaseComponent& _component);

		//
		virtual ~AeroTableRecord() {};

		//
		virtual AeroTableRecord* GiveCopy() { return new AeroTableRecord(*this); };

		virtual bool SetField(int _item, InputFieldType _id);

		virtual bool SetField(float _item, InputFieldType _id);

		virtual bool SetField(double &_input, InputFieldType _id);

		virtual bool SetField(std::vector<int> _item, InputFieldType _id);

		virtual bool SetField(HBXFEMDef::Node  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>>  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id);

		InputFileResult_t GiveField(int &answer, InputFieldType _id) {};

		InputFileResult_t GiveField(float &answer, InputFieldType _id) {};

		InputFileResult_t GiveField(double &answer, InputFieldType _id) {};

		InputFileResult_t GiveField(std::shared_ptr<std::vector<HBXFEMDef::Node>>* answer, InputFieldType _id) { std::cout << "气动数据表内无节点数据" << std::endl; };

		InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserCalPrec> >* answer, InputFieldType _id) { std::cout << "气动数据表内无单元" << std::endl; };

		InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserCalPrec> > answer, InputFieldType _id) { std::cout << "气动数据表内无截面数据" << std::endl; };

		InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserCalPrec> > answer, InputFieldType _id) { std::cout << "气动数据表内无材料数据" << std::endl; };

		InputFileResult_t GiveField(std::shared_ptr<HBXFEMDef::Set> answer, InputFieldType _id) { std::cout << "气动数据表内无集合数据" << std::endl; };

		InputFileResult_t GiveField(double &answer, InputFieldType _id);

		virtual HBXDef::_AEROTABLE* GiveField( InputFieldType _id);

	};



}
