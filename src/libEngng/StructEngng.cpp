#include "StructEngng.h"


namespace HBXFEMDef
{

	StructEngng::StructEngng(  Engng* _master, int _n )
		:Engng( _master, _n )
	{

	}

	StructEngng::~StructEngng()
	{
	}


	void StructEngng::UpdateInternalState(TimeStep * _ts)
	{
		//遍历所有part
		for (auto &MyDom: domainList)
		{
			
			for ( auto& _bc:MyDom->_vLoad)
			{

			}
#ifndef CUDA
			//遍历每个单元进行运算
			for (auto &elem : MyDom->_vElmt)
			{
				
			}
#else
			//并行算法
#endif
		}
		
	}

	void StructEngng::UpdataSelf(TimeStep * _ts)
	{
		this->UpdateInternalState(_ts);
		Engng::UpdataSelf(_ts);
	}

}

