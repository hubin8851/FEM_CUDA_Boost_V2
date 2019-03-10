#include "StructEngng.h"
#include <boost\bind.hpp>
#include <libCUFEM\timestep.h>

//2018-6-15，胡斌星，修改
namespace HBXFEMDef
{

	StructEngng::StructEngng(  Engng* _master, int _n, int _threadNum)
		:Engng( _master, _n )
	{
		if (_cuda == bParralel)
		{
			m_threadpool = new boost::threadpool::pool(_threadNum);
			N_workloads = 5;
			workloads = (HbxCuDef::EltMatCalworkload *)malloc(N_workloads * sizeof(HbxCuDef::EltMatCalworkload));
			thread_barrier = cutCreateBarrier(N_workloads);
		}

	}

	StructEngng::~StructEngng()
	{
	}


	void StructEngng::InitAt(TimeStep * _ts)
	{
		if (_cuda == bParralel)
		{
			//初始化负载
			for (int i = 0; i < N_workloads; ++i)
			{
				workloads[i].id = i;
				workloads[i].cudaDeviceID = gpuInfo[i % m_MaxGPUs]; // i % N_gpus;
				workloads[i].eNum = i * 100;

				workloads[i].h_data = nullptr;
				workloads[i].d_data = nullptr;
				workloads[i].h_Outdata = nullptr;
				workloads[i].d_Outdata = nullptr;

			}
		}
	}



	void StructEngng::GetInternalForces(int _id, TimeStep * _stp)
	{
		Domain* _tmpDomain = this->GetDomain(_id);
		_stp->GetNum();
		//...

	}

	void StructEngng::UpdateInternalState(TimeStep * _ts)
	{
		//遍历所有part,即域
		for (auto &MyDom: domainList)
		{
			
			for ( auto& _bc:MyDom._vLoad)
			{

			}

			//遍历每个单元进行运算
// 			for (auto &elem : MyDom->_vElmt)
// 			{
// 				
// 			}

		}
		
	}

	void StructEngng::UpdataSelf(TimeStep * _ts)
	{
		this->UpdateInternalState(_ts);
		Engng::UpdataSelf(_ts);
	}

}

