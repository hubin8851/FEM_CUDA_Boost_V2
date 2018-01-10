#include <iostream>
#include "Dispatch.h"
#include <libCUFEM\ClassFactory.h>
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
	MessageDispatcher * MessageDispatcher::Instance()
	{
		static MessageDispatcher instance;
		return &instance;
	}

	void MessageDispatcher::DispatchMessage(double delay, int sender, int receiver, int msg, void * ExtraInfo)
	{
		//get pointers to the sender and receiver
		ClassFactory& classFactory = GetClassFactory();
		BaseComponent* pSender = classFactory.GetEntityFromID(sender);
		BaseComponent* pReceiver = classFactory.GetEntityFromID(receiver);

		//确保接受对象正常
		if (pReceiver == NULL)
		{
			std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";
			return;
		}

		//创建消息包
		Package tmpPackage( 0, sender, receiver, msg, ExtraInfo );

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
