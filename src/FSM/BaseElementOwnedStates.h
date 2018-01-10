#pragma once


#include "FSM\State.h"


namespace HBXFEMDef
{
	class BaseElem;
	class Package;


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

		virtual bool OnMessage(BaseElem* _ele, const Package& msg) { return false; };
	};


	//��ǰ�������������ܴ���һ�����ڶԶ��ͬ����element���м���ʱ��ʹ����
	class FreshAttrCal :public State<BaseElem>
	{
	private:
		FreshAttrCal() {}
		FreshAttrCal(const FreshAttrCal&);
		FreshAttrCal& operator=(const FreshAttrCal&);
	protected:
	public:


		virtual void Enter(BaseElem* _ele){};

		virtual void Execute(BaseElem* _ele) {};

		virtual void Exit(BaseElem* _ele) {};

		virtual bool OnMessage(BaseElem* _ele, const Package& msg) { return false; };
	};

}
