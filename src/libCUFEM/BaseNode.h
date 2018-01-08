#pragma once
#include <boost/dll.hpp>
#include <ExportDef.h>


namespace HBXFEMDef
{
	//以xyz三个方向的坐标分别存储成向量形式，用以GPU的向量化计算
	template<typename _T>
	struct BOOST_SYMBOL_EXPORT _Nodes
	{
		_T* _X;
		_T* _Y;
		_T* _Z;
	};

	//单个节点，包含xyz三方向的坐标
// 	template<typename _T>
// 	struct _Node
// 	{
// 		_T _X;
// 		_T _Y;
// 		_T _Z;
// 	};

	struct BOOST_SYMBOL_EXPORT Node
	{
		double _X;
		double _Y;
		double _Z;
	};

}

