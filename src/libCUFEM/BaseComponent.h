#pragma once

//组件的状态对象，提供了组件的状态形式。每个组件状态有
//1.唯一标识id
//2.进入状态回调
//3.退出状态回调
//4.每帧调用处理逻辑的回调
//5.用来接受消息的回调
namespace HBXFEMDef
{
	class Package;


	class BaseComponent
	{
	private:
		void* _data;

		//映射表对应的ID
		unsigned int m_MyID;
		unsigned int m_iNextID;

	public:
		//该函数用来调用以保证ID设置的正确性。其保证传给算法的值大于或者等于ID，以保证map树状图内的值不冲突
		void ResetID(int _id);

		int GetID() const { return m_MyID; };

//自从有了typeid，妈妈再也不用担心获取classname的问题了
//		virtual const char* GetClassName() const = 0;

		BaseComponent( int _id = classType::FEMCOMPONENTCLASS):m_MyID(_id)
		{
			
		}
		virtual ~BaseComponent() {};

		//所有的组件均通过该函数刷新数据
		virtual void  Update() = 0;

		//数据交互的接口。使用messageDispatcher类发送
		virtual bool HandleMessage(const Package& msg) = 0;

		//
//		void(*OnEnter)(BaseComponent* _cpn);

		//
//		void(*OnExit)(BaseComponent* _cpn);

//		void(*Update)(BaseComponent* _cpn);

		//当接收到调用后返回布尔值
//		bool(*OnMessage)(BaseComponent* _cpt, void* sender, int subject, void* data);


	};

}

