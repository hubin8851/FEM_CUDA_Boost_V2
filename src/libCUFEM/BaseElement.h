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
// 		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
// 		size_t _iMat;	//单元材料索引
// 
// 	};

	struct BOOST_SYMBOL_EXPORT BaseElem
	{
		size_t* _iNode;	//单元内节点编号，在继承后根据类别完成内存分配
		size_t _iMat;	//单元材料索引

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
