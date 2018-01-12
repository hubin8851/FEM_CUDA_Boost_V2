#pragma once


#include <HBXPreDef.h>
#include "libCUFEM\BaseNode.h"
#include <libCUFEM\BaseElement.h.>
#include <libCUFEM\EngngMod.h>
#include <FSM\StateMachine.h>

namespace HBXFEMDef
{
	using namespace HBXDef;

	template <class Entity_type> class State;
	template<typename _T> struct _Nodes;

	class Domain;
	// 	template<typename _T>
	// 	struct _BaseElem
	// 	{
	// 		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
	// 		size_t _iMat;	//��Ԫ��������
	// 
	// 	};

	class BOOST_SYMBOL_EXPORT Tri2D3NElemt :public BaseElem
	{
	private:
	protected:
		StateMachine<Tri2D3NElemt>* m_pStateMachine;
	public:

		size_t * _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
		size_t _iMat;	//��Ԫ��������

		Tri2D3NElemt( Engng* _engng, Domain* _dm = nullptr, int _id = TRI2D3NELEMET);

		Tri2D3NElemt(HBXDef::UserCalPrec* _begin, size_t _lgth);


		virtual ~Tri2D3NElemt();

		//�˺�������ˢ�µ�ǰ��Ԫ��״̬
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<Tri2D3NElemt>* GetFSM() const { return m_pStateMachine; };

		//������
//		void ResetDomain(Domain* _dm);

		//�նȾ������
		void StiffMatCal();
		//�����������
		void MassMatCal();


	};


}