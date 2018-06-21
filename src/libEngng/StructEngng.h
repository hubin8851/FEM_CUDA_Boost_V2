#pragma once
#include <ExportDef.h>
#include <HbxCudaDef\HbxCuDefStruct.h>
#include <HbxCudaDef\multithreading.h>
#include <boost\threadpool.hpp>
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

		boost::threadpool::pool* m_threadpool;	//boost�̳߳�ָ��
		CUTBarrier thread_barrier;	//�߳�դ�񣬸���ص��ͻ�ȡ�߳�ID
		int N_workloads;			//�������طֿ飬������Ӧ���ݴ���/����ռ�ȶ�̬����
		HbxCuDef::EltMatCalworkload* workloads;	//��Ԫ���㸺��ָ��,һ�����ض�Ӧһ�ֵ�Ԫ

	public:
		StructEngng( Engng* _master = nullptr, int _n = classType::ENGNG, int _threadNum = 4 );
		virtual ~StructEngng();

#pragma region ������غ���

		//����,����Ҫ�ĺ���,ֱ�ӵ��û��ຯ��
//		virtual void Solve();

		//�ṹ��������ÿһ����ʼ��ʱ�����õ�GPU������Ҫ��Բ�ͬ�ĵ�Ԫ
		virtual void InitAt(TimeStep* _ts);

		//���㵱ǰ��
		virtual void SolveAt(TimeStep* _ts);

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

