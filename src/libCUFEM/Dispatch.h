#pragma once

#include <set>
#include <ExportDef.h>
//#include <libCUFEM\BaseComponent.h>





namespace HBXFEMDef
{
	class Engng;
	class BaseComponent;
	class Package;

	CUFEM_API class MessageDispatcher
	{
	private:
	protected:
		Engng* MyEngng;
		//�ӳٷ��Ͱ��Ķ���
		std::set<Package> PriorityQ;

		void Discharge(BaseComponent* _pRec, const Package& msg);
		
		MessageDispatcher(const MessageDispatcher&);
		MessageDispatcher& operator=(const MessageDispatcher&);
	public:
		//this class is a singleton
//		static MessageDispatcher* Instance();

		MessageDispatcher(Engng* _Engng) :MyEngng(_Engng) {};

		//send a message to another agent. Receiving agent is referenced by ID.
		void DispatchMes(double  delay,
			int    sender,
			int    receiver,
			int    msg,
			void*  ExtraInfo);

		//�����ӳ���Ϣ��ֻ����������������ʱʹ��
		void DispatchDelayedMessages();

		Engng* GetEngng() const { return this->MyEngng; };
	};



}
