#include "StructEngng.h"


namespace HBXFEMDef
{

	StructEngng::StructEngng(int _n, Engng * _master)
		:Engng(_n, _master)
	{

	}

	StructEngng::~StructEngng()
	{
	}


	void StructEngng::UpdateInternalState(TimeStep * _ts)
	{
		//��������part
		for (auto &MyDom: domainList)
		{
			
			for ( auto& _bc:MyDom->_vLoad)
			{

			}
#ifndef CUDA
			//����ÿ����Ԫ��������
			for (auto &elem : MyDom->_vElmt)
			{
				
			}
#else
			//�����㷨
#endif
		}
		
	}

	void StructEngng::UpdataSelf(TimeStep * _ts)
	{
		this->UpdateInternalState(_ts);
		Engng::UpdataSelf(_ts);
	}

}
