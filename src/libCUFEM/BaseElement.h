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
// 		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
// 		size_t _iMat;	//单元材料索引
// 
// 	};

	class BOOST_SYMBOL_EXPORT BaseElem:public BaseComponent
	{
	private:
	protected:
		StateMachine<BaseElem>* m_pStateMachine;
	public:

		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

		BaseElem(int _id, Domain* _dm);

		BaseElem(HBXDef::UserCalPrec* _begin, size_t _lgth);


		virtual ~BaseElem();

		//此函数用以刷新当前单元的状态
		virtual void Update();

		virtual bool HandleMessage(const Telegram& msg);

		StateMachine<BaseElem>* GetFSM() const { return m_pStateMachine; };

		//自身函数
		const char* GetClassName() const { return "BaseElement"; };
	};


}
