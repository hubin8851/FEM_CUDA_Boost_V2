#pragma once
#pragma warning(disable : 4786 )
#include <vector>
#include <string>
#include <map>
#include <ExportDef.h>
#include <BoostPreDef.h>
#include <HBXFEMDefStruct.h>
#include <libCUFEM\NSortMat.h>
#include <libCUFEM\InputRecord.h>
#include <libCUFEM\BaseComponent.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h>

namespace HBXFEMDef
{
	using namespace std;

	//该类的主要作用是添加新的记录集。输入记录可以经由任意方法实现，
	//但最终由该类统一格式。
	class CUFEM_API DynamicInputRecord:public InputRecord
	{
	protected:
		std::string RecordKeyWord;
		int RecordNum;

		// Record representation.
		std::set< std::string > emptyRecord; ///< Fields without values
		std::map< std::string, int > intRecord;
		std::map< std::string, float > floatRecord;
		std::map< std::string, double > doubleRecord;
		std::map< std::string, bool > boolRecord;
 		std::map< std::string, std::string > stringRecord;
		std::map< std::string, std::vector< std::string > > stringListRecord;
 		std::map< std::string, std::vector<int> > intArrayRecord;
		std::map< std::string, std::vector<HBXFEMDef::UserReadPrec> > CalDataArraRecord;	//计算数据
 		std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> > NodeArrayRecord;//节点向量，在考虑使用散列表
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> > ElemtArrayRecord;//单元
		std::map< std::string, std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>> >  MatRecord;//材料,可能重名
		std::map< std::string, std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec>> >  SectionRecord;//截面，可能重名
		boost::unordered_map< std::string, std::shared_ptr< HBXFEMDef::Set> > SetRecord;//集合

	public:
		//默认构造函数
		DynamicInputRecord(std::string _keyword = "", int _id = 0);
		//拷贝构造函数
		DynamicInputRecord(const DynamicInputRecord&);
		DynamicInputRecord& operator = (const DynamicInputRecord&);
		//从给定的组件中提取记录
		DynamicInputRecord(BaseComponent& _component);

		//
		virtual ~DynamicInputRecord() {};

		//
		virtual DynamicInputRecord* GiveCopy() {return new DynamicInputRecord(*this);	};

		virtual bool SetField(int _item, InputFieldType _id);

		virtual bool SetField(float _item, InputFieldType _id);

		virtual bool SetField(double &_input, InputFieldType _id);

		virtual bool SetField(std::vector<int> _item, InputFieldType _id);

		virtual bool SetField(HBXFEMDef::Node  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>>  _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id);

		virtual bool SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id);

		//输出为void指针，访问其下所有数据。
		//前缀为ALL_的id宏，表明获取某种（比如单元，节点）数据的完整数据结构
		virtual InputFileResult_t GiveField(void* answer, DataType _id);

		virtual InputFileResult_t GiveField(int &answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(float &answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(double &answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::string &answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::shared_ptr<std::vector<HBXFEMDef::Node>> answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::vector<HBXFEMDef::UserReadPrec>* answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::shared_ptr<HBXFEMDef::NSortMat<HBXDef::UserCalPrec>>* answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserCalPrec> > answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserCalPrec> > answer, InputFieldType _id);

		virtual InputFileResult_t GiveField(std::shared_ptr<HBXFEMDef::Set> answer, InputFieldType _id);

		virtual HBXDef::_AEROTABLE* GiveField( InputFieldType _id);

		std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> >* GetNodeArray() { return &this->NodeArrayRecord; };
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserCalPrec>> >*
			GetNSortMat() { return &this->ElemtArrayRecord; };
	};


	DynamicInputRecord* CreateNodeIR(int _id, InputFieldType nodeType, Node _n);

	DynamicInputRecord* CreateElementIR(int _id, InputFieldType elementType, std::vector<int> nodes, int cs = 0);

}