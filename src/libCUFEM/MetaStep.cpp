#include "MetaStep.h"

namespace HBXFEMDef
{
	MetaStep::MetaStep( HBXDef::MyCounter _n, Engng* _belong )
	{
		this->_iNum = _n;
		this->_EngBelong = _belong;
		this->NumOfSteps = 0;
	}

	MetaStep::MetaStep(HBXDef::MyCounter _n, Engng * _belong, HBXDef::MyCounter _nStp)
	{
		this->_iNum = _n;
		this->_EngBelong = _belong;
		this->NumOfSteps = _nStp;
	}

	UserStatusError_t MetaStep::InitFrom(AttriRecord* _attrRec)
	{
		this->_AttrRecord = *_attrRec;
		return UserStatusError_t::USER_STATUS_SUCCESS;
	}

	MetaStep::~MetaStep()
	{

	}

	int MetaStep::SetStepBounds(int _StartStepNum)
	{
		StartIdx = _StartStepNum;
		return StartIdx + NumOfSteps;
	}

	void MetaStep::SetNumOfSteps(HBXDef::MyCounter _newNum)
	{
		this->NumOfSteps = _newNum;
	}



}


