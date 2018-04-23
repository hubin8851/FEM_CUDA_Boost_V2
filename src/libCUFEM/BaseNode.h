#pragma once
#include <HbxDefMacro.h>
#include <ExportDef.h>

namespace HBXFEMDef
{

#define _EX_Node_Name "node"
#define _EX_Node_coords "coords"

	//以xyz三个方向的坐标分别存储成向量形式，用以GPU的向量化计算
	template<typename _T>
	struct CUFEM_API _Nodes
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

	struct CUFEM_API Node
	{
		HBXDef::UserReadPrec _X;
		HBXDef::UserReadPrec _Y;
		HBXDef::UserReadPrec _Z;

		Node(){};
		Node(HBXDef::UserReadPrec _x, HBXDef::UserReadPrec _y, HBXDef::UserReadPrec _z) :_X(_x), _Y(_y), _Z(_z) {};
	};

}

