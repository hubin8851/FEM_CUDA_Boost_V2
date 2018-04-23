#pragma once
#include <ExportDef.h>
#include "libCUFEM\EngngMod.h"
#include "libCUFEM\ClassFactory.h"



namespace HBXFEMDef
{
#define _EX_StructEngng_Name "StructProblem"

	//Engng基类的拓展类，声明和实现结构计算中的常用函数和成员
	class CUFEM_API StructEngng:
			public Engng
	{
	private:

	protected:

	public:
		StructEngng( Engng* _master = nullptr, int _n = classType::ENGNG );
		virtual ~StructEngng();

#pragma region 计算相关函数

		//计算,最主要的函数,直接调用基类函数
//		virtual void Solve();
		//计算当前步
		virtual void SolveAt(TimeStep* _ts) {};

		//计算节点状态变化
		void UpdateInternalState(TimeStep* _ts);

		//在完成计算步后完成内部数据的更新（例如所有的数据在之前的解算中需要更新）。单元内的积分点和材料属性同样需要更新。
		virtual void UpdataSelf(TimeStep* _ts);

		//结束时间步。默认情况下调用print函数输出至给定的流
		virtual void Terminate(TimeStep* _ts) {};

		//结束计算
		virtual void TerminateCal() {};
#pragma endregion 计算相关函数

		//获得当前类名
		virtual const char* GetClassName() const { return "StructEngng"; };

	};



}

