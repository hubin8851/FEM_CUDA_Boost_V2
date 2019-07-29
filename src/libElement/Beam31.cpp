#include <math.h>
#include "Beam31.h"


namespace HBXFEMDef
{

	Beam31Elemt::Beam31Elemt(Engng * _engng, Domain * _dm, int _id)
		:BaseElem(_engng, _dm, _id)
	{

	}

	Beam31Elemt::~Beam31Elemt()
	{
	}


	void Beam31Elemt::StiffMatCal(int _iMatSort, int _iSecSort, HBXDef::UserCalPrec _x1, HBXDef::UserCalPrec3 _y1, HBXDef::UserCalPrec _z1, HBXDef::UserCalPrec _x2, HBXDef::UserCalPrec3 _y2, UserCalPrec _z2, UserCalPrec _theta)
	{

	}

	void Beam31Elemt::StiffMatCal(int _iMatSort, int _iSecSort, HBXDef::UserCalPrec3 _N1, HBXDef::UserCalPrec3 _N2, HBXDef::UserCalPrec _theta)
	{

		//计算转动惯量
		//UserCalPrec Iy = _w * pow(_h, 3) / 12;
		//UserCalPrec Iz = pow(_w, 3) *_h / 12;
		//UserCalPrec J = 0.229 * _w *pow(_h, 3);

	}

	void Beam31Elemt::MassMatCal()
	{

	
	}

	UserCalPrec Beam31Elemt::ElemtLengthCal(HBXDef::UserCalPrec3 _N1, HBXDef::UserCalPrec3 _N2)
	{
		return sqrt( (_N2.x - _N1.x)*(_N2.x - _N1.x) + (_N2.y - _N1.y)*(_N2.y - _N1.y) + (_N2.z - _N1.z)*(_N2.z - _N1.z));
	}


	void Beam31Elemt::GetShearedParam(	UserCalPrec _E, UserCalPrec _w, UserCalPrec _h,
										
										UserCalPrec & _phaiy, UserCalPrec & _phaiz)
	{
		UserCalPrec Asy = 5 * _w*_h / 6;
		UserCalPrec Asz = 5 * _w*_h / 6;

		//_phaiy = 12 * _E*Iz / (G*Asy*L ^ 2);
		//_phaiz = 12 * _E*Iy / (G*Asz*L ^ 2);
		return;
	}
}
