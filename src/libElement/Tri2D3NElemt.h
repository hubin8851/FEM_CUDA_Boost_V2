#pragma once


#include <ExportDef.h>
#include "libCUFEM\BaseNode.h"
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
	// 	template<typename _T>
	// 	struct _BaseElem
	// 	{
	// 		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
	// 		size_t _iMat;	//单元材料索引
	// 
	// 	};

	class CUFEM_API Tri2D3NElemt :public BaseElem
	{
	private:
	protected:
		StateMachine<Tri2D3NElemt>* m_pStateMachine;
	public:

		size_t * _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

		Tri2D3NElemt(		Engng* _engng, Domain* _dm,
						int _id /* = TRI2D3NELEMET */ );


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

#define _EX_Tri2D3NElemt_Name "Tri3N"
	//向工厂类中注册该解算引擎
	REGIST_ELEMT(Tri2D3NElemt);
}