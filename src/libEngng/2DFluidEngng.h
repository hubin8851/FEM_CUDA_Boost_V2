#pragma once

#include <ExportDef.h>
#include "libCUFEM\EngngMod.h"
#include "libCUFEM\ClassFactory.h"

namespace HBXFEMDef
{
#define _EX_Fluid2DEngng_Name "Fluid2DProblem"

	class BaseNumMethod;	//��ֵ�㷨�Ļ���

	//��ά��������������Խ���2ά���͵���������
	//��Ҫ��Ϊ�Ĵ�
	//1.ͨ�������ٶȼ�����
	//2���������ٶ���
	//3.
	class CUFEM_API Fluid2DEngng:
		public Engng
	{
	private:
	protected:
		void advectVelocity();


	public:
		static int m_xDim; //������x����ĳߴ�
		static int m_yDim; //������y����ĳߴ�

		Fluid2DEngng(Engng* _master = nullptr, int _n = classType::ENGNG);
		virtual ~Fluid2DEngng();

#pragma region ������غ���

		//����,����Ҫ�ĺ���,ֱ�ӵ��û��ຯ��
		//		virtual void Solve();
		//���㵱ǰ��
		virtual void SolveAt(TimeStep* _ts) {};

		//����ڵ�״̬�仯
		void UpdateInternalState(TimeStep* _ts);

		//����ɼ��㲽������ڲ����ݵĸ��£��������е�������֮ǰ�Ľ�������Ҫ���£�����Ԫ�ڵĻ��ֵ�Ͳ�������ͬ����Ҫ���¡�
		virtual void UpdataSelf(TimeStep* _ts);

		//����ʱ�䲽��Ĭ������µ���print�����������������
		virtual void Terminate(TimeStep* _ts) {};

		//��������
		virtual void TerminateCal() {};
#pragma endregion ������غ���

		//��õ�ǰ����
		virtual const char* GetClassName() const { return "Fluid2DEngng"; };

	};

	//�򹤳�����ע��ý�������
	REGIST_ENGNG(Fluid2DEngng);
}
