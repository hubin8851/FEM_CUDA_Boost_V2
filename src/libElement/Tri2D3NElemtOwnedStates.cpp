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
		//���±�־λ�Ƿ���ڵ�ǰ������
		if (!_ele->isNeedFresh())//��������Ҫˢ��
		{
			_ele->GetFSM()->RevertToPrevious();//������һ��
		}

	}

	void Tri2D3NReadyToCal::Execute(Tri2D3NElemt * _ele)
	{
		//һ����Ҫˢ�£����¼���նȾ���,������ɺ�����Ϣ���˻����ȴ�����״̬
		if (_ele->isNeedFresh())
		{
			_ele->StiffMatCal();
			_ele->ResetFreshFlag();

			//֪ͨ�����������ǰ��Ԫ����������
			_ele->GetEngng()->GetDispatcher()->DispatchMessage(SEND_MSG_IMMEDIATELY, _ele->GetID(), classType::NUMERICALMETHOD,
				Msg_ElemtStiffReady, NO_ADDITIONAL_INFO);
		}

	}

	void Tri2D3NReadyToCal::Exit(Tri2D3NElemt * _ele)
	{
		//�л�����һ״̬�ĳ��ں���
	}

	bool Tri2D3NReadyToCal::OnMessage(Tri2D3NElemt* _ele, const Package& msg)
	{
		switch (msg.Msg)
		{
		case HBXDef::Msg_MaterialChange:
			_ele->SetFreshFlag();	//��־λ��Ϊtrue������Ҫ���¼���
			_ele->ResetDomain( (Domain*)(msg.DataInfo) );	//ˢ������
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
		//��֪�����±�־λ�Ƿ���ڵ�ǰ��������֮�κ�����������
		if (!_ele->isNeedFresh())//��������Ҫˢ��
		{
			_ele->GetFSM()->RevertToPrevious();//������һ��
		}
		_ele->SetFreshFlag();
	}

	void Tri2D3NFreshCal::Execute(Tri2D3NElemt * _ele)
	{
		//һ����Ҫˢ�£����¼���նȾ���,������ɺ�����Ϣ���˻����ȴ�����״̬
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


