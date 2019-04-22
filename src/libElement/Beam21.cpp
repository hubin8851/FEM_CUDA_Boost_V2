#include <math.h>
#include <HbxDefMacro.h>
#include "Beam21.h"

namespace HBXFEMDef
{

	Beam21Elemt::Beam21Elemt(Engng* _engng, Domain* _dm , int _id)
	{

	}

	Beam21Elemt::~Beam21Elemt()
	{
	}

	void Beam21Elemt::StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec _x1, UserCalPrec _y1, UserCalPrec _x2, UserCalPrec _y2, UserCalPrec _theta)
	{
		//�㷨���ա�����Ԫ�ṹ�������м��㡷P76
		//����matlab����Ԫ������Ӧ��P115
		UserCalPrec2 _N1,_N2;
		_N1.x = _x1;
		_N1.y = _y1;
		_N2.x = _x2;
		_N2.y = _y2;

		UserCalPrec _tmpL = ElemtLengthCal(_N1, _N2);
		UserCalPrec _E = _iMatSort;//������
		UserCalPrec _A = _iSecSort;//������
		UserCalPrec _I = _iSecSort;//������
		UserCalPrec w11 = _E * _A / _tmpL;
		UserCalPrec w22 = 12 * _E * _I / pow(_tmpL, 3);
		UserCalPrec w23 = 6 * _E * _I / pow(_tmpL, 2);
		UserCalPrec w33 = 4 * _E * _I / _tmpL;

		UserCalPrec x = _theta * 3.1415926 / 180;
		UserCalPrec C = cos(x);
		UserCalPrec S = sin(x);
		UserCalPrec K11 = C * C * w11 + S * S * w22;
		UserCalPrec K21 = S * C * (w11 - w22);
		UserCalPrec K31 = -S * w23;
		UserCalPrec K22 = S * S * w11 + C * C * w22;
		UserCalPrec K32 = C * w23;

#pragma region ��ͳ���ޱ�����
		CMatrix<UserCalPrec> _y(6, 6);
		_y(0, 0) = K11;
		_y(1, 0) = _y(0, 1) = K21;
		_y(1, 1) = K22;

		_y(2, 0) = _y(0, 2) = K31;
		_y(2, 1) = _y(1, 2) = K32;
		_y(2, 2) = _y(2, 2) = w33;

		_y(3, 0) = _y(0, 3) = -K11;
		_y(3, 1) = _y(1, 3) = -K21;
		_y(3, 2) = _y(2, 3) = -K31;
		_y(3, 3) = _y(3, 3) = K11;

		_y(4, 0) = _y(0, 4) = -K21;
		_y(4, 1) = _y(1, 4) = -K22;
		_y(4, 2) = _y(2, 4) = -K32;
		_y(4, 3) = _y(3, 4) = K21;
		_y(4, 4) = _y(4, 4) = K22;

		_y(5, 0) = _y(0, 5) = K31;
		_y(5, 1) = _y(1, 5) = K32;
		_y(5, 2) = _y(2, 5) = w33 / 2;
		_y(5, 3) = _y(3, 5) = -K31;
		_y(5, 4) = _y(4, 5) = -K32;
		_y(5, 5) = _y(5, 5) = w33;
#pragma endregion ��ͳ���ޱ�����
	}

	void Beam21Elemt::StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec2 _N1, UserCalPrec2 _N2, UserCalPrec _theta)
	{
		//�㷨���ա�����Ԫ�ṹ�������м��㡷P76
		//����matlab����Ԫ������Ӧ��P115
		UserCalPrec _tmpL = ElemtLengthCal( _N1, _N2 );
		UserCalPrec _E = _iMatSort;//������
		UserCalPrec _A = _iSecSort;//������
		UserCalPrec _I = _iSecSort;//������
		UserCalPrec w11 = _E * _A / _tmpL;
		UserCalPrec w22 = 12* _E * _I / pow(_tmpL, 3 );
		UserCalPrec w23 = 6 * _E * _I / pow(_tmpL, 2 );
		UserCalPrec w33 = 4 * _E * _I / _tmpL;

		UserCalPrec x = _theta * 3.1415926 / 180;
		UserCalPrec C = cos(x);
		UserCalPrec S = sin(x);
		UserCalPrec K11 = C * C * w11 + S * S * w22;
		UserCalPrec K21 = S * C * (w11 - w22);
		UserCalPrec K31 = - S * w23;
		UserCalPrec K22 = S * S * w11 + C * C * w22;
		UserCalPrec K32 = C * w23;

#pragma region ��ͳ���ޱ�����
		CMatrix<UserCalPrec> _y(6,6);
		_y(0, 0) = K11;
		_y(1, 0) = _y(0, 1) = K21;
		_y(1, 1) = K22;

		_y(2, 0) = _y(0, 2) = K31;
		_y(2, 1) = _y(1, 2) = K32;
		_y(2, 2) = _y(2, 2) = w33;

		_y(3, 0) = _y(0, 3) = -K11;
		_y(3, 1) = _y(1, 3) = -K21;
		_y(3, 2) = _y(2, 3) = -K31;
		_y(3, 3) = _y(3, 3) = K11;

		_y(4, 0) = _y(0, 4) = -K21;
		_y(4, 1) = _y(1, 4) = -K22;
		_y(4, 2) = _y(2, 4) = -K32;
		_y(4, 3) = _y(3, 4) = K21;
		_y(4, 4) = _y(4, 4) = K22;

		_y(5, 0) = _y(0, 5) = K31;
		_y(5, 1) = _y(1, 5) = K32;
		_y(5, 2) = _y(2, 5) = w33 / 2;
		_y(5, 3) = _y(3, 5) = -K31;
		_y(5, 4) = _y(4, 5) = -K32;
		_y(5, 5) = _y(5, 5) = w33;
#pragma endregion ��ͳ���ޱ�����
	}

	void Beam21Elemt::MassMatCal()
	{
	}





	UserCalPrec Beam21Elemt::ElemtLengthCal( UserCalPrec2 _N1, UserCalPrec2 _N2 )
	{
		return sqrt( (_N2.x - _N1.x)*(_N2.x - _N1.x) + (_N2.y - _N1.y)*(_N2.y - _N1.y) );
	}

}

