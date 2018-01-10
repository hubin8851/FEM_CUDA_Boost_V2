#pragma once

#include <FSM\State.h>

namespace HBXFEMDef
{
	template<class Entity_type>
	class StateMachine
	{
	private:
	protected:
		//该状态机拥有者的指针
		Entity_type * m_pOwner;

		//
		State<Entity_type>*	m_pCurrState;

		State<Entity_type>*	m_pPreviousState;

		//每次状态机均在update函数中调用
		State<Entity_type>*	m_pGlobalState;
	public:
		StateMachine(Entity_type* _Owner) : m_pOwner(_Owner),
			m_pCurrState(nullptr), m_pPreviousState(nullptr), m_pGlobalState(nullptr)
		{

		}

		virtual ~StateMachine() {};

		//初始化状态机
		void SetCurrentState(State<Entity_type>* s) { m_pCurrState = s; }
		void SetGlobalState(State<Entity_type>* s) { m_pGlobalState = s; }
		void SetPreviousState(State<Entity_type>* s) { m_pPreviousState = s; }

		//刷新状态机
		void  Update()const
		{
			//如果全局状态存在，调用该方法
			if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);
			//
			if (m_pCurrState) m_pCurrState->Execute(m_pOwner);
		}


		bool HandleMessage(const Telegram& msg) const
		{
			//如果当前状态可用，则发送消息
			if (m_pCurrState && m_pCurrState->OnMessage(m_pOwner, msg))
			{
				return true;
			}
			//若当前状态无效而全局状态有效，则发送至全局状态
			if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
			{
				return true;
			}

			return false;
		}

		//改变至一个新状态
		//流程为：记录当前状态至上一状态，解除该状态，传入新状态至当前状态，绑定
		void ChangeState(State<Entity_type>* pNewState)
		{
			
			m_pPreviousState = m_pCurrState;

			m_pCurrState->Exit(m_pOwner);

			m_pCurrState = pNewState;

			m_pCurrState->Enter(m_pOwner);
		}

		void RevertToPrevious()
		{
			ChangeState(m_pPreviousState);
		}


	};





}