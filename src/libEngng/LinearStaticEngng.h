#pragma once

#include <ExportDef.h>
#include <libEngng\StructEngng.h>
#include <libCUFEM\BaseNumMethod.h>
#include <libCUFEM\ClassFactory.h>
#include <SpMatrix.h>
#include <SpMatrixType.h>
#include <SolverType.h>

namespace HBXFEMDef
{
	using namespace HBXDef;

//	class LinearSystemNM;	//线性系统数值计算方法
	class BaseNumMethod;	//数值算法的基类

	//ps：个人认为线性和非线性应该合二为一，因为可能在小扰动中时线性，而大变形中时非线性...
	//该类实现线性的静态分析，实则就是计算Ax=b的问题
	//在多载荷下仅考虑线性弹性段，而不考虑加载历史等
	//（在非线性条件下夹在历史可能导致在下一步中的加载项数值增加导致实际应变与应力不满足胡克定律）；
	class CUFEM_API LinearStaticEngng:
			public StructEngng
	{
	private:
		std::unique_ptr<HBXFEMDef::SparseMat> _StiffMtx;			//按当前解算器使用稀疏矩阵
		HBXDef::UserCalPrec*	_LoadArray;	//载荷向量
		HBXDef::UserCalPrec*	_DisplayArray;//位移向量

	protected:
		bool bInit;	//初始化标志位

		//解算器类型枚举
		HBXDef::SolveMethod_t solverType;
		//稀疏矩阵存储类型
		HBXDef::SpMatrixType_t spMatType;
		//数值算法
		std::unique_ptr< BaseNumMethod >  _MyMethod;
	public:
		LinearStaticEngng( Engng* _master = nullptr, int _n = classType::ENGNG, int _threadNum = 4);
		virtual ~LinearStaticEngng();

		//初始化
		virtual void init();

#pragma region 计算相关函数

		//计算,最主要的函数
		virtual void Solve();

		//计算当前步；线性静力学问题没有材料变化，故么有重载InitAt函数
		virtual void SolveAt(TimeStep* _ts);

		//在完成计算步后完成内部数据的更新（例如所有的数据在之前的解算中需要更新）。单元内的积分点和材料属性同样需要更新。
		virtual void UpdataSelf(TimeStep* _ts);

		//结束时间步。默认情况下调用print函数输出至给定的流
		virtual void Terminate(TimeStep* _ts);

		//结束计算
		virtual void TerminateCal();
#pragma endregion 计算相关函数

		//保存特定步的上下文
		void SaveStepContext(TimeStep* _ts);

		virtual BaseNumMethod* GetNumericalMethod( MetaStep* _mstp );

		virtual const char* GetClassName() const { return "LinearStatic"; }
	};

#define _EX_LinearStaticEngng_Name "LinearStaticProblem"
	//向工厂类中注册该解算引擎
	REGIST_ENGNG(LinearStaticEngng);


}