#pragma once
#include <HbxDefMacro.h>
#include <ExportDef.h>
#include <HBXFEMDefMacro.h>
#include <iostream>

namespace HBXFEMDef
{
	class Engng;

	//�����ʾ���㲽�����ڲ��������õ�����ָ��
	//�������ʱ�����Ľ���ʱ�䡣��һ������Ĺ���ʱ�䣬ͨ����Ŀ��ʱ����ͬ��
	//�ù���ʱ���ʾ������ϵ��������ϲ����ڽ���ʱ��ˢ�¡�
	//Ĭ������±��α߽�������instrinsicʱ�䣬�Ա�����̬ϵͳ�͵�ʽ��ÿ��ʱ�䲽�ı仯
	//

	class CUFEM_API  TimeStep
	{
	private:
		Engng * _myEngng;

	protected:

		//��ǰ����ʱ�������
		double _deltaT;

		//���״̬����
		unsigned long _MySlnStateCount;

		//��ǰĿ��ʱ�䣬����ǰʱ�䲽�Ľ���ʱ��
		double _targetT;

		//����ʱ��߽�ĵ�ǰ����ʱ��
		double _instrinsicT;

		//�����ߵ����
		int number;

		//��Ӧ��metastep��
		int  _mStepNum;

		//ʱ����ɢ�������ַ���
		TimeDiscType _tDiscret;

	public:
		TimeStep( Engng* _master, int _n, double tt, double dt, TimeDiscType td = TD_Unspecified );
		TimeStep(const TimeStep&);

		void IncreaseStateCounter() { this->_MySlnStateCount++; };

		int GetNum() { return number; };

		int GetMetaStepNum() { return _mStepNum; };


		const char* GetClassName() const { return typeid(TimeStep).name(); };

	};

}