#pragma once

#include <set>
#include <HBXPreDef.h>
#include <libCUFEM\BaseComponent.h>
#include <Package.h>




namespace HBXFEMDef
{

	class MessageDispatcher
	{
	private:
	protected:
		//
		std::set<Package> PriorityQ;

		void Discharge(BaseComponent* _pRec, const Package& msg);

		MessageDispatcher() {}
		MessageDispatcher(const MessageDispatcher&);
		MessageDispatcher& operator=(const MessageDispatcher&);
	public:
		//this class is a singleton
		static MessageDispatcher* Instance();

		//send a message to another agent. Receiving agent is referenced by ID.
		void DispatchMessage(double  delay,
			int    sender,
			int    receiver,
			int    msg,
			void*  ExtraInfo);

		//send out any delayed messages. This method is called each time through   
		//the main game loop.
		void DispatchDelayedMessages();
	};



}
