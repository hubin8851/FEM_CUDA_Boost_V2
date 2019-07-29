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

		size_t * _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

		S4RElemt(	Engng* _engng, Domain* _dm, 
					int _id /*= S4RELEMENT */ );


		virtual ~S4RElemt();

		//此函数用以刷新当前单元的状态
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<S4RElemt>* GetFSM() const { return m_pStateMachine; };

		//自身函数
		//		void ResetDomain(Domain* _dm);

		//刚度矩阵计算
		void StiffMatCal();
		//质量矩阵计算
		void MassMatCal();


	};

#define _EX_S4RElemt_Name "S4R"
	//向工厂类中注册该解算引擎
	REGIST_ELEMT(S4RElemt);

}
