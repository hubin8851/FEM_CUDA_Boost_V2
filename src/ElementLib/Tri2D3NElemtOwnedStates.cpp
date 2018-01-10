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
		//��֪�����±�־λ�Ƿ���ڵ�ǰ��������֮�κ�����������
		if (!_ele->isNeedFresh())//��������Ҫˢ��
		{
			_ele->GetFSM()->RevertToPrevious();//������һ��
		}
		_ele->SetFreshFlag();
	}

	void Tri2D3NFreshCal::Execute(Tri2D3NElemt * _ele)
	{
		//һ����Ҫˢ�£����¼���նȾ���
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


