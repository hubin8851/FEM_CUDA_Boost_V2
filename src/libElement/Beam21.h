#pragma once
#include <ExportDef.h>
#include <HbxDefMacro.h>
#include <libCUFEM\BaseNode.h>
#include <libCUFEM\BaseElement.h.>
#include <libCUFEM\EngngMod.h>
#include "libCUFEM\ClassFactory.h"


namespace HBXFEMDef
{
	using namespace HBXDef;


	class CUFEM_API Beam21Elemt :public BaseElem
	{
	public:
		Beam21Elemt(Engng* _engng, Domain* _dm = nullptr, int _id = B21ELEMENT);
		virtual ~Beam21Elemt();



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



}