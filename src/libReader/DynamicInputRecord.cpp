
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
		this->NodeRecord[_id] = std::move(_item);
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
		this->CrossRecord[_id] = std::move(_item);
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


	InputFileResult_t DynamicInputRecord::GiveField(double & answer, InputFieldType _id)
	{
		std::map< std::string, double > ::iterator _it = this->doubleRecord.find(_id);
		if (_it == this->doubleRecord.end())
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


