#pragma once

#include <ExportDef.h>
#include "libCUFEM\EngngMod.h"
#include "libCUFEM\ClassFactory.h"

namespace HBXFEMDef
{
#define _EX_Fluid2DEngng_Name "Fluid2DProblem"

	class BaseNumMethod;	//数值算法的基类

	//二维流体解算器，用以解算2维翼型的气动特性
	//主要分为四大步
	//1.通过流体速度计算力
	//2更新流体速度域
	//3.
	class CUFEM_API Fluid2DEngng:
		public Engng
	{
	private:
	protected:
		void advectVelocity();


	public:
		static int m_xDim; //解算域x方向的尺寸
		static int m_yDim; //解算域y方向的尺寸

		Fluid2DEngng(Engng* _master = nullptr, int _n = classType::ENGNG);
		virtual ~Fluid2DEngng();

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
		virtual const char* GetClassName() const { return "Fluid2DEngng"; };

	};

	//向工厂类中注册该解算引擎
	REGIST_ENGNG(Fluid2DEngng);
}
