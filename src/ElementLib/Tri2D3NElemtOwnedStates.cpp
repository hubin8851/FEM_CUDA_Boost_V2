#include "Tri2D3NElemtOwnedStates.h"
#include <ElementLib\Tri2D3NElemt.h>



namespace HBXFEMDef
{

	Tri2D3NReadyToCal* Tri2D3NReadyToCal::Instance()
	{
		static Tri2D3NReadyToCal instance;
		return &instance;
	}

	void Tri2D3NReadyToCal::Enter(Tri2D3NElemt * _ele)
	{
//		std::cout<<"\n"<< GetClassFactory.

	}

	void Tri2D3NReadyToCal::Execute(Tri2D3NElemt * _ele)
	{
		
	}

	void Tri2D3NReadyToCal::Exit(Tri2D3NElemt * _ele)
	{
	}

	bool Tri2D3NReadyToCal::OnMessage(Tri2D3NElemt* _ele, const Package& msg)
	{
		switch (msg.Msg)
		{
		case HBXDef::Msg_AeroDynReady :
			_ele->GetFSM()->ChangeState(Tri2D3NFreshCal::Instance() );
			return true;
		}
		return false;
	}

	Tri2D3NFreshCal * Tri2D3NFreshCal::Instance()
	{
		static Tri2D3NFreshCal instance;
		return &instance;
	}

	void Tri2D3NFreshCal::Enter(Tri2D3NElemt * _ele)
	{
		//不知道更新标志位是否放在当前函数还是之形函数，待定。
		if (!_ele->isNeedFresh())//如若不需要刷新
		{
			_ele->GetFSM()->RevertToPrevious();//延续上一步
		}
		_ele->SetFreshFlag();
	}

	void Tri2D3NFreshCal::Execute(Tri2D3NElemt * _ele)
	{
		//一旦需要刷新，重新计算刚度矩阵
		_ele->StiffMatCal();

		Dispatch->DispatchMessage();
	}

	void Tri2D3NFreshCal::Exit(Tri2D3NElemt * _ele)
	{
	}

	bool Tri2D3NFreshCal::OnMessage(Tri2D3NElemt * _ele, const Package & msg)
	{
		return false;
	}

}


