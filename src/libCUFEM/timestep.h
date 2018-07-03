#pragma once
#include <HbxDefMacro.h>
#include <ExportDef.h>
#include <HBXFEMDefMacro.h>
#include <iostream>

namespace HBXFEMDef
{
	class Engng;

	//该类表示解算步。其内部包含调用的引擎指针
	//该类持有时间间隔的结束时间。有一个额外的固有时间，通常与目标时间相同。
	//该固有时间表示本构关系，例如材料并不在结束时间刷新。
	//默认情况下变形边界条件在instrinsic时间，以表征静态系统和等式在每个时间步的变化
	//

	class CUFEM_API  TimeStep
	{
	private:
		Engng * _myEngng;

	protected:

		//当前绝对时间的增量
		double _deltaT;

		//求解状态计数
		unsigned long _MySlnStateCount;

		//当前目标时间，即当前时间步的结束时间
		double _targetT;

		//引入时间边界的当前固有时间
		double _instrinsicT;

		//接受者的序号
		int number;

		//相应的metastep数
		int  _mStepNum;

		//时间离散，即积分方法
		TimeDiscType _tDiscret;

	public:
		TimeStep( Engng* _master, int _n, double tt, double dt, TimeDiscType td = TD_Unspecified );
		TimeStep(const TimeStep&);

		void IncreaseStateCounter() { this->_MySlnStateCount++; };

		int GetNum() { return number; };

		int GetMetaStepNum() { return _mStepNum; };


		const char* GetClassName() const { return typeid(TimeStep).name(); };

	};

}