
#include "LinearStaticEngng.h"

namespace HBXFEMDef
{

	LinearStaticEngng::LinearStaticEngng(Engng* _master, int _n, int _threadNum):StructEngng(_master, _n, _threadNum)
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

