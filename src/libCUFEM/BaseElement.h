#pragma once
#include <ExportDef.h>
#include <HBXPreDef.h>
#include "libCUFEM\BaseNode.h"

namespace HBXFEMDef
{
	using namespace HBXDef;
	template<typename _T> struct _Nodes;

	class Domain;
// 	template<typename _T>
// 	struct _BaseElem
// 	{
// 		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
// 		size_t _iMat;	//��Ԫ��������
// 
// 	};

	struct BOOST_SYMBOL_EXPORT BaseElem
	{
		size_t* _iNode;	//��Ԫ�ڽڵ��ţ��ڼ̳к�����������ڴ����
		size_t _iMat;	//��Ԫ��������

		BaseElem(int _n, Domain* _dm) {};

		BaseElem(UserCalPrec* _begin, size_t _lgth)
		{
			_iNode = new size_t(_lgth);
			for ( size_t i=0; i!= _lgth; i++ )
			{
				_iNode[i] = _begin[i];
			}
		}
	};


}
