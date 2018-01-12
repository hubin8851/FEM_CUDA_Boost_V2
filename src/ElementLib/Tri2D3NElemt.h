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
	// 		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
	// 		size_t _iMat;	//单元材料索引
	// 
	// 	};

	class BOOST_SYMBOL_EXPORT Tri2D3NElemt :public BaseElem
	{
	private:
	protected:
		StateMachine<Tri2D3NElemt>* m_pStateMachine;
	public:

		size_t * _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

		Tri2D3NElemt( Engng* _engng, Domain* _dm = nullptr, int _id = TRI2D3NELEMET);

		Tri2D3NElemt(HBXDef::UserCalPrec* _begin, size_t _lgth);


		virtual ~Tri2D3NElemt();

		//此函数用以刷新当前单元的状态
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<Tri2D3NElemt>* GetFSM() const { return m_pStateMachine; };

		//自身函数
//		void ResetDomain(Domain* _dm);

		//刚度矩阵计算
		void StiffMatCal();
		//质量矩阵计算
		void MassMatCal();


	};


}