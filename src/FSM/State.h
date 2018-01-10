#pragma once
#include <Package.h>


namespace HBXFEMDef
{

	class Package;

	template< class Entity_type>
	class State
	{
	private:

	public:
		virtual ~State() {};

		//状态起始时入口函数
		virtual void Enter(Entity_type*) = 0;

		//this is the states normal update function
		virtual void Execute(Entity_type*) = 0;

		//this will execute when the state is exited. 
		virtual void Exit(Entity_type*) = 0;

		//this executes if the agent receives a message from the 
		//message dispatcher
		virtual bool OnMessage(Entity_type*, const Package&) = 0;


	};
}




