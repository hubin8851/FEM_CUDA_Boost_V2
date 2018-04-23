#pragma once
#include <ExportDef.h>
#include "libCUFEM\EngngMod.h"
#include "libCUFEM\ClassFactory.h"



namespace HBXFEMDef
{
#define _EX_StructEngng_Name "StructProblem"

	//Engng�������չ�࣬������ʵ�ֽṹ�����еĳ��ú����ͳ�Ա
	class CUFEM_API StructEngng:
			public Engng
	{
	private:

	protected:

	public:
		StructEngng( Engng* _master = nullptr, int _n = classType::ENGNG );
		virtual ~StructEngng();

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
		virtual const char* GetClassName() const { return "StructEngng"; };

	};



}

