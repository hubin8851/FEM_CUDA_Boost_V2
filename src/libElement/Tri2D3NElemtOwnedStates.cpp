#include "Tri2D3NElemtOwnedStates.h"
#include <libElement\Tri2D3NElemt.h>
#include <libCUFEM\Dispatch.h>

namespace HBXFEMDef
{

	Tri2D3NReadyToCal* Tri2D3NReadyToCal::Instance()
	{
		static Tri2D3NReadyToCal instance;
		return &instance;
	}

	void Tri2D3NReadyToCal::Enter(Tri2D3NElemt * _ele)
	{
		//更新标志位是否放在当前函数。
		if (!_ele->isNeedFresh())//如若不需要刷新
		{
			_ele->GetFSM()->RevertToPrevious();//延续上一步
		}

	}

	void Tri2D3NReadyToCal::Execute(Tri2D3NElemt * _ele)
	{
		//一旦需要刷新，重新计算刚度矩阵,计算完成后发送消息并退回至等待计算状态
		if (_ele->isNeedFresh())
		{
			_ele->StiffMatCal();
			_ele->ResetFreshFlag();

			//通知其他组件，当前单元矩阵计算完成
			_ele->GetEngng()->GetDispatcher()->DispatchMessage(SEND_MSG_IMMEDIATELY, _ele->GetID(), classType::NUMERICALMETHOD,
				Msg_ElemtStiffReady, NO_ADDITIONAL_INFO);
		}

	}

	void Tri2D3NReadyToCal::Exit(Tri2D3NElemt * _ele)
	{
		//切换至另一状态的出口函数
	}

	bool Tri2D3NReadyToCal::OnMessage(Tri2D3NElemt* _ele, const Package& msg)
	{
		switch (msg.Msg)
		{
		case HBXDef::Msg_MaterialChange:
			_ele->SetFreshFlag();	//标志位设为true，即需要重新计算
			_ele->ResetDomain( (Domain*)(msg.DataInfo) );	//刷新数据
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
		//一旦需要刷新，重新计算刚度矩阵,计算完成后发送消息并退回至等待计算状态
// 		_ele->StiffMatCal();
// 
// 		Dispatcher->DispatchMessage( SEND_MSG_IMMEDIATELY, _ele->GetID(), 1, 
// 								Msg_ElasticReady, NO_ADDITIONAL_INFO );
// 
// 		_ele->GetFSM()->ChangeState(Tri2D3NReadyToCal::Instance());
	}

	void Tri2D3NFreshCal::Exit(Tri2D3NElemt * _ele)
	{
	}

	bool Tri2D3NFreshCal::OnMessage(Tri2D3NElemt * _ele, const Package & msg)
	{
// 		switch (msg.Msg)
// 		{
// 		case HBXDef::Msg_AeroDynReady:
// 			_ele->GetFSM()->ChangeState(Tri2D3NFreshCal::Instance());
// 			return true;
// 		}
 		return false;
	}

}


