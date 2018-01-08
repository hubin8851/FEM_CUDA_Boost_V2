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
		HBXDef::MyCounter _iNum;	//��ǰԪ��ID
		Engng*	_EngBelong;
		HBXDef::MyCounter NumOfSteps;	//��ǰԪ�����Ӳ���
		AttriRecord		_AttrRecord;	//��ǰԪ�����Ա�
	public:
		MetaStep( HBXDef::MyCounter _n, Engng* _belong );
		MetaStep(HBXDef::MyCounter _n, Engng* _belong, HBXDef::MyCounter _nStp );

		UserStatusError_t InitFrom(AttriRecord * _attrRec);

		~MetaStep();

		//��ȡ��ǰԪ��ID
		HBXDef::MyCounter GetNum() const { return _iNum; };

		//���õ�ǰԪ���µ��Ӳ���
		void SetNumOfSteps( HBXDef::MyCounter _newNum);
		//��ĵ�ǰԪ���µ��Ӳ���
		HBXDef::MyCounter GetNumOfSteps() const { return this->NumOfSteps; };

		//���ص�ǰԪ�������Ա�
		AttriRecord* GetAttrRecord() { return &this->_AttrRecord; };

	};

}
