#pragma once

#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <boost\dll.hpp>
#include <libCUFEM\AttriRecord.h>

namespace HBXFEMDef
{
	class Engng;
	class BaseReader;
	class AttriRecord;

	//��νmetastep�����ǽ��㲽��һ�����С�metastep��Ϊ���ڹ��������ֲ�ͬʱ���εĳ�������
	//���磬metastep����һ�����㲽���У�����ģ�͵ĳ������ԡ�Ϊ�洢��Щ������ԣ�metastep��¼���Ա���ȡ
	class CUFEM_API MetaStep
	{
	private:
	protected:
		HBXDef::MyCounter _iNum;	//��ǰԪ��ID
		Engng*	_EngBelong;

		AttriRecord		_AttrRecord;	//��ǰԪ�����Ա�

		HBXDef::MyCounter NumOfSteps;	//��ǰԪ�����Ӳ���

		//��Ҫ���յ���������ģ�����ʼ��������
		int StartIdx;
	public:
		MetaStep( HBXDef::MyCounter _n, Engng* _belong );
		MetaStep(HBXDef::MyCounter _n, Engng* _belong, HBXDef::MyCounter _nStp );

		UserStatusError_t InitFrom(AttriRecord * _attrRec);

		~MetaStep();

		//��ȡ��ǰԪ��ID
		HBXDef::MyCounter GetNum() const { return _iNum; };

		//���ø�������������������
		//ʵ���ǵ����ɲ������ƫ����
		int SetStepBounds( int _StartStepNum );

		//���õ�ǰԪ���µ��Ӳ���
		void SetNumOfSteps( HBXDef::MyCounter _newNum);
		//��ĵ�ǰԪ���µ��Ӳ���
		HBXDef::MyCounter GetNumOfSteps() const { return this->NumOfSteps; };

		//��ȡ��Բ���
		int GetRelativeStepNum(int _nStep) { return (_nStep - StartIdx + 1); };

		//���ص�ǰԪ�������Ա�
		AttriRecord* GetAttrRecord() { return &this->_AttrRecord; };

	};

}
