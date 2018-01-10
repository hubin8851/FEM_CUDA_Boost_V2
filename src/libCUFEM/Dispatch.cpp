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

		//ȷ�����ܶ�������
		if (pReceiver == NULL)
		{
			std::cout << "\nWarning! No Receiver with ID of " << receiver << " found";
			return;
		}

		//������Ϣ��
		Package tmpPackage( 0, sender, receiver, msg, ExtraInfo );

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
