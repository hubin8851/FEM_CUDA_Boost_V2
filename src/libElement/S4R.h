#pragma once

#include <ExportDef.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h.>
#include <libCUFEM\EngngMod.h>
#include "libCUFEM\ClassFactory.h"
#include <FSM\StateMachine.h>

namespace HBXFEMDef
{
	using namespace HBXDef;

	template <class Entity_type> class State;
	template<typename _T> struct _Nodes;

	class Domain;

	class CUFEM_API S4RElemt :public BaseElem
	{
	private:
	protected:
		StateMachine<S4RElemt>* m_pStateMachine;
	public:

		size_t * _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
		size_t _iMat;	//��Ԫ��������

		S4RElemt(	Engng* _engng, Domain* _dm, 
					int _id /*= S4RELEMENT */ );


		virtual ~S4RElemt();

		//�˺�������ˢ�µ�ǰ��Ԫ��״̬
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<S4RElemt>* GetFSM() const { return m_pStateMachine; };

		//������
		//		void ResetDomain(Domain* _dm);

		//�նȾ������
		void StiffMatCal();
		//�����������
		void MassMatCal();


	};

#define _EX_S4RElemt_Name "S4R"
	//�򹤳�����ע��ý�������
	REGIST_ELEMT(S4RElemt);

}
