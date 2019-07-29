#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <HBXFEMDefMacro.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h.>
#include <libCUFEM\EngngMod.h>
#include "libCUFEM\ClassFactory.h"


namespace HBXFEMDef
{
	using namespace HBXDef;


	class CUFEM_API Beam21Elemt 
	{
	public:
		Beam21Elemt(	Engng* _engng, Domain* _dm, int _id
					/*= B21ELEMENT */ );
		virtual ~Beam21Elemt();


		//�նȾ������
		//��������ģ�������桢�͵�Ԫ����
		//_iMatSort:��������
		void StiffMatCal(	int _iMatSort, int _iSecSort, 
							UserCalPrec _x1, UserCalPrec _y1,
							UserCalPrec _x2, UserCalPrec _y2,
							UserCalPrec _theta);

		//�նȾ������
		//��������ģ�������桢�͵�Ԫ����
		//_iMatSort:��������
		void StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec2 _N1, UserCalPrec2 _N2, UserCalPrec _theta);
		//�����������
		void MassMatCal();

	private:

		//Ӧ���ڽ������м���
		UserCalPrec ElemtLengthCal( UserCalPrec2 _N1, UserCalPrec2 _N2 );

		

	};

//#define _EX_Beam21Elemt_Name "B21"
//	//�򹤳�����ע��ý�������
// 	REGIST_ELEMT(Beam21Elemt);

}
