#pragma once
#include <ExportDef.h>
#include <HBXPreDef.h>
#include "libCUFEM\BaseNode.h"
#include "libCUFEM\BaseComponent.h"
#include <FSM\StateMachine.h>

namespace HBXFEMDef
{
	template <class Entity_type> class State;

	using namespace HBXDef;
	template<typename _T> struct _Nodes;

	class Domain;
// 	template<typename _T>
// 	struct _BaseElem
// 	{
// 		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
// 		size_t _iMat;	//��Ԫ��������
// 
// 	};

	class BOOST_SYMBOL_EXPORT BaseElem:public BaseComponent
	{
	private:
	protected:
		StateMachine<BaseElem>* m_pStateMachine;

		bool m_bFresh;//ˢ�±�־λ��������Ҫ����������󡢸նȾ����ˢ��
	public:

		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
		size_t _iMat;	//��Ԫ��������

		BaseElem(int _id, Domain* _dm);

		BaseElem(HBXDef::UserCalPrec* _begin, size_t _lgth);


		virtual ~BaseElem();

		//�˺�������ˢ�µ�ǰ��Ԫ��״̬
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<BaseElem>* GetFSM() const { return m_pStateMachine; };

		//������
		const char* GetClassName() const { return "BaseElement"; };

		void SetFreshFlag() { m_bFresh = true; };//������Ҫ���±�־λΪtrue

		void ResetFreshFlag() { m_bFresh = false; };//������Ҫ���±�־λΪfalse

		bool isNeedFresh() const { return m_bFresh; };
	};


}
