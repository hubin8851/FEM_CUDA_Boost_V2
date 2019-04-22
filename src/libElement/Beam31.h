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


		//�նȾ������
		//��������ģ�������桢�͵�Ԫ����
		void StiffMatCal(	int _iMatSort, int _iSecSort, 
							UserCalPrec _x1, UserCalPrec3 _y1, UserCalPrec _z1,
							UserCalPrec _x2, UserCalPrec3 _y2, UserCalPrec _z2,
							UserCalPrec _theta);
		//�նȾ������
		//��������ģ�������桢�͵�Ԫ����
		void StiffMatCal(int _iMatSort, int _iSecSort, UserCalPrec3 _N1, UserCalPrec3 _N2, UserCalPrec _theta);
		//�����������
		void MassMatCal();

	private:
		//Ӧ���ڽ������м���
		UserCalPrec ElemtLengthCal(UserCalPrec3 _N1, UserCalPrec3 _N2);


		//��Midas���������ԭ���ֲᡷ
		//http://blog.sina.com.cn/s/blog_6d963b350100x24i.html
		//https ://wenku.baidu.com/view/0df0fecba6c30c2258019eb2.html
		void GetShearedParam(UserCalPrec _E, UserCalPrec _w, UserCalPrec _h, UserCalPrec& _phaiy, UserCalPrec& _phaiz );

	};

//#define _EX_Beam31Elemt_Name "B31"
//	//�򹤳�����ע��ý�������
//	REGIST_ELEMT(Beam31Elemt);

}