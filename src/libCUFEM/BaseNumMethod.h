#pragma once

#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\BaseComponent.h>

namespace HBXFEMDef
{
	class Engng;
	class Domain;
	class BaseComponent;

	//数值计算方法的基类。
	//针对特定的实例会选用一系列不同的数值方法。派生类会针对不同的问题派生不同的函数体。
	//根据虚函数派生出来的接口通过相关参数得到，算法返回算法状态枚举值。
	//数据通过传参的方式传给该类。每一个特定的引擎实例负责将相应的线性方程部分传给相应的数值计算组件。
	//这样可以将算法实现和数据剥离
	class CUFEM_API BaseNumMethod:BaseComponent
	{
	private:
	protected:
		Domain*	MyDomain;
		Engng*	MyEngng;


	public:
		BaseNumMethod(Domain* _dm, Engng* _eng, int _id = classType::NUMERICALMETHOD);

		virtual ~BaseNumMethod();

		//返回当前算法的拥有者
		Engng*	GetEngng() { return MyEngng; };

		//从文件中读取数据
		virtual InputFileResult InitFrom() { return InputFileResult::IRRT_OK; };

		virtual void Reset();

		virtual void ResetDomain(Domain* _dm) { MyDomain = _dm; };

		virtual const char* GetClassName()const { return typeid(BaseNumMethod).name(); };

		virtual UserStatusError_t Solve( SparseMat& _A, HBXDef::UserCalPrec& b, HBXDef::UserCalPrec& _x ) = 0;
	};

	template<typename T> BaseNumMethod* NumerialSolverCreator() { return new T(); };
	template<typename T> BaseNumMethod* NumerialSolverCreator(Domain *_dm, Engng* _eg) { return new T(_dm, _eg); };

}
