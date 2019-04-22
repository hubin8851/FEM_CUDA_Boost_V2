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


	class CUFEM_API Beam31Elemt :public BaseElem
	{
	public:
		Beam31Elemt(Engng* _engng, Domain* _dm = nullptr, int _id = B31ELEMENT);
		virtual ~Beam31Elemt();


		//刚度矩阵计算
		//传入杨氏模量、截面、和单元长度
		void StiffMatCal(	int _iMatSort, int _iSecSort, 
							UserCalPrec _x1, UserCalPrec3 _y1, UserCalPrec _z1,
							UserCalPrec _x2, UserCalPrec3 _y2, UserCalPrec _z2,
							UserCalPrec _theta);
		//刚度矩阵计算
		//传入杨氏模量、截面、和单元长度
		void StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec3 _N1, UserCalPrec3 _N2, UserCalPrec _theta);
		//质量矩阵计算
		void MassMatCal();

	private:
		//应该在截面类中计算
		UserCalPrec ElemtLengthCal(UserCalPrec3 _N1, UserCalPrec3 _N2);


		//《Midas分析和设计原理手册》
		//http://blog.sina.com.cn/s/blog_6d963b350100x24i.html
		//https ://wenku.baidu.com/view/0df0fecba6c30c2258019eb2.html
		void GetShearedParam(UserCalPrec _E, UserCalPrec _w, UserCalPrec _h, UserCalPrec& _phaiy, UserCalPrec& _phaiz );

	};

//#define _EX_Beam31Elemt_Name "B31"
//	//向工厂类中注册该解算引擎
//	REGIST_ELEMT(Beam31Elemt);

}