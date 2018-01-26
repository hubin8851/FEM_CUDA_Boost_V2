#pragma once
#include <HBXPreDef.h>

namespace HBXFEMDef
{

#define _EX_Node_Name "node"
#define _EX_Node_coords "coords"

	//��xyz�������������ֱ�洢��������ʽ������GPU������������
	template<typename _T>
	struct BOOST_SYMBOL_EXPORT _Nodes
	{
		_T* _X;
		_T* _Y;
		_T* _Z;
	};

	//�����ڵ㣬����xyz�����������
// 	template<typename _T>
// 	struct _Node
// 	{
// 		_T _X;
// 		_T _Y;
// 		_T _Z;
// 	};

	struct BOOST_SYMBOL_EXPORT Node
	{
		HBXDef::UserReadPrec _X;
		HBXDef::UserReadPrec _Y;
		HBXDef::UserReadPrec _Z;

		Node(){};
		Node(HBXDef::UserReadPrec _x, HBXDef::UserReadPrec _y, HBXDef::UserReadPrec _z) :_X(_x), _Y(_y), _Z(_z) {};
	};

}
