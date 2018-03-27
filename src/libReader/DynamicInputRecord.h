#pragma once
#pragma warning(disable : 4786 )
#include <vector>
#include <string>
#include <map>
#include <HBXPreDef.h>
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

	//�������Ҫ����������µļ�¼���������¼���Ծ������ⷽ��ʵ�֣�
	//�������ɸ���ͳһ��ʽ��
	class BOOST_SYMBOL_EXPORT DynamicInputRecord:public InputRecord
	{
	protected:
		std::string RecordKeyWord;
		int RecordNum;

		// Record representation.
		std::set< std::string > emptyRecord; ///< Fields without values
		std::map< std::string, int > intRecord;
		std::map< std::string, double > doubleRecord;
		std::map< std::string, bool > boolRecord;
 		std::map< std::string, std::string > stringRecord;
 		std::map< std::string, std::vector<int> > intArrayRecord;
		std::map<  std::string, Node>	NodeRecord;
 		std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> > NodeArrayRecord;//�ڵ��������ڿ���ʹ��ɢ�б�
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> > ElemtArrayRecord;//��Ԫ
		boost::unordered_map< std::string, std::shared_ptr< HBXFEMDef::Set> > SetRecord;//����
		std::map< std::string, std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec>> >  CrossRecord;//���棬��������
		boost::unordered_map< std::string, std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec>> >  MatRecord;//����,��������
 		std::map< std::string, std::vector< std::string > > stringListRecord;

	public:
		//Ĭ�Ϲ��캯��
		DynamicInputRecord(std::string _keyword = "", int _id = 0);
		//�������캯��
		DynamicInputRecord(const DynamicInputRecord&);
		DynamicInputRecord& operator = (const DynamicInputRecord&);
		//�Ӹ������齨����ȡ��¼
		DynamicInputRecord(BaseComponent& _component);

		//
		virtual ~DynamicInputRecord() {};

		//
		virtual DynamicInputRecord* GiveCopy() {return new DynamicInputRecord(*this);	};

		virtual void SetField(int _item, InputFieldType _id);

		virtual void SetField(double &_input, InputFieldType _id);

		virtual void SetField(std::vector<int> _item, InputFieldType _id);

		virtual void SetField(HBXFEMDef::Node  _item, InputFieldType _id);

		virtual void SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>>  _item, InputFieldType _id);

		virtual void SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id);

		virtual void SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual void SetField(std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual void SetField(std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id);

		virtual InputFileResult_t GiveField(double &answer, InputFieldType _id);

		std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> >* GetNodeArray() { return &this->NodeArrayRecord; };
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec>> >* 
			GetNSortMat() { return &this->ElemtArrayRecord; };
	};


	DynamicInputRecord* CreateNodeIR(int _id, InputFieldType nodeType, Node _n);

	DynamicInputRecord* CreateElementIR(int _id, InputFieldType elementType, std::vector<int> nodes, int cs = 0);

}