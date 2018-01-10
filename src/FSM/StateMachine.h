#pragma once

#include <FSM\State.h>

namespace HBXFEMDef
{
	template<class Entity_type>
	class StateMachine
	{
	private:
	protected:
		//��״̬��ӵ���ߵ�ָ��
		Entity_type * m_pOwner;

		//
		State<Entity_type>*	m_pCurrState;

		State<Entity_type>*	m_pPreviousState;

		//ÿ��״̬������update�����е���
		State<Entity_type>*	m_pGlobalState;
	public:
		StateMachine(Entity_type* _Owner) : m_pOwner(_Owner),
			m_pCurrState(nullptr), m_pPreviousState(nullptr), m_pGlobalState(nullptr)
		{

		}

		virtual ~StateMachine() {};

		//��ʼ��״̬��
		void SetCurrentState(State<Entity_type>* s) { m_pCurrState = s; }
		void SetGlobalState(State<Entity_type>* s) { m_pGlobalState = s; }
		void SetPreviousState(State<Entity_type>* s) { m_pPreviousState = s; }

		//ˢ��״̬��
		void  Update()const
		{
			//���ȫ��״̬���ڣ����ø÷���
			if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);
			//
			if (m_pCurrState) m_pCurrState->Execute(m_pOwner);
		}


		bool HandleMessage(const Telegram& msg) const
		{
			//�����ǰ״̬���ã�������Ϣ
			if (m_pCurrState && m_pCurrState->OnMessage(m_pOwner, msg))
			{
				return true;
			}
			//����ǰ״̬��Ч��ȫ��״̬��Ч��������ȫ��״̬
			if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
			{
				return true;
			}

			return false;
		}

		//�ı���һ����״̬
		//����Ϊ����¼��ǰ״̬����һ״̬�������״̬��������״̬����ǰ״̬����
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