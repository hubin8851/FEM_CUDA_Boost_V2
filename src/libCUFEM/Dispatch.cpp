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
		//获取接受者和发送者的指针
		BaseComponent* pSender = this->GetEngng()->GetEntityFromID(sender);

		if (message_type::Msg_BroadCast == receiver)	//判断是否是广播通知
		{
			for (auto iter = this->GetEngng()->GetEntity()->begin(); this->GetEngng()->GetEntity()->end() != iter; iter++)
			{
				BaseComponent* pReceiver = GetEngng()->GetEntityFromID(iter->first);
				
				if (pReceiver == NULL)
				{
					std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";
					return;
				}

				//创建消息包
				Package tmpPackage(0, sender, receiver, msg, ExtraInfo);
				//如果未有异常，直接抛出                     
				if (delay <= 0.0f)
				{
					//将消息包发给接收端
					Discharge(pReceiver, tmpPackage);
				}
				else	//当需要延迟时统计时间
				{
					HBXDef::MyTimer _tmpTimer;
					double CurrentTime = _tmpTimer.GetTime();

					tmpPackage.DispatchTime = CurrentTime + delay;
					//放入队列
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
		//如果未有异常，直接抛出                     
		if (delay <= 0.0f)
		{
			//将消息包发给接收端
			Discharge(pReceiver, tmpPackage);
		}
		else	//当需要延迟时统计时间
		{
			HBXDef::MyTimer _tmpTimer;
			double CurrentTime = _tmpTimer.GetTime();

			tmpPackage.DispatchTime = CurrentTime + delay;
			//放入队列
			PriorityQ.insert(tmpPackage);
		}
	}


}
