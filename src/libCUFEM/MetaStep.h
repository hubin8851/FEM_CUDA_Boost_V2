#pragma once

#include <HBXPreDef.h>
#include <boost\dll.hpp>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	class Engng;
	class BaseReader;
	class AttriRecord;

	class BOOST_SYMBOL_EXPORT MetaStep
	{
	private:
	protected:
		HBXDef::MyCounter _iNum;	//当前元步ID
		Engng*	_EngBelong;
		HBXDef::MyCounter NumOfSteps;	//当前元步内子步数
		AttriRecord		_AttrRecord;	//当前元步属性表
	public:
		MetaStep( HBXDef::MyCounter _n, Engng* _belong );
		MetaStep(HBXDef::MyCounter _n, Engng* _belong, HBXDef::MyCounter _nStp );

		UserStatusError_t InitFrom(AttriRecord * _attrRec);

		~MetaStep();

		//获取当前元步ID
		HBXDef::MyCounter GetNum() const { return _iNum; };

		//设置当前元步下的子步数
		void SetNumOfSteps( HBXDef::MyCounter _newNum);
		//获的当前元步下的子步数
		HBXDef::MyCounter GetNumOfSteps() const { return this->NumOfSteps; };

		//返回当前元步的属性表
		AttriRecord* GetAttrRecord() { return &this->_AttrRecord; };

	};

}
