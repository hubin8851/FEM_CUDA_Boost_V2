#pragma once

#include <HbxDefMacro.h>
#include <HBXDefStruct.h>
#include <ExportDef.h>
#include <libCUFEM\InputRecord.h>
#include <libCUFEM\BaseComponent.h>


namespace HBXFEMDef
{

#define _EX_Element_nodes "nodes"
#define _EX_Element_crosssect "crosssect"


	template <class Entity_type> class State;
	template <class Entity_type> class StateMachine;

	using namespace HBXDef;
	template<typename _T> struct _Nodes;

	class Domain;
	class Engng;
	class BaseComponent;
// 	template<typename _T>
// 	struct _BaseElem
// 	{
// 		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
// 		size_t _iMat;	//单元材料索引
// 
// 	};

	//单元基类实现接口，派生类完成各种单元特征矩阵的算法填充。
	class CUFEM_API BaseElem:public BaseComponent
	{
	private:
	protected:
		StateMachine<BaseElem>* m_pStateMachine;

		bool m_bFresh;//刷新标志位，可能需要完成质量矩阵、刚度矩阵的刷新

		Domain*	MyDomain;
		Engng*	MyEngng;

	public:

		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

		//域在前，id号在后，如果有新的单元类型，只需要在classtype.h头文件中添加后编译相关模块即可
		//单元类所在的域必须有，数据可以没有，单元类型有默认值
		BaseElem(Engng* _engng, Domain* _dm, int _id = classType::ELEMT);

//		BaseElem(HBXDef::UserCalPrec* _begin, size_t _lgth, int _id);


		virtual ~BaseElem();

		//初始化输入数据，获取材料的相关属性
		virtual InputFileResult_t InitFrom(InputRecord * _dr);

		//此函数用以刷新当前单元的状态
		virtual void Update();

		virtual bool HandleMessage(const Package& msg);

		StateMachine<BaseElem>* GetFSM() const { return m_pStateMachine; };

		//自身函数
		void ResetDomain(Domain* _dm) { this->MyDomain = _dm; };
		Domain* GetDomain()const { return this->MyDomain; };
		
		void ResetEngng(Engng* _engng) { this->MyEngng = _engng; };
		Engng* GetEngng()const { return this->MyEngng; };

		const char* GetClassName() const { return "BaseElement"; };

		void SetFreshFlag() { m_bFresh = true; };//重置需要更新标志位为true

		void ResetFreshFlag() { m_bFresh = false; };//重置需要更新标志位为false

		bool isNeedFresh() const { return m_bFresh; };
	};

	//用以在classfactory中注册
	template<typename T> BaseElem* ElemtCreator(Engng * _master, Domain* _dm, int _id) { return (new T(_master, _dm, _id)); }
}
