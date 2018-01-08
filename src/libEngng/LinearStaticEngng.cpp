
#include "LinearStaticEngng.h"

namespace HBXFEMDef
{

	LinearStaticEngng::LinearStaticEngng(int _n, Engng* _master):StructEngng(_n, _master)
	{
		bInit = false;
		solverType = HBXDef::SolveMethod_t::QR_Mtd;
	}

	LinearStaticEngng::~LinearStaticEngng()
	{
	}


	void LinearStaticEngng::init()
	{
	}

	void LinearStaticEngng::Solve()
	{
		if ( this->isParallel() )
		{
			//to be continue
		}
		StructEngng::Solve();
	}

	void LinearStaticEngng::SolveAt(TimeStep * _ts)
	{
// 		if ()
// 		{
// 		}
	}

	void LinearStaticEngng::UpdataSelf(TimeStep * _ts)
	{
	}

	void LinearStaticEngng::Terminate(TimeStep * _ts)
	{
	}

	void LinearStaticEngng::TerminateCal()
	{
	}

	void LinearStaticEngng::SaveStepContext(TimeStep * _ts)
	{
	}

	BaseNumMethod * LinearStaticEngng::GetNumericalMethod(MetaStep * _mstp)
	{

		return nullptr;
	}

}

