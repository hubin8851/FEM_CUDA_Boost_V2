#pragma once

#include <HBXDEF/HbxDefMacro.h>
#include <..\..\CUFEM\StructEngng.h>
#include <CUFEM\BaseNumMethod.h>



namespace HBXFEMDef
{


//	class LinearSystemNM;	//����ϵͳ��ֵ���㷽��
	class BaseNumMethod;	//��ֵ�㷨�Ļ���

	//ps��������Ϊ���Ժͷ�����Ӧ�ú϶�Ϊһ����Ϊ������С�Ŷ���ʱ���ԣ����������ʱ������...
	//����ʵ�����Եľ�̬������ʵ����Ǽ���Ax=b������
	//�ڶ��غ��½��������Ե��ԶΣ��������Ǽ�����ʷ��
	//���ڷ����������¼�����ʷ���ܵ�������һ���еļ�������ֵ���ӵ���ʵ��Ӧ����Ӧ����������˶��ɣ���
	class BOOST_SYMBOL_EXPORT LinearStaticEngng:
			public StructEngng
	{
	private:
	protected:
		bool bInit;	//��ʼ����־λ

		HBXDef::SolveMethod_t solverType;	//
		//��ֵ�㷨
		std::unique_ptr< BaseNumMethod >  Method;
	public:
		LinearStaticEngng( int _n, Engng* _master = nullptr );
		virtual ~LinearStaticEngng();

		//��ʼ��
		virtual void init();

#pragma region ������غ���

		//����,����Ҫ�ĺ���
		virtual void Solve();
		//���㵱ǰ��
		virtual void SolveAt( TimeStep* _ts );

		//����ɼ��㲽������ڲ����ݵĸ��£��������е�������֮ǰ�Ľ�������Ҫ���£�����Ԫ�ڵĻ��ֵ�Ͳ�������ͬ����Ҫ���¡�
		virtual void UpdataSelf(TimeStep* _ts);

		//����ʱ�䲽��Ĭ������µ���print�����������������
		virtual void Terminate(TimeStep* _ts);

		//��������
		virtual void TerminateCal();
#pragma endregion ������غ���

		//�����ض�����������
		void SaveStepContext(TimeStep* _ts);

		virtual BaseNumMethod* GetNumericalMethod( MetaStep* _mstp );

		virtual const char *giveClassName() const { return "LinearStatic"; }
	};

#define _EX_LinearStaticEngng_Name "LinearStaticProblem"
	//�򹤳�����ע��ý�������
	REGIST_ENGNG(LinearStaticEngng);


}