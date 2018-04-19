#include <iostream>
#include "Dispatch.h"
#include <libCUFEM\ClassFactory.h>
#include <libCUFEM\BaseComponent.h>
#include <libCUFEM\EngngMod.h>
#include <MyTimer.h>

namespace HBXFEMDef
{
	void MessageDispatcher::Discharge(BaseComponent * _pRec, const Package & msg)
	{
		if (!_pRec->HandleMessage(msg))
		{
			//telegram could not be handled
			std::cout << "Message not handled";
		}
	}

// 	MessageDispatcher * MessageDispatcher::Instance()
// 	{
// 		static MessageDispatcher instance;
// 		return &instance;
// 	}

	void MessageDispatcher::DispatchMes(double delay, int sender, int receiver, int msg, void * ExtraInfo)
	{
		//��ȡ�����ߺͷ����ߵ�ָ��
		BaseComponent* pSender = this->GetEngng()->GetEntityFromID(sender);

		if (message_type::Msg_BroadCast == receiver)	//�ж��Ƿ��ǹ㲥֪ͨ
		{
			for (auto iter = this->GetEngng()->GetEntity()->begin(); this->GetEngng()->GetEntity()->end() != iter; iter++)
			{
				BaseComponent* pReceiver = GetEngng()->GetEntityFromID(iter->first);
				
				if (pReceiver == NULL)
				{
					std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";
					return;
				}

				//������Ϣ��
				Package tmpPackage(0, sender, receiver, msg, ExtraInfo);
				//���δ���쳣��ֱ���׳�                     
				if (delay <= 0.0f)
				{
					//����Ϣ���������ն�
					Discharge(pReceiver, tmpPackage);
				}
				else	//����Ҫ�ӳ�ʱͳ��ʱ��
				{
					HBXDef::MyTimer _tmpTimer;
					double CurrentTime = _tmpTimer.GetTime();

					tmpPackage.DispatchTime = CurrentTime + delay;
					//�������
					PriorityQ.insert(tmpPackage);
				}
			}
		}

//		const auto _MmethodRange = this->GetEngng()->m_ComponentMap.equal_range(receiver);	

		BaseComponent* pReceiver = GetEngng()->GetEntityFromID(receiver);
		if (pReceiver == NULL)
		{
			std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";

			return;
		}
		Package tmpPackage(0, sender, receiver, msg, ExtraInfo);
		//���δ���쳣��ֱ���׳�                     
		if (delay <= 0.0f)
		{
			//����Ϣ���������ն�
			Discharge(pReceiver, tmpPackage);
		}
		else	//����Ҫ�ӳ�ʱͳ��ʱ��
		{
			HBXDef::MyTimer _tmpTimer;
			double CurrentTime = _tmpTimer.GetTime();

			tmpPackage.DispatchTime = CurrentTime + delay;
			//�������
			PriorityQ.insert(tmpPackage);
		}
	}


}
