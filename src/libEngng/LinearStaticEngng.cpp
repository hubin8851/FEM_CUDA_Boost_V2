
#include "LinearStaticEngng.h"
#include <libCUFEM\TimerManager.h>
#include <libCUFEM\timestep.h>
#include "..\libElement\ElementStiffCal.h"

namespace HBXFEMDef
{

	LinearStaticEngng::LinearStaticEngng(Engng* _master, int _n, int _threadNum):StructEngng(_master, _n, _threadNum)
	{
		bInit = false;
		spMatType = HBXDef::SpMatrixType_t::SMT_CSR;
		solverType = HBXDef::SolveMethod_t::QR_Mtd;
	}

	LinearStaticEngng::~LinearStaticEngng()
	{
	}


	void LinearStaticEngng::init()
	{
	}

	void LinearStaticEngng::Solve()
	{
		//��ʼ��ʱ
		this->_timer.startTimer(EngngTimer::EMT_ANALYSIS);
		StructEngng::Solve();
		this->_timer.stopTimer(EngngTimer::EMT_ANALYSIS);
	}

	void LinearStaticEngng::SolveAt(TimeStep * _ts)
	{
		//�����δ��ʼ��,��ɳ�ʼ������
		if (!InitFlag)
		{
			//ָ�����ã�ָ���µ�ϡ���������
			_StiffMtx.reset(classFactory.CreatSparseMatrix(spMatType));
			if (!_StiffMtx) {
				std::cerr<<"sparse matrix creation failed"<<std::endl;
			}

			_StiffMtx->BuildInternalParam(this, 1);

			if (_serial != bParralel)
			{
				//�����㷨���㵥Ԫ�նȾ���
				for (int i = 0; i < N_workloads; ++i)
				{
					m_threadpool->schedule(boost::bind(EletStiffCal, &workloads[i]));
				}
				m_threadpool->wait();
				cutWaitForBarrier(&thread_barrier);
				printf("����%d�ֵ�Ԫ�նȾ����м������ :\n", N_workloads);

			}

			this->Assemble( *_StiffMtx, _ts, this->GetDomain(1) );
		}
#ifdef _DEBUG
		std::cout << "ϵͳ�նȾ�����װ���" << std::endl;
#endif // _DEBUG

		//��ʼ���غ�������λ������
		delete[] _LoadArray;
		_LoadArray = new HBXDef::UserCalPrec[this->GetNumOfDomainEquations(1)];
		delete[] _DisplayArray;
		_DisplayArray = new HBXDef::UserCalPrec[this->GetNumOfDomainEquations(1)];

		//ѡ����ֵ���㷽��
		this->GetNumericalMethod(this->GetMetaStep(_ts->GetMetaStepNum()));

		UserStatusError_t _state = this->_MyMethod->Solve(*_StiffMtx, *_LoadArray, *_DisplayArray);
		//���������״̬
		CheckUserDefErrors(_state);
		//״̬��������
		_ts->IncreaseStateCounter();
	}

	void LinearStaticEngng::UpdataSelf(TimeStep * _ts)
	{
	}

	void LinearStaticEngng::Terminate(TimeStep * _ts)
	{
	}

	void LinearStaticEngng::TerminateCal()
	{
	}

	void LinearStaticEngng::SaveStepContext(TimeStep * _ts)
	{
	}

	BaseNumMethod * LinearStaticEngng::GetNumericalMethod(MetaStep * _mstp)
	{
		if (!_MyMethod)
		{
			_MyMethod.reset(classFactory.CreateNumMethod(solverType, this->GetDomain(1), this));
		}
		return _MyMethod.get();
	}

}

