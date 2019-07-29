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


		//刚度矩阵计算
		//传入杨氏模量、截面、和单元长度
		//_iMatSort:材料索引
		void StiffMatCal(	int _iMatSort, int _iSecSort, 
							UserCalPrec _x1, UserCalPrec _y1,
							UserCalPrec _x2, UserCalPrec _y2,
							UserCalPrec _theta);

		//刚度矩阵计算
		//传入杨氏模量、截面、和单元长度
		//_iMatSort:材料索引
		void StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec2 _N1, UserCalPrec2 _N2, UserCalPrec _theta);
		//质量矩阵计算
		void MassMatCal();

	private:

		//应该在截面类中计算
		UserCalPrec ElemtLengthCal( UserCalPrec2 _N1, UserCalPrec2 _N2 );

		

	};

//#define _EX_Beam21Elemt_Name "B21"
//	//向工厂类中注册该解算引擎
// 	REGIST_ELEMT(Beam21Elemt);

}
