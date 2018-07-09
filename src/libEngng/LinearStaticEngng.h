#pragma once

#include <ExportDef.h>
#include <libEngng\StructEngng.h>
#include <libCUFEM\BaseNumMethod.h>
#include <libCUFEM\ClassFactory.h>
#include <SpMatrix.h>
#include <SpMatrixType.h>
#include <SolverType.h>

namespace HBXFEMDef
{
	using namespace HBXDef;

//	class LinearSystemNM;	//����ϵͳ��ֵ���㷽��
	class BaseNumMethod;	//��ֵ�㷨�Ļ���

	//ps��������Ϊ���Ժͷ�����Ӧ�ú϶�Ϊһ����Ϊ������С�Ŷ���ʱ���ԣ����������ʱ������...
	//����ʵ�����Եľ�̬������ʵ����Ǽ���Ax=b������
	//�ڶ��غ��½��������Ե��ԶΣ��������Ǽ�����ʷ��
	//���ڷ����������¼�����ʷ���ܵ�������һ���еļ�������ֵ���ӵ���ʵ��Ӧ����Ӧ����������˶��ɣ���
	class CUFEM_API LinearStaticEngng:
			public StructEngng
	{
	private:
		std::unique_ptr<HBXDef::SparseMat> _StiffMtx;			//����ǰ������ʹ��ϡ�����
		HBXDef::UserCalPrec*	_LoadArray;	//�غ�����
		HBXDef::UserCalPrec*	_DisplayArray;//λ������

	protected:
		bool bInit;	//��ʼ����־λ

		//����������ö��
		HBXDef::SolveMethod_t solverType;
		//ϡ�����洢����
		HBXDef::SpMatrixType_t spMatType;
		//��ֵ�㷨
		std::unique_ptr< BaseNumMethod >  _MyMethod;
	public:
		LinearStaticEngng( Engng* _master = nullptr, int _n = classType::ENGNG, int _threadNum = 4);
		virtual ~LinearStaticEngng();

		//��ʼ��
		virtual void init();

#pragma region ������غ���

		//����,����Ҫ�ĺ���
		virtual void Solve();

		//���㵱ǰ�������Ծ���ѧ����û�в��ϱ仯����ô������InitAt����
		virtual void SolveAt(TimeStep* _ts);

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

		virtual const char* GetClassName() const { return "LinearStatic"; }
	};

#define _EX_LinearStaticEngng_Name "LinearStaticProblem"
	//�򹤳�����ע��ý�������
	REGIST_ENGNG(LinearStaticEngng);


}