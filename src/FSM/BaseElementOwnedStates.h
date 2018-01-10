#pragma once


#include "FSM\State.h"


namespace HBXFEMDef
{
	class BaseElem;
	class Telegram;


	class ReadyToCal: public State<BaseElem>
	{
	private:
		ReadyToCal() {}
		ReadyToCal(const ReadyToCal&);
		ReadyToCal& operator=(const ReadyToCal&);
	protected:
	public:
		static ReadyToCal* Instance();

		virtual void Enter(BaseElem* _ele) {};

		virtual void Execute(BaseElem* _ele) {};

		virtual void Exit(BaseElem* _ele) {};

		virtual bool OnMessage(BaseElem* _ele, const Telegram& msg) { return false; };
	};


	//��ǰ�������������ܴ���һ�����ڶԶ��ͬ����element���м���ʱ��ʹ����
	class StiffMatCal :public State<BaseElem>
	{
	private:
		StiffMatCal() {}
		StiffMatCal(const StiffMatCal&);
		StiffMatCal& operator=(const StiffMatCal&);
	protected:
	public:


		virtual void Enter(BaseElem* _ele){};

		virtual void Execute(BaseElem* _ele) {};

		virtual void Exit(BaseElem* _ele) {};

		virtual bool OnMessage(BaseElem* _ele, const Telegram& msg) { return false; };
	};

}
