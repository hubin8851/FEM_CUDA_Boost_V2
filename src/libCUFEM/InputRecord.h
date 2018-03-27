#pragma once
#include <HBXPreDef.h>
#include <BoostPreDef.h>
#include <HBXFEMDefStruct.h>
#include <libCUFEM\Set.h>
#include <libCUFEM\NSortMat.h>
#include <libCUFEM\BaseMaterial.h>
#include <libCUFEM\BaseSection.h>
#include <libCUFEM\BaseNode.h>

namespace HBXFEMDef
{
	typedef const char *InputFieldType;


	class BOOST_SYMBOL_EXPORT InputRecord
	{
	public:
		//默认构造函数
		InputRecord() {};
		//拷贝构造函数
		InputRecord(const InputRecord&) {};
		//
		virtual ~InputRecord() {};

		InputRecord& operator = (const InputRecord&) { return *this; };

		virtual InputRecord* GiveCopy() = 0;

		virtual void SetField(int _item, InputFieldType _id) = 0;

		virtual void SetField(double &_input, InputFieldType _id) = 0;

		virtual void SetField(std::vector<int> _item, InputFieldType _id) = 0;

		virtual void SetField(HBXFEMDef::Node  _item, InputFieldType _id) = 0;

		virtual void SetField(std::shared_ptr<std::vector<HBXFEMDef::Node>>  _item, InputFieldType _id) = 0;

		virtual void SetField(std::shared_ptr<HBXFEMDef::Set> _item, InputFieldType _id) = 0;

		virtual void SetField(std::shared_ptr< HBXFEMDef::NSortMat<HBXDef::UserReadPrec> > _item, InputFieldType _id) = 0;

		virtual void SetField(std::shared_ptr< HBXFEMDef::_Section<HBXDef::UserReadPrec> > _item, InputFieldType _id) = 0;

		virtual void SetField(std::shared_ptr< HBXFEMDef::_Material<HBXDef::UserReadPrec> > _item, InputFieldType _id) = 0;


		virtual InputFileResult_t GiveField(double &answer, InputFieldType id) = 0;

		 
	};



}
