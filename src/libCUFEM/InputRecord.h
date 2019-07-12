#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXFEMDefStruct.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\NSortMat.h>
#include <libCUFEM\BaseMaterial.h>
#include <libCUFEM\BaseSection.h>
#include <libCUFEM\BaseNode.h>
#include <HbxXmlDef\AeroTable.h>
#include <helper_hbx.h>

namespace HBXFEMDef
{
	typedef const char *InputFieldType;

	class Set;
	template<typename _T> class NSortMat;

#define GET_FIELD_FROM_IR(__ir, __value, __id) result = __ir->GiveField(__value, __id);\
CheckUserDefErrors(result);

	class CUFEM_API InputRecord
	{
	public:
		typedef enum DataType
		{
			ALL_INT,
			ALL_FLOAT,
			ALL_DOUBLE,
			ALL_NODE,
			ALL_ELEMENT,
			ALL_MATERIAL,
			ALL_SECTION,
			ALL_SET
		}DataType_t;
	public:
		//默认构造函数
		InputRecord() {};
		//拷贝构造函数
		InputRecord(const InputRecord&) {};
		//
		virtual ~InputRecord() {};

		InputRecord& operator = (const InputRecord&) { return *this; };

		virtual InputRecord* GiveCopy() = 0;

		virtual bool SetField(int _item, InputFieldType _id) = 0;

		virtual bool SetField(float _item, InputFieldType _id) = 0;

		virtual bool SetField(double &_input, InputFieldType _id) = 0;

		virtual bool SetField(std::vector<int> _item, InputFieldType _id) = 0;

		virtual bool SetField(HBXFEMDef::Node  _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr<HBXFEMDef::NSortMat<HBXDef::UserReadPrec>> _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr< std::vector<HBXFEMDef::Node >> _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserReadPrec> > _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserReadPrec> > _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id) = 0;

		virtual bool SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id ) = 0;

		//输出为void指针，访问其下所有数据。
		//前缀为ALL_的id宏，表明获取某种（比如单元，节点）数据的完整数据结构
		virtual InputFileResult_t GiveField(void* answer, DataType _id) = 0;

		virtual InputFileResult_t GiveField(int &answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(float &answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(double &answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::string &answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::shared_ptr< std::vector<Node>> answer, std::string _strName) = 0;//为了domain提供接口

		virtual InputFileResult_t GiveField(std::vector<HBXFEMDef::Node>& answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField( std::vector<HBXDef::UserReadPrec>* answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserCalPrec> >* answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserCalPrec> > answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserCalPrec> > answer, InputFieldType _id) = 0;

		virtual InputFileResult_t GiveField(std::shared_ptr<HBXFEMDef::Set> answer, InputFieldType _id) = 0;

		virtual HBXDef::_AEROTABLE* GiveField(InputFieldType _id) = 0;
	};



}
