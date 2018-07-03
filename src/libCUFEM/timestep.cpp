#include "timestep.h"

namespace HBXFEMDef
{
	TimeStep::TimeStep(Engng * _master, int _n, double tt, double dt, TimeDiscType td):
		_myEngng(_master),number(_n), _targetT(tt), _instrinsicT(tt), _deltaT(dt), _tDiscret(td)
	{


	}

	TimeStep::TimeStep(const TimeStep & _rhs)
	{
		this->_deltaT = _rhs._deltaT;
		this->_instrinsicT = _rhs._instrinsicT;
		this->number = _rhs.number;
		this->_mStepNum = _rhs._mStepNum;
		this->_myEngng = _rhs._myEngng;
		this->_targetT = _rhs._targetT;
		this->_tDiscret = _rhs._tDiscret;
	}

}

