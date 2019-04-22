
#include "DynamicInputRecord.h"

namespace HBXFEMDef
{
	DynamicInputRecord::DynamicInputRecord(std::string _keyword, int _id):InputRecord(),
		RecordKeyWord(_keyword), RecordNum(_id)
	{

	}

	DynamicInputRecord::DynamicInputRecord(const DynamicInputRecord & _src) : InputRecord(_src),
		RecordKeyWord(_src.RecordKeyWord),
		RecordNum(_src.RecordNum),
		emptyRecord(_src.emptyRecord),
		intRecord(_src.intRecord)
//		,stringListRecord(_src.stringListRecord)
	{

	}

	DynamicInputRecord & DynamicInputRecord::operator=(const DynamicInputRecord &)
	{
		// TODO: 在此处插入 return 语句

		return *this;
	}

	DynamicInputRecord::DynamicInputRecord(BaseComponent & _component)
	{
	}

	bool DynamicInputRecord::SetField(int _item, InputFieldType _id)
	{
		this->intRecord[_id] = _item;
		return true;
	}

	bool DynamicInputRecord::SetField(float _item, InputFieldType _id)
	{
		this->floatRecord[_id] = _item;
		return true;
	}

	bool DynamicInputRecord::SetField(double & _input, InputFieldType _id)
	{
		this->doubleRecord[_id] = _input;
		return true;
	}

	bool DynamicInputRecord::SetField(std::vector<int> _item, InputFieldType _id)
	{
		this->intArrayRecord[_id] = std::move(_item);
		return true;
	}

	bool DynamicInputRecord::SetField(HBXFEMDef::Node _item, InputFieldType _id)
	{
		return true;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>> _item, InputFieldType _id)
	{
		this->NodeArrayRecord[_id] = std::move(_item);
		return true;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id)
	{
		this->SetRecord.insert(std::make_pair(_id, _item));
		return true;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr< HBXFEMDef::_Section<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id)
	{
		this->SectionRecord[_id] = std::move(_item);
		return true;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr< HBXFEMDef::_Material<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id)
	{
		this->MatRecord.insert(std::make_pair(_id, _item));
		return true;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr< HBXDef::_AEROTABLE > _item, InputFieldType _id)
	{
		std::cout << "动态输入记录集内无该类型集合" << std::endl;
		return false;
	}

	bool DynamicInputRecord::SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserReadPrec> > _item, InputFieldType _id)
	{
		this->ElemtArrayRecord[_id] = std::move(_item);
		return true;
	}

	InputFileResult_t DynamicInputRecord::GiveField(void * answer, DataType _id)
	{
		switch (_id)
		{
		case ALL_INT:
			answer = (void*)&this->intRecord;
			break;
		case ALL_FLOAT:
			answer = (void*)&this->floatRecord;
			break;
		case ALL_DOUBLE:
			answer = (void*)&this->doubleRecord;
			break;
		case ALL_NODE:
			answer = (void*)&this->NodeArrayRecord;
			break;
		case ALL_ELEMENT:
			answer = (void*)&this->ElemtArrayRecord;
			break;
		case ALL_MATERIAL:
			answer = (void*)&this->MatRecord;
			break;
		case ALL_SECTION:
			answer = (void*)&this->SectionRecord;
			break;
		case ALL_SET:
			answer = (void*)&this->SetRecord;
			break;
		default:
			std::cerr << "未找到当前搜寻类型" << std::endl;
		}
		return InputFileResult_t();
	}

	InputFileResult_t DynamicInputRecord::GiveField(int & answer, InputFieldType _id)
	{
		std::map< std::string, int > ::iterator _it = this->intRecord.find(_id);
		if (_it == this->intRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = _it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(float & answer, InputFieldType _id)
	{
		std::map< std::string, float > ::iterator _it = this->floatRecord.find(_id);
		if (_it == this->floatRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = _it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(double & answer, InputFieldType _id)
	{
		std::map< std::string, double > ::iterator _it = this->doubleRecord.find(_id);
		if (_it == this->doubleRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = _it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::string & answer, InputFieldType _id)
	{
		std::map< std::string, std::string > ::iterator _it = this->stringRecord.find(_id);
		if (_it == this->stringRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = _it->second;

		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::vector<HBXFEMDef::Node>& answer,
													InputFieldType _id)
	{
		std::map< std::string, std::shared_ptr<std::vector<HBXFEMDef::Node>> > ::iterator _it = this->NodeArrayRecord.find(_id);
		if (_it == this->NodeArrayRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = *_it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::vector<HBXFEMDef::UserReadPrec>* answer, InputFieldType _id)
	{
		std::map< std::string, std::vector<HBXFEMDef::UserReadPrec> > ::iterator _it = this->CalDataArraRecord.find(_id);
		if (_it == this->CalDataArraRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = &_it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::shared_ptr<HBXFEMDef::NSortMat<HBXDef::UserCalPrec>>* answer,
													InputFieldType _id)
	{
		std::map< std::string, std::shared_ptr< HBXFEMDef::NSortMat<HBXFEMDef::UserCalPrec>> > ::iterator _it = this->ElemtArrayRecord.find(_id);
		if (_it == this->ElemtArrayRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = &_it->second;
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::shared_ptr<HBXFEMDef::_Section<HBXDef::UserCalPrec>> answer,
													InputFieldType _id)
	{
		std::map< std::string, std::shared_ptr<HBXFEMDef::_Section<HBXDef::UserCalPrec>> > ::iterator _it = this->SectionRecord.find(_id);
		if (_it == this->SectionRecord.end())
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		answer = _it->second;
		return InputFileResult_t::IRRT_OK;
	}

	
	InputFileResult_t DynamicInputRecord::GiveField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserCalPrec> > answer,
													InputFieldType _id)
	{
		answer = MatRecord[_id];
		if (answer == nullptr)
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		return InputFileResult_t::IRRT_OK;
	}

	InputFileResult_t DynamicInputRecord::GiveField(std::shared_ptr<HBXFEMDef::Set> answer, InputFieldType _id)
	{
		answer = SetRecord[_id];
		if (answer == nullptr)
		{
			return InputFileResult_t::IRRT_NOTFOUND;
		}
		return InputFileResult_t::IRRT_OK;
	}

	HBXDef::_AEROTABLE* DynamicInputRecord::GiveField( InputFieldType _id)
	{
		std::cout << "动态输入记录集内无该类型集合" << std::endl;
		return nullptr;
	}


	DynamicInputRecord * CreateNodeIR(int _id, InputFieldType nodeType, Node _n)
	{
		DynamicInputRecord *result = new DynamicInputRecord(nodeType, _id);
		result->SetField(_n, _EX_Node_coords);
		return result;
	}
	DynamicInputRecord * CreateElementIR(int _id, InputFieldType elementType, std::vector<int> nodes, int cs)
	{
		DynamicInputRecord *result = new DynamicInputRecord(elementType, _id);
		result->SetField(nodes, _EX_Element_nodes);
		if (0 != cs)
		{
			result->SetField(cs, _EX_Element_crosssect);
		}
		return result;
	}
}


